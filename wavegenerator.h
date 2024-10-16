#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H

#include <QVector>
#include <random>

class WaveGenerator
{
public:
    enum class WaveType {
        Sine,
        Square,
        Triangle,
        Sawtooth,
        WhiteNoise
    };

    WaveGenerator(double amplitude, double frequency, double samplingFrequency);
    QVector<double> generateWave(WaveType type, double duration, double startTime);

private:
    double amplitude;
    double frequency;
    double samplingFrequency;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
};

#endif // WAVEGENERATOR_H
