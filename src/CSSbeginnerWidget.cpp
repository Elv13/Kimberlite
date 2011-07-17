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
#include <KColorDialog>

CSSBeginnerWidget::CSSBeginnerWidget(QWidget* parent) : QWidget (parent),btnColor(NULL),minimal(true) {
  createWidget();
}

CSSBeginnerWidget::CSSBeginnerWidget(QWidget* parent, QString tagName) : QWidget (parent),btnColor(NULL),minimal(false) {
  createWidget();
  setProperty(tagName);
}

void CSSBeginnerWidget::createWidget() {
    hlCbbTag = new QHBoxLayout(this);
  hlCbbTag->setObjectName(QString::fromUtf8("hlCbbTag"));
  hlCbbTag->setContentsMargins(0,0,0,0);
  
  if (minimal == false) {
    ckbName = new QCheckBox(this);
    ckbName->setObjectName(QString::fromUtf8("ckbName"));
    ckbName->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
    ckbName->setMinimumSize(QSize(160, 0));
    ckbName->setMaximumSize(QSize(160, 999999));
    hlCbbTag->addWidget(ckbName);
  }

  cbbValue = new QComboBox(this);
  cbbValue->setObjectName(QString::fromUtf8("cbbValue"));
  cbbValue->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
  hlCbbTag->addWidget(cbbValue);

  cbbUnit = new QComboBox(this);
  if (!minimal) {
    cbbUnit->setMinimumSize(QSize(75, 0));
    cbbUnit->setMaximumSize(QSize(75, 99));
  }
  else {
    cbbUnit->setMinimumSize(QSize(40, 0));
    cbbUnit->setMaximumSize(QSize(40, 99));
  }
  cbbUnit->setObjectName(QString::fromUtf8("cbbUnit"));
  cbbUnit->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  hlCbbTag->addWidget(cbbUnit);

}

void CSSBeginnerWidget::setProperty(QString tagName) {
  this->tagName = tagName;
  QSqlQuery query;
  query.exec("SELECT VALUE,UNIT FROM TCSS_TAG WHERE TITLE = '"+ tagName +"'");
  
  while (query.next()) {
    QStringList possibleValues = query.value(0).toString().split(";");
    for (int i =0;i < possibleValues.count(); i++) {
      if (possibleValues[i].left(4) == "UNIT") {
	cbbValue->addItem(possibleValues[i].remove(0,4));
	isEditable.push_back(true);
      }
      else if (possibleValues[i] == "color") {
	cbbValue->addItem(possibleValues[i]);
	fetchColorName();
      }
      else if (possibleValues[i].left(3) == "TAG") {
	QString toFetch = possibleValues[i].right(possibleValues[i].size()-3);
	QSqlQuery query2;
	query2.exec("SELECT VALUE,UNIT FROM TCSS_TAG WHERE TITLE = '"+ toFetch +"'");
	while (query2.next()) {
	  QStringList itemList = query2.value(0).toString().split(";");
	  foreach (QString aValue, itemList) {
	    if (aValue.left(4) == "UNIT") {
	      cbbValue->addItem(aValue.remove(0,4));
	      isEditable.push_back(true);
	    }
	    else if (possibleValues[i] == "color") {
	      fetchColorName();
	    }
	    else {
	      cbbValue->addItem(aValue);
	      isEditable.push_back(false);
	    }
	  }
	  if (!query2.value(1).toString().isEmpty()) //TODO prevent duplicate
	    cbbUnit->addItems(query2.value(1).toString().split(";"));
	}
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
  if (cbbValue->count())
    cbbValue_changed(0);
  if (!minimal) {
    ckbName->setText(tagName);
    cbbUnit->setEnabled(false);
    cbbValue->setEnabled(false);

    connect( ckbName, SIGNAL( stateChanged(int) ), this, SLOT( ckbName_checked(int) ) );
  }
  connect( cbbValue, SIGNAL( currentIndexChanged(int) ), this, SLOT( cbbValue_changed(int) ) );
  connect( cbbUnit, SIGNAL( currentIndexChanged(int) ), this, SLOT( valueChanged() ) );
  connect( cbbValue, SIGNAL( editTextChanged(QString) ), this, SLOT( valueChanged() ) );

  if ((cbbUnit->count() == 0) && (minimal))
    cbbUnit->setVisible(false);
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
    if (btnColor)
      btnColor->setEnabled(true);
  }
  else if (state == Qt::Unchecked) {
    cbbUnit->setEnabled(false);
    cbbValue->setEnabled(false);
    if (btnColor)
      btnColor->setEnabled(false);
  }
}

void CSSBeginnerWidget::cbbValue_changed(int index) {
  if (btnColor) {
    if (btnColor->isVisible() == true) {
      btnColor->setVisible(false);
      if ((cbbUnit->count() > 0) && minimal)
	cbbUnit->setVisible(true);
    }
  }
  
  if (index >= isEditable.size()) {
    qDebug() << "Indice out ou bound (CssbegWidget)";
  }
  else if (isEditable[index] == true) {
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
    if (cbbValue->itemText(index) == "color") {
      if (!btnColor) {
	btnColor = new QPushButton(this);
	if (!minimal)
	  btnColor->setText("Color");
	btnColor->setIcon(KIcon("fill-color"));
	hlCbbTag->addWidget(btnColor);
	if (!minimal) {
	  btnColor->setMinimumSize(QSize(75, 0));
	  btnColor->setMaximumSize(QSize(75, 99));
	}
	connect( btnColor, SIGNAL( clicked() ), this, SLOT( selectColor() ) );
	if (!minimal)
	  if (ckbName->isChecked() == false)
	    btnColor->setDisabled(true);
      }
      else {
	btnColor->setVisible(true);
	btnColor->setEnabled(true);
      }
      cbbUnit->setVisible(false);
    }
    cbbValue->setFocus();
  }
  else if (minimal) {
    cbbValue->setEditable(false);
    cbbUnit->setEnabled(false);
    if (btnColor)
      btnColor->setEnabled(false);
  }
  valueChanged();
}

void CSSBeginnerWidget::fillMe(QString line) {
  if (!minimal)
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
  if (!minimal)
    ckbName->setChecked(false);
  cbbValue->setCurrentIndex(0);
  cbbValue->setStyleSheet(QString::fromUtf8(""));
  cbbUnit->setCurrentIndex(0);
}

void CSSBeginnerWidget::fetchColorName() {
  isEditable.push_back(true);
  QSqlQuery query2;
  query2.exec("SELECT NAME FROM TCOLOR");
  while (query2.next()) {
    cbbValue->addItem(query2.value(0).toString());
    isEditable.push_back(false);
  }
}

void CSSBeginnerWidget::selectColor() {
  KColorDialog aDialog(this,true);
  QColor aColor;
  int result = aDialog.getColor( aColor );
  if ( result == KColorDialog::Accepted ) {
    cbbValue->lineEdit()->setText(aColor.name());
  }
}

void CSSBeginnerWidget::setValue(QString value) { //TODO deprecate fillMe
  //setProperty(CssParser::getPropriety(value));
  qDebug() << "Fill line: " << value;
  //fillMe(tagName + ": " + value+";");
}

QString CSSBeginnerWidget::getValue() {
  QString value;
  value = tagName + ":" +cbbValue->currentText();
  if (cbbUnit->isEnabled())
    value += cbbUnit->currentText();
  return value;
}


void CSSBeginnerWidget::valueChanged() {
  QString value = getValue();
  //qDebug() << "Emmiting: " << value;
  emit textEdited(value);
}