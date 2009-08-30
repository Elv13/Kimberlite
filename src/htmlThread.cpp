#include "htmlThread.h"
#include "../mainwindow.h"

ParserThread::ParserThread(QObject *parent) : QThread(parent),needUpdate(false),timeOver(false) {
  aTimer = new QTimer(this);
  aTimerCss = new QTimer(this);
  connect(aTimer, SIGNAL(timeout()), this, SLOT(timerDone()));
  connect(aTimerCss, SIGNAL(timeout()), this, SLOT(timerDoneCss()));
}

void ParserThread::getReady(uint time) {
  aTimer->start(time);
  needUpdate = true;
}

void ParserThread::run() {
  while (true) {
    if ((timeOver == true) && (needUpdate == true)) {
      needUpdate = false;
      timeOver = false;
      HtmlData someData = HtmlParser::getHtmlData(rtfHtml->toPlainText());
      updateHtmlTree(someData);
    }
    else if ((timeOverCss == true) && (needUpdateCss == true)) { //They will never be true at the same time
      needUpdateCss = false;
      timeOverCss = false;
      updateClassTree();
    }
    else
      sleep(1);
  }
}

void ParserThread::timerDone() {
  timeOver = true;
  aTimer->stop();
}

void ParserThread::updateHtmlTree(HtmlData &pageData) {
  for (int i=0;i<pageData.tagList.size();i++)
    if (pageData.tagList[i].left(5).toUpper() =="<?PHP")
      pageData.tagList[i] = "<?php CODE ?>";
    else if ((pageData.tagList[i].left(7).toUpper() =="<SCRIPT") && (pageData.tagList.size() > i+2))
      if (pageData.tagList[i+2].toUpper() == "</SCRIPT>")
	pageData.tagList[i+1] = "JavaScript code";
  IndexedTreeWidgetItem* previousNode(NULL);
  IndexedTreeWidgetItem* aNode(NULL);
  int index(0), size(pageData.tagList.size());
  QVector<IndexedTreeWidgetItem*> topLvlItem;
  for (int j=0; j < size;j++) {
    if (pageData.levelList[j] == 0) {
      aNode = new IndexedTreeWidgetItem((QTreeWidget*)0,QStringList(pageData.tagList[j]),index);
      topLvlItem.push_back(aNode);
    }
    else if (pageData.levelList[j] > pageData.levelList[(j > 0)?j-1:0]) 
      aNode = new IndexedTreeWidgetItem(previousNode,QStringList(pageData.tagList[j]),index);
    else if (pageData.levelList[j] == pageData.levelList[(j > 0)?j-1:0]) 
      aNode = new IndexedTreeWidgetItem(previousNode->parent(),QStringList(pageData.tagList[j]),index);
    else 
      aNode = new IndexedTreeWidgetItem(previousNode->parent()->parent(),QStringList(pageData.tagList[j]),index);
    previousNode = aNode;
    index += pageData.tagList[j].count()+1 + (3*(pageData.levelList[j+((size-1!=j)?1:0)]));
  }
  
  for (int i=0; i<topLvlItem.size();i++)
    emit updateTree(topLvlItem[i],(i)?false:true);
}

void ParserThread::getReadyCss() {
  aTimerCss->start(2000);
  needUpdateCss = true;
}

void ParserThread::timerDoneCss() {
  timeOverCss = true;
  aTimerCss->stop();
}

void ParserThread::updateClassTree(bool useDefault) {
  if (!useDefault)
    CssParser::cssFile = rtfCss->toPlainText();
  QTreeWidgetItem* anItem =  new QTreeWidgetItem();
  anItem->setText(0,"Style");
  
  QStringList classList = CssParser::getClassList();

  for (int j = 0; j < classList.count(); j++) {
    MainWindow::splitSubClass(classList[j], anItem);
  }
  emit updateCssTree(anItem,true);
} //updateClassTree