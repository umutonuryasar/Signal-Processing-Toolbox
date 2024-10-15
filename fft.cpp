#include "fft.h"
#include <cmath>
#include <stdexcept>
#include "ui_mainwindow.h"

FFT::FFT(Ui::MainWindow *ui) : ui(ui), fftInput(nullptr), fftOutput(nullptr), plan(nullptr), currentSize(0)
{
}

FFT::~FFT()
{
    freeMemory();
}

void FFT::allocateMemory(int size)
{
    if (size != currentSize) {
        freeMemory();
        fftInput = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
        fftOutput = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
        plan = fftw_plan_dft_1d(size, fftInput, fftOutput, FFTW_FORWARD, FFTW_ESTIMATE);
        currentSize = size;
    }
}

void FFT::freeMemory()
{
    if (fftInput) {
        fftw_free(fftInput);
        fftInput = nullptr;
    }
    if (fftOutput) {
        fftw_free(fftOutput);
        fftOutput = nullptr;
    }
    if (plan) {
        fftw_destroy_plan(plan);
        plan = nullptr;
    }
    currentSize = 0;
}

void FFT::compute(const QVector<double> &inputData)
{
    int size = inputData.size();
    if (size == 0) {
        throw std::runtime_error("Input data is empty");
    }

    allocateMemory(size);

    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();

    for (int i = 0; i < size; i++)
    {
        fftInput[i][0] = inputData[i];
        fftInput[i][1] = 0.0;
    }

    fftw_execute(plan);

    outputData.clear();
    freqSamples.clear();
    outputData.reserve(size / 2);
    freqSamples.reserve(size / 2);

    for(int i = 0; i < size / 2; i++)
    {
        double magnitude = std::abs(std::sqrt(fftOutput[i][0] * fftOutput[i][0] + fftOutput[i][1] * fftOutput[i][1])) / size;

        // Handle near-zero magnitude values (avoid log(0))
        if (magnitude < 1e-10)
            magnitude = 1e-10; // Set a small non-zero value

        double magdB = 20 * std::log10(magnitude);
        double value = (i * samplingFrequency / size);

        freqSamples.append(value);
        outputData.append(magdB);
    }
}

const QVector<double> &FFT::getFFTOutput() const
{
    return outputData;
}

const QVector<double> &FFT::getFreqSamples() const
{
    return freqSamples;
}
