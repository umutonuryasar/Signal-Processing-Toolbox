#include "timedomainplot.h"

TimeDomainPlot::TimeDomainPlot(QCustomPlot *plot) : plot(plot) {}

void TimeDomainPlot::setupPlot()
{
    plot->addGraph();
    plot->xAxis->setLabel("Time(s)");
    plot->yAxis->setLabel("Amplitude(V)");
}

void TimeDomainPlot::updatePlot(const QVector<double> &time, const QVector<double> &input)
{
    plot->graph(0)->setData(time, input);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->rescaleAxes();
    plot->replot();
}
