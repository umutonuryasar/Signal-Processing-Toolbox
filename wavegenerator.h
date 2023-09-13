#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H

#include <QVector>
#include <qmath.h>

class WaveGenerator
{
public:
    enum class WaveType
    {
        Sine,
        Square,
        Triangle,
        Sawtooth
    };

    WaveGenerator(double amplitude, double frequency, double samplingFrequency);
    QVector<double> generateWave(WaveType type, double duration);

private:
    double amplitude;
    double frequency;
    double samplingFrequency;
};

#endif // WAVEGENERATOR_H
