#include "fft.h"
#include <cmath>

FFT::FFT() : fftInput(nullptr), fftOutput(nullptr), plan(nullptr), currentSize(0)
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
    // The plan refers to the buffers, so it has to go first.
    if (plan) {
        fftw_destroy_plan(plan);
        plan = nullptr;
    }
    if (fftInput) {
        fftw_free(fftInput);
        fftInput = nullptr;
    }
    if (fftOutput) {
        fftw_free(fftOutput);
        fftOutput = nullptr;
    }
    currentSize = 0;
}

bool FFT::compute(const QVector<double> &inputData, double samplingFrequency)
{
    // Called from timer slots, so report failures instead of throwing.
    int size = inputData.size();
    if (size < 2 || samplingFrequency <= 0.0) {
        outputData.clear();
        freqSamples.clear();
        return false;
    }

    allocateMemory(size);
    if (!fftInput || !fftOutput || !plan) {
        outputData.clear();
        freqSamples.clear();
        return false;
    }

    for (int i = 0; i < size; i++)
    {
        fftInput[i][0] = inputData[i];
        fftInput[i][1] = 0.0;
    }

    fftw_execute(plan);

    // Resize vectors instead of clearing and recreating
    outputData.resize(size / 2);
    freqSamples.resize(size / 2);

    for(int i = 0; i < size / 2; i++)
    {
        double magnitude = std::sqrt(fftOutput[i][0] * fftOutput[i][0] + fftOutput[i][1] * fftOutput[i][1]) / size;

        // Single sided spectrum: every bin except DC carries the energy of its
        // mirrored counterpart as well.
        if (i > 0)
            magnitude *= 2.0;

        // Handle near-zero magnitude values (avoid log(0))
        if (magnitude < 1e-10)
            magnitude = 1e-10; // Set a small non-zero value

        outputData[i] = 20 * std::log10(magnitude);
        freqSamples[i] = (i * samplingFrequency / size);
    }

    return true;
}

const QVector<double> &FFT::getFFTOutput() const
{
    return outputData;
}

const QVector<double> &FFT::getFreqSamples() const
{
    return freqSamples;
}
