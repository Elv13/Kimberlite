#include "tagEditor.h"

TagEditor::TagEditor(QWidget* parent) : QDockWidget(parent) {
  setWindowTitle("Tag Editor");
  QWidget* centralWidget = new QWidget(this);
  setWidget(centralWidget);
  QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
  subTagTree = new QTreeWidget(centralWidget);
  mainLayout->addWidget(subTagTree);
  subTagTree->setColumnCount(2);
  QTreeWidgetItem* anItem = new QTreeWidgetItem();
  anItem->setText(0,"id");
  QLineEdit* aLineEdit = new QLineEdit(subTagTree);
  
  subTagTree->addTopLevelItem(anItem);
  subTagTree->setItemWidget(anItem,1,aLineEdit);
  
  createAttribute("id");
  createAttribute("name");
  createAttribute("class");
}

AttrComboBox* TagEditor::createAttribute(QString name) {
  QTreeWidgetItem* anItem = new QTreeWidgetItem();
  anItem->setText(0,name);
  AttrComboBox* aLineEdit = new AttrComboBox(subTagTree);
  aLineEdit->attribute = name;
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
  }
  //aLineEdit->setText(tag);
  if (HtmlParser::getAttribute(tag,"id") != NULL) {
    if (hshAttribute.find("id") == hshAttribute.end())
      createAttribute("id");
    ((AttrComboBox*)subTagTree->itemWidget(hshAttribute["id"],1))->lineEdit()->setText(HtmlParser::getAttribute(tag,"id"));
  }
}

void TagEditor::setTagAttribute(QString attribute, QString text) {
  emit setAttribute(attribute,text);
}