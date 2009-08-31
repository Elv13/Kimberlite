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
#include <QPainter>
#include <QItemDelegate>
#include "htmlParser.h"
#include "qtpropertybrowser-2.5-opensource/src/QtProperty"
#include "qtpropertybrowser-2.5-opensource/src/QtStringPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtEnumPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtTreePropertyBrowser"

enum PropertyType {
  STRING,
  BOOL,
  COMBOBOX,
  NUMBER
};

struct Property {
  QtProperty* propPtr;
  PropertyType type;
};

struct CbbProperty : public Property {
  QStringList valueList;
  bool edited;
};

class TagEditor : public QDockWidget {
  Q_OBJECT
  public:
    TagEditor(QWidget* parent);
  private:
    void loadTagAttr(QString tagName);
    Property* createProperty(QString &attr);
    QHash<QString, QtProperty*> hshStyle;
    QHash<QString, Property*> hshStd;
    QHash<QString, Property*> hshSpecific;
    QHash<QString, Property*> hshEvent;
    QtStringPropertyManager* stringPropManager;
    QtEnumPropertyManager* cbbPropManager;
    QtProperty* metaPropStd;
    QtProperty* metaPropSpecific;
    QtProperty* metaPropStyle;
    QStringList normalAttr;
    QStringList styleAttr;
    QStringList specificAttr;
    QString currentTag;
    QtTreePropertyBrowser* htmlPropertyBrowser;
  private slots:
    void setStringAttr(QtProperty* property, const QString& value);
    void setCbbAttr(QtProperty* property, const int value);
  public slots:
    void displayAttribute(QString tag);
  signals:
    void setAttribute(QString name,QString value);
};

#endif // TRANSLATEE11004_H