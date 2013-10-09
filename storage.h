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

#ifndef STORAGE_H
#define STORAGE_H

#include <QList>
#include <QAbstractListModel>

#include "meta.h"

class signaldata;
class QVariant;

class storage : public QAbstractListModel
{
    Q_OBJECT
public:
    storage(QObject *parent = 0);
    ~storage();

    void appendDataset(dataset &newdataset);
    dataset getDataset(int index);
    bool deleteDataset(int index);
    signaldata* getData(meta::channel channel, int index = -1);
    signaldata* getData(unsigned char channel, int index = -1);

    unsigned int maximumDatasets() const;
    unsigned int datasetInterval() const;

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

public slots:
    void setMaximumDatasets(int maximum);
    //void setDatasetInterval(int interval);

private:
    QList<dataset> datasets;
    unsigned int p_maximumDatasets;
    //unsigned int p_datasetInterval;
    void deleteSpares();
};

#endif // STORAGE_H
