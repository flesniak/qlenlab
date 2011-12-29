#include <QPalette>
#include <QTimerEvent>

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_layout.h>

#include "plot.h"
#include "signaldata.h"

Plot::Plot(signaldata* data, QWidget *parent) : QwtPlot(parent), interval(0.0, 20.0), data(data)
{
    setAxisTitle(QwtPlot::xBottom, "Zeit [ms]");
    setAxisTitle(QwtPlot::yLeft, "Spannung [V]");
    plotLayout()->setAlignCanvasToScales(true);

    QPalette pal = canvas()->palette();
    pal.setColor(QPalette::Window, Qt::black);
    canvas()->setPalette(pal);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(Qt::white, 0.0, Qt::DotLine));
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);
}

void Plot::updateViewportX(const int msecs)
{
    interval.setMaxValue(interval.minValue()+msecs);
    setAxisScale(QwtPlot::xBottom, interval.minValue(), interval.maxValue());
    replot();
}

void Plot::updateViewportY(const double lower, const double upper)
{
    setAxisScale(QwtPlot::yLeft, lower, upper);
    replot();
}

void Plot::timerEvent(QTimerEvent *event)
{
    if( event->timerId() == timerId ) {
        //update graph
    }

    QwtPlot::timerEvent(event);
}

void Plot::start()
{
    timerId = startTimer(50);
}

void Plot::stop()
{
    killTimer(timerId);
}