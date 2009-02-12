#ifndef RTF_CSS_EDITOR
#define RTF_CSS_EDITOR
#include <QStringList>
#include <KTextEdit>
#include <QCompleter>
#include <QSqlDatabase>
#include <QSyntaxHighlighter>
#include <QtSql>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QScrollBar>
/*#include <>
#include <>
#include <>
#include <>
#include <>*/


 class RtfCssEditor : public KTextEdit
 {
     Q_OBJECT

 public:
     RtfCssEditor(QWidget *parent = 0);
     ~RtfCssEditor();

     void setCompleter(QCompleter *c);
     QCompleter *completer() const;

 protected:
     void keyPressEvent(QKeyEvent *e);
     void focusInEvent(QFocusEvent *e);

 private slots:
     void insertCompletion(const QString &completion);

 private:
     QString textUnderCursor() const;

 private:
     QCompleter *c;
 };
 
 
class CssSyntaxHighlighter : public QSyntaxHighlighter {
public:
  CssSyntaxHighlighter(QTextEdit* parent) : QSyntaxHighlighter(parent) {
 HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    //keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

  QSqlQuery query;
  query.exec("SELECT TITLE FROM TCSS_TAG");
  
  while (query.next()) {
    keywordPatterns << "\\b" + query.value(0).toString() + "\\b";
  }
    foreach (QString pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern); //TOTEST QRegExp("<"+pattern+"\b[^>]*>");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    singleLineCommentFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);
    multiLineCommentFormat.setFontWeight(QFont::Bold);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::green);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

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
    QTextCharFormat functionFormat;
};

 
 #endif