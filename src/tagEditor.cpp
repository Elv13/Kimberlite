#include "tagEditor.h"
#include <QtSql>
#include "qtpropertybrowser-2.5-opensource/src/QtBoolPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtLineEditFactory"
#include "qtpropertybrowser-2.5-opensource/src/QtEnumEditorFactory"
#include "qtpropertybrowser-2.5-opensource/src/QtGroupPropertyManager"
#include "qtpropertybrowser-2.5-opensource/src/QtBrowserItem"

TagEditor::TagEditor(QWidget* parent) : QDockWidget(parent) {
  setWindowTitle("Tag Editor");
  QWidget* centralWidget = new QWidget(this);
  setWidget(centralWidget);
  QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->setContentsMargins(0,0,0,0);
  resize(250,250);
  
  htmlPropertyBrowser = new QtTreePropertyBrowser(this);
  htmlPropertyBrowser->setAlternatingRowColors(true);
  mainLayout->addWidget(htmlPropertyBrowser);
  
  QtGroupPropertyManager *groupManager = new QtGroupPropertyManager(this);
  
  metaPropStd = groupManager->addProperty("Standard Properties");
  htmlPropertyBrowser->addProperty(metaPropStd);
  
  metaPropSpecific = groupManager->addProperty("Specific Properties");
  htmlPropertyBrowser->addProperty(metaPropSpecific);
  
  metaPropStyle = groupManager->addProperty("Embeded Style");
  htmlPropertyBrowser->addProperty(metaPropStyle);
  
  stringPropManager = new QtStringPropertyManager(this);
  cbbPropManager = new QtEnumPropertyManager(this);
  
  connect(stringPropManager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(setStringAttr(QtProperty*,QString)));
  connect(cbbPropManager, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setCbbAttr(QtProperty*,int)));
  QtLineEditFactory* lineEditFactory = new QtLineEditFactory(this);
  QtEnumEditorFactory* comboBoxFactory = new QtEnumEditorFactory(this);
  
  htmlPropertyBrowser->setFactoryForManager(stringPropManager, lineEditFactory);
  htmlPropertyBrowser->setFactoryForManager(cbbPropManager, comboBoxFactory);

  normalAttr << "id" << "name" << "class";
  
  foreach (QString attr, normalAttr) {
    hshStd[attr] = stringPropManager->addProperty(attr);
    metaPropStd->addSubProperty(hshStd[attr]);
  }
  
  QSqlQuery query22;
  query22.exec("SELECT TITLE FROM TCSS_TAG");
  
  while (query22.next())
    styleAttr <<  query22.value(0).toString();
  
  styleAttr.sort();
  
  QStringList lstCssGroup;
  lstCssGroup << "background" << "margin" << "padding" << "border" << "font" << "text" << "outline" << "list";
  
  QHash<QString, QtProperty*> hshTagGroup;

  foreach (QString group, lstCssGroup) {
    hshTagGroup[group] = stringPropManager->addProperty(group);
    metaPropStyle->addSubProperty(hshTagGroup[group]);
  }
  
  QtProperty *grpPropBorderLeft = groupManager->addProperty("left");
  hshTagGroup["border"]->addSubProperty(grpPropBorderLeft);
  
  QtProperty *grpPropBorderRight = groupManager->addProperty("right");
  hshTagGroup["border"]->addSubProperty(grpPropBorderRight);
  
  QtProperty *grpPropBorderTop = groupManager->addProperty("top");
  hshTagGroup["border"]->addSubProperty(grpPropBorderTop);
  
  QtProperty *grpPropBorderBottom = groupManager->addProperty("bottom");
  hshTagGroup["border"]->addSubProperty(grpPropBorderBottom);
  
  foreach (QString attr, styleAttr) {
    QtProperty *item3;
    bool found = false;
    foreach (QString group, lstCssGroup) {
      if (attr.indexOf(group+"-") == 0) {
	if (group == "border") {
	  if (attr.indexOf(group+"-left-") == 0) {
	    item3 = stringPropManager->addProperty(attr.right(attr.size()-group.size()-6));
	    grpPropBorderLeft->addSubProperty(item3);
	  }
	  else if (attr.indexOf(group+"-right-") == 0) {
	    item3 = stringPropManager->addProperty(attr.right(attr.size()-group.size()-7));
	    grpPropBorderRight->addSubProperty(item3);
	  }
	  else if (attr.indexOf(group+"-top-") == 0) {
	    item3 = stringPropManager->addProperty(attr.right(attr.size()-group.size()-5));
	    grpPropBorderTop->addSubProperty(item3);
	  }
	  else if (attr.indexOf(group+"-bottom-") == 0) {
	    item3 = stringPropManager->addProperty(attr.right(attr.size()-group.size()-8));
	    grpPropBorderBottom->addSubProperty(item3);
	  }
	  else {
	    item3 = stringPropManager->addProperty(attr.right(attr.size()-group.size()-1));
	    hshTagGroup[group]->addSubProperty(item3);
	  }
	}
	else {
	  item3 = stringPropManager->addProperty(attr.right(attr.size()-group.size()-1));
	  hshTagGroup[group]->addSubProperty(item3);
	}
	found = true;
	break;
      }
    }
    
    if (!found) {
      item3 = stringPropManager->addProperty(attr);
      metaPropStyle->addSubProperty(item3);
    }
    hshStyle[attr] = item3;
  }
  
  QList<QtBrowserItem*> itemList =  htmlPropertyBrowser->topLevelItems();
  foreach (QtBrowserItem* item, itemList) {
    if (item->property() == metaPropStyle) {
      htmlPropertyBrowser->setExpanded(item,false);
      break;
    }
  }  
}

void TagEditor::displayAttribute(QString tag) {
  if (this->isVisible()) {
    if ((tag.trimmed().left(2) == "</") || ((tag.trimmed()[0] != '<'))) {
      htmlPropertyBrowser->setDisabled(true);
    }
    else {
      htmlPropertyBrowser->setDisabled(false);
      loadTagAttr(HtmlParser::getTag(tag.trimmed()));
    }
    
    foreach (QString attr, normalAttr)
      if (HtmlParser::getAttribute(tag,attr) != NULL)
	((QtStringPropertyManager*)hshStd[attr]->propertyManager())->setValue(hshStd[attr],HtmlParser::getAttribute(tag,attr));
      else
	((QtStringPropertyManager*)hshStd[attr]->propertyManager())->setValue(hshStd[attr],"");
	
      
    foreach (QString attr, specificAttr)
      if (HtmlParser::getAttribute(tag,attr) != NULL) {
	qDebug() << "Setting arg " << attr;
	if (hshSpecific[attr]->type == STRING) {
	  qDebug() << "Its a string";
	  ((QtStringPropertyManager*)hshSpecific[attr]->propPtr->propertyManager())->setValue(hshSpecific[attr]->propPtr,HtmlParser::getAttribute(tag,attr));
	}
	else if (hshSpecific[attr]->type == COMBOBOX) {
	  qDebug() << "Its a cbb";
	  int index;
	  if (((CbbProperty*)hshSpecific[attr])->valueList.indexOf(HtmlParser::getAttribute(tag,attr)) != -1)
	    index = ((CbbProperty*)hshSpecific[attr])->valueList.indexOf(HtmlParser::getAttribute(tag,attr))+1;
	  else {
	    qDebug() << "Value not found";
	    if (((CbbProperty*)hshSpecific[attr])->edited == false)
	      ((CbbProperty*)hshSpecific[attr])->valueList << HtmlParser::getAttribute(tag,attr);
	    else
	      ((CbbProperty*)hshSpecific[attr])->valueList[((CbbProperty*)hshSpecific[attr])->valueList.size()-1] = HtmlParser::getAttribute(tag,attr);
	    ((CbbProperty*)hshSpecific[attr])->edited = true;
	    QStringList value = ((CbbProperty*)hshSpecific[attr])->valueList;
	    value.insert(0,"-none-");
	    disconnect(cbbPropManager, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setCbbAttr(QtProperty*,int)));

	    cbbPropManager->setEnumNames(hshSpecific[attr]->propPtr,value);
	    connect(cbbPropManager, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setCbbAttr(QtProperty*,int)));
	    index = value.size()-1;
	  }
	  qDebug() <<"settins combo "<<index;
	  ((QtEnumPropertyManager*)hshSpecific[attr]->propPtr->propertyManager())->setValue(hshSpecific[attr]->propPtr,index);
	}
      }
      else
	((QtStringPropertyManager*)hshSpecific[attr]->propPtr->propertyManager())->setValue(hshSpecific[attr]->propPtr,"");
  }
}

void TagEditor::loadTagAttr(QString tagName) { 
  if (tagName != currentTag) { //BUG WILL not always work
    
    foreach (Property* aProp, hshSpecific) {
      metaPropSpecific->removeSubProperty(aProp->propPtr);
    }

    
    specificAttr.clear();
    
    QSqlQuery query23;
    query23.exec("SELECT PROPRIETIES FROM THTML_TAG WHERE NAME ='" + tagName.toUpper() +"'" );
      
    QString attrList;
    query23.next();
    attrList = query23.value(0).toString().toLower();
    
    specificAttr = attrList.split(';');
    
    foreach (QString attr, specificAttr) {
      QtProperty *item3;
      
      if (attr.indexOf("(") != -1) {
	hshSpecific[attr.left(attr.indexOf("("))] = new CbbProperty;
	((CbbProperty*)hshSpecific[attr.left(attr.indexOf("("))])->valueList = attr.mid(attr.indexOf("(")+1, attr.size() - attr.indexOf("(") -2).split(',');
	specificAttr[specificAttr.indexOf(attr)]=attr.left(attr.indexOf("("));
	attr = attr.left(attr.indexOf("("));
	hshSpecific[attr]->type = COMBOBOX;
	QStringList value = ((CbbProperty*)hshSpecific[attr])->valueList;
	((CbbProperty*)hshSpecific[attr])->edited = false;
	value.insert(0,"-none-");
	
	item3 = cbbPropManager->addProperty(attr);
	cbbPropManager->setEnumNames(item3,value);
	
      }
      else {
	hshSpecific[attr] = new Property;
	item3 = stringPropManager->addProperty(attr);
	hshSpecific[attr]->type = STRING;
      }
      hshSpecific[attr]->propPtr = item3;
      metaPropSpecific->addSubProperty(item3);
    }
      
    currentTag = tagName;
  }
}

void TagEditor::setStringAttr(QtProperty* property, const QString& value) {
      hshStyle;//TODO
  foreach (Property* aProp, hshSpecific) {
    if ((property == aProp->propPtr) && (!value.isEmpty())) {
      emit setAttribute(hshSpecific.key(aProp),value);
    }
  }
  
  foreach (QtProperty* aProp, hshStd) {
    if ((property == aProp) && (!value.isEmpty())) {
      emit setAttribute(hshStd.key(aProp),value);
    }
  }
}

void TagEditor::setCbbAttr(QtProperty* property, const int value) {
  foreach (Property* aProp, hshSpecific) {
    if (property == aProp->propPtr) {
      if (!value)
	emit setAttribute(hshSpecific.key(aProp),"");
      else
	emit setAttribute(hshSpecific.key(aProp),((CbbProperty*)aProp)->valueList[value-1]);
    }
  }
}