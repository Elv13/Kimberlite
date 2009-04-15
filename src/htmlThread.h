#ifndef HTML_THREAD2
#define HTML_THREAD2
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <KTextEdit>
#include <QTreeWidget>
#include "htmlParser.h"

class ParserThread : public QThread {
    Q_OBJECT

    public:
        ParserThread(QObject *parent = 0);
	~ParserThread() {}
        void run();
	KTextEdit* rtfHtml;
	QTreeWidget* treeHtml;
    public slots:
      void getReady();
      void timerDone();
    private:
      QTimer* aTimer;
      bool needUpdate;
      bool timeOver;
      void updateHtmlTree(HtmlData &pageData);

};
#endif