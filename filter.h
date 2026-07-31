#ifndef FILTER_H
#define FILTER_H

#include <QVector>

// Single pole IIR (RC) filters. The filter keeps its state between process()
// calls, so a signal that arrives as consecutive blocks is filtered as one
// continuous stream instead of restarting at every block boundary.
class Filter
{
public:
    // Values match the item order of comboBox_filterType in mainwindow.ui
    enum class Type {
        None = 0,
        LowPass = 1,
        HighPass = 2
    };

    Filter();

    // Recomputes the coefficient and resets the state only when a parameter
    // actually changed. Repeated calls with the same parameters keep the state.
    void configure(Type type, double cutoffFrequency, double samplingFrequency);
    QVector<double> process(const QVector<double> &input);
    void reset();

private:
    Type type;
    double cutoffFrequency;
    double samplingFrequency;
    double alpha;

    bool primed;
    double previousInput;
    double previousOutput;
};

#endif // FILTER_H
