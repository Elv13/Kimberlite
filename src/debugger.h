#ifndef DEBUGGEUR
#define DEBUGGEUR
#include <QString>
#include <QVector>
#include "htmlParser.h"

class debugItem {
  public:
    debugItem(int _line, int _icon, QString _message) : line(_line), icon(_icon), message(_message) {}
    debugItem() {};
    int line;
    int icon; /*0 = warning, 1 = error, 3 = information*/
    int character;
    QString message;
};

class HtmlDebugger {
  public:
    static QVector<debugItem> debug(HtmlData& pageData);
};

#endif