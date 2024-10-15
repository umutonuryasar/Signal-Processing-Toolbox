#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H

#include <QVector>

class WaveGenerator
{
public:
    enum class WaveType {
        Sine,
        Square,
        Triangle,
        Sawtooth
    };

    WaveGenerator(double amplitude, double frequency, double samplingFrequency);
    QVector<double> generateWave(WaveType type, double duration, double startTime);

private:
    double amplitude;
    double frequency;
    double samplingFrequency;
};

#endif // WAVEGENERATOR_H
