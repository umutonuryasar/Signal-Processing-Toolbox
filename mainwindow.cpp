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
    , fft(new FFT(ui))
    , waveGenerator(nullptr)
    , isRunning(false)
    , currentTime(0.0)
    , mediaPlayer(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
    , playbackTimer(new QTimer(this))
    , graphUpdateTimer(new QTimer(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Signal Processing Toolbox");
    setupGraphs();
    mediaPlayer->setAudioOutput(audioOutput);

    connect(playbackTimer, &QTimer::timeout, this, &MainWindow::updatePlaybackInfo);
    connect(timer, &QTimer::timeout, this, &MainWindow::updater);
    connect(ui->pushButton_loadFile, &QPushButton::clicked, this, &MainWindow::on_loadWavButton_clicked);
    connect(ui->audioButton_play, &QPushButton::clicked, this, &MainWindow::on_playButton_clicked);
    connect(ui->audioButton_stop, &QPushButton::clicked, this, &MainWindow::on_stopButton_clicked);
    connect(graphUpdateTimer, &QTimer::timeout, this, &MainWindow::updateGraphs);

    setupGraphUpdateTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fft;
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

    return true;
}

void MainWindow::generateWave()
{
    if (!validateInputs()) {
        return;
    }

    double amplitude = ui->lineEdit_amplitude->text().toDouble();
    double frequency = ui->lineEdit_frequency->text().toDouble();
    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();

    delete waveGenerator;
    waveGenerator = new WaveGenerator(amplitude, frequency, samplingFrequency);
}

void MainWindow::updateSignal()
{
    if (!waveGenerator) return;

    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();
    double duration = static_cast<double>(updateInterval) / 1000.0; // updateInterval milisecond to second

    WaveGenerator::WaveType waveType = static_cast<WaveGenerator::WaveType>(ui->comboBox_waveType->currentIndex());
    QVector<double> newSamples = waveGenerator->generateWave(waveType, duration, currentTime);

    // Filter
    int filterType = ui->comboBox_filterType->currentIndex();
    if (filterType > 0) // 0 = No Filter
    {
        double cutoffFrequency = ui->lineEdit_cutoffFrequency->text().toDouble();
        switch(filterType)
        {
        case 1: // Low Pass Filter
            newSamples = Filter::lowPassFilter(newSamples, cutoffFrequency, samplingFrequency);
            break;
        case 2: // High Pass Filter
            newSamples = Filter::highPassFilter(newSamples, cutoffFrequency, samplingFrequency);
            break;
        }
    }

    signal.append(newSamples);

    int maxSamples = static_cast<int>(displayDuration * samplingFrequency);
    if (signal.size() > maxSamples) {
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

    double dt = 1.0 / samplingFrequency;
    time.resize(signal.size());
    for (int i = 0; i < signal.size(); i++)
    {
        time[i] = currentTime - (signal.size() - i) * dt;
    }

    TimeDomainPlot timeplot(ui->timePlot);
    timeplot.setupPlot();
    timeplot.updatePlot(time, signal);
}

void MainWindow::computeFFT()
{
    if (signal.isEmpty()) {
        return;
    }

    fft->compute(signal);
    fftOutput = fft->getFFTOutput();
    fftFreqSamp = fft->getFreqSamples();
}

void MainWindow::plotFFT()
{
    if (fftOutput.isEmpty() || fftFreqSamp.isEmpty()) {
        return;
    }

    FreqDomainPlot fftPlot(ui->fftPlot);
    fftPlot.setupPlot();
    fftPlot.updatePlot(fftFreqSamp, fftOutput);
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

        // Grafikleri temizle
        signal.clear();
        time.clear();
        fftOutput.clear();
        fftFreqSamp.clear();
        currentTime = 0.0;

        ui->timePlot->graph(0)->data()->clear();
        ui->fftPlot->graph(0)->data()->clear();
        ui->timePlot->replot();
        ui->fftPlot->replot();
    }
}

void MainWindow::on_loadWavButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open WAV File", "", "WAV Files (*.wav)");
    if (filePath.isEmpty()) return;

    if (WaveReader::readWavFile(filePath, wavSamples, wavSampleRate)) {
        wavDuration = wavSamples.size() / wavSampleRate;
        ui->label_fileName->setText("File: " + QFileInfo(filePath).fileName());

        mediaPlayer->setSource(QUrl::fromLocalFile(filePath));

        ui->timePlot->graph(0)->setData(QVector<double>(), QVector<double>());
        ui->fftPlot->graph(0)->setData(QVector<double>(), QVector<double>());
        ui->timePlot->rescaleAxes();
        ui->fftPlot->rescaleAxes();
        ui->timePlot->replot();
        ui->fftPlot->replot();

        qDebug() << "WAV file loaded, graphs reset";

        updateUIForPlaybackState(false);
    } else {
        QMessageBox::warning(this, "Error", "Failed to read WAV file.");
    }
}

void MainWindow::setupGraphs()
{
    // Time domain plot
    ui->timePlot->addGraph();
    ui->timePlot->xAxis->setLabel("Time (s)");
    ui->timePlot->yAxis->setLabel("Amplitude");
    ui->timePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // Frequency domain plot
    ui->fftPlot->addGraph();
    ui->fftPlot->xAxis->setLabel("Frequency (Hz)");
    ui->fftPlot->yAxis->setLabel("Magnitude");
    ui->fftPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    qDebug() << "Graphs setup completed";
}

void MainWindow::setupAudioPlayback()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open WAV File", "", "WAV Files (*.wav)");
    if (filePath.isEmpty()) return;

    mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
}

void MainWindow::on_playButton_clicked()
{
    if (mediaPlayer->source().isEmpty()) {
        QMessageBox::warning(this, "Error", "No audio file loaded.");
        return;
    }
    mediaPlayer->play();
    playbackTimer->start(100);
    graphUpdateTimer->start(50);  // Bu satırın var olduğundan emin olun
    updateUIForPlaybackState(true);

    qDebug() << "Play button clicked, timers started";  // Debug mesajı ekleyin
}

void MainWindow::on_stopButton_clicked()
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
    graphUpdateTimer = new QTimer(this);
    connect(graphUpdateTimer, &QTimer::timeout, this, &MainWindow::updateGraphs);
    graphUpdateTimer->setInterval(50);
}

void MainWindow::updateGraphs()
{
    qDebug() << "Updating graphs...";
    if (!mediaPlayer || mediaPlayer->playbackState() != QMediaPlayer::PlayingState || wavSamples.isEmpty()) {
        qDebug() << "Media not playing or samples empty";
        return;
    }

    qint64 position = mediaPlayer->position();
    int windowSize = wavSampleRate / 10;

    QVector<double> currentWindow(windowSize);
    QVector<double> time(windowSize);

    qint64 samplePosition = (position * wavSampleRate) / 1000;

    for (int i = 0; i < windowSize; ++i) {
        int index = samplePosition + i;
        if (index >= 0 && index < wavSamples.size()) {
            currentWindow[i] = wavSamples[index];
        } else {
            currentWindow[i] = 0;
        }
        time[i] = i / static_cast<double>(wavSampleRate);
    }

    ui->timePlot->graph(0)->setData(time, currentWindow);
    ui->timePlot->rescaleAxes();
    ui->timePlot->replot();

    fft->compute(currentWindow);
    QVector<double> fftOutput = fft->getFFTOutput();
    QVector<double> fftFreqSamp = fft->getFreqSamples();

    ui->fftPlot->graph(0)->setData(fftFreqSamp, fftOutput);
    ui->fftPlot->rescaleAxes();
    ui->fftPlot->replot();

    qDebug() << "Graphs updated";
}
