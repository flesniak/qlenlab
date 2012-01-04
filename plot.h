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

#ifndef PLOT_H
#define PLOT_H

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_interval.h>

class communicator;

class Plot : public QwtPlot
{
    Q_OBJECT
public:
    explicit Plot(communicator* data, QWidget *parent = 0);

private:
    QwtInterval interval;
    QwtPlotCurve* curve[4];
    int timerId;
    communicator* com;

protected:
    void timerEvent(QTimerEvent *event);

public slots:
    void updateViewportX(const int msecs);
    void updateViewportY(const double lower, const double upper);
    void start();
    void stop();
    void setData();
};

#endif // PLOT_H
