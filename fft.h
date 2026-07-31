#ifndef FFT_H
#define FFT_H

#include <QVector>
#include <fftw3.h>

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

    fftw_complex *fftInput;
    fftw_complex *fftOutput;
    fftw_plan plan;
    int currentSize;

    QVector<double> outputData;
    QVector<double> freqSamples;
};

#endif // FFT_H
