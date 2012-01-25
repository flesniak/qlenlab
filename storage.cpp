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
    //p_datasetInterval = 1;
}

storage::~storage()
{
    while( datasets.size() > 0 )
        deleteDataset(0);
}

void storage::setPlotData(datawrapper *wrapper, meta::channel channel, int index)
{
    if( index == -1 )
        index = datasets.size()-1;

    signaldata *data = datasets.at(index).channel[chan2num(channel)];
    if( data != 0 ) {
        data->setParent(wrapper);
        wrapper->setData(data);
    }
    else
        wrapper->unsetData();
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
    if( index < (unsigned int)datasets.size() ) {
        dataset todelete = datasets.takeAt(index);
        delete todelete.timestamp;
        delete todelete.channel[0];
        delete todelete.channel[1];
        delete todelete.channel[2];
        delete todelete.channel[3];
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
        return "Nr. "+QString::number(index.row()+1)+" ("+datasets.at(index.row()).timestamp->toString("hh:mm:ss.zzz")+")";
    return QVariant();
}

Qt::ItemFlags storage::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled;
}

/*unsigned int storage::datasetInterval() const
{
    return p_datasetInterval;
}

void storage::setDatasetInterval(int interval)
{
    if( interval > 0 )
        p_datasetInterval = interval;
}*/
