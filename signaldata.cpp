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

// ########## DATAWRAPPER ##########

datawrapper::datawrapper()
{
    p_nulldata = new signaldata;
    p_data = p_nulldata;
}

QPointF datawrapper::sample(size_t i) const
{
    return QPointF(i*p_data->p_interval,p_data->p_data.value(i+p_data->p_offset));
}

size_t datawrapper::size() const
{
    return p_data->p_data.size()-p_data->p_offset;
}

QRectF datawrapper::boundingRect() const
{
    return p_data->p_boundingRect;
}

void datawrapper::setData(signaldata *data)
{
    p_data = data;
}

signaldata* datawrapper::currentData() const
{
    return p_data;
}

void datawrapper::unsetData()
{
    p_data = p_nulldata;
}

// ########## SIGNALDATA ##########


signaldata::signaldata(datawrapper *parent) : p_parent(parent), p_boundingRect(0.0,0.0,0.0,0.0)
{
    p_offset = 0;
    p_interval = 0;
}

signaldata::~signaldata()
{
    if( p_parent != 0 )
        if( p_parent->currentData() == this )
            p_parent->unsetData();
}

void signaldata::setParent(datawrapper *parent)
{
    p_parent = parent;
}

void signaldata::smooth(float factor)
{
    if( factor == 0 )
        return;
    for(int value=1; value < p_data.size(); value++) {
        p_data[value]=(1-factor)*p_data[value]+factor*p_data[value-1];
    }
}

int signaldata::getTriggerOffset() const
{
    return p_offset;
}

void signaldata::inheritTriggerOffset(const int offset)
{
    if( offset >= p_data.size() )
        p_offset = 0;
    else
        p_offset = offset;
}

bool signaldata::setTrigger(const meta::triggermode mode, const double trigger, const double tolerance)
{
    p_offset = 0;
    if( p_data.size() == 0 )
        return false;
    const unsigned int minimumPoints = 20; //trigger requires 10 following points to be greater trigger level
    switch( mode ) {
    case meta::both : {
                      bool above = (p_data[p_offset] >= trigger);
                      while( (p_data[p_offset] <= trigger) != above ) {
                          p_offset++;
                          if( p_offset >= p_data.size() ) {
                              p_offset = 0;
                              return false;
                          }
                      }
                      return true;
                      break;
                      }
    case meta::rising : {
                            /* OLD ALGORITHM
                            while( p_data[p_offset] > trigger-tolerance ) { //search from below trigger level
                                p_offset++;
                                if( p_offset >= p_data.size() ) {
                                    p_offset = 0;
                                    return false;
                                }
                            }
                            p_offset += 5; //Generic p_offset to prevent triggering on imprecise rising edges
                            while( p_data[p_offset] < trigger+tolerance ) {
                                p_offset++;
                                if( p_offset >= p_data.size() ) {
                                    p_offset = 0;
                                    return false;
                                }
                            }
                            return true; */
                            //NEW ALGORITHM - that shall work better...
                            while( p_data[p_offset] > trigger-tolerance ) { //in case we are already above or on the trigger level, advance until we are not anymore
                                p_offset++;
                                if( p_offset >= p_data.size() ) {
                                    p_offset = 0;
                                    return false;
                                }
                            } //now we are under the trigger level
                            unsigned int satisfyingPoints;
                            do {
                                satisfyingPoints = 0;
                                while( p_data[p_offset] < trigger ) { //search until we pass our trigger level
                                    p_offset++;
                                    if( p_offset >= p_data.size() ) {
                                        p_offset = 0;
                                        return false;
                                    }
                                }
                                while( p_data[p_offset] >= trigger ) { //count how many points are over the trigger level
                                    p_offset++;
                                    satisfyingPoints++;
                                    if( p_offset >= p_data.size() )
                                        break;
                                }
                            } while( satisfyingPoints < minimumPoints ); //loop while we dont have enough
                            p_offset -= satisfyingPoints; //we have found a p_offset at which minimumPoints points are already over our trigger level, to lets calculate the right offset
                            break;
                         }
    case meta::falling : {
                            /* OLD ALGORITHM
                            while( p_data[p_offset] < trigger+tolerance ) { //search from below trigger level
                                p_offset++;
                                if( p_offset >= p_data.size() ) {
                                    p_offset = 0;
                                    return false;
                                }
                            }
                            p_offset += 5; //Generic p_offset to prevent triggering on imprecise rising edges
                            while( p_data[p_offset] > trigger-tolerance ) {
                                p_offset++;
                                if( p_offset >= p_data.size() ) {
                                    p_offset = 0;
                                    return false;
                                }
                            }
                            return true; */
                            //NEW ALGORITHM - that shall work better...
                            while( p_data[p_offset] < trigger+tolerance ) { //in case we are already above or on the trigger level, advance until we are not anymore
                                p_offset++;
                                if( p_offset >= p_data.size() ) {
                                    p_offset = 0;
                                    return false;
                                }
                            } //now we are under the trigger level
                            unsigned int satisfyingPoints;
                            do {
                                satisfyingPoints = 0;
                                while( p_data[p_offset] > trigger ) { //search until we pass our trigger level
                                    p_offset++;
                                    if( p_offset >= p_data.size() ) {
                                        p_offset = 0;
                                        return false;
                                    }
                                }
                                while( p_data[p_offset] <= trigger ) { //count how many points are over the trigger level
                                    p_offset++;
                                    satisfyingPoints++;
                                    if( p_offset >= p_data.size() )
                                        break;
                                }
                            } while( satisfyingPoints < minimumPoints ); //loop while we dont have enough
                            p_offset -= satisfyingPoints; //we have found a p_offset at which minimumPoints points are already over our trigger level, to lets calculate the right offset
                            break;
                         }
    default :            p_offset = 0;
                         return false;
    }
    return false;
}

double signaldata::average(unsigned int position, unsigned int valueCount)
{
    /*if( (unsigned int)p_data.size() < valueCount )
        valueCount = p_data.size();*/
    int values = valueCount;
    double result = 0;
    while( values > 0 ) {
        result += p_data.at(position-values+1);
        values--;
    }
    result /= valueCount;
    return result;
}

void signaldata::setTimeInterval(const double interval)
{
    p_interval = interval;
}

void signaldata::append(const double voltage)
{
    p_data.append(voltage);
    if( voltage > p_boundingRect.top() )
        p_boundingRect.setTop(voltage);
    if( voltage < p_boundingRect.bottom() )
        p_boundingRect.setBottom(voltage);
}

int signaldata::size()
{
    return p_data.size();
}

void signaldata::clear()
{
    p_data.clear();
    p_boundingRect = QRectF(0.0,0.0,0.0,0.0);
    p_offset = 0;
    p_interval = 0;
}

// ########## BODEDATA ##########
bodedata::bodedata()
{
    d_boundingRect = QRectF(0,-1,0,-1);
    p_freqStart = 0;
    p_freqEnd = 0;
    p_min = 0;
    p_max = 0;
}

void bodedata::append(QPointF point)
{
    p_data.append(point);
    if( point.y() > p_max )
        p_max = point.y();
    if( point.y() < p_min )
        p_min = point.y();
}

QPointF bodedata::sample(size_t i) const
{
    return p_data.at(i);
}

size_t bodedata::size() const
{
    return p_data.size();
}

QRectF bodedata::boundingRect() const
{
    d_boundingRect.setCoords(p_freqStart,p_max,p_freqEnd,p_min);
    return d_boundingRect;
}

void bodedata::setFreqs(unsigned int freqStart, unsigned int freqEnd)
{
    p_freqStart = freqStart;
    p_freqEnd = freqEnd;
}
