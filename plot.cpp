/***************************************************************************
 * Copyright (C) 2011-2012 Fabian Lesniak <fabian.lesniak@student.kit.edu> *
 *                         Max Bruckner   <max.bruckner@student.kit.edu>   *
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

#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>

#include "fftthread.h"
#include "plot.h"
#include "signaldata.h"
#include "storage.h"

plot::plot(meta::plotmode mode, storage *datastorage, QWidget *parent) : QwtPlot(parent), p_mode(mode), interval(0.0, 20.0), p_storage(datastorage), p_autoscale(false), p_autoscaleGrid(0.0), p_currentIndex(-2)
{
    plotLayout()->setAlignCanvasToScales(true);

    zoomer = new plotzoomer(canvas());
    zoomer->setTrackerMode(QwtPicker::AlwaysOn);
    zoomer->setRubberBand(QwtPicker::RectRubberBand);

    grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);

    switch(mode) {
    case meta::scope :
        setWindowTitle(tr("Plot"));
        setAxisTitle(QwtPlot::xBottom, tr("Zeit [ms]"));
        setAxisTitle(QwtPlot::yLeft, tr("Spannung [V]"));
        zoomer->setUnits("ms","V");
        break;
    case meta::fft :
        setWindowTitle(tr("Fourier-Analyse"));
        setAxisTitle(QwtPlot::xBottom, tr("Frequenz [Hz]"));
        setAxisTitle(QwtPlot::yLeft, tr("Spannung [V]"));
        zoomer->setUnits("Hz","V");
        break;
    default : qDebug() << "[plot] unhandled meta::plotmode";
    }

    p_datawrapper[0] = new datawrapper;
    p_datawrapper[1] = new datawrapper;
    p_datawrapper[2] = new datawrapper;
    p_datawrapper[3] = new datawrapper;

    curve[0] = new QwtPlotCurve(tr("Kanal 1"));
    curve[0]->setData(p_datawrapper[0]);
    curve[0]->attach(this);
    curve[1] = new QwtPlotCurve(tr("Kanal 2"));
    curve[1]->setData(p_datawrapper[1]);
    curve[1]->attach(this);
    curve[2] = new QwtPlotCurve(tr("Kanal 3"));
    curve[2]->setData(p_datawrapper[2]);
    curve[2]->attach(this);
    curve[3] = new QwtPlotCurve(tr("Kanal 4"));
    curve[3]->setData(p_datawrapper[3]);
    curve[3]->attach(this);

    if( p_mode == meta::fft ) {
        p_fftthread = new fftthread(this);
        connect(p_fftthread,SIGNAL(finished()),SLOT(showFftDataset()));
    }
}

plot::~plot()
{
    if( p_mode == meta::fft )
        delete p_fftthread; //calls destructor which stops fftthread in case its running
}

int plot::getCurrentIndex() const
{
    return p_currentIndex;
}

meta::plotmode plot::getMode() const
{
    return p_mode;
}

void plot::showDataset(const int index)
{
    p_currentIndex = index;
    p_dataset = p_storage->getDataset(index);
    switch(p_mode) {
    case meta::scope :
        p_datawrapper[0]->setData(p_dataset.channel[0]);
        p_datawrapper[1]->setData(p_dataset.channel[1]);
        p_datawrapper[2]->setData(p_dataset.channel[2]);
        p_datawrapper[3]->setData(p_dataset.channel[3]);
        break;
    case meta::fft :
        bool allFftsFound = true;
        for(unsigned int i = 0; i < 4; i++)
            if( p_dataset.channel[i]->size() && p_dataset.channel[i]->getFft() == 0 ) {
                allFftsFound = false;
                break;
            }
        if( !allFftsFound ) {
            p_fftthread->setDataset(p_dataset);
            p_fftthread->start(); //dataset will be shown when thread is finished
            return; //don't start drawing if ffts are not calculated yet
        } else {
            p_datawrapper[0]->setData(p_dataset.channel[0]->getFft());
            p_datawrapper[1]->setData(p_dataset.channel[1]->getFft());
            p_datawrapper[2]->setData(p_dataset.channel[2]->getFft());
            p_datawrapper[3]->setData(p_dataset.channel[3]->getFft());
        }
        break;
    }

    if( p_autoscale )
        autoscale();
    replot();
}

void plot::showFftDataset()
{
    p_dataset = p_fftthread->getDataset();
    p_datawrapper[0]->setData(p_dataset.channel[0]->getFft());
    p_datawrapper[1]->setData(p_dataset.channel[1]->getFft());
    p_datawrapper[2]->setData(p_dataset.channel[2]->getFft());
    p_datawrapper[3]->setData(p_dataset.channel[3]->getFft());

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
         QRectF rect = p_dataset.channel[i]->boundingRect();
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
                            color.setBlueF(1.0-color.blueF());
                            color.setRedF(1.0-color.redF());
                            color.setGreenF(1.0-color.greenF());
                            zoomer->setTrackerPen(QPen(color));
                            zoomer->setRubberBandPen(QPen(color));
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
    QRectF zoomBase = zoomer->zoomBase();
    zoomBase.setLeft(interval.minValue());
    zoomBase.setRight(interval.maxValue());
    zoomer->setZoomBase(zoomBase);
    replot();
}

void plot::updateViewportY(const double lower, const double upper)
{
    p_autoscale = false;
    setAxisScale(QwtPlot::yLeft, lower, upper);
    QRectF zoomBase = zoomer->zoomBase();
    zoomBase.setTop(upper);
    zoomBase.setBottom(lower);
    zoomer->setZoomBase(zoomBase);
    replot();
}

signaldata** plot::getCurrentData()
{
    return p_dataset.channel;
}

plotzoomer::plotzoomer(QwtPlotCanvas* canvas, bool doReplot) : QwtPlotZoomer(canvas, doReplot)
{
}

plotzoomer::plotzoomer(int xAxis, int yAxis, QwtPlotCanvas* canvas, bool doReplot) : QwtPlotZoomer(xAxis, yAxis, canvas, doReplot)
{
}

void plotzoomer::setUnits(const QString& hUnit, const QString& vUnit)
{
    p_hUnit = hUnit;
    p_vUnit = vUnit;
}

QwtText plotzoomer::trackerTextF(const QPointF& pos) const
{
    QString text;

    switch ( rubberBand() )
    {
        case HLineRubberBand:
            text.sprintf( "%.3f%s", pos.y(), p_vUnit.toStdString().c_str() );
            break;
        case VLineRubberBand:
            text.sprintf( "%.3f%s", pos.x(), p_hUnit.toStdString().c_str() );
            break;
        default:
            text.sprintf( "%.3f%s, %.3f%s", pos.x(), p_hUnit.toStdString().c_str(), pos.y(), p_vUnit.toStdString().c_str() );
    }
    return QwtText( text );
}
