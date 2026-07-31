#include "filter.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Filter::Filter()
    : type(Type::None), cutoffFrequency(0.0), samplingFrequency(0.0), alpha(0.0),
    primed(false), previousInput(0.0), previousOutput(0.0)
{
}

void Filter::configure(Type type, double cutoffFrequency, double samplingFrequency)
{
    if (this->type == type
        && this->cutoffFrequency == cutoffFrequency
        && this->samplingFrequency == samplingFrequency) {
        return;
    }

    this->type = type;
    this->cutoffFrequency = cutoffFrequency;
    this->samplingFrequency = samplingFrequency;

    // An unusable cutoff or sample rate degrades to a pass through filter
    // instead of producing infinities.
    if (cutoffFrequency <= 0.0 || samplingFrequency <= 0.0) {
        this->type = Type::None;
        alpha = 0.0;
    } else {
        double dt = 1.0 / samplingFrequency;
        double RC = 1.0 / (2 * M_PI * cutoffFrequency);
        alpha = (this->type == Type::LowPass) ? dt / (RC + dt) : RC / (RC + dt);
    }

    reset();
}

void Filter::reset()
{
    primed = false;
    previousInput = 0.0;
    previousOutput = 0.0;
}

QVector<double> Filter::process(const QVector<double> &input)
{
    if (type == Type::None || input.isEmpty()) {
        return input;
    }

    QVector<double> output;
    output.reserve(input.size());

    for (int i = 0; i < input.size(); ++i)
    {
        double sample = input[i];
        double filteredValue;

        if (!primed) {
            // First sample ever seen: seed the state with it.
            filteredValue = sample;
            primed = true;
        } else if (type == Type::LowPass) {
            filteredValue = previousOutput + alpha * (sample - previousOutput);
        } else {
            filteredValue = alpha * (previousOutput + sample - previousInput);
        }

        previousInput = sample;
        previousOutput = filteredValue;
        output.append(filteredValue);
    }

    return output;
}
