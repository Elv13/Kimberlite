#include "tagEditor.h"
#include <QtSql>

TagEditor::TagEditor(QWidget* parent) : QDockWidget(parent) {
  setWindowTitle("Tag Editor");
  QWidget* centralWidget = new QWidget(this);
  setWidget(centralWidget);
  QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->setContentsMargins(0,0,0,0);
  subTagTree = new QTreeWidget(centralWidget);
  /*subTagTree->setStyleSheet("QTreeView::item {\
      border: 1px solid #d9d9d9;\
     border-top-color: transparent;\
     margin:0px;\
 }\
");*/
  mainLayout->addWidget(subTagTree);
  subTagTree->setColumnCount(2);
  resize(250,250);
  
  QTreeWidgetItem* tviHeader1 = new QTreeWidgetItem();
  tviHeader1->setText(0,"Attribute");
  tviHeader1->setText(1,"Value");
  subTagTree->setHeaderItem(tviHeader1);
  
  btnExecute = new KPushButton(this);
  btnExecute->setText("Exec");
  mainLayout->addWidget(btnExecute);
  connect(btnExecute, SIGNAL(clicked()), this, SLOT(setAttrTest()));
  
  QTreeWidgetItem* tviStandard = new QTreeWidgetItem();
  tviStandard->setText(0,"Standard");
  
  subTagTree->addTopLevelItem(tviStandard);
  normalAttr << "id" << "name" << "class";
  foreach (QString attr, normalAttr)
    createAttribute(attr,tviStandard);
  tviStandard->setExpanded(true);
  
  tviSpecific = new QTreeWidgetItem();
  tviSpecific->setText(0,"Specific");
  subTagTree->addTopLevelItem(tviSpecific);
  
  QSqlQuery query22;
  query22.exec("SELECT TITLE FROM TCSS_TAG");
  
  while (query22.next())
    styleAttr <<  query22.value(0).toString();
  
  styleAttr.sort();
  
  QTreeWidgetItem* tviStyle = new QTreeWidgetItem();
  tviStyle->setText(0,"StyleSheet");
  subTagTree->addTopLevelItem(tviStyle);
  
  foreach (QString attr, styleAttr)
    createAttribute(attr,tviStyle);
  
}

AttrComboBox* TagEditor::createAttribute(QString name, QTreeWidgetItem* parent) {
  QTreeWidgetItem* anItem = new QTreeWidgetItem(parent);
  anItem->setText(0,name);
  AttrComboBox* aLineEdit = new AttrComboBox(subTagTree);
  aLineEdit->attribute = name;
  if (!parent)
    subTagTree->addTopLevelItem(anItem);
  subTagTree->setItemWidget(anItem,1,aLineEdit);
  hshAttribute[name] = anItem;
  connect(aLineEdit,SIGNAL(textChanged(QString,QString)),this,SLOT(setTagAttribute(QString,QString)));
  return aLineEdit;
}

void TagEditor::displayAttribute(QString tag) {
  if (tag.trimmed().left(2) == "</") {
    subTagTree->setDisabled(true);
  }
  else {
    subTagTree->setDisabled(false);
    loadTagAttr(HtmlParser::getTag(tag.trimmed()));
  }
  //aLineEdit->setText(tag);
  foreach (QString attr, normalAttr)
    if (HtmlParser::getAttribute(tag,attr) != NULL) {
      if (hshAttribute.find(attr) == hshAttribute.end())
	createAttribute(attr);
      ((AttrComboBox*)subTagTree->itemWidget(hshAttribute[attr],1))->lineEdit()->setText(HtmlParser::getAttribute(tag,attr));
    }
    
  foreach (QString attr, specificAttr)
    if (HtmlParser::getAttribute(tag,attr) != NULL) {
      if (hshAttribute.find(attr) == hshAttribute.end())
	createAttribute(attr);
      ((AttrComboBox*)subTagTree->itemWidget(hshAttribute[attr],1))->lineEdit()->setText(HtmlParser::getAttribute(tag,attr));
    }
}

void TagEditor::setAttrTest() {
  emit setAttribute("id",((AttrComboBox*)subTagTree->itemWidget(hshAttribute["id"],1))->lineEdit()->text());
}

void TagEditor::setTagAttribute(QString attribute, QString text) {
  emit setAttribute(attribute,text);
}

void TagEditor::loadTagAttr(QString tagName) {
  if (tagName != currentTag) {
    foreach (QString attr, specificAttr)
      hshAttribute.remove(attr);
    
    for (int i=0; i<tviSpecific->childCount();i++)
      tviSpecific->removeChild(tviSpecific->child(0));
    
    specificAttr.clear();
    
    QSqlQuery query23;
    query23.exec("SELECT PROPRIETIES FROM THTML_TAG WHERE NAME ='" + tagName.toUpper() +"'" );
      
    QString attrList;
    query23.next();
    attrList = query23.value(0).toString().toLower();
    
    specificAttr = attrList.split(';');
    
    foreach (QString attr, specificAttr)
      if (!attr.isEmpty())
	createAttribute(attr,tviSpecific);
      
    currentTag = tagName;
    tviSpecific->setExpanded(true);
  }
}