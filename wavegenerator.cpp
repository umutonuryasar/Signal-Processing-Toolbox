#include "wavegenerator.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

WaveGenerator::WaveGenerator(double amplitude, double frequency, double samplingFrequency)
    : amplitude(amplitude), frequency(frequency), samplingFrequency(samplingFrequency) {}

QVector<double> WaveGenerator::generateWave(WaveType type, double duration, double startTime)
{
    QVector<double> wave;

    double dt = 1.0 / samplingFrequency;
    int numSamples = static_cast<int>(duration * samplingFrequency);

    wave.reserve(numSamples);

    for (int i = 0; i < numSamples; i++)
    {
        double t = startTime + static_cast<double>(i) * dt;
        double value = 0.0;

        switch (type)
        {
        case WaveType::Sine:
            value = amplitude * std::sin(2 * M_PI * frequency * t);
            break;
        case WaveType::Square:
            value = amplitude * (std::sin(2 * M_PI * frequency * t) >= 0 ? 1.0 : -1.0);
            break;
        case WaveType::Triangle:
            value = (2 * amplitude / M_PI) * std::asin(std::sin(2 * M_PI * frequency * t));
            break;
        case WaveType::Sawtooth:
            value = (2 * amplitude / M_PI) * std::atan(std::tan(M_PI * frequency * t));
            break;
        }

        wave.append(value);
    }

    return wave;
}
