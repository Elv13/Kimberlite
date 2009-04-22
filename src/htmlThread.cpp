#include "htmlThread.h"

ParserThread::ParserThread(QObject *parent) : QThread(parent),needUpdate(false),timeOver(false) {
  aTimer = new QTimer(this);
  connect(aTimer, SIGNAL(timeout()), this, SLOT(timerDone()));
}

void ParserThread::getReady() {
  aTimer->start(2000);
  needUpdate = true;
}

void ParserThread::run() {
  while (true) {
    if ((timeOver == true) && (needUpdate == true)) {
      needUpdate = false;
      timeOver = false;
      HtmlData someData = HtmlParser::getHtmlData(rtfHtml->toPlainText());
      updateHtmlTree(someData);
      treeHtml->expandAll();
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
  IndexedTreeWidgetItem* previousNode(NULL);
  IndexedTreeWidgetItem* aNode(NULL);
  int index(0), size(pageData.tagList.size());
  treeHtml->clear();
  for (int j=0; j < size;j++) {
    if (pageData.levelList[j] == 0)
      aNode = new IndexedTreeWidgetItem(treeHtml,QStringList(pageData.tagList[j]),index);
    else if (pageData.levelList[j] > pageData.levelList[(j > 0)?j-1:0]) 
      aNode = new IndexedTreeWidgetItem(previousNode,QStringList(pageData.tagList[j]),index);
    else if (pageData.levelList[j] == pageData.levelList[(j > 0)?j-1:0]) 
      aNode = new IndexedTreeWidgetItem(previousNode->parent(),QStringList(pageData.tagList[j]),index);
    else 
      aNode = new IndexedTreeWidgetItem(previousNode->parent()->parent(),QStringList(pageData.tagList[j]),index);
    previousNode = aNode;
    index += pageData.tagList[j].count()+1 + (3*(pageData.levelList[j+((size-1!=j)?1:0)]));
  }
  treeHtml->expandAll();
}