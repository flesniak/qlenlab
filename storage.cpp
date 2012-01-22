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

#include "storage.h"
#include "signaldata.h"

#include <QTime>
#include <QVariant>

storage::storage(QObject *parent) : QAbstractListModel(parent)
{
    p_maximumDatasets = 1;
    p_emptySignaldata = new signaldata;
}

signaldata* storage::getPlotData(meta::channel channel, int index)
{
    if( index == -1 )
        index = datasets.size()-1;
    switch(channel) {
    case meta::ch1a : { signaldata *ch1data = datasets.at(index).channel1;
                      if( ch1data != 0 )
                          return ch1data;
                      else
                          return p_emptySignaldata;
                      break; }
    case meta::ch1b : { signaldata *ch2data = datasets.at(index).channel2;
                      if( ch2data != 0 )
                          return ch2data;
                      else
                          return p_emptySignaldata;
                      break; }
    case meta::ch2a : { signaldata *ch3data = datasets.at(index).channel3;
                      if( ch3data != 0 )
                          return ch3data;
                      else
                          return p_emptySignaldata;
                      break; }
    case meta::ch2b : { signaldata *ch4data = datasets.at(index).channel4;
                      if( ch4data != 0 )
                          return ch4data;
                      else
                          return p_emptySignaldata;
                      break; }
    default : return p_emptySignaldata;
    }
}

dataset storage::getDataset(unsigned int index)
{
    return datasets.at(index);
}

unsigned int storage::maximumDatasets() const
{
    return p_maximumDatasets;
}

void storage::setMaximumDatasets(int maximum)
{
    if( maximum > 0 )
        p_maximumDatasets = maximum;
}

void storage::appendDataset(dataset &newdataset)
{
    datasets.append(newdataset);
    deleteSpares();
    emit dataChanged(createIndex(0,0),createIndex(datasets.size()-1,0));
}

bool storage::deleteDataset(unsigned int index)
{
    qDebug() << "[storage] delete dataset" << index;
    if( index < (unsigned int)datasets.size() ) {
        //Don't we have to clean up?
        datasets.removeAt(index);
        return true;
    }
    else
        return false;
}

void storage::deleteSpares()
{
    while( (unsigned int)datasets.size() > p_maximumDatasets )
        deleteDataset(0);
}

int storage::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return datasets.size();
}

QVariant storage::data(const QModelIndex &index, int role) const
{
    if( index.column() != 0 )
        return QVariant();
    if( role == Qt::DisplayRole && index.row() < datasets.size() )
        return "Nr. "+QString::number(index.row()+1)+" ("+datasets.at(index.row()).timestamp->toString("hh:mm:ss")+")";
    return QVariant();
}

Qt::ItemFlags storage::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled;
}
