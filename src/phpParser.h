#ifndef PHP_PARSER
#define PHP_PARSER
#include <QStringList>
#include <QHash>
#include "htmlParser.h"

class PhpParser{
  public:
    static QHash<int, QString> extractPhp(HtmlData &page);
    static void restorePhp(HtmlData &page, QHash<int, QString> snipet);
    static void testReplacePhp(HtmlData &page);
};
#endif