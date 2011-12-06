#ifndef PLOT_H
#define PLOT_H

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_interval.h>
#include <qwt/qwt_plot_directpainter.h>

class Plot : public QwtPlot
{
    Q_OBJECT
public:
    explicit Plot(QWidget *parent = 0);

private:
    QwtInterval interval;
    QwtPlotDirectPainter painter;
    int timerId;

protected:
    void timerEvent(QTimerEvent *event);

signals:

public slots:
    void updateViewportX(const int msecs);
    void updateViewportY(const double lower, const double upper);
    void start();
    void stop();
};

#endif // PLOT_H
