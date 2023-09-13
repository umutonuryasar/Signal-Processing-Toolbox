#ifndef TIMEDOMAINPLOT_H
#define TIMEDOMAINPLOT_H

#include <QVector>
#include <qcustomplot.h>

class TimeDomainPlot
{
public:
    TimeDomainPlot(QCustomPlot *plot);

    void setupPlot();
    void updatePlot(const QVector<double> &time, const QVector<double> &input);

private:
    QCustomPlot *plot;
};

#endif // TIMEDOMAINPLOT_H
