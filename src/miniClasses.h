#ifndef MINI_CLASSES
#define MINI_CLASSES
#include <QTreeWidgetItem>

class TreeItem : public QTreeWidgetItem {
  public:
    TreeItem(QTreeWidget* parent) : QTreeWidgetItem (parent),type(0) {}
    TreeItem(QTreeWidgetItem* parent) : QTreeWidgetItem (parent) {}
    int type; //0 = folder, 1 = HTML page
};
#endif