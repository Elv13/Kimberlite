#include "rtfHtmlEditor.h"
#include "htmlParser.h"
#include "cssParser.h"

#include <KColorDialog>

RtfHtmlEditor::RtfHtmlEditor(QWidget *parent) : KTextEdit(parent), c(0),defaultCompletion(true),is3rdLvlCompleter(false) {   
    this->parent = parent;
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
    
    QStringList tagList;
    QSqlQuery query23;
    query23.exec("SELECT NAME FROM THTML_TAG");
    
    while (query23.next()) {
      tagList <<  query23.value(0).toString();
    }

    htmlCompleter = new QCompleter(tagList, parent);
    htmlCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    //cssCompleter->setWidget(rtfCSSEditor);
    setCompleter(htmlCompleter);
 }
 
RtfHtmlEditor::~RtfHtmlEditor() {
  
}

void RtfHtmlEditor::fillTmpList(QString tag) {
  tmpList.clear();
  QCompleter* oldCompleter;
  tmpList << "class" << "name" << "id" << "style";
  QSqlQuery query23;
  query23.exec("SELECT PROPRIETIES FROM THTML_TAG WHERE NAME ='" + tag.toUpper() +"'" );
    
  while (query23.next()) {
    QString tmp =  query23.value(0).toString();
    while (tmp.indexOf(";") != -1) {
      if (tmp.left(tmp.indexOf(";")).indexOf("(") != -1)
        tmpList << tmp.left(tmp.indexOf(";")).left(tmp.left(tmp.indexOf(";")).indexOf("("));
      else
        tmpList << tmp.left(tmp.indexOf(";"));
      tmp.remove(0,tmp.indexOf(";")+1);
    }
  }
  
  if (defaultCompletion == false) {
    oldCompleter = tmpCompleter;
  }
  tmpCompleter = new QCompleter(tmpList, parent);
  setCompleter(tmpCompleter);
  if (defaultCompletion == false)
    delete oldCompleter;
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
    if (defaultCompletion == true) {
      tc.movePosition(QTextCursor::Left);
      tc.movePosition(QTextCursor::EndOfWord);
      int currentPos = tc.position();
      tc.movePosition(QTextCursor::Left);
      tc.select(QTextCursor::WordUnderCursor);
      QString toAdd;
      if ((tc.selectedText()[0] != '<') && (extra == completion.count()))
	toAdd = "<";
      tc.setPosition(currentPos);
      tc.select(QTextCursor::LineUnderCursor);
      QString currentText(tc.selectedText()),tab;
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
	  tc.insertText(toAdd + completion.right(extra).toLower() + "></"+completion.toLower()+ ">");
	  tc.setPosition(tc.position() - 3 - completion.count() /*+ toAdd.count()*/);
	}
	else {
	  tc.insertText(toAdd + completion.right(extra).toLower() + ">\n" + tab + "   \n" + tab + "</"+completion.toLower()+ ">");
	  tc.setPosition(tc.position() - 4 - tab.count() - completion.count() /*+ toAdd.count()*/);
	}
      }
      else {
	if (noNewLineTags.indexOf(completion) != -1) 
	  tc.insertText(toAdd + completion.right(extra).toLower() + ">");
	else {
	  tc.insertText(toAdd + completion.right(extra).toLower() + ">\n" + tab);
      }
    }
 }
 else {
   if (is3rdLvlCompleter == true) {
     tc.insertText(completion.right(extra).toLower());
     //tc.movePosition(QTextCursor::Left);
     is3rdLvlCompleter = false;
   }
   else {
      tc.insertText(completion.right(extra).toLower() + "=\"\"");
      tc.movePosition(QTextCursor::Left);
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
    }
    tc.movePosition(QTextCursor::EndOfLine);
    int endPos = tc.position();
    tc.setPosition(currentPos);
    tc.select(QTextCursor::LineUnderCursor);
    
    
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
    
    if (endPos > currentPos) {
      tc.setPosition(currentPos);
      tc.select(QTextCursor::LineUnderCursor);
      tc.removeSelectedText();
      //tc.removeText();
      if ((currentText.right(endPos - currentPos)[0] == '<') &&(currentText.right(endPos - currentPos)[1] != '/')) //TODO add a gettag to check orphelin tags
        tab += "   ";
      tc.insertText(currentText.left(currentText.count() - (endPos - currentPos)) + "\n" + tab + currentText.right(endPos - currentPos)); //TODO there must be a better way to do that :/
      setTextCursor(tc);
      return;
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
      if ((currentText[(i > 0)?i-1:0] == '<') && (orphelinTags.indexOf(HtmlParser::getTag(currentText).toUpper()) == -1))
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
 
bool RtfHtmlEditor::isAtribute() {
  QTextCursor tc = textCursor();
  int currentPos = tc.position();
  tc.select(QTextCursor::LineUnderCursor);
  int open = tc.selectedText().indexOf("<");
  int close = tc.selectedText().indexOf(">");
  if ((open != -1) && (close != -1) && (tc.selectedText()[open+1] != '/')) {
    tc.setPosition(currentPos);
    tc.movePosition(QTextCursor::StartOfLine);
    int startposition = tc.position() + open;
    int endposition = tc.position() + close;
    if ((startposition < currentPos) && (endposition > (currentPos -1)))
      return true;
  }
  return false;
}

QString RtfHtmlEditor::getCurrentAtribute() { //TODO if ="|" != -2 but -1
  QTextCursor tc = textCursor();
  int currentPos = tc.position();
  tc.movePosition(QTextCursor::StartOfLine);
  int currentPosInLine = currentPos - tc.position();
  tc.select(QTextCursor::LineUnderCursor);
  QString line = tc.selectedText();
  if ((line[(currentPosInLine)?currentPosInLine-1:0] == '=') || ((line[(currentPosInLine)?currentPosInLine-1:0] == '\"') && (line[(currentPosInLine > 1)?currentPosInLine-2:0] == '='))) {
    return line.mid(line.lastIndexOf(" ", currentPosInLine), currentPosInLine - line.lastIndexOf(" ", currentPosInLine)-2);
  }
  else {
    return QString();
  }
}

void RtfHtmlEditor::completeAttribute(QString attribute, QString tag) {
  tmpList.clear();
  QCompleter* oldCompleter;
  if (attribute.trimmed() == "CLASS") {
    tmpList = CssParser::getClassList();
  }
  else if (attribute.trimmed() == "STYLE") {
    QSqlQuery query23;
    query23.exec("SELECT TITLE FROM TCSS_TAG" );
      
    while (query23.next()) {
	  tmpList << query23.value(0).toString();
    }
  }
  else if ((attribute.trimmed() == "BGCOLOR") || (attribute.trimmed() == "COLOR")) {
    KColorDialog aDialog(this,true);
    QColor aColor;
    int result = aDialog.getColor( aColor );
    if ( result == KColorDialog::Accepted ) {
      tmpList << aColor.name();
    }
  }
  else {
    QSqlQuery query23;
    query23.exec("SELECT PROPRIETIES FROM THTML_TAG WHERE NAME ='" + tag.toUpper() +"'"  );
      
    while (query23.next()) {
      QStringList attributeList = query23.value(0).toString().split(";");
      for (int i =0; i < attributeList.size();i++) {
	int indexP = attributeList[i].indexOf("(");
	if (indexP != -1) {
	  if (attributeList[i].left(indexP).trimmed() == attribute.toUpper().trimmed()) {
	    QStringList valueList = attributeList[i].mid(indexP+1,attributeList[i].indexOf(")") - (indexP+1)).split(",");
	    for (int j =0; j < valueList.size();j++)
	      tmpList << valueList[j];
	  }
	}
      }
    }
  }
    
  
  if (defaultCompletion == false) {
    oldCompleter = tmpCompleter;
  }
  tmpCompleter = new QCompleter(tmpList, parent);
  setCompleter(tmpCompleter);
  if (defaultCompletion == false)
    delete oldCompleter;
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
    bool isInAtribute = isAtribute();
    bool isShortcut = ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_Space)); // CTRL+S
    bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    
    if (c->popup()->isVisible()) {
      //c->popup()->setHidden(true);
      QString completionPrefix = textUnderCursor();
      if (completionPrefix[0] == '<') {
          completionPrefix = completionPrefix.remove(0,1);
      }
      //completionPrefix.chop(1);
      if (completionPrefix != c->completionPrefix()) {
         c->setCompletionPrefix(completionPrefix);
         c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
      }
    }
    
    if ((defaultCompletion == false) && (isInAtribute == false)){
      defaultCompletion = true;
      setCompleter(htmlCompleter);
      delete tmpCompleter;
      this->setFocus();
    }
    
    
    if (isInAtribute && isShortcut/*&& (defaultCompletion == true)*/) {
      QTextCursor tc = textCursor();
      int currentPos = tc.position();
      tc.select(QTextCursor::LineUnderCursor);
      QString tag = HtmlParser::getTag(tc.selectedText()).toUpper();
      if (tag[0] == '<')
	tag = tag.remove(0,1);
      tc.setPosition(currentPos);
      
      QString currentAttribute = getCurrentAtribute();
      if (currentAttribute != "") {
	is3rdLvlCompleter = true;
	completeAttribute(currentAttribute.toUpper(), tag);
      }
      else
	fillTmpList(tag.toUpper());
      
      defaultCompletion = false; 
      QRect cr = cursorRect();
      cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
      c->complete(cr);
      return;
    }
    else if (isShortcut == true) {
      static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
      bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
      QString completionPrefix = textUnderCursor();
      if (completionPrefix[0] == '<') {
          completionPrefix = completionPrefix.remove(0,1);
      }
      
      if (completionPrefix != c->completionPrefix()) {
         c->setCompletionPrefix(completionPrefix);
         c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
      }
     
      QRect cr = cursorRect();
      cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
      c->complete(cr);
      return;
    }
    
    if (((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return)) && (c->popup()->isVisible() == false)) {
	insertTabulation();
        this->setFocus();
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
    else if ((e->key() == Qt::Key_Space) && (isInAtribute) && (defaultCompletion == true)) {
      QTextCursor tc = textCursor();
      tc.insertText(" ");
      int currentPos = tc.position();
      tc.select(QTextCursor::LineUnderCursor);
      QString tag = HtmlParser::getTag(tc.selectedText());
      if (tag[0] == '<')
	tag = tag.remove(0,1);
      tc.setPosition(currentPos);
      fillTmpList(tag.toUpper());
      defaultCompletion = false;
      QRect cr = cursorRect();
      cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
      c->complete(cr); 
      return;
    }
    else if (e->key() == Qt::Key_Space) {
      QTextCursor tc = textCursor();
      tc.insertText(" ");
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
    
    QTextEdit::keyPressEvent(e);
}
 
void RtfHtmlEditor::findText() {
 slotFind(); 
}