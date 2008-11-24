/**     @file stringToTemplate.h

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
#ifndef UI_UNTITLED_H
#define UI_UNTITLED_H

#include <iostream>
#include <vector>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <KDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QStringList>

QT_BEGIN_NAMESPACE
using namespace std;

class StringToMarker : public QDialog
{
  Q_OBJECT
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblMessage;
    QLabel *lblText;
    QLineEdit *txtNewText;
    QDialogButtonBox *buttonBox;

    StringToMarker(vector<string> *tagList2, int index2, string* comment2, QWidget* parent, QStringList* markerList2);
    ~StringToMarker(){};

private:
    vector<string>* tagList;
    int index;
    string* comment;
    QStringList* markerList;

private slots:
    void replaceString();
};

/*namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui*/

QT_END_NAMESPACE

#endif // UI_UNTITLED_H
