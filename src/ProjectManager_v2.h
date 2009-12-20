#ifndef PROJECT_MANAGER2
#define PROJECT_MANAGER2

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>
#include <QByteArray>
#include "miniClasses.h"

QT_BEGIN_NAMESPACE
class ProjectManager2 : public QTreeWidget {
    Q_OBJECT

  public:
    ProjectManager2(QWidget *parent = 0);
    bool read(QIODevice *device);
    bool write(QIODevice *device);
    QDomElement getDomElement(QTreeWidgetItem* anItem);
    QDomDocument* getDomDocument();
    void setProjectName(QString name);
    TreeItem* htmlPage;
    TreeItem* script;
    QString projectTitle;
    void exportProject(QString path,QTreeWidgetItem* parent =0);
    QString getProjectName();

  public slots:
    void updateDomElement(QTreeWidgetItem *item, QString html);
    void loadPage(QTreeWidgetItem* anItem);
    void addHtmlPage(QString title, QString name, QString body = "", QString foldeName = "", int type=0);
    void addScript(QString name, QString language, QString foldeName);
    void addFolder(QString title, QTreeWidgetItem* parent);
    void saveCss();
    QByteArray createFile();
    QByteArray readFile(QIODevice *device);

  private:
    void parseFolderElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    TreeItem* createItem(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    void parseProjectElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    void parseHtmlElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    void parseScriptElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    void parseRessourcesElement(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
    QString toHTML(QString input); /*Those method are used to prevent damaging the project file with invalid HTML*/
    QString fromHTML(QString input);/*Those method are used to prevent damaging the project file with invalid HTML*/
    QDomNode getElement(QDomNode &aNode, QString tagName, QString attribute, QString value);
    QTreeWidgetItem* getFolder(QString title);
    bool firstPage;
    
    QDomDocument domDocument;
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QIcon folderIcon;
    QIcon bookmarkIcon;
    QString styleSheethName;
    QDomElement cssPage;
    QHash<QString, QByteArray> fileArray;
    
  signals:
    void htmlPageChanged(QTreeWidgetItem*,QString);
    void javaScriptChanged(QTreeWidgetItem*,QString);
    void loadCss(QString);
};
QT_END_NAMESPACE
#endif
