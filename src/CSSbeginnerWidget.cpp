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
  hlCbbTag = new QHBoxLayout(this);
  hlCbbTag->setObjectName(QString::fromUtf8("hlCbbTag"));
  hlCbbTag->setContentsMargins(0,0,0,0);
  ckbName = new QCheckBox(this);
  ckbName->setObjectName(QString::fromUtf8("ckbName"));
  ckbName->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
  ckbName->setMinimumSize(QSize(160, 0));
  ckbName->setMaximumSize(QSize(160, 999999));
  hlCbbTag->addWidget(ckbName);

  cbbValue = new QComboBox(this);
  cbbValue->setObjectName(QString::fromUtf8("cbbValue"));
  cbbValue->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
  hlCbbTag->addWidget(cbbValue);

  cbbUnit = new QComboBox(this);
  cbbUnit->setMinimumSize(QSize(75, 0));
  cbbUnit->setMaximumSize(QSize(75, 99));
  cbbUnit->setObjectName(QString::fromUtf8("cbbUnit"));
  cbbUnit->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  hlCbbTag->addWidget(cbbUnit);

  QSqlQuery query;
  query.exec("SELECT VALUE,UNIT FROM TCSS_TAG WHERE TITLE = '"+ tagName +"'");
  
  while (query.next()) {
    QStringList possibleValues = query.value(0).toString().split(";");
    for (int i =0;i < possibleValues.count(); i++) {
      if (possibleValues[i].left(4) == "UNIT") {
	cbbValue->addItem(possibleValues[i].remove(0,4));
	isEditable.push_back(true);
      }
      else {
	cbbValue->addItem(possibleValues[i]);
	isEditable.push_back(false);
      }
    }
    QStringList possibleUnit = query.value(1).toString().split(";");
    for (int i =0;i < possibleUnit.count(); i++)
      if (possibleUnit[i].isEmpty() == false)
	cbbUnit->addItem(possibleUnit[i]);
  }
  ckbName->setText(tagName);
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
  QString anUnit = CssParser::getUnit(line);
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
    cbbValue->lineEdit()->setText(CssParser::getValue(line));
  }
  else {
    QString aValue = CssParser::getValue(line);
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
      cbbValue->lineEdit()->setText(CssParser::getValue(line));
      cbbValue->setStyleSheet(QString::fromUtf8("background-color:#FFA1A1;"));
    }
  }
}

void CSSBeginnerWidget::clear() {
  ckbName->setChecked(false);
  cbbValue->setCurrentIndex(0);
  cbbValue->setStyleSheet(QString::fromUtf8(""));
  cbbUnit->setCurrentIndex(0);
}