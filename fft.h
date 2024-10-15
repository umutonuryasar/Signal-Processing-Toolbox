#ifndef FFT_H
#define FFT_H

#include <QVector>
#include <fftw3.h>

// Forward declaration
namespace Ui {
class MainWindow;
}

class FFT
{
public:
    FFT(Ui::MainWindow *ui);
    ~FFT();

    void compute(const QVector<double> &inputData);
    const QVector<double> &getFFTOutput() const;
    const QVector<double> &getFreqSamples() const;

private:
    void allocateMemory(int size);
    void freeMemory();

    Ui::MainWindow *ui;
    fftw_complex *fftInput;
    fftw_complex *fftOutput;
    fftw_plan plan;
    int currentSize;

    QVector<double> outputData;
    QVector<double> freqSamples;
};

#endif // FFT_H
