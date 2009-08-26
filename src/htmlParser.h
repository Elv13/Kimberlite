/**     @file htmlParser.h

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
#ifndef HTML_PARSER
#define HTML_PARSER
#include <QStringList>
#include <QTreeWidgetItem>
#include <QVector>
#include <QtSql>

struct HtmlData {
  QVector<QString> tagList;
  QVector<uint> levelList;
};

struct W3Cinfo {
  QStringList orphelinTags;
  QStringList noNewLineTags;
  QStringList needNewLineOnClose;
  QStringList needNewLineOnOpen;
};

class IndexedTreeWidgetItem : public QTreeWidgetItem {
  public:
    IndexedTreeWidgetItem(QTreeWidget* parent, uint id) :QTreeWidgetItem(parent),index(id) {}
    IndexedTreeWidgetItem(QTreeWidgetItem* parent, const QStringList &strings, uint id) :QTreeWidgetItem(parent,strings),index(id){}
    IndexedTreeWidgetItem(QTreeWidget* parent, const QStringList &strings, uint id) :QTreeWidgetItem(parent,strings),index(id){}
    IndexedTreeWidgetItem(const QStringList &strings, uint id) :QTreeWidgetItem (strings),index(id) {}
    uint index;
};

class HtmlParser{
  public:
    HtmlParser();
    static QString getTag(QString aTag);
    static HtmlData getHtmlData(QString inputFile);
    static QString getParsedHtml(HtmlData &pageData);
    static QString getParsedHtml(QString inputFile);
    static W3Cinfo htmlInfo;
    static QString setAttribute(QString tag, QString attribute, QString value);
    static void setAttribute(HtmlData &pageData, QString tag, uint index, QString attribute, QString value);
    static QString getAttribute(QString tag, QString attribute);
  private:
    static QVector<uint> levelParser(QVector<QString> tagList);
    static QVector<QString> listTag(QString inputFile);
};
#endif //HTML_PARSER