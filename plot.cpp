/************************************************************************
 * Copyright (C) 2011 Fabian Lesniak <fabian.lesniak@student.kit.edu>   *
 *                                                                      *
 * This file is part of the QLenLab project.                            *
 *                                                                      *
 * QLenLab is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * QLenLab is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with QLenLab. If not, see <http://www.gnu.org/licenses/>.      *
 ***********************************************************************/

#include <QPalette>
#include <QTimerEvent>
#include <QPen>
#include <QColor>

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_plot_curve.h>

#include "plot.h"
#include "communicator.h"
#include "signaldata.h"

plot::plot(communicator* com, QWidget *parent) : QwtPlot(parent), interval(0.0, 20.0), com(com)
{
    setAxisTitle(QwtPlot::xBottom, "Zeit [ms]");
    setAxisTitle(QwtPlot::yLeft, "Spannung [V]");
    plotLayout()->setAlignCanvasToScales(true);

    QPalette pal = canvas()->palette();
    pal.setColor(QPalette::Window, Qt::black);
    canvas()->setPalette(pal);

    grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);

    curve[0] = new QwtPlotCurve(tr("Kanal 1"));
    curve[0]->setData(com->getdata(0));
    curve[0]->attach(this);
    curve[1] = new QwtPlotCurve(tr("Kanal 2"));
    curve[1]->setData(com->getdata(1));
    curve[1]->attach(this);
    curve[2] = new QwtPlotCurve(tr("Kanal 3"));
    curve[2]->setData(com->getdata(2));
    curve[2]->attach(this);
    curve[3] = new QwtPlotCurve(tr("Kanal 4"));
    curve[3]->setData(com->getdata(3));
    curve[3]->attach(this);

    connect(com,SIGNAL(newDataset()),SLOT(setData()));
}

void plot::setData()
{
    replot();
}

void plot::updateColor(meta::colorindex ci, QColor color)
{
    switch( ci ) {
    case meta::background : canvas()->setPalette(QPalette(QPalette::Base,color));
                            break;
    case meta::grid       : grid->setPen(QPen(color, 0.0, Qt::DotLine));
                            break;
    case meta::channel1   : curve[0]->setPen(QPen(color));
                            break;
    case meta::channel2   : curve[1]->setPen(QPen(color));
                            break;
    case meta::channel3   : curve[2]->setPen(QPen(color));
                            break;
    case meta::channel4   : curve[3]->setPen(QPen(color));
                            break;
    }
    replot();
}

void plot::updateViewportX(const int msecs)
{
    interval.setMaxValue(interval.minValue()+msecs);
    setAxisScale(QwtPlot::xBottom, interval.minValue(), interval.maxValue());
    replot();
}

void plot::updateViewportY(const double lower, const double upper)
{
    setAxisScale(QwtPlot::yLeft, lower, upper);
    replot();
}
