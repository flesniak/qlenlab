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

#ifndef PLOT_H
#define PLOT_H

#include "meta.h"

#include <qwt_interval.h>
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_zoomer.h>

class storage;
class signaldata;
class datawrapper;
class fftthread;
class QColor;
class QwtPlotCurve;
class QwtPlotGrid;
class plotzoomer;

class plot : public QwtPlot
{
    Q_OBJECT
public:
    explicit plot(meta::plotmode mode, storage *datastorage, QWidget *parent = 0);
    virtual ~plot();
    meta::plotmode getMode() const;
    int getCurrentIndex() const;

private:
    meta::plotmode p_mode;
    QwtInterval interval;
    QwtPlotCurve* curve[4];
    QwtPlotGrid* grid;
    plotzoomer* zoomer;
    storage* p_storage;
    dataset* p_dataset;
    datawrapper* p_datawrapper[4];
    fftthread* p_fftthread;
    bool p_autoscale;
    double p_autoscaleGrid;
    int p_currentIndex;

    void autoscale();

public slots:
    void updateViewportX(const int msecs);
    void updateViewportY(const double lower, const double upper);
    void updateColor(meta::colorindex ci, QColor color);
    void setYAutoscale(bool on);
    void setYAutoscaleGrid(const double grid = 0);
    void showDataset(const int index = -1);
    void showFftDataset();
    signaldata** getCurrentData();
};

class plotzoomer : public QwtPlotZoomer
{
    Q_OBJECT
public:
    plotzoomer(QWidget* canvas, bool doReplot = true);
    plotzoomer(int xAxis, int yAxis, QWidget* canvas, bool doReplot = true);

public slots:
    void setUnits(const QString& hUnit, const QString& vUnit);

protected:
    virtual QwtText trackerTextF(const QPointF& pos) const;

private:
    QString p_hUnit;
    QString p_vUnit;
};

#endif // PLOT_H
