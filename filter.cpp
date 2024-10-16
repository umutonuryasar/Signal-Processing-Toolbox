#include "filter.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

QVector<double> Filter::lowPassFilter(const QVector<double>& input, double cutoffFrequency, double samplingFrequency)
{
    QVector<double> output;
    output.reserve(input.size());

    double dt = 1.0 / samplingFrequency;
    double RC = 1.0 / (2 * M_PI * cutoffFrequency);
    double alpha = dt / (RC + dt);

    output.append(input[0]);

    for (int i = 1; i < input.size(); ++i)
    {
        double filteredValue = output[i-1] + alpha * (input[i] - output[i-1]);
        output.append(filteredValue);
    }

    return output;
}

QVector<double> Filter::highPassFilter(const QVector<double>& input, double cutoffFrequency, double samplingFrequency)
{
    QVector<double> output;
    output.reserve(input.size());

    double dt = 1.0 / samplingFrequency;
    double RC = 1.0 / (2 * M_PI * cutoffFrequency);
    double alpha = RC / (RC + dt);

    output.append(input[0]);

    for (int i = 1; i < input.size(); ++i)
    {
        double filteredValue = alpha * (output[i-1] + input[i] - input[i-1]);
        output.append(filteredValue);
    }

    return output;
}
