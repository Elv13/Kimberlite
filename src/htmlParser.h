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
#include <QStringList>
#include <QThread>
#include <QSyntaxHighlighter>
#include <QTreeWidgetItem>
#include <QVector>
#include <QString>
#include <QFile>
#include <QtSql>
#include "stringToTemplate.h"

using namespace std;

class debugItem {
  public:
    debugItem(int _line, int _icon, QString _message) : line(_line), icon(_icon), message(_message) {}
    debugItem() {};
    int line;
    int icon; /*0 = warning, 1 = error, 3 = information*/
    int character;
    QString message;
};

class HtmlParser{
    //Q_OBJECT
  public:
    HtmlParser();
    QString compressFile(QString path);
    QString compressString(QString file);
    static QString getTag(QString aTag);
    QString htmlParser(QString inputFile, bool debug, bool toTemplate, bool mode, QTreeWidget* aTree);
    QVector<QString> listTag(QString inputFile);
    QVector<debugItem> debugVector;
    
  private:
    QStringList orphelinTags;
    QStringList noNewLineTags;
    QStringList needNewLineOnClose;
    QStringList needNewLineOnOpen;
    QStringList markerList;
    QVector<QString> ConvertToTemplate(QVector<QString> tagList, QString &markerDefinition);
    QVector<QString> translate(QVector<QString> tagList, QString markerDefinition);
    
    QString indentHtml(bool toTemplate, QVector<QString> tagList, QVector<int> levelList);
    void updateTree(bool toTemplate, QVector<QString> tagList, QVector<int> levelList, QTreeWidget* aTree);
    
};

/*class ParserThread : public QThread {
    Q_OBJECT

    public:
        ParserThread(QObject *parent = 0) : QThread(parent) {
	  aParser = new HtmlParser();
	}
	~ParserThread() {delete aParser;}
        void run() {
	  //aParser->analyseCommand(aCommand.toStdString());
        }
      HtmlParser* aParser;
    //private:
      //QString aCommand;

};*/
