#ifndef FFT_H
#define FFT_H

#include <QVector>
#include <fftw3.h>

// Magnitude spectrum of a real signal.
//
// The input is Hann windowed before the transform, which trades a slightly
// wider main lobe for far lower sidelobes: without it a tone that does not land
// exactly on a bin centre smears its energy across the whole spectrum.
//
// The windowed block is then zero padded to the next power of two. That keeps
// FFTW on its fastest code path and stops it re-planning whenever the input
// length changes. Padding interpolates the spectrum, it does not add real
// resolution: that is still set by the unpadded block length.
class FFT
{
public:
    FFT();
    ~FFT();

    // Returns false when the input is empty or the sampling frequency is invalid.
    bool compute(const QVector<double> &inputData, double samplingFrequency);
    const QVector<double> &getFFTOutput() const;
    const QVector<double> &getFreqSamples() const;

private:
    void allocateMemory(int size);
    void freeMemory();
    void updateWindow(int size);

    fftw_complex *fftInput;
    fftw_complex *fftOutput;
    fftw_plan plan;
    int currentSize;

    QVector<double> window;
    int windowSize;
    double windowGain;

    QVector<double> outputData;
    QVector<double> freqSamples;
};

#endif // FFT_H
