#include "fft.h"
#include <cmath>

FFT::FFT(int size, Ui::MainWindow *ui):size(size), ui(ui)
{
    fftInput = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
    fftOutput = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
    plan = fftw_plan_dft_1d(size, fftInput, fftOutput, FFTW_FORWARD, FFTW_ESTIMATE);
}

FFT::~FFT()
{
    fftw_free(fftInput);
    fftw_free(fftOutput);
    fftw_destroy_plan(plan);
}

void FFT::compute(const QVector<double> &inputData)
{
    size = inputData.size();
    double samplingFrequency = ui->lineEdit_samplingFrequency->text().toDouble();

    for (int i = 0; i < size; i++)
    {
        fftInput[i][0] = inputData[i];
        fftInput[i][1] = 0.0;
    }

    fftw_execute(plan);

    for(int i = 0; i < size / 2; i++)
    {
        double magnitude = abs(sqrt(fftOutput[i][0] * fftOutput[i][0] + fftOutput[i][1] * fftOutput[i][1])) / size;

        /*
        // Handle near-zero magnitude values (avoid log(0))
        if (magnitude < 1e-10)
            magnitude = 1e-10; // Set a small non-zero value
        */

        double magdB = 20 * log10(magnitude);
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
