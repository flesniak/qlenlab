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

class datawrapper;
class signaldata
{
public:
    signaldata(datawrapper *parent = 0);
    ~signaldata();
    void append(const double voltage);
    int getTriggerOffset() const;
    void inheritTriggerOffset(const int offset);
    bool setTrigger(const meta::triggermode mode, const double trigger, const double tolerance = 0);
    void setParent(datawrapper *parent = 0);
    void setTimeInterval(const double interval);
    void smooth(float factor);
    void clear();
    int size();

    friend class datawrapper;

private:
    QVector<double> p_data;
    int p_offset;
    double p_interval;
    datawrapper* p_parent;
    QRectF p_boundingRect;

    double average(unsigned int position, unsigned int valueCount);
};

class datawrapper : public QwtSeriesData<QPointF>
{
public:
    datawrapper();
    void setData(signaldata* data);
    void unsetData();
    signaldata* currentData() const;

    QPointF sample(size_t i) const;
    size_t size() const;
    QRectF boundingRect() const;

private:
    signaldata* p_data;
    signaldata* p_nulldata;
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

#endif // SIGNALDATA_H
