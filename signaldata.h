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

#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <qwt_series_data.h>

#include "meta.h"

//class datawrapper;
class signaldata : public QwtSeriesData<QPointF>
{
public:
    signaldata();
    ~signaldata();
    void    append(const double value);
    QRectF  boundingRect() const;
    void    clear();
    const double* constRawData() const;
    signaldata* getFft() const;
    double  getInterval() const;
    int     getTriggerOffset() const;
    void    inheritTriggerOffset(const int offset);
    double* rawData(unsigned int size = 0);
    size_t  rawSize() const;
    void    reserve(unsigned int count);
    QPointF sample(size_t i) const;
    void    setFft(signaldata* fft);
    bool    setTrigger(const meta::triggermode mode, const double triggerValue, const double tolerance = 0);
    void    setInterval(const double interval);
    size_t  size() const;
    void    smooth(float factor);

private:
    QVector<double> p_data;
    QRectF          p_boundingRect;
    signaldata*     p_fft;
    double          p_interval;
    int             p_offset;

    double average(unsigned int position, unsigned int valueCount);
};

//class datawrapper has the only purpose working around an uncomfortable QwtPlotCurve behaviour:
//using QwtPlotCurve::setData deletes the old data, thus we use this datawrapper to leave signaldata intact
//qwt 6.1.0 introduces QwtSeriesStore, which includes swapData that does not seem to delete old data
//however, qlenlab is designed to work with qwt 6.0.x, thus this class
class datawrapper : public QwtSeriesData<QPointF>
{
public:
    datawrapper(signaldata* data = 0);
    void setData(signaldata* data);
    signaldata* currentData() const;

    QPointF sample(size_t i) const;
    size_t size() const;
    QRectF boundingRect() const;

private:
    signaldata* p_data;
};

class bodedata : public QwtSeriesData<QPointF>
{
public:
    bodedata();
    void append(QPointF point);
    void setFreqs(unsigned int freqStart, unsigned int freqEnd);
    QPointF sample(size_t i) const;
    size_t size() const;
    QRectF boundingRect() const;

private:
    QVector<QPointF> p_data;
    unsigned int p_freqStart, p_freqEnd;
    double p_min, p_max;
};

//class fftdata : public QwtSeriesData<QPointF>
//{
//public:
//    fftdata();
//    void append(QPointF point);
//    void setFreqs(unsigned int freqStart, unsigned int freqEnd);
//    QPointF sample(size_t i) const;
//    size_t size() const;
//    QRectF boundingRect() const;

//private:
//    QVector<QPointF> p_data;
//    unsigned int p_freqStart, p_freqEnd;
//    double p_min, p_max;
//};

#endif // SIGNALDATA_H
