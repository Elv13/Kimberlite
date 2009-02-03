#ifndef PROJECT_MANAGER2
#define PROJECT_MANAGER2

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class ProjectManager2 : public QTreeWidget {
    Q_OBJECT

  public:
    ProjectManager2(QWidget *parent = 0);
    bool read(QIODevice *device);
    bool write(QIODevice *device);

  private slots:
    void updateDomElement(QTreeWidgetItem *item, int column);
    void test(QTreeWidgetItem* item, QString text);

  private:
    void parseFolderElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    QTreeWidgetItem* createItem(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    void parseProjectElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    void parseHtmlElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    void parseScriptElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    void parseRessourcesElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    QString toHTML(QString input); /*Those method are used to prevent damaging the project file with invalid HTML*/
    QString fromHTML(QString input);/*Those method are used to prevent damaging the project file with invalid HTML*/
    
    QDomDocument domDocument;
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QIcon folderIcon;
    QIcon bookmarkIcon;
    QString styleSheethName;
    
  signals:
    void htmlPageChanged(QTreeWidgetItem*,QString);
};
QT_END_NAMESPACE
#endif
