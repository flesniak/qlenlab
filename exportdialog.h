/***************************************************************************
 * Copyright (C) 2011-2012 Fabian Lesniak <fabian.lesniak@student.kit.edu> *
 *                         Max Bruckner   <max.bruckner@student.kit.edu>   *
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

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QtGui>

#include "meta.h"

class plot;
class bodeplot;
class QwtPlotRenderer;
class QwtPlot;

class exportdialog : public QDialog
{
    Q_OBJECT

public:
    explicit exportdialog(plot *plot, bodeplot *bodeplot, QWidget *parent = 0);
    ~exportdialog();

public slots:
    int exec();
    void setBode(bodeplot *newBode);
    
private:
	QwtPlotRenderer *renderer;
	QwtPlot *plot_export;
	
	plot *mainPlot;
	bodeplot *bode;
	
	QComboBox *comboBox_format;
	QSpinBox *spinBox_height;
	QSpinBox *spinBox_width;
	QSpinBox *spinBox_res;
	QPushButton *pushButton_export;
	QRadioButton *radioButton_plot;
	QRadioButton *radioButton_bode;

private slots:
	void exportPlot();

};

#endif // EXPORTDIALOG_H
