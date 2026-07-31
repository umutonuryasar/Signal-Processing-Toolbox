#include "freqdomainplot.h"

FreqDomainPlot::FreqDomainPlot(QCustomPlot *plot) : plot(plot) {}

void FreqDomainPlot::setupPlot()
{
    // Only ever one graph: setupPlot() must stay safe to call more than once.
    if (plot->graphCount() == 0)
        plot->addGraph();

    plot->xAxis->setLabel("Frequency(Hz)");
    plot->yAxis->setLabel("Magnitude(dB)");
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void FreqDomainPlot::updatePlot(const QVector<double> &freqSample, const QVector<double> &input)
{
    plot->graph(0)->setData(freqSample, input);
    plot->rescaleAxes();
    plot->replot();
}

void FreqDomainPlot::clearPlot()
{
    plot->graph(0)->data()->clear();
    plot->replot();
}
