#include "timedomainplot.h"

TimeDomainPlot::TimeDomainPlot(QCustomPlot *plot) : plot(plot) {}

void TimeDomainPlot::setupPlot()
{
    // Only ever one graph: setupPlot() must stay safe to call more than once.
    if (plot->graphCount() == 0)
        plot->addGraph();

    plot->xAxis->setLabel("Time(s)");
    plot->yAxis->setLabel("Amplitude(V)");
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void TimeDomainPlot::updatePlot(const QVector<double> &time, const QVector<double> &input)
{
    plot->graph(0)->setData(time, input);
    plot->rescaleAxes();
    plot->replot();
}

void TimeDomainPlot::clearPlot()
{
    plot->graph(0)->data()->clear();
    plot->replot();
}
