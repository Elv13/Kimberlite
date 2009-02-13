#ifndef RTF_HTML_EDITOR
#define RTF_HTML_EDITOR
#include <QStringList>
#include <KTextEdit>
#include <QCompleter>
#include <QSqlDatabase>
#include <QSyntaxHighlighter>
#include <QtSql>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QScrollBar>
#include <QStringList>


 class RtfHtmlEditor : public KTextEdit {
     Q_OBJECT

 public:
     RtfHtmlEditor(QWidget *parent = 0);
     ~RtfHtmlEditor();

     void setCompleter(QCompleter *c);
     QCompleter *completer() const;

 protected:
     void keyPressEvent(QKeyEvent *e);
     void focusInEvent(QFocusEvent *e);

 private slots:
     void insertCompletion(const QString &completion);
      void insertTabulation();
 private:
    QWidget* parent;
    QString textUnderCursor() const;
    QStringList orphelinTags;
    QStringList noNewLineTags;
    QStringList tagList;
    QStringList tmpList;
    bool defaultCompletion;
    bool isAtribute();
    void fillTmpList(QString tag);
    QCompleter* htmlCompleter;
    QCompleter* tmpCompleter;

 private:
     QCompleter *c;
 };


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

 
 #endif