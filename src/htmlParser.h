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
#include <iostream>
#include <QStringList>
#include <vector>
#include <QThread>
#include <QSyntaxHighlighter>
#include <QTreeWidgetItem>
#include <QVector>

using namespace std;

struct debugItem {
  int line;
  int icon; /*0 = warning, 1 = error, 3 = information*/
  int character;
  QString message;
};

class HtmlParser{
    //Q_OBJECT
  public:
    HtmlParser();
    std::string compressFile(std::string path);
    std::string compressString(std::string file);
    std::string getTag(std::string aTag);
    std::string htmlParser(std::string inputFile, bool debug, bool toTemplate, bool mode, QTreeWidget* aTree);
    vector<string> listTag(string inputFile);
    QVector<debugItem> debugVector;
    
  private:
    QStringList orphelinTags;
    QStringList noNewLineTags;
    QStringList needNewLineOnClose;
    QStringList needNewLineOnOpen;
    QStringList markerList;
    vector<string> ConvertToTemplate(vector<string> tagList, string &markerDefinition);
    vector<string> translate(vector<string> tagList, string markerDefinition);
    
    string indentHtml(bool toTemplate, vector<string> tagList, vector<int> levelList);
    void updateTree(bool toTemplate, vector<string> tagList, vector<int> levelList, QTreeWidget* aTree);
    
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

class HtmlSyntaxHighlighter : public QSyntaxHighlighter {
public:
  HtmlSyntaxHighlighter(QTextEdit* parent) : QSyntaxHighlighter(parent) {
 HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    //keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

  //while (query.next()) {
    keywordPatterns << "value=";
    keywordPatterns << "name=";
    keywordPatterns << "style=";
    keywordPatterns << "href=";
    keywordPatterns << "class=";
    keywordPatterns << "align=";
    keywordPatterns << "valign=";
    keywordPatterns << "height=";
    keywordPatterns << "onmouseover=";
    keywordPatterns << "onmouseout=";
    keywordPatterns << "onclick=";
    keywordPatterns << "rel=";
    keywordPatterns << "frame=";
    keywordPatterns << "rules=";
    /*keywordPatterns << "name=";
    keywordPatterns << "name=";
    keywordPatterns << "name=";
    keywordPatterns << "name=";*/

  //}
    foreach (QString pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("<\\/?[a-zA-Z]+ ?>?");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkMagenta);
    singleLineCommentFormat.setFontWeight(QFont::Bold);
    //rule.pattern = QRegExp("//[^\n]*");
    rule.pattern = QRegExp(">");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);
    multiLineCommentFormat.setFontWeight(QFont::Bold);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\" ");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    /*functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::green);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);*/

    commentStartExpression = QRegExp("<!--");
    commentEndExpression = QRegExp("-->");

  };
  void highlightBlock(const QString &text)
  {
    foreach (HighlightingRule rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {

        int endIndex = text.indexOf(commentEndExpression, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression,
                                                startIndex + commentLength);
    }
  }
private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    //QTextCharFormat functionFormat;
};