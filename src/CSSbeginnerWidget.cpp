/**     @file CSSbeginnerWidget.cpp

	This file is part of the Kimberlite project
	Copyright (C) 2008 Emmanuel Lepage Vallee <elv1313@gmail.com>
	
	This software is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 or 3 of the License, or (at your option) any later version.

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public License
	along with this library; see the file COPYING.LIB.  If not, write to
	the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
	Boston, MA 02111-1307, USA.


        @author Emmanuel Lepage Vallée
        @date 13 August 2008
        @version 0.1-pre-alpah0
*/
#include "CSSbeginnerWidget.h"
#include "../mainwindow.h"
#include <QStringList>
#include <QtSql>
#include <QLineEdit>

CSSBeginnerWidget::CSSBeginnerWidget(QWidget* parent, QString tagName) : QWidget (parent) {
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    //sizePolicy.setHeightForWidth(tabWMenu->sizePolicy().hasHeightForWidth());

    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    //sizePolicy1.setHeightForWidth(cbbFontSize->sizePolicy().hasHeightForWidth());
    //setStyleSheet("border:0px;margin:0px;spacing:0px;padding:0px;");
    
    hlCbbTag = new QHBoxLayout(this);
    hlCbbTag->setObjectName(QString::fromUtf8("hlCbbTag"));
    hlCbbTag->setContentsMargins(0,0,0,0);
    ckbName = new QCheckBox(this);
    ckbName->setObjectName(QString::fromUtf8("ckbName"));
    QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(ckbName->sizePolicy().hasHeightForWidth());
    ckbName->setSizePolicy(sizePolicy4);
    ckbName->setMinimumSize(QSize(160, 0));
    ckbName->setMaximumSize(QSize(160, 999999));


    hlCbbTag->addWidget(ckbName);

    cbbValue = new QComboBox(this);
    cbbValue->setObjectName(QString::fromUtf8("cbbValue"));
    sizePolicy.setHeightForWidth(cbbValue->sizePolicy().hasHeightForWidth());
    cbbValue->setSizePolicy(sizePolicy);
    hlCbbTag->addWidget(cbbValue);

    cbbUnit = new QComboBox(this);
    cbbUnit->setMinimumSize(QSize(75, 0));
    cbbUnit->setObjectName(QString::fromUtf8("cbbUnit"));
    sizePolicy1.setHeightForWidth(cbbUnit->sizePolicy().hasHeightForWidth());
    cbbUnit->setSizePolicy(sizePolicy1);

    hlCbbTag->addWidget(cbbUnit);

    QSqlQuery query;
    query.exec("SELECT VALUE,UNIT FROM TCSS_TAG WHERE TITLE = '"+ tagName +"'");
    
    while (query.next()) {
      QStringList possibleValues = query.value(0).toString().split(";");
      for (int i =0;i < possibleValues.count(); i++) {
	if (possibleValues[i].left(4) == "UNIT") {
	  cbbValue->addItem(possibleValues[i].remove(0,4));
	  //cbbValue->setEditable(true);
	  isEditable.push_back(true);
	}
	else {
	  cbbValue->addItem(possibleValues[i]);
	  isEditable.push_back(false);
	}
      }
      QStringList possibleUnit = query.value(1).toString().split(";");
      for (int i =0;i < possibleUnit.count(); i++) {
	if (possibleUnit[i].isEmpty() == false)
	  cbbUnit->addItem(possibleUnit[i]);
      }
      //cbbUnit->addItem(query.value(1).toString());
    }
    ckbName->setText(tagName);
    //setCentralLayout(hlCbbTag);
    cbbUnit->setEnabled(false);
    cbbValue->setEnabled(false);
    connect( ckbName, SIGNAL( stateChanged(int) ), this, SLOT( ckbName_checked(int) ) );
    connect( cbbValue, SIGNAL( currentIndexChanged(int) ), this, SLOT( cbbValue_changed(int) ) );
}

void CSSBeginnerWidget::ckbName_checked(int state) {
  if (state == Qt::Checked) {
    cbbValue->setEnabled(true);
    if ((cbbUnit->count() != 0) && (isEditable[cbbValue->currentIndex()] == true)) {
      cbbUnit->setEnabled(true);
      cbbValue->setEditable(true);
      cbbValue->clearEditText();
    }
      cbbValue->setFocus();
    //cbbValue_changed(cbbValue->currentIndex());
  }
  else if (state == Qt::Unchecked) {
    cbbUnit->setEnabled(false);
    cbbValue->setEnabled(false);
  }
}

void CSSBeginnerWidget::cbbValue_changed(int index) {
  if (isEditable[index] == true) {
    cbbValue->setEditable(true);
    if (cbbUnit->count() != 0)
      cbbUnit->setEnabled(true);
    if (cbbValue->itemText(index).indexOf("(") == -1)
      cbbValue->clearEditText();
    else {
      if ((cbbValue->lineEdit()->text().indexOf(")") - cbbValue->lineEdit()->text().indexOf("(")) != 1)
	cbbValue->lineEdit()->setText(cbbValue->lineEdit()->text().remove(cbbValue->itemText(index).indexOf("(")+1, (cbbValue->itemText(index).indexOf(")") - (cbbValue->itemText(index).indexOf("(")+1))));
      cbbValue->lineEdit()->setCursorPosition(cbbValue->itemText(index).indexOf("(")+1);
    }
      cbbValue->setFocus();
  }
  else {
    cbbValue->setEditable(false);
    cbbUnit->setEnabled(false);
  }
}

void CSSBeginnerWidget::fillMe(QString line) {
      ckbName->setChecked(true);




      QString anUnit = MainWindow::getUnit(line);
      if (anUnit.count()) {
	bool found = false;
	int i = 0;
	while((found == false) && i < cbbUnit->count()) {
	  if (cbbUnit->itemText(i) == anUnit) {
	    found = true;
	    cbbUnit->setCurrentIndex(i);
	    cbbUnit->setEnabled(true);
	  }
	  i++;
	}
	cbbValue->setCurrentIndex(cbbValue->count() -1);
	cbbValue->setEditable(true);
	cbbValue->lineEdit()->setText(MainWindow::getValue(line));
	/*cbbUnit->setEnabled(true);
	cbbUnit->setEditable(true);
	cbbUnit->lineEdit()->setText(anUnit);*/
      }
      else {
	QString aValue = MainWindow::getValue(line);
	bool found = false;
	int i = 0;
	while((found == false) && i < cbbValue->count()) {
	  if (cbbValue->itemText(i) == aValue) {
	    found = true;
	    cbbValue->setCurrentIndex(i);
	    cbbValue->setEnabled(true);
	  }
	  i++;
	}
	if (found == false) {
	  cbbValue->setCurrentIndex(cbbValue->count() -1);
	  cbbValue->setEditable(true);
	  cbbValue->lineEdit()->setText(MainWindow::getValue(line));
	  cbbValue->setStyleSheet(QString::fromUtf8("background-color:#FFA1A1;"));
	}
      }

      //cssHeight->cbbUnit->lineEdit()->setText(getPropriety(aClass[i]));
      //cssHeight->cbbValue->setEditable(false);
      //cssHeight->cbbUnit->setEditable(false);
}

void CSSBeginnerWidget::clear() {
  ckbName->setChecked(false);
  cbbValue->setCurrentIndex(0);
  cbbValue->setStyleSheet(QString::fromUtf8(""));
  cbbUnit->setCurrentIndex(0);
}