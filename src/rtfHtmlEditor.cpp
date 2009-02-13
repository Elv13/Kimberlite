#include "rtfHtmlEditor.h"
#include "htmlParser.h"

RtfHtmlEditor::RtfHtmlEditor(QWidget *parent) : KTextEdit(parent), c(0) {              
    QSqlQuery query;
    query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDCLOSE = 0");
    while (query.next()) 
      orphelinTags << query.value(0).toString();
    
    query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 0");
    while (query.next()) 
      noNewLineTags << query.value(0).toString();
    
    query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 3");
    while (query.next()) 
      noNewLineTags << query.value(0).toString();
 }
 
RtfHtmlEditor::~RtfHtmlEditor() {
  
}
 
void RtfHtmlEditor::setCompleter(QCompleter *completer) {
     if (c)
         QObject::disconnect(c, 0, this, 0);

     c = completer;

     if (!c)
         return;

     c->setWidget(this);
     c->setCompletionMode(QCompleter::PopupCompletion);
     c->setCaseSensitivity(Qt::CaseInsensitive);
     QObject::connect(c, SIGNAL(activated(const QString&)),
                      this, SLOT(insertCompletion(const QString&)));
 }
 
QCompleter *RtfHtmlEditor::completer() const {
     return c;
 }
 
void RtfHtmlEditor::insertCompletion(const QString& completion) {
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    int currentPos = tc.position();
    tc.select(QTextCursor::LineUnderCursor);
    QString currentText = tc.selectedText();
    QString tab;
    bool exit = false;
    int i =0;
    while ((exit == false) && (i < currentText.count())) {
      if (currentText[i] != ' ') 
        exit = true;
      else
        tab += " ";
      i++;
    }
    tc.setPosition(currentPos);
    if (orphelinTags.indexOf(completion) == -1) {
      if (noNewLineTags.indexOf(completion) != -1) {
        tc.insertText(completion.right(extra).toLower() + "></"+completion.toLower()+ ">");
        tc.setPosition(tc.position() - 3 - completion.count());
      }
      else {
        tc.insertText(completion.right(extra).toLower() + ">\n" + tab + "   \n" + tab + "</"+completion.toLower()+ ">");
        tc.setPosition(tc.position() - 4 - tab.count() - completion.count());
      }
    }
    else {
      if (noNewLineTags.indexOf(completion) != -1) {
        tc.insertText(completion.right(extra).toLower() + ">");
        //tc.setPosition(tc.position());
      }
      else {
        tc.insertText(completion.right(extra).toLower() + ">\n" + tab);
        //tc.setPosition(tc.position());
      }
    }
    setTextCursor(tc);
 }
 
 
void RtfHtmlEditor::insertTabulation() {
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int currentPos = tc.position();
    QString nextLine;
    tc.select(QTextCursor::LineUnderCursor);
    QString currentText = tc.selectedText();
    if (currentText.indexOf("</") != -1) {
      tc.movePosition(QTextCursor::Down);
      tc.select(QTextCursor::LineUnderCursor);
      nextLine = tc.selectedText();
      
      tc.setPosition(currentPos);
      tc.select(QTextCursor::LineUnderCursor);
    }
    
    QString tab;
    bool exit = false;
    int i =0;
    while ((exit == false) && (i < currentText.count())) {
      if (currentText[i] != ' ') 
        exit = true;
      else
        tab += " ";
      i++;
    }
    
    if ((currentText.indexOf("</") != -1) && (tab.count() >=3)) {
      bool exit2 = false;
      int i2 =0;
      while ((exit2 == false) && (i2 < nextLine.count())) {
        if (nextLine[i2] != ' ') 
          exit2 = true;
        i2++;
      }
      printf("This is the difference: %d \n",(i -i2));
      if ((i - i2) != 3) {
        tc.removeSelectedText();
        currentText = currentText.remove(0,3);
        tc.insertText(currentText);
        tab =tab.remove(0,3);
        tc.setPosition(currentPos);
        tc.movePosition(QTextCursor::Up);
      }
    }
    else if (currentText.count() >= i) {
      if ((currentText[i-1] == '<') && (orphelinTags.indexOf(HtmlParser::getTag(currentText).toUpper()) == -1))
        tab += "   ";
      tc.setPosition(currentPos);
    }
    else
      tc.setPosition(currentPos);
      
    
    //tc.setPosition(currentPos);
    tc.movePosition(QTextCursor::EndOfLine);
    tc.insertText("\n" + tab);
    //tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    setTextCursor(tc);
 }
 
 
QString RtfHtmlEditor::textUnderCursor() const {
     QTextCursor tc = textCursor();
     tc.select(QTextCursor::WordUnderCursor);
     return tc.selectedText();
}
 
void RtfHtmlEditor::focusInEvent(QFocusEvent *e) {
     if (c)
         c->setWidget(this);
     QTextEdit::focusInEvent(e);
}
 
void RtfHtmlEditor::keyPressEvent(QKeyEvent *e) {
    if (((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return)) && (c->popup()->isVisible() == false)) {
	insertTabulation();
	return;
    }
    else if (e->key() == Qt::Key_Less) {
      QTextCursor tc = textCursor();
      tc.insertText("<");
      QRect cr = cursorRect();
      cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
      c->complete(cr); 
      return;
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
        case Qt::Key_Less:
        case Qt::Key_Backtab:
             e->ignore();
             return; // let the completer do default behavior
        default:
            break;
        }
     }

     bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
     if (!c || !isShortcut) // dont process the shortcut when we have a completer
         QTextEdit::keyPressEvent(e);
          const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
     if (!c || (ctrlOrShift && e->text().isEmpty()))
         return;

     static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
     bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
     QString completionPrefix = textUnderCursor();
     if (completionPrefix[0] == '<') {
        completionPrefix = completionPrefix.remove(0,1);
     }

     if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                       || eow.contains(e->text().right(1)))) {
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
 
 