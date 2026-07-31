#include "wavegenerator.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace {

// Polynomial band limited step. Added at every discontinuity of a naive
// waveform, it rounds the jump over roughly one sample and cancels most of the
// aliasing that the raw step would fold back below Nyquist.
// phase and phaseIncrement are both normalised to one period.
double polyBLEP(double phase, double phaseIncrement)
{
    if (phaseIncrement <= 0.0)
        return 0.0;

    if (phase < phaseIncrement) {
        // Just after the step.
        double t = phase / phaseIncrement;
        return t + t - t * t - 1.0;
    }

    if (phase > 1.0 - phaseIncrement) {
        // Just before the next one.
        double t = (phase - 1.0) / phaseIncrement;
        return t * t + t + t + 1.0;
    }

    return 0.0;
}

// Normalised phase in [0, 1) for the given absolute time.
double phaseAt(double frequency, double t)
{
    double phase = std::fmod(frequency * t, 1.0);
    if (phase < 0.0)
        phase += 1.0;
    return phase;
}

} // namespace

WaveGenerator::WaveGenerator(double amplitude, double frequency, double samplingFrequency)
    : amplitude(amplitude), frequency(frequency), samplingFrequency(samplingFrequency),
    generator(std::random_device{}()), distribution(0.0, 1.0)
{
}

QVector<double> WaveGenerator::generateWave(WaveType type, double duration, double startTime)
{
    QVector<double> wave;

    double dt = 1.0 / samplingFrequency;
    int numSamples = static_cast<int>(duration * samplingFrequency);

    wave.reserve(numSamples);

    // How far the phase advances per sample: the width of the PolyBLEP window.
    double phaseIncrement = frequency / samplingFrequency;

    for (int i = 0; i < numSamples; i++)
    {
        double t = startTime + static_cast<double>(i) * dt;
        double value = 0.0;

        switch (type)
        {
        case WaveType::Sine:
            value = amplitude * std::sin(2 * M_PI * frequency * t);
            break;
        case WaveType::Square: {
            double phase = phaseAt(frequency, t);
            // Naive square, then correct both edges of the period.
            double square = (phase < 0.5) ? 1.0 : -1.0;
            square += polyBLEP(phase, phaseIncrement);
            square -= polyBLEP(std::fmod(phase + 0.5, 1.0), phaseIncrement);
            value = amplitude * square;
            break;
        }
        case WaveType::Triangle:
            // A triangle's harmonics fall off as 1/n^2, so what aliases is far
            // below the square and sawtooth and the closed form stays fine.
            value = (2 * amplitude / M_PI) * std::asin(std::sin(2 * M_PI * frequency * t));
            break;
        case WaveType::Sawtooth: {
            double phase = phaseAt(frequency, t);
            // Naive rising ramp with its single wrap point corrected.
            double saw = 2.0 * phase - 1.0;
            saw -= polyBLEP(phase, phaseIncrement);
            value = amplitude * saw;
            break;
        }
        case WaveType::WhiteNoise:
            // amplitude is the standard deviation here, see the header.
            value = amplitude * distribution(generator);
            break;
        }

        wave.append(value);
    }

    return wave;
}
