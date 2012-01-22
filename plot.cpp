/***************************************************************************
 * Copyright (C) 2011-2012 Fabian Lesniak <fabian.lesniak@student.kit.edu> *
 *                                                                         *
 * This file is part of the QLenLab project.                               *
 *                                                                         *
 * QLenLab is free software: you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * QLenLab is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 * for more details.                                                       *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with QLenLab. If not, see <http://www.gnu.org/licenses/>.               *
 **************************************************************************/

#include <QPalette>
#include <QTimerEvent>
#include <QPen>
#include <QColor>

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_plot_curve.h>

#include "plot.h"
#include "storage.h"
#include "signaldata.h"

plot::plot(storage *datastorage, QWidget *parent) : QwtPlot(parent), interval(0.0, 20.0), p_storage(datastorage)
{
    setAxisTitle(QwtPlot::xBottom, tr("Zeit [ms]"));
    setAxisTitle(QwtPlot::yLeft, tr("Spannung [V]"));
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
    curve[0]->attach(this);
    curve[1] = new QwtPlotCurve(tr("Kanal 2"));
    curve[1]->attach(this);
    curve[2] = new QwtPlotCurve(tr("Kanal 3"));
    curve[2]->attach(this);
    curve[3] = new QwtPlotCurve(tr("Kanal 4"));
    curve[3]->attach(this);
}

void plot::showDataset(int index)
{
    qDebug() << "[plot] curve 0 pointer" << p_storage->getPlotData(meta::ch1a,index);
    qDebug() << "[plot] curve 0 data size" << p_storage->getPlotData(meta::ch1a,index)->size();
    qDebug() << "[plot] curve 0 pointer unref 1" << p_storage->getPlotData(meta::ch1a,index)->sample(1);
    qDebug() << "[plot] curve 0 pointer unref last" << p_storage->getPlotData(meta::ch1a,index)->sample(p_storage->getPlotData(meta::ch1a,index)->size()-1);
    //curve[0]->setData(p_storage->getPlotData(meta::ch1a,index));
    /*curve[1]->setData(p_storage->getPlotData(meta::ch1b,index));
    curve[2]->setData(p_storage->getPlotData(meta::ch2a,index));
    curve[3]->setData(p_storage->getPlotData(meta::ch2b,index));*/
    replot();
}

void plot::setYAutoscale(bool on)
{
    qDebug() << "[plot] set y autoscale" << on;
    setAxisAutoScale(QwtPlot::yLeft,on);
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
