#ifndef TAG_EDITOR
#define TAG_EDITOR

#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHash>
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
    AttrComboBox* createAttribute(QString name);
    QTreeWidget* subTagTree;
    QHash<QString, QTreeWidgetItem*> hshAttribute;
  private slots:
    void setTagAttribute(QString attribute, QString text);
  public slots:
    void displayAttribute(QString tag);
  signals:
    void setAttribute(QString name,QString value);
};

#endif // TRANSLATEE11004_H