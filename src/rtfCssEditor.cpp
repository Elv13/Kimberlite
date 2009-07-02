#include "rtfCssEditor.h"
#include <QtSql>
#include <KColorDialog>
#include "cssParser.h"

RtfCssEditor::RtfCssEditor(QWidget *parent) : KTextEdit(parent), c(0),currentCompleter(DEFAULT),alternateCompleter(NULL) {
  QStringList wordList;
  QSqlQuery query22;
  query22.exec("SELECT TITLE FROM TCSS_TAG");
  
  while (query22.next())
    wordList <<  query22.value(0).toString();

  standardCompleter = new QCompleter(wordList, this);
  standardCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  setCompleter(standardCompleter);
}
 
RtfCssEditor::~RtfCssEditor() {
}
 
void RtfCssEditor::setCompleter(QCompleter *completer) {
  if (c)
    QObject::disconnect(c, 0, this, 0);
  c = completer;
  if (!c)
    return;

  c->setWidget(this);
  c->setCompletionMode(QCompleter::PopupCompletion);
  c->setCaseSensitivity(Qt::CaseInsensitive);
  QObject::connect(c, SIGNAL(activated(const QString&)), this, SLOT(insertCompletion(const QString&)));
}
 
QCompleter *RtfCssEditor::completer() const {
  return c;
}
 
void RtfCssEditor::insertCompletion(const QString& completion) {
  if (c->widget() != this)
    return;
  QTextCursor tc = textCursor();
  int extra = completion.length() - c->completionPrefix().length();
  tc.movePosition(QTextCursor::Left);
  tc.movePosition(QTextCursor::EndOfWord);
  if (currentCompleter == DEFAULT) {
    tc.insertText(completion.right(extra) + ": ;");
  }
  else {
    if (completion == "color") {
      KColorDialog aDialog(this,true);
      QColor aColor;
      int result = aDialog.getColor( aColor );
      if ( result == KColorDialog::Accepted ) {
	tc.insertText(aColor.name().toUpper());
	customColorList.push_back(aColor.name().toUpper());
      }
    }
    else {
      tc.insertText(completion.right(extra));
    }
  }
  tc.movePosition(QTextCursor::Left);
  setTextCursor(tc);
}

void RtfCssEditor::insertTabulation() {
  if (c->widget() != this)
    return;
  QTextCursor tc = textCursor();
  int currentPos = tc.position();
  tc.select(QTextCursor::LineUnderCursor);
  if ((tc.selectedText().isEmpty() == false) && (tc.selectedText().indexOf("}") == -1)) {
    tc.setPosition(currentPos);
    tc.insertText("\n    ");
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
  }
  else {
    tc.setPosition(currentPos);
    tc.insertText("\n");
  }
  tc.movePosition(QTextCursor::EndOfWord);
  setTextCursor(tc);
}
 
void RtfCssEditor::insertBrace() {
  if (c->widget() != this)
    return;
  QTextCursor tc = textCursor();
  int currentPos = tc.position();
  tc.movePosition(QTextCursor::Down);
  tc.select(QTextCursor::LineUnderCursor);
  if (tc.selectedText().isEmpty() == true) {
    tc.setPosition(currentPos);
    //if (QTextCursor::NextWord)
    tc.insertText("\n    \n}");
    /*tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::Left);*/
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::Left);
    setTextCursor(tc);
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
  }
  else {
    tc.setPosition(currentPos);
    tc.insertText("\n    ");
  }
  tc.movePosition(QTextCursor::EndOfWord);
  QRect cr = cursorRect();
  cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
  c->complete(cr); // popup it up!
    
}
 
QString RtfCssEditor::textUnderCursor() const {
  QTextCursor tc = textCursor();
  tc.select(QTextCursor::WordUnderCursor);
  return tc.selectedText();
}
 
void RtfCssEditor::focusInEvent(QFocusEvent *e) {
  if (c)
    c->setWidget(this);
  QTextEdit::focusInEvent(e);
}
 
void RtfCssEditor::keyPressEvent(QKeyEvent *e) {
  if (currentCompleter == ALTERNATE) {
    currentCompleter = DEFAULT;
    setCompleter(standardCompleter);
  }
  
  //If enter is pressed
  if (((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return)) && (!c->popup()->isVisible())) {
    QTextCursor tc = textCursor();
    int currentPos = tc.position();
    tc.select(QTextCursor::LineUnderCursor);
    QString line = tc.selectedText();
    tc.setPosition(currentPos);
  
    if (line.indexOf("{") != -1) {
      insertBrace();
      return;
    }
    else {
      insertTabulation();
      return;
    }
  }
  
  if (c && c->popup()->isVisible()) {
      // The following keys are forwarded by the completer to the widget
    switch (e->key()) {
    case Qt::Key_Enter:
      return;
    case Qt::Key_Return:
      return;
    case Qt::Key_Escape:
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
	  e->ignore();
	  return; // let the completer do default behavior
    default:
	break;
    }
  }

  bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+E
  if (!c || !isShortcut) // dont process the shortcut when we have a completer
    QTextEdit::keyPressEvent(e);
  const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if (!c || (ctrlOrShift && e->text().isEmpty()))
    return;

  static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
  bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
  QString completionPrefix = textUnderCursor();
  
  if (isUnit() && isShortcut) {
    insertPropriety(UNIT);
    return;
  }
  
  if (isPropriety() && isShortcut) {
    insertPropriety(VALUE);
    return;
  }


  if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3 || eow.contains(e->text().right(1)))) {
    c->popup()->hide();
    return;
  }

  if (completionPrefix != c->completionPrefix()) {
    c->setCompletionPrefix(completionPrefix);
    c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
  }
  QRect cr = cursorRect();
  cr.setWidth(c->popup()->sizeHintForColumn(0)
	      + c->popup()->verticalScrollBar()->sizeHint().width());
  c->complete(cr); // popup it up!
}
 
 
bool RtfCssEditor::isPropriety() {
  QTextCursor tc = textCursor();
  int currentPos = tc.position();
  tc.select(QTextCursor::LineUnderCursor);
  QString line = tc.selectedText();
  tc.movePosition(QTextCursor::StartOfLine);
  int startposition = tc.position();

  tc.setPosition(currentPos);
  if ((line.indexOf(':') > 2) && (startposition + line.indexOf(':') < currentPos)) {
    if ((line.indexOf(';') != -1) && (startposition + line.indexOf(';') >= currentPos)) {
      return true;
    }
    else if (line.indexOf(';') == -1) {
      return true;
    }
  }
  return false;
}

bool RtfCssEditor::isUnit() {
  QTextCursor tc = textCursor();
  int currentPos = tc.position();
  tc.select(QTextCursor::LineUnderCursor);
  QString line = tc.selectedText();
  tc.setPosition(currentPos);
  QString value = CssParser::getValue(line);
  if (value.isEmpty())
    return false; //it end here
  bool isNumeric;
  int numericValue = value.toInt(&isNumeric,10);
  if (isNumeric) {
    tc.movePosition(QTextCursor::StartOfLine);
    int startposition = tc.position();
    tc.setPosition(currentPos);
    if (startposition + line.indexOf(value) + value.count() <= currentPos) {
      if ((line.indexOf(';') != -1) && (startposition + line.indexOf(';') < currentPos))
	return false;
      return true;
    }
  }
  return false;
}

void  RtfCssEditor::insertPropriety(bool insertType) {
  QTextCursor tc = textCursor();
  int currentPos = tc.position();
  tc.select(QTextCursor::LineUnderCursor);
  QString line = tc.selectedText();
  tc.setPosition(currentPos);
  
  QString tag = CssParser::getPropriety(line);

  if (alternateCompleter) {
    delete alternateCompleter;
    alternateCompleter = NULL;
  }
  
  QString value = CssParser::getValue(line);
  if ((value.isEmpty() == false) && (insertType == VALUE)) {
    return; //Do not add it twice
  }
  
  QSqlQuery query;
  query.exec("SELECT "+ QString((insertType)?"VALUE":"UNIT") +" FROM TCSS_TAG WHERE TITLE = '"+ tag +"'");
  QStringList propList;
    
  while (query.next()) {
    propList = query.value(0).toString().split(";");
  }
  
  int i =0;
  while (i < propList.count()) {
    if (propList[i].left(3) == "TAG") {
      QString toFetch = propList[i].right(propList[i].size()-3);
      QSqlQuery query2;
      query2.exec("SELECT "+ QString((insertType)?"VALUE":"UNIT") +" FROM TCSS_TAG WHERE TITLE = '"+ toFetch +"'");
      propList.removeAt(i);
      while (query2.next()) {
	propList += query2.value(0).toString().split(";");
      }
    }
    else if (propList[i].left(4) == "UNIT")
      propList.removeAt(i);
    else
      i++;
  }
  
  if (propList.indexOf("color") != -1) {
    propList += customColorList;
    QSqlQuery query2;
    query2.exec("SELECT NAME FROM TCOLOR");
    while (query2.next()) {
      qDebug() << "I am here";
      propList += query2.value(0).toString();
    }
  }

  if (propList.count()) {
    currentCompleter = ALTERNATE;
    alternateCompleter = new QCompleter(propList,this);
    setCompleter(alternateCompleter);
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
    alternateCompleter->complete(cr);
  }
  
}