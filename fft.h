#ifndef FFT_H
#define FFT_H

#include "ui_mainwindow.h"
#include <QVector>
#include <fftw3.h>

class FFT
{
public:
    FFT(int size, Ui::MainWindow *ui);
    ~FFT();

    void compute(const QVector<double> &inputData);
    const QVector<double> &getFFTOutput() const;
    const QVector<double> &getFreqSamples() const;

private slots:
    void on_generatorButton_start_clicked();

private:
    int size;

    Ui::MainWindow *ui;

    fftw_complex *fftInput, *fftOutput;
    fftw_plan plan;

    QVector <double> outputData, freqSamples;
};

#endif // FFT_H
