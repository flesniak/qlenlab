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
#include "signaldata.h"

#include <qwt_plot_renderer.h>
#include <qwt_plot.h>
#include <QSizeF>
#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QErrorMessage>
#include <QDebug>

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
    comboBox_format->addItem("csv");
    
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
    connect(comboBox_format, SIGNAL(currentIndexChanged(int)),SLOT(updateGUI()));
    
    updateGUI();
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
	{
		QMessageBox::warning(this,tr("Fehler"),tr("Sie müssen einen Plot auswählen!"));
		return;
	}
	
	if(comboBox_format->currentText() == "csv")
	{
		exportCSV(filename);
	}
	else
	{
		renderer->renderDocument(plot_export, filename, comboBox_format->currentText(), *new QSizeF(spinBox_width->value(),spinBox_height->value()), 300);
	}
}

void exportdialog::setBode(bodeplot *newBode)
{
	bode = newBode;
	
	updateGUI();
		
}

void exportdialog::exportCSV(QString filename)
{
	QFile csvFile(filename);
		if(csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
		{		
			QTextStream outstream(&csvFile);

            p_data = mainPlot->getCurrentData();
			
			//Write first line
			bool first = false;
			if(p_data[0]->size())
			{	
				outstream << "Zeit [ms],Kanal 1 [V]";
				first = true;
			}
			if(p_data[1]->size())
			{
				if(first)
					outstream << ",";
				else
				{
					first = true;
					outstream << "Zeit [ms],";
				}
				outstream << "Kanal 2 [V]";
			}
			if(p_data[2]->size())
			{
				if(first)
					outstream << ",";
				else
				{
					first = true;
					outstream << "Zeit [ms],";
				}
				outstream << "Kanal 3 [V]";
			}
			if(p_data[3]->size())
			{
				if(first)
					outstream << ",";
				else
					outstream << "Zeit [ms],";
				outstream << "Kanal 4 [V]";
			}
			outstream << "\n";
			
			//Write csv
			for(unsigned int i = 0; (i < p_data[0]->size()) || (i < p_data[1]->size()) || (i < p_data[2]->size()) || (i < p_data[3]->size()); i++)
			{
				first = false;
				if(p_data[0]->size())
				{
					first = true;	
					outstream << p_data[0]->sample(i).x() << "," << p_data[0]->sample(i).y();
				}
				if(p_data[1]->size())
				{
					if(first)
						outstream << ",";
					else
					{
						first = true;
						outstream << p_data[1]->sample(i).x() << ",";
					}
					outstream << p_data[1]->sample(i).y();
				}
				if(p_data[2]->size())
				{
					if(first)
						outstream << ",";
					else
					{
						first = true;
						outstream << p_data[2]->sample(i).x() << ",";
					}
					outstream << p_data[2]->sample(i).y();
				}
				if(p_data[3]->size())
				{
					if(first)
						outstream << ",";
					else
						outstream << p_data[1]->sample(i).x() << ",";
					outstream << p_data[3]->sample(i).y();
				}
				outstream << "\n";
			}
		}
		else
		{
			QMessageBox::warning(this, tr("Schreibfehler"),
                                tr("Exportieren fehlgeschlagen!"));
		}
}

void exportdialog::updateGUI()
{
	if(bode == NULL)
	{
		radioButton_bode->setCheckable(false);
		radioButton_plot->setChecked(true);
	}
	else
		radioButton_bode->setCheckable(true);

	if(comboBox_format->currentText() == "csv")
	{
		spinBox_height->setReadOnly(true);
		spinBox_width->setReadOnly(true);
		spinBox_res->setReadOnly(true);
	}
	else
	{
		spinBox_height->setReadOnly(false);
		spinBox_width->setReadOnly(false);
		spinBox_res->setReadOnly(false);
	}
}
