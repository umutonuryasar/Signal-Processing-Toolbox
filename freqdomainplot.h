#ifndef FREQDOMAINPLOT_H
#define FREQDOMAINPLOT_H

#include <QVector>
#include <qcustomplot.h>

class FreqDomainPlot
{
public:
    FreqDomainPlot(QCustomPlot *plot);

    void setupPlot();
    void updatePlot(const QVector<double> &freqSample, const QVector<double> &input);

private:
    QCustomPlot *plot;
};

#endif // FREQDOMAINPLOT_H
