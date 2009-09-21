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
#include "cssParser.h"
#include "qtpropertybrowser-2.5-opensource/src/QtProperty"
#include "qtpropertybrowser-2.5-opensource/src/QtStringPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtEnumPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtGroupPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtTreePropertyBrowser"
#include "qtpropertybrowser-2.5-opensource/src/QtBoolPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtLineEditFactory"
#include "qtpropertybrowser-2.5-opensource/src/QtEnumEditorFactory"
#include "qtpropertybrowser-2.5-opensource/src/QtGroupPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtBrowserItem"
#include "qtpropertybrowser-2.5-opensource/src/QtBoolPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtLineEditFactory"
#include "qtpropertybrowser-2.5-opensource/src/QtEnumEditorFactory"
#include "qtpropertybrowser-2.5-opensource/src/QtGroupPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtBrowserItem"

enum PropertyType {
  STRING,
  BOOL,
  COMBOBOX,
  NUMBER,
  CSS
};

struct Property {
  QtProperty* propPtr;
  PropertyType type;
};

struct CbbProperty : public Property {
  QStringList valueList;
  bool edited;
};

typedef QHash<QString, Property*> PropertiesHash;

class TagEditor : public QDockWidget {
  Q_OBJECT
  public:
    TagEditor(QWidget* parent);
  private:
    void loadTagAttr(QString tagName);
    Property* createProperty(QString &attr);
    void clear();
    void createStyleMetaProp(QtProperty* parentProperty, PropertiesHash &parentHsh);
    void expandTopProperty(QtProperty* aProp, bool expand);
    void setStyleContent(PropertiesHash* toFill, QStringList content);
    PropertiesHash hshStyle;
    PropertiesHash hshStyleId;
    PropertiesHash hshStyleClass;
    PropertiesHash hshStd;
    PropertiesHash hshSpecific;
    PropertiesHash hshEvent;
    QList<Property*> lstModified;
    QtStringPropertyManager* stringPropManager;
    QtStringPropertyManager* cssPropManager;
    QtEnumPropertyManager* cbbPropManager;
    QtGroupPropertyManager* groupManager;
    QtCssEditFactory* cssFactory;
    QtProperty* metaPropStd;
    QtProperty* metaPropSpecific;
    QtProperty* metaPropStyle;
    QtProperty* metaPropIdStyle;
    QtProperty* metaPropClassStyle;
    QStringList normalAttr;
    QStringList styleAttr;
    QStringList specificAttr;
    QString currentTag;
    QtTreePropertyBrowser* htmlPropertyBrowser;
    bool idStyleActive;
    bool classStyleActive;
  private slots:
    void setStringAttr(QtProperty* property, const QString& value);
    void setCbbAttr(QtProperty* property, const int value);
    void setStyleAttr(QtProperty* property, const QString& value);
  public slots:
    void displayAttribute(QString tag);
  signals:
    void setAttribute(QString name,QString value);
};

#endif // TRANSLATEE11004_H