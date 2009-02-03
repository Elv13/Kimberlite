/**     @file stringToTemplate.cpp

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
#include "stringToTemplate.h"

StringToMarker::StringToMarker(QVector<QString> *tagList2, int index2, QString* comment2, QWidget* parent, QStringList* markerList2) : QDialog( parent )
    {
    tagList = tagList2;
    markerList;
    index = index2;
    comment = comment2;
    markerList = markerList2;
    

    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("Dialog"));
    this->resize(471, 121);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    lblMessage = new QLabel(this);
    lblMessage->setObjectName(QString::fromUtf8("lblMessage"));

    verticalLayout->addWidget(lblMessage);

    lblText = new QLabel(this);
    lblText->setObjectName(QString::fromUtf8("lblText"));
    lblText->setText("<b>" + tagList->at(index) + "</b>");

    verticalLayout->addWidget(lblText);

    txtNewText = new QLineEdit(this);
    txtNewText->setObjectName(QString::fromUtf8("txtNewText"));

    verticalLayout->addWidget(txtNewText);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ignore|QDialogButtonBox::Ok);

    verticalLayout->addWidget(buttonBox);


    //retranslateUi(this);
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(replaceString()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QMetaObject::connectSlotsByName(this);

    this->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
    lblMessage->setText(QApplication::translate("Dialog", "Enter the marker for:", 0, QApplication::UnicodeUTF8));
    //lblText->setText(QApplication::translate("Dialog", "<b>name</b>", 0, QApplication::UnicodeUTF8));
    } // constructor

void StringToMarker::replaceString() {
   if (markerList->indexOf(txtNewText->text()) != -1) {
      //while (markerList->indexOf(txtNewText->text()) != -1) {
	    lblMessage->setText("!This marker is currently used!");
	    txtNewText->setStyleSheet("background-color:#FFBCBD");
      //}
   }
   else {
      QString thisString = tagList->at(index);
      //cout << tagList->at(index) << endl;
      *comment += "\n###" + txtNewText->text() + "### = " + thisString;
      (*tagList)[index] = "###" + txtNewText->text() + "###";
      markerList->append(txtNewText->text());
      accept();
   }
}