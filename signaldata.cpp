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

#include "signaldata.h"

signaldata::signaldata()
{
    d_boundingRect = QRectF(0.0,-1.0,0.0,-1.0);
    offset = 0;
}

signaldata::signaldata(const signaldata &d)
{
    d_boundingRect = d.boundingRect();
    offset = d.offset;
    p_data = QVector<QPointF>(d.p_data);
}

QPointF signaldata::sample(size_t i) const
{
    return QPointF(p_data.value(i).x(),p_data.value(i+offset).y());
}

size_t signaldata::size() const
{
    return p_data.size()-offset;
}

QRectF signaldata::boundingRect() const
{
    return d_boundingRect;
}

bool signaldata::setTrigger(meta::triggermode mode, double trigger, double tolerance)
{
    offset = 0;
    if( p_data.size() == 0 )
        return false;
    switch( mode ) {
    case meta::both : {
                      bool above = (p_data[offset].y() >= trigger);
                      while( (p_data[offset].y() <= trigger) != above ) {
                          offset++;
                          if( offset >= p_data.size() ) {
                              offset = 0;
                              return false;
                          }
                      }
                      return true;
                      break;
                      }
    case meta::rising : while( p_data[offset].y() > trigger-tolerance ) {
                            offset++;
                            if( offset >= p_data.size() ) {
                                offset = 0;
                                return false;
                            }
                        }
                        offset += 10; //Generic offset to prevent triggering on imprecise falling edges
                        while( p_data[offset].y() < trigger-tolerance ) {
                            offset++;
                            if( offset >= p_data.size() ) {
                                offset = 0;
                                return false;
                            }
                        }
                        return true;
                        break;
    case meta::falling : while( p_data[offset].y() < trigger+tolerance ) {
                             offset++;
                             if( offset >= p_data.size() ) {
                                 offset = 0;
                                 return false;
                             }
                         }
                         offset += 10; //Generic offset to prevent triggering on imprecise rising edges
                         while( p_data[offset].y() > trigger+tolerance ) {
                             offset++;
                             if( offset >= p_data.size() ) {
                                 offset = 0;
                                 return false;
                             }
                         }
                         return true;
                         break;
    default :            return false;
    }
}

void signaldata::append(const QPointF &pos)
{
    p_data.append(pos);
    if( !d_boundingRect.contains(pos) ) {
        if( pos.x() > d_boundingRect.right() )
            d_boundingRect.setRight(pos.x());
        if( pos.x() < d_boundingRect.left() )
            d_boundingRect.setLeft(pos.x());
        if( pos.y() > d_boundingRect.top() )
            d_boundingRect.setTop(pos.y());
        if( pos.y() < d_boundingRect.bottom() )
            d_boundingRect.setBottom(pos.y());
    }
}

void signaldata::clear()
{
    p_data.clear();
    d_boundingRect = QRectF(0.0,-1.0,0.0,-1.0);
    offset = 0;
}
