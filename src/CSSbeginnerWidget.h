/**     @file CSSbeginnerWidget.h

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
#ifndef CSSBEGINNERWIDGET_DEF
#define CSSBEGINNERWIDGET_DEF

#include <QtGui/QWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QList>
#include <kcombobox.h>

QT_BEGIN_NAMESPACE

class CSSBeginnerWidget : public QWidget {
Q_OBJECT
public:
  CSSBeginnerWidget(QWidget* parent, QString tagName);
  ~CSSBeginnerWidget(){};
  void fillMe(QString line);
  void clear();
  QHBoxLayout *hlCbbTag;
  QCheckBox *ckbName;
  QComboBox *cbbValue;
  QComboBox *cbbUnit;
private:
  QList<bool> isEditable;

private slots:
  void ckbName_checked(int state);
  void cbbValue_changed(int index);
};
#endif
