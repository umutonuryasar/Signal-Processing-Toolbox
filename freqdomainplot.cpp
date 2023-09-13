#include "freqdomainplot.h"

FreqDomainPlot::FreqDomainPlot(QCustomPlot *plot) : plot(plot) {}

void FreqDomainPlot::setupPlot()
{
    plot->addGraph();
    plot->xAxis->setLabel("Frequency(Hz)");
    plot->yAxis->setLabel("Magnitude(dB)");
}

void FreqDomainPlot::updatePlot(const QVector<double> &freqSample, const QVector<double> &input)
{
    plot->graph(0)->setData(freqSample, input);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->rescaleAxes();
    plot->replot();
}
