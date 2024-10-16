#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wavegenerator.h"
#include "timedomainplot.h"
#include "freqdomainplot.h"
#include "fft.h"
#include <QMessageBox>

const int updateInterval = 100;
const int displayDuration = 3;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , fft(new FFT(ui))
    , waveGenerator(nullptr)
    , isRunning(false)
    , currentTime(0.0)
{
    ui->setupUi(this);
    this->setWindowTitle("Signal Processing Toolbox");

    connect(timer, &QTimer::timeout, this, &MainWindow::updater);
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
    double duration = static_cast<double>(updateInterval) / 1000.0; // updateInterval ms'yi saniyeye çevir

    WaveGenerator::WaveType waveType = static_cast<WaveGenerator::WaveType>(ui->comboBox_waveType->currentIndex());
    QVector<double> newSamples = waveGenerator->generateWave(waveType, duration, currentTime);

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
