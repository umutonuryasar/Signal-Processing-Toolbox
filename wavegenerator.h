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

    // Square and sawtooth are bandlimited with PolyBLEP, so their harmonics stop
    // at Nyquist instead of folding back down into the spectrum.
    //
    // For WhiteNoise the amplitude is the standard deviation of a Gaussian, not
    // a peak value: individual samples can and do exceed it.
    WaveGenerator(double amplitude, double frequency, double samplingFrequency);
    QVector<double> generateWave(WaveType type, double duration, double startTime);

private:
    double amplitude;
    double frequency;
    double samplingFrequency;
    std::mt19937 generator;
    std::normal_distribution<double> distribution;
};

#endif // WAVEGENERATOR_H
