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

class RichTreeWidget : public QTreeWidget {
  Q_OBJECT
  public:
    RichTreeWidget(QWidget* parent) : QTreeWidget(parent) {}
  public:
    void drawRow(QPainter* p, const QStyleOptionViewItem &opt, const QModelIndex &idx) const {
      QTreeWidget::drawRow(p, opt, idx);
      for (int col = 0; col < columnCount(); ++col) {
	QModelIndex s = idx.sibling(idx.row(), col);
	if (s.isValid()) {
	  QRect rect = visualRect(s);
	  p->setPen(Qt::DotLine);
	  p->drawRect(rect);
	}
      }
    }
    /*void paint(QPainter* p, const QStyleOptionViewItem &opt, const QModelIndex &idx) const {
	QItemDelegate::paint(p, opt, idx);
	if (idx.isValid()) {
	    p->setPen(Qt::DotLine);
	    p->drawRect(opt.rect);
	}
    }*/
};

class TagEditor : public QDockWidget {
  Q_OBJECT
  public:
    TagEditor(QWidget* parent);
  private:
    AttrComboBox* createAttribute(QString name, QTreeWidgetItem* parent = NULL);
    void loadTagAttr(QString tagName);
    RichTreeWidget* subTagTree;
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