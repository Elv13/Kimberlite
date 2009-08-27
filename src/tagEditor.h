#ifndef TAG_EDITOR
#define TAG_EDITOR

#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHash>
#include <KPushButton>
#include <QStringList>
#include "htmlParser.h"

class AttrComboBox : public QComboBox {
  Q_OBJECT
  public:
    AttrComboBox(QWidget* parent = 0) : QComboBox(parent) {
      setEditable(true);
      connect(lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(getNewText(QString)));
    }
    QString attribute;
  private slots:
    void getNewText(QString text) {
      emit textChanged(attribute,text);
    }
  signals:
    void textChanged(QString, QString);
};

class TagEditor : public QDockWidget {
  Q_OBJECT
  public:
    TagEditor(QWidget* parent);
  private:
    AttrComboBox* createAttribute(QString name, QTreeWidgetItem* parent = NULL);
    void loadTagAttr(QString tagName);
    QTreeWidget* subTagTree;
    QHash<QString, QTreeWidgetItem*> hshAttribute;
    KPushButton* btnExecute;
    QStringList normalAttr;
    QStringList styleAttr;
    QStringList specificAttr;
    QTreeWidgetItem* tviSpecific;
    QString currentTag;
  private slots:
    void setTagAttribute(QString attribute, QString text);
    void setAttrTest();
  public slots:
    void displayAttribute(QString tag);
  signals:
    void setAttribute(QString name,QString value);
};

#endif // TRANSLATEE11004_H