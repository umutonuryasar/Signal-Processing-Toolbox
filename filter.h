#ifndef FILTER_H
#define FILTER_H

#include <QVector>

class Filter
{
public:
    static QVector<double> lowPassFilter(const QVector<double>& input, double cutoffFrequency, double samplingFrequency);
    static QVector<double> highPassFilter(const QVector<double>& input, double cutoffFrequency, double samplingFrequency);
};

#endif // FILTER_H
