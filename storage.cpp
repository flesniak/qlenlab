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
    deleteDataset(-1); //delete all datasets
}

signaldata* storage::getData(meta::channel channel, int index)
{
    return getData(chan2num(channel),index);
}

signaldata* storage::getData(unsigned char channel, int index)
{
    if( index == -1 )
        index = datasets.size()-1;
    return datasets.at(index)->channel[channel];
}

dataset* storage::getDataset(int index)
{
    if( index == -1 )
        index = datasets.size()-1;
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

void storage::appendDataset(dataset* newdataset)
{
    datasets.append(newdataset);
    deleteSpares();
    emit dataChanged(createIndex(0,0),createIndex(datasets.size()-1,0));
}

bool storage::deleteDataset(int index)
{
    if( index < 0 ) {
        while( datasets.size() > 0 )
            deleteDataset(0);
        return true;
    }

    if( index < datasets.size() ) {
        dataset* todelete = datasets.at(index);
        if( todelete->flags )
            return deleteDataset(index+1); //we are not allowed to delete this one, try next
        datasets.removeAt(index);
        delete todelete->timestamp;
        delete todelete->channel[0];
        delete todelete->channel[1];
        delete todelete->channel[2];
        delete todelete->channel[3];
        delete todelete;
        return true;
    }
    else
        return false;
}

void storage::deleteSpares()
{
    for(unsigned int i = datasets.size(); i > p_maximumDatasets; i--)
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
        return QString::number(index.row()+1).rightJustified(2,'0')+(datasets.at(index.row())->flags ? "p " : " ")+"("+datasets.at(index.row())->timestamp->toString("hh:mm:ss.zzz")+")";
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
