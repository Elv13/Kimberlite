#ifndef MINI_CLASSES
#define MINI_CLASSES
#include <QTreeWidgetItem>
#include "ProjectManager_v2.h"

class TreeItem : public QTreeWidgetItem {
  public:
    TreeItem(QTreeWidget* parent) : QTreeWidgetItem (parent),type(0) {}
    TreeItem(QTreeWidgetItem* parent) : QTreeWidgetItem (parent) {}
    int type; //0 = folder, 1 = HTML page
};

enum PageType {
  HTMLv4 =1,
  HTMLv5 =2,
  xHTMLv1 =3,
  PHP4 = 4,
  PHP5 = 5
};
#endif