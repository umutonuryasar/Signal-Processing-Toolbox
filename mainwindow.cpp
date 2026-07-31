#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wavegenerator.h"
#include "timedomainplot.h"
#include "freqdomainplot.h"
#include "fft.h"
#include "filter.h"
#include <QMessageBox>
#include "wavreader.h"
#include <QFileDialog>

const int updateInterval = 100;
const int displayDuration = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , fft(new FFT())
    , filter(new Filter())
    , waveGenerator(nullptr)
    , timeDomainPlot(nullptr)
    , freqDomainPlot(nullptr)
    , isRunning(false)
    , currentTime(0.0)
    , mediaPlayer(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
    , playbackTimer(new QTimer(this))
    , wavSampleRate(0.0)
    , graphUpdateTimer(new QTimer(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Signal Processing Toolbox");

    // The plot wrappers own no widgets, they only drive the ones from the form.
    timeDomainPlot = new TimeDomainPlot(ui->timePlot);
    freqDomainPlot = new FreqDomainPlot(ui->fftPlot);

    setupGraphs();
    mediaPlayer->setAudioOutput(audioOutput);

    connect(playbackTimer, &QTimer::timeout, this, &MainWindow::updatePlaybackInfo);
    connect(timer, &QTimer::timeout, this, &MainWindow::updater);
    connect(ui->pushButton_loadFile, &QPushButton::clicked, this, &MainWindow::loadWavFile);
    connect(ui->audioButton_play, &QPushButton::clicked, this, &MainWindow::startPlayback);
    connect(ui->audioButton_stop, &QPushButton::clicked, this, &MainWindow::stopPlayback);
    connect(graphUpdateTimer, &QTimer::timeout, this, &MainWindow::updateGraphs);

    setupGraphUpdateTimer();
}

MainWindow::~MainWindow()
{
    delete timeDomainPlot;
    delete freqDomainPlot;
    delete ui;
    delete fft;
    delete filter;
    delete waveGenerator;
}

bool MainWindow::validateInputs()
{
    bool ok;
    double amplitude = ui->lineEdit_amplitude->text().toDouble(&ok);
    if (!ok || amplitude <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid positive number for amplitude.");
        return false;
    }

    double frequency = ui->lineEdit_frequency->text().toDouble(&ok);
    if (!ok || frequency <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid positive number for frequency.");
        return false;
    }

    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble(&ok);
    if (!ok || samplingFrequency <= 0 || samplingFrequency <= 2*frequency) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid sampling frequency (must be > 2 * signal frequency).");
        return false;
    }

    if (ui->comboBox_filterType->currentIndex() > 0) {
        double cutoffFrequency = ui->lineEdit_cutoffFrequency->text().toDouble(&ok);
        if (!ok || cutoffFrequency <= 0 || cutoffFrequency >= samplingFrequency / 2.0) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid cutoff frequency (must be > 0 and < sampling frequency / 2).");
            return false;
        }
    }

    return true;
}

void MainWindow::generateWave()
{
    double amplitude = ui->lineEdit_amplitude->text().toDouble();
    double frequency = ui->lineEdit_frequency->text().toDouble();
    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();

    delete waveGenerator;
    waveGenerator = new WaveGenerator(amplitude, frequency, samplingFrequency);

    filter->reset();
}

void MainWindow::updateSignal()
{
    if (!waveGenerator) return;

    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();
    if (samplingFrequency <= 0) return;

    double duration = static_cast<double>(updateInterval) / 1000.0; // updateInterval milisecond to second

    WaveGenerator::WaveType waveType = static_cast<WaveGenerator::WaveType>(ui->comboBox_waveType->currentIndex());
    QVector<double> newSamples = waveGenerator->generateWave(waveType, duration, currentTime);

    // The filter keeps its state across blocks, so configure() only resets it
    // when the user actually changed the filter settings.
    Filter::Type filterType = static_cast<Filter::Type>(ui->comboBox_filterType->currentIndex());
    double cutoffFrequency = ui->lineEdit_cutoffFrequency->text().toDouble();
    filter->configure(filterType, cutoffFrequency, samplingFrequency);
    newSamples = filter->process(newSamples);

    signal.append(newSamples);

    int maxSamples = static_cast<int>(displayDuration * samplingFrequency);
    if (maxSamples > 0 && signal.size() > maxSamples) {
        signal = signal.mid(signal.size() - maxSamples);
    }

    currentTime += duration;
}

void MainWindow::plotWave()
{
    if (signal.isEmpty()) {
        return;
    }

    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();
    if (samplingFrequency <= 0) {
        return;
    }

    double dt = 1.0 / samplingFrequency;
    time.resize(signal.size());
    for (int i = 0; i < signal.size(); i++)
    {
        time[i] = currentTime - (signal.size() - i) * dt;
    }

    timeDomainPlot->updatePlot(time, signal);
}

void MainWindow::computeFFT()
{
    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();

    if (!fft->compute(signal, samplingFrequency)) {
        fftOutput.clear();
        fftFreqSamp.clear();
        return;
    }

    fftOutput = fft->getFFTOutput();
    fftFreqSamp = fft->getFreqSamples();
}

void MainWindow::plotFFT()
{
    if (fftOutput.isEmpty() || fftFreqSamp.isEmpty()) {
        return;
    }

    freqDomainPlot->updatePlot(fftFreqSamp, fftOutput);
}

void MainWindow::updater()
{
    updateSignal();
    plotWave();
    computeFFT();
    plotFFT();
}

void MainWindow::on_generatorButton_start_clicked()
{
    if (!isRunning && validateInputs()) {
        generateWave();
        timer->start(updateInterval);
        isRunning = true;
        ui->generatorButton_start->setEnabled(false);
        ui->generatorButton_stop->setEnabled(true);
    }
}

void MainWindow::on_generatorButton_stop_clicked()
{
    if (isRunning) {
        timer->stop();
        isRunning = false;
        ui->generatorButton_start->setEnabled(true);
        ui->generatorButton_stop->setEnabled(false);

        // Clear the graphs
        signal.clear();
        time.clear();
        fftOutput.clear();
        fftFreqSamp.clear();
        currentTime = 0.0;
        filter->reset();

        timeDomainPlot->clearPlot();
        freqDomainPlot->clearPlot();
    }
}

void MainWindow::loadWavFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open WAV File", "", "WAV Files (*.wav)");
    if (filePath.isEmpty()) return;

    if (!WaveReader::readWavFile(filePath, wavSamples, wavSampleRate) || wavSampleRate <= 0.0) {
        wavSamples.clear();
        wavSampleRate = 0.0;
        QMessageBox::warning(this, "Error", "Failed to read WAV file.");
        return;
    }

    ui->label_fileName->setText("File: " + QFileInfo(filePath).fileName());

    mediaPlayer->setSource(QUrl::fromLocalFile(filePath));

    timeDomainPlot->clearPlot();
    freqDomainPlot->clearPlot();

    updateUIForPlaybackState(false);
}

void MainWindow::setupGraphs()
{
    timeDomainPlot->setupPlot();
    freqDomainPlot->setupPlot();
}

void MainWindow::startPlayback()
{
    if (mediaPlayer->source().isEmpty()) {
        QMessageBox::warning(this, "Error", "No audio file loaded.");
        return;
    }
    mediaPlayer->play();
    playbackTimer->start(100);
    graphUpdateTimer->start();
    updateUIForPlaybackState(true);
}

void MainWindow::stopPlayback()
{
    mediaPlayer->stop();
    playbackTimer->stop();
    graphUpdateTimer->stop();
    updateUIForPlaybackState(false);
}

void MainWindow::on_progressSlider_valueChanged(int value)
{
    if (!ui->progressSlider->isSliderDown()) {
        return;
    }
    qint64 duration = mediaPlayer->duration();
    mediaPlayer->setPosition((value * duration) / 100);
}

void MainWindow::updatePlaybackInfo()
{
    qint64 position = mediaPlayer->position();
    qint64 duration = mediaPlayer->duration();

    ui->label_time->setText(QString("%1 / %2")
                               .arg(QTime::fromMSecsSinceStartOfDay(position).toString("mm:ss"))
                               .arg(QTime::fromMSecsSinceStartOfDay(duration).toString("mm:ss")));

    if (duration > 0) {
        ui->progressSlider->setValue((position * 100) / duration);
    }
}

void MainWindow::updateUIForPlaybackState(bool isPlaying)
{
    ui->audioButton_play->setEnabled(!isPlaying);
    ui->audioButton_stop->setEnabled(isPlaying);
    ui->progressSlider->setEnabled(true);
}

void MainWindow::setupGraphUpdateTimer()
{
    // The timer itself is created and connected in the constructor.
    graphUpdateTimer->setInterval(50);
}

void MainWindow::updateGraphs()
{
    if (!mediaPlayer || mediaPlayer->playbackState() != QMediaPlayer::PlayingState
        || wavSamples.isEmpty() || wavSampleRate <= 0.0) {
        return;
    }

    qint64 position = mediaPlayer->position();
    int windowSize = static_cast<int>(wavSampleRate / 10);
    if (windowSize < 2) {
        return;
    }

    QVector<double> currentWindow(windowSize);
    QVector<double> windowTime(windowSize);

    qint64 samplePosition = static_cast<qint64>(position * wavSampleRate / 1000.0);

    for (int i = 0; i < windowSize; ++i) {
        qint64 index = samplePosition + i;
        if (index >= 0 && index < wavSamples.size()) {
            currentWindow[i] = wavSamples[index];
        } else {
            currentWindow[i] = 0;
        }
        windowTime[i] = i / wavSampleRate;
    }

    timeDomainPlot->updatePlot(windowTime, currentWindow);

    // The WAV has its own sample rate, which is unrelated to the generator one.
    if (fft->compute(currentWindow, wavSampleRate)) {
        freqDomainPlot->updatePlot(fft->getFreqSamples(), fft->getFFTOutput());
    }
}
