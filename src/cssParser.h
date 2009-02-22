#ifndef CSS_PARSER
#define CSS_PARSER
#include <QStringList>
#include <QtSql>

class CssParser{
  public:
    static QString setClass(QString className, QString content);
    static QStringList getClassList();
    static QString getUnit(QString line);
    static QString getValue(QString line);
    static QStringList getClass(QString className);
    static QString getPropriety(QString line);
    static QString parseCSS();
    static QString cssFile;
};
#endif