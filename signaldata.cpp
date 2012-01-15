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

#include "signaldata.h"

signaldata::signaldata() : cachedRect(0.0,0.0,0.0,0.0)
{
    offset = 0;
}

QPointF signaldata::sample(size_t i) const
{
    return QPointF(data.value(i).x(),data.value(i+offset).y());
}

size_t signaldata::size() const
{
    return data.size()-offset;
}

QRectF signaldata::boundingRect() const
{
    return cachedRect;
}

bool signaldata::setTrigger(meta::triggermode mode, double trigger, double tolerance)
{
    offset = 0;
    if( data.size() == 0 )
        return false;
    switch( mode ) {
    case meta::both : {
                      bool above = (data[offset].y() >= trigger);
                      while( (data[offset].y() <= trigger) != above ) {
                          offset++;
                          if( offset >= data.size() ) {
                              offset = 0;
                              return false;
                          }
                      }
                      return true;
                      break;
                      }
    case meta::rising : while( data[offset].y() > trigger-tolerance ) {
                            offset++;
                            if( offset >= data.size() ) {
                                offset = 0;
                                return false;
                            }
                        }
                        while( data[offset].y() < trigger-tolerance ) {
                            offset++;
                            if( offset >= data.size() ) {
                                offset = 0;
                                return false;
                            }
                        }
                        return true;
                        break;
    case meta::falling : while( data[offset].y() < trigger+tolerance ) {
                             offset++;
                             if( offset >= data.size() ) {
                                 offset = 0;
                                 return false;
                             }
                         }
                         while( data[offset].y() > trigger+tolerance ) {
                             offset++;
                             if( offset >= data.size() ) {
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
    data.append(pos);
    if( !cachedRect.contains(pos) ) {
        if( pos.y() > 0 )
            cachedRect.setTop(pos.y());
        else
            cachedRect.setBottom(pos.y());
    }
}

void signaldata::clear()
{
    data.clear();
    cachedRect = QRectF(0.0,0.0,0.0,0.0);
}
