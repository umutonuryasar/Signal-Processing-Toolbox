#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wavegenerator.h"
#include "timedomainplot.h"
#include "freqdomainplot.h"
#include "fft.h"

const int timeInterval = 1000;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Signal Processing Toolbox");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updater()));
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::generateWave(QVector<double> &wave)
{
    double amplitude = ui->lineEdit_amplitude->text().toDouble();
    double frequency = ui->lineEdit_frequency->text().toDouble();
    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();
    double duration = 1.0;

    WaveGenerator waveformGenerator(amplitude, frequency, samplingFrequency);
    WaveGenerator::WaveType waveType = static_cast<WaveGenerator::WaveType>(ui->comboBox_waveType->currentIndex());
    QVector<double> waveInput = waveformGenerator.generateWave(waveType, duration);
    wave.append(waveInput);
}

void MainWindow::plotWave(QVector<double> &wave)
{
    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();
    double duration = 1.0;

    double dt = 1.0 / samplingFrequency;
    int numSamples = static_cast<int>(duration * samplingFrequency);

    for (int i=0; i<numSamples; i++)
    {
        time.append(t);
        t += dt;
    }

    TimeDomainPlot timeplot(ui->timePlot);
    timeplot.setupPlot();
    timeplot.updatePlot(time, wave);
}

void MainWindow::computeFFT(const QVector<double> &inputData)
{
    FFT fft(inputData.size(), ui);
    fft.compute(inputData);
    fftOutput = fft.getFFTOutput();
    fftFreqSamp = fft.getFreqSamples();
}

void MainWindow::plotFFT(QVector<double> &fftFreqSamp, QVector<double> &fftOutput)
{
    FreqDomainPlot fftPlot(ui->fftPlot);
    fftPlot.setupPlot();
    fftPlot.updatePlot(fftFreqSamp, fftOutput);
}

void MainWindow::updater()
{
    generateWave(signal);
    computeFFT(signal);
    plotWave(signal);
    plotFFT(fftFreqSamp, fftOutput);
}

void MainWindow::on_generatorButton_start_clicked()
{
    if (validateInputs()) {
        timer->start(timeInterval);
    }
}

void MainWindow::on_generatorButton_pause_clicked()
{
    timer->stop();
}

void MainWindow::on_generatorButton_stop_clicked()
{
    timer->stop();
    time.clear();
    signal.clear();
    t=0;
}
