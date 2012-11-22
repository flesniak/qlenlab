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

#include "exportdialog.h"
#include "plot.h"
#include "bodeplot.h"

#include <qwt/qwt_plot_renderer.h>
#include <qwt/qwt_plot.h>
#include <QSizeF>
#include <QFileDialog>

exportdialog::exportdialog(plot *inputplot, bodeplot* bodeplot, QWidget *parent) : QDialog(parent)
{
    bode = bodeplot;
    mainPlot = inputplot;
    
    setWindowTitle(tr("Messung exportieren"));
	
    renderer = new QwtPlotRenderer(this);
    
    QGroupBox *box_format = new QGroupBox(this);
    box_format->setTitle(tr("Dateiformat"));
    comboBox_format = new QComboBox(box_format);
    comboBox_format->addItem("pdf");
    comboBox_format->addItem("svg");
    comboBox_format->addItem("ps");
    comboBox_format->addItem("png");
    comboBox_format->addItem("bmp");
    comboBox_format->addItem("jpg");
    comboBox_format->addItem("ppm");
    comboBox_format->addItem("tiff");
    comboBox_format->addItem("xpm");
    
    QVBoxLayout *layout_format = new QVBoxLayout(box_format);
    layout_format->addWidget(comboBox_format);
    
    QGroupBox *box_size = new QGroupBox(this);
    box_size->setTitle(tr("Größe"));
    QLabel *label_width = new QLabel("Breite:",box_size);
    spinBox_width = new QSpinBox(box_size);
    spinBox_width->setRange(10,1000);
    spinBox_width->setValue(200);
    QLabel *label_width_unit = new QLabel("mm",box_size);
    QLabel *label_height = new QLabel("Höhe:",box_size);
    spinBox_height = new QSpinBox(box_size);
    spinBox_height->setRange(10,1000);
    spinBox_height->setValue(200);
    QLabel *label_height_unit = new QLabel("mm",box_size);
    
    QGridLayout *layout_size = new QGridLayout(box_size);
    layout_size->addWidget(label_width,0,0);
    layout_size->addWidget(spinBox_width,0,1);
    layout_size->addWidget(label_width_unit,0,2);
    layout_size->addWidget(label_height,1,0);
    layout_size->addWidget(spinBox_height,1,1);
    layout_size->addWidget(label_height_unit,1,2);
    
    QGroupBox *box_res = new QGroupBox(this);
    box_res->setTitle(tr("Auflösung"));
    spinBox_res = new QSpinBox(box_res);
    spinBox_res->setRange(60,4800);
    spinBox_res->setValue(300);
    QLabel *label_res_unit = new QLabel("dpi",box_res);
    
    QHBoxLayout *layout_res = new QHBoxLayout(box_res);
    layout_res->addWidget(spinBox_res);
    layout_res->addWidget(label_res_unit);
    
    QGroupBox *box_select = new QGroupBox(this);
    radioButton_plot = new QRadioButton("Plot",box_select);
    radioButton_plot->setChecked(true);
    radioButton_bode = new QRadioButton("Bode-Plot",box_select);
    
    QHBoxLayout *layout_select = new QHBoxLayout(box_select);
    layout_select->addWidget(radioButton_plot);
    layout_select->addWidget(radioButton_bode);
    
    pushButton_export = new QPushButton("Exportieren",this);
    
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(box_format,0,0);
    layout->addWidget(box_size,1,0,1,2,0);
    layout->addWidget(box_res,0,1);
    layout->addWidget(box_select,2,0,1,2,0);
    layout->addWidget(pushButton_export,3,0,1,2,0);
    
    connect(pushButton_export,SIGNAL(clicked()),SLOT(exportPlot()));
    
    if(bode == NULL)
	{
		radioButton_bode->setCheckable(false);
		radioButton_plot->setChecked(true);
	}
	else
		radioButton_bode->setCheckable(true);
}

exportdialog::~exportdialog()
{
}

int exportdialog::exec()
{
    return QDialog::exec();
}

void exportdialog::exportPlot()
{
	QString filename = QFileDialog::getSaveFileName(this,tr("Messung exportieren"),"~/");
	if(filename == "")
		return;
		
	if(radioButton_plot->isChecked())
		plot_export = mainPlot;
	else if(radioButton_bode->isChecked())
		plot_export = bode;
	else
		return;
	
	
	renderer->renderDocument(plot_export, filename, comboBox_format->currentText(), *new QSizeF(spinBox_width->value(),spinBox_height->value()), 300);
}

void exportdialog::setBode(bodeplot *newBode)
{
	bode = newBode;
	
	if(bode == NULL)
	{
		radioButton_bode->setCheckable(false);
		radioButton_plot->setChecked(true);
	}
	else
		radioButton_bode->setCheckable(true);
}
