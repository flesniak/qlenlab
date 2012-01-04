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

#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <qwt/qwt_series_data.h>

class signaldata : public QwtSeriesData<QPointF>
{
public:
    signaldata();
    QPointF sample(size_t i) const;
    size_t size() const;
    QRectF boundingRect() const;

public slots:
    void append(const QPointF &pos);
    void clear();

private:
    QRectF cachedRect;
    QVector<QPointF> data;
};

#endif // SIGNALDATA_H
