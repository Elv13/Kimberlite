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
  
  QtProperty* metaPropEvent = groupManager->addProperty("Events");
  
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

  normalAttr << "id" << "name" << "class" << "title" << "lang(american,ar,ar_aa,ar_sa,arabic,bg,bg_bg,bulgarian,c-french,c,c_c,cextend,chinese-s,chinese-t,croatian,cs,cs_cs,cs_cz,cz,cz_cz,czech,da,da_dk,danish,de,de_at,de_ch,de_de,dutch,ee,el,el_gr,en,en_au,en_ca,en_gb,en_ie,en_nz,en_uk,en_us,eng_gb,english,english_uk,english_united-states,english_us,es,es_ar,es_bo,es_cl,es_co,es_cr,es_ec,es_es,es_gt,es_mx,es_ni,es_pa,es_pe,es_py,es_sv,es_uy,es_ve,et,et_ee,fi,fi_fi,finnish,fr,fr_be,fr_ca,fr_ch,fr_fr,fre_fr,french,french_france,ger_de,german,german_germany,greek,hebrew,hr,hr_hr,hu,hu_hu,hungarian,icelandic,id,id_id,is,is_is,iso-8859-1,iso-8859-15,iso8859-1,iso8859-15,iso_8859_1,iso_8859_15,it,it_ch,it_it,italian,iw,iw_il,ja,ja.jis,ja.sjis,ja_jp,ja_jp.ajec,ja_jp.euc,ja_jp.eucjp,ja_jp.iso-2022-jp,ja_jp.jis,ja_jp.jis7,ja_jp.mscode,ja_jp.sjis,ja_jp.ujis,japan,japanese,japanese-euc,japanese.euc,jp_jp,ko,ko_kr,ko_kr.euc,korean,lt,lv,mk,mk_mk,nl,nl_be,nl_nl,no,no_no,norwegian,pl,pl_pl,polish,portuguese,portuguese_brazil,posix,posix-utf2,pt,pt_br,pt_pt,ro,ro_ro,ru,ru_ru,rumanian,russian,serbocroatian,sh,sh_hr,sh_sp,sh_yu,sk,sk_sk,sl,sl_cs,sl_si,slovak,slovene,sp,sp_yu,spanish,spanish_spain,sr_sp,sv,sv_se,swedish,th_th,tr,tr_tr,turkish,univ,universal,zh,zh_cn,zh_cn.big5,zh_cn.euc,zh_tw,zh_tw.euc)" << "dir(ltr,rtl)";
  QStringList evenList;
  evenList << "onclick" << "ondblclick" << "onmousedown" << "onmouseup" << "onmouseover" << "onmousemove" << "onmouseout" << "onkeypress" << "onkeyup" << "onkeydown";
  
  foreach (QString attr, evenList) {
    Property* aProp = createProperty(attr);
    hshEvent[attr] = aProp;
    metaPropEvent->addSubProperty(hshEvent[attr]->propPtr);
  }
  
  foreach (QString attr, normalAttr) {
    int idx = normalAttr.indexOf(attr);
    Property* aProp = createProperty(attr);
    if (idx != -1)
      normalAttr[idx] = attr;
    hshStd[attr] = aProp;
    metaPropStd->addSubProperty(hshStd[attr]->propPtr);
  }
  metaPropStd->addSubProperty(metaPropEvent);
  
  QSqlQuery query22;
  query22.exec("SELECT TITLE FROM TCSS_TAG");
  
  while (query22.next())
    styleAttr <<  query22.value(0).toString();
  
  styleAttr.sort();
  
  QStringList lstCssGroup;
  lstCssGroup << "background" << "margin" << "padding" << "border" << "font" << "text" << "outline" << "list";
  
  QHash<QString, QtProperty*> hshTagGroup;

  foreach (QString group, lstCssGroup) {
    QtProperty *item3;

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
    Property* aProp;
    aProp = new Property;
    hshStyle[attr] = aProp;
    hshStyle[attr]->propPtr = item3;
  }
  
  QList<QtBrowserItem*> itemList =  htmlPropertyBrowser->topLevelItems();
  foreach (QtBrowserItem* item, itemList) {
    if (item->property() == metaPropStyle) {
      htmlPropertyBrowser->setExpanded(item,false);
      //break;
    }
    else if (item->property() == metaPropStd) {
      foreach (QtBrowserItem* item2, item->children()) {
	if (item2->property() == metaPropEvent) {
	  htmlPropertyBrowser->setExpanded(item2,false);
	  break;
	}
      }
    }
  }
  
 /* itemList =  metaPropStd->children();
  foreach (QtBrowserItem* item, itemList) {
    if (item->property() == metaPropEvent) {
      htmlPropertyBrowser->setExpanded(item,false);
      break;
    }
  }*/
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
      if (HtmlParser::getAttribute(tag,attr) != NULL) {
	if (hshStd[attr]->type == STRING)
	  ((QtStringPropertyManager*)hshStd[attr]->propPtr->propertyManager())->setValue(hshStd[attr]->propPtr,HtmlParser::getAttribute(tag,attr));
	//else if (hshStd[attr]->type == COMBOBOX)
	  //((QtEnumPropertyManager*)hshStd[attr]->propPtr->propertyManager())->setValue(hshStd[attr]->propPtr,HtmlParser::getAttribute(tag,attr));
      }
      else {
	if (hshStd[attr]->type == STRING)
	  ((QtStringPropertyManager*)hshStd[attr]->propPtr->propertyManager())->setValue(hshStd[attr]->propPtr,"");
	else if (hshStd[attr]->type == COMBOBOX)
	  ((QtEnumPropertyManager*)hshStd[attr]->propPtr->propertyManager())->setValue(hshStd[attr]->propPtr,0);
      }
	
      
    foreach (QString attr, specificAttr)
      if (HtmlParser::getAttribute(tag,attr) != NULL) {
	if (hshSpecific[attr]->type == STRING) {
	  ((QtStringPropertyManager*)hshSpecific[attr]->propPtr->propertyManager())->setValue(hshSpecific[attr]->propPtr,HtmlParser::getAttribute(tag,attr));
	}
	else if (hshSpecific[attr]->type == COMBOBOX) {
	  int index;
	  if (((CbbProperty*)hshSpecific[attr])->valueList.indexOf(HtmlParser::getAttribute(tag,attr)) != -1)
	    index = ((CbbProperty*)hshSpecific[attr])->valueList.indexOf(HtmlParser::getAttribute(tag,attr))+1;
	  else {
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
 
      
    if (HtmlParser::getAttribute(tag,"style") != NULL) {
      QString content = HtmlParser::getAttribute(tag,"style");
      if (content.trimmed().right(1) != ";")
	content += ";";
      QStringList tagList = CssParser::getContent(content);
      foreach (QString property, tagList) {
	if (hshStyle.find(CssParser::getPropriety(property)) != hshStyle.end())
	  ((QtStringPropertyManager*)hshStyle[CssParser::getPropriety(property)]->propPtr->propertyManager())->setValue(hshStyle[CssParser::getPropriety(property)]->propPtr,CssParser::getValue(property));
      }
    }
  }
}

void TagEditor::loadTagAttr(QString tagName) { 
  //clear();
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
    int oldIdx = specificAttr.indexOf(attr);
    Property* aProp = createProperty(attr);
    hshSpecific[attr] = aProp;
    specificAttr[oldIdx] = attr;
    metaPropSpecific->addSubProperty(aProp->propPtr);
  }
      
  currentTag = tagName;
  
  }
}

void TagEditor::setStringAttr(QtProperty* property, const QString& value) {  
  QList< PropertiesHash* >  hshList;
  hshList << &hshStd << &hshSpecific << &hshEvent;
  foreach (PropertiesHash* anHsh, hshList) {
    foreach (Property* aProp, *anHsh) {
      if (property == aProp->propPtr) {
	emit setAttribute(anHsh->key(aProp),value);
	lstModified.push_back(aProp);
	return;
      }
    }
  }
  /*If it get here, it is a style prop*/
  QString accumulation;
  bool found = false;
  foreach (Property* aProp, hshStyle) {
    if (property == aProp->propPtr)
      found = true;
    if (aProp->propPtr->valueText() != "")
      accumulation += hshStyle.key(aProp)+":"+aProp->propPtr->valueText()+";";
    lstModified.push_back(aProp);
  }
  if (found)
    emit setAttribute("style",accumulation);
}

void TagEditor::setCbbAttr(QtProperty* property, const int value) {
  QList< PropertiesHash* >  hshList;
  hshList << &hshStd << &hshSpecific << &hshEvent;
  foreach (PropertiesHash* anHsh, hshList) {
    foreach (Property* aProp, *anHsh) {
      if (property == aProp->propPtr) {
	if (!value)
	  emit setAttribute(anHsh->key(aProp),"");
	else
	  emit setAttribute(anHsh->key(aProp),((CbbProperty*)aProp)->valueList[value-1]);
	lstModified.push_back(aProp);
	return;
      }
    }
  }
  
}

Property* TagEditor::createProperty(QString &attr) {
  QtProperty *item3;
  Property* aProp;
  if (attr.indexOf("(") != -1) {
    aProp = new CbbProperty;
    ((CbbProperty*)aProp)->valueList = attr.mid(attr.indexOf("(")+1, attr.size() - attr.indexOf("(") -2).split(',');
    //aProp=attr.left(attr.indexOf("("));
    attr = attr.left(attr.indexOf("("));
    aProp->type = COMBOBOX;
    QStringList value = ((CbbProperty*)aProp)->valueList;
    ((CbbProperty*)aProp)->edited = false;
    value.insert(0,"-none-");
    
    item3 = cbbPropManager->addProperty(attr);
    cbbPropManager->setEnumNames(item3,value);
    
  }
  else {
    aProp = new Property;
    item3 = stringPropManager->addProperty(attr);
    aProp->type = STRING;
  }
  aProp->propPtr = item3;
  return aProp;
}

void TagEditor::clear() {
  qDebug() << "Starting clearing";
  disconnect(cbbPropManager, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setCbbAttr(QtProperty*,int)));
  disconnect(stringPropManager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(setStringAttr(QtProperty*,QString)));
  foreach (Property* aProp, lstModified) {
    qDebug() << "Clearing tag";
    if (aProp->type == STRING)
      ((QtStringPropertyManager*)aProp->propPtr->propertyManager())->setValue(aProp->propPtr,"");
    else if (aProp->type == COMBOBOX)
      ((QtEnumPropertyManager*)aProp->propPtr->propertyManager())->setValue(aProp->propPtr,0);
  }
  connect(stringPropManager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(setStringAttr(QtProperty*,QString)));
  connect(cbbPropManager, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setCbbAttr(QtProperty*,int)));
}