#ifndef HTML_THREAD2
#define HTML_THREAD2
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <KTextEdit>
#include <QTreeWidget>
#include "htmlParser.h"
#include "cssParser.h"

class ParserThread : public QThread {
    Q_OBJECT

    public:
      ParserThread(QObject *parent = 0);
      ~ParserThread() {}
      void run();
      void updateClassTree(bool useDefault = false);
      KTextEdit* rtfHtml;
      KTextEdit* rtfCss;
    public slots:
      void getReady(uint time = 2000);
      void timerDone();
      void getReadyCss();
      void timerDoneCss();
    private:
      QTimer* aTimer;
      QTimer* aTimerCss;
      bool needUpdate;
      bool needUpdateCss;
      bool timeOver;
      bool timeOverCss;
      void updateHtmlTree(HtmlData &pageData);
    signals:
      void updateTree(IndexedTreeWidgetItem*,bool);
      void updateCssTree(QTreeWidgetItem*,bool);
};
#endif
