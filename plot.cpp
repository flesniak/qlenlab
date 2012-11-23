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
#include <qwt/qwt_plot_zoomer.h>

#include "plot.h"
#include "storage.h"
#include "signaldata.h"

plot::plot(storage *datastorage, QWidget *parent) : QwtPlot(parent), interval(0.0, 20.0), p_storage(datastorage), p_autoscale(false), p_autoscaleGrid(0.0)
{
    setAxisTitle(QwtPlot::xBottom, tr("Zeit [ms]"));
    setAxisTitle(QwtPlot::yLeft, tr("Spannung [V]"));
    plotLayout()->setAlignCanvasToScales(true);

    zoomer = new QwtPlotZoomer(canvas());

    grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);

    p_data[0] = new datawrapper;
    p_data[1] = new datawrapper;
    p_data[2] = new datawrapper;
    p_data[3] = new datawrapper;

    curve[0] = new QwtPlotCurve(tr("Kanal 1"));
    curve[0]->setData(p_data[0]);
    curve[0]->attach(this);
    curve[1] = new QwtPlotCurve(tr("Kanal 2"));
    curve[1]->setData(p_data[1]);
    curve[1]->attach(this);
    curve[2] = new QwtPlotCurve(tr("Kanal 3"));
    curve[2]->setData(p_data[2]);
    curve[2]->attach(this);
    curve[3] = new QwtPlotCurve(tr("Kanal 4"));
    curve[3]->setData(p_data[3]);
    curve[3]->attach(this);
}

void plot::showDataset(const int index)
{
    p_storage->setPlotData(p_data[0],meta::ch1a,index);
    p_storage->setPlotData(p_data[1],meta::ch1b,index);
    p_storage->setPlotData(p_data[2],meta::ch2a,index);
    p_storage->setPlotData(p_data[3],meta::ch2b,index);
    if( p_autoscale )
        autoscale();
    replot();
}

void plot::setYAutoscaleGrid(const double grid)
{
    p_autoscaleGrid = grid;
    replot();
}

void plot::setYAutoscale(bool on)
{
    p_autoscale = on;
    replot();
}

void plot::autoscale()
{
    double lower = 0, upper = 0;
    for(int i=0;i<4;i++) {
         QRectF rect = p_data[i]->boundingRect();
         if( rect.bottom() < lower )
             lower = rect.bottom();
         if( rect.top() > upper )
             upper = rect.top();
         if( p_autoscaleGrid > 0 ) {
             lower = floor(lower/p_autoscaleGrid)*p_autoscaleGrid;
             upper = ceil(upper/p_autoscaleGrid)*p_autoscaleGrid;
         }
    }
    setAxisScale(QwtPlot::yLeft, lower, upper);
}

void plot::updateColor(meta::colorindex ci, QColor color)
{
    switch( ci ) {
    case meta::background : //canvas()->setPalette(QPalette(QPalette::Base,color));
                            setCanvasBackground(color);
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
    p_autoscale = false;
    setAxisScale(QwtPlot::yLeft, lower, upper);
    replot();
}
