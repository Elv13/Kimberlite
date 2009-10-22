#include "ProjectManager_v2.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QTextStream>
#include <KIcon>
#include <QDebug>
#include "cssParser.h"
#include "htmlParser.h"

ProjectManager2::ProjectManager2(QWidget *parent) : QTreeWidget(parent),firstPage(true) {
  setHeaderHidden(true);
  QStringList labels;
  labels << tr("Project");
  this->setColumnCount(1);

  header()->setResizeMode(QHeaderView::Stretch);
  setHeaderLabels(labels);

  folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),QIcon::Normal, QIcon::Off);
  folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),QIcon::Normal, QIcon::On);
  bookmarkIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(loadPage(QTreeWidgetItem *)));
}

bool ProjectManager2::read(QIODevice *device) {
  QString errorStr;
  int errorLine;
  int errorColumn;
  
  //fileArray[QString("KimberliteInternalData")] =device->readAll(); //TODO Remove that and implement the real one
  //device->seek(0);

  if (!domDocument.setContent(readFile(device), true, &errorStr, &errorLine,&errorColumn)) {
    QMessageBox::information(window(), tr("DOM Bookmarks"),tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
    return false;
  }

  QDomElement root = domDocument.documentElement();
  if (root.tagName() != "xbel") {
    QMessageBox::information(window(), tr("DOM Bookmarks"),tr("The file is not an XBEL file."));
    return false;
  } 
  else if (root.hasAttribute("version")&& root.attribute("version") != "1.0") {
    QMessageBox::information(window(), tr("DOM Bookmarks"), tr("The file is not an XBEL version 1.0 file."));
    return false;
  }

  clear();

  QDomElement child = root.firstChildElement("project");
  while (!child.isNull()) {
    parseProjectElement(child);
    child = child.nextSiblingElement("project");
  }
  return true;
}

bool ProjectManager2::write(QIODevice *device) {
  const int IndentSize = 4;
  QTextStream out(device);
  domDocument.save(out, IndentSize);
  return true;
}

void ProjectManager2::updateDomElement(QTreeWidgetItem *item, QString html) {
  QDomElement element = domElementForItem.value(item);
  if (!element.isNull()) {
    QDomElement oldTitleElement = element.firstChildElement("content");
    QDomElement newTitleElement = domDocument.createElement("content");
    QDomText newTitleText = domDocument.createTextNode(fromHTML(html));
    newTitleElement.appendChild(newTitleText);
    element.replaceChild(newTitleElement, oldTitleElement);
  }
}

void ProjectManager2::parseProjectElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
  TreeItem *item = createItem(element, parentItem);

  QString title = element.attribute("title");
  projectTitle = title;
  if (title.isEmpty())
    title = QObject::tr("Folder");

  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setIcon(0, folderIcon);
  item->setText(0, title);

  bool folded = (element.attribute("folded") != "no");
  setItemExpanded(item, !folded);

  QDomElement child = element.firstChildElement();
  while (!child.isNull()) {
    if (child.tagName() == "folder") 
      parseFolderElement(child, item);
    else if (child.tagName() == "html") 
      parseHtmlElement(child, item);
    else if (child.tagName() == "style") {
      styleSheethName = child.firstChildElement("file").attribute("name");
      emit loadCss(child.firstChildElement("file").text().trimmed());
      cssPage = child;
    }  
    else if (child.tagName() == "script") 
      parseScriptElement(child, item);  
    else if (child.tagName() == "ressources") 
      parseRessourcesElement(child, item);
    child = child.nextSiblingElement();
  }
}

void ProjectManager2::parseFolderElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
  TreeItem *item = createItem(element, parentItem);
  item->type =1;
  QString title = element.attribute("title");
  if (title.isEmpty())
    title = QObject::tr("Folder");

  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setIcon(0, folderIcon);
  item->setText(0, title);

  bool folded = (element.attribute("folded") != "no");
  setItemExpanded(item, !folded);

  QDomElement child = element.firstChildElement();
  while (!child.isNull()) {
    if (child.tagName() == "folder") 
      parseFolderElement(child, item);
    else if (child.tagName() == "page") {
    TreeItem *item2 = createItem(child, item);
    item2->setIcon(0,KIcon("application-xhtml+xml"));
    item2->setText(0, child.attribute("name"));
    }
    else if (child.tagName() == "image") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("image-gif"));
    } 
    else if (child.tagName() == "swf") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("x-kde-nsplugin-generated"));
    }
    else if (child.tagName() == "flv") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("video-mpeg"));
    }
    else if (child.tagName() == "file") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("application-javascript"));
    }
    child = child.nextSiblingElement();
  }
}

void ProjectManager2::parseHtmlElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
  TreeItem *item = createItem(element, parentItem);
  htmlPage = item;
  QString title = "HTML document";
  if (title.isEmpty())
      title = QObject::tr("Folder");

  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setIcon(0, folderIcon);
  item->setText(0, title);

  bool folded = (element.attribute("folded") != "no");
  setItemExpanded(item, !folded);

  QDomElement child = element.firstChildElement();
  while (!child.isNull()) {
    if (child.tagName() == "folder") 
      parseFolderElement(child, item);
    else if (child.tagName() == "page") {
      TreeItem* item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("application-xhtml+xml"));
      if (firstPage == true) {
	item2->setSelected(true);
	htmlPageChanged(item2, toHTML(domElementForItem.value(item2).text()));
	firstPage = false;
      }
    } 
    child = child.nextSiblingElement();
  }
}

void ProjectManager2::parseScriptElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
  TreeItem *item = createItem(element, parentItem);
  script = item;
  QString title = "JavaScripts";
  if (title.isEmpty())
    title = QObject::tr("Folder");

  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setIcon(0, folderIcon);
  item->setText(0, title);

  bool folded = (element.attribute("folded") != "no");
  setItemExpanded(item, !folded);

  QDomElement child = element.firstChildElement();
  while (!child.isNull()) {
    if (child.tagName() == "folder") 
      parseFolderElement(child, item);
    else if (child.tagName() == "file") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("application-javascript"));
    } 
    child = child.nextSiblingElement();
  }
}

void ProjectManager2::parseRessourcesElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
  TreeItem *item = createItem(element, parentItem);

  QString title = "Ressourses";
  if (title.isEmpty())
    title = QObject::tr("Folder");

  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setIcon(0, folderIcon);
  item->setText(0, title);

  bool folded = (element.attribute("folded") != "no");
  setItemExpanded(item, !folded);

  QDomElement child = element.firstChildElement();
  while (!child.isNull()) {
    if (child.tagName() == "folder") 
      parseFolderElement(child, item);
    else if (child.tagName() == "image") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("image-gif"));
    } 
    else if (child.tagName() == "swf") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("x-kde-nsplugin-generated"));
    }
    else if (child.tagName() == "flv") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
      item2->setIcon(0,KIcon("video-mpeg"));
    }
    else if (child.tagName() == "file") {
      TreeItem *item2 = createItem(child, item);
      item2->setText(0, child.attribute("name"));
    }
    child = child.nextSiblingElement();
  }
}

TreeItem* ProjectManager2::createItem(const QDomElement &element, QTreeWidgetItem *parentItem) {
  TreeItem* item;
  if (parentItem) 
    item = new TreeItem(parentItem);
  else
    item = new TreeItem(this);
  domElementForItem.insert(item, element);
  return item;
}

QString ProjectManager2::toHTML(QString input) {
  while (input.indexOf("[[@O@]]") != -1) 
    input = input.replace(input.indexOf("[[@O@]]"),7,"<");
  while (input.indexOf("[[@C@]]") != -1) 
    input = input.replace(input.indexOf("[[@C@]]"),7,">");
  return input;
}
 
QString ProjectManager2::fromHTML(QString input) {
  while (input.indexOf("<") != -1) 
    input = input.replace(input.indexOf("<"),1,"[[@O@]]");
  while (input.indexOf(">") != -1) 
    input = input.replace(input.indexOf(">"),1,"[[@C@]]");
  return input;
}

void ProjectManager2::loadPage(QTreeWidgetItem* anItem) {
  qDebug() << "I am goinf to show page!!! Name:" << domElementForItem.value(anItem).firstChildElement().attribute("name");
  if (domElementForItem.value(anItem).attribute("name").indexOf(".htm") != -1) {
    qDebug() << "tree Pointer:" << anItem << " dom element:" << &domElementForItem.value(anItem);
    emit htmlPageChanged(anItem, toHTML(domElementForItem.value(anItem).text()));
  }
  else if (domElementForItem.value(anItem).attribute("name").indexOf(".js") != -1) {
    emit javaScriptChanged(anItem, domElementForItem.value(anItem).text());
  }
}

QDomElement ProjectManager2::getDomElement(QTreeWidgetItem* anItem) {
  return domElementForItem.value(anItem);
}

QDomDocument* ProjectManager2::getDomDocument() {
  return &domDocument;
}

void ProjectManager2::addHtmlPage(QString title, QString name, QString body, QString foldeName, int type) {
  QDomElement anElement = domDocument.createElement("page");
  anElement.setAttribute("title", title);
  anElement.setAttribute("name", name);
  anElement.setAttribute("folder",foldeName);
  if (!type)
    type =1;
  QDomElement newContentElement = domDocument.createElement("content");
  QDomText newContentText;
  if (body.isEmpty())
    newContentText= domDocument.createTextNode(fromHTML("<html><head><link rel=\"stylesheet\" href=\"./StyleSheet.css\" type=\"text/css\"><title>"+title+"</title></head><body></body></html>"));
  else
    newContentText= domDocument.createTextNode(fromHTML(body));
  newContentElement.appendChild(newContentText);
  anElement.appendChild(newContentElement);
  QDomElement element2 = domElementForItem.value(getFolder(foldeName));
  element2.appendChild(anElement);
  TreeItem* anItem = createItem(anElement,getFolder(foldeName));
  anItem->setText(0,name);
  anItem->setIcon(0, KIcon("application-xhtml+xml"));
  qDebug() << "XML data: \n" << domDocument.toString();
  setCurrentItem(anItem);
}

void ProjectManager2::addFolder(QString title, QTreeWidgetItem* parent) {
  QDomElement anElement = domDocument.createElement("folder");
  anElement.setAttribute("title", title);
  QDomElement parentElement = domElementForItem.value(parent);
  parentElement.appendChild(anElement);
  TreeItem* anItem = createItem(anElement,parent);
  anItem->type =1;
  anItem->setText(0,title);
  anItem->setIcon(0, folderIcon);
}

QDomNode ProjectManager2::getElement(QDomNode &aNode, QString tagName, QString attribute, QString value) {
  QDomNodeList aList = aNode.childNodes();
  for (int i =0; i < aList.count();i++) 
    if (aList.item(i).toElement().tagName() == tagName) 
      if (aList.item(i).toElement().attribute(attribute) == value)
	return aList.item(i);
  return domElementForItem.value(htmlPage);
}

QTreeWidgetItem* ProjectManager2::getFolder(QString title) {
  if (title == "@@@ROOT")
    return htmlPage;
  else if (title == "@@@ROOTJS")
    return script;
  foreach(QDomElement item, domElementForItem) {
    if (item.toElement().tagName() == "folder") {
      if (item.toElement().attribute("title") == title) {
	return domElementForItem.key(item);
      }
    }
  }
}

void ProjectManager2::setProjectName(QString name) {
  QDomElement root = domDocument.documentElement();
  QDomElement child = root.firstChildElement("project");
  child.setAttribute("title",name);
  TreeItem* anItem = (TreeItem*) domElementForItem.key(child);
  if (anItem != NULL)
    anItem->setText(0,name);
}

QString ProjectManager2::getProjectName() {
  return domDocument.documentElement().firstChildElement("project").attribute("title");
}

void ProjectManager2::saveCss() {
  QDomText newStyle = domDocument.createTextNode(CssParser::cssFile);
  QDomElement anElement = domDocument.createElement("file");
  anElement.appendChild(newStyle);
  cssPage.replaceChild(anElement ,cssPage.firstChildElement("file"));
}

//File manager
QByteArray ProjectManager2::createFile() {
  QByteArray projectFile,tmpArray,compressedTmpArray;
  projectFile.append("WKPv01");
  foreach(QByteArray item, fileArray) {
    tmpArray.clear();
    compressedTmpArray.clear();
    tmpArray.append(getDomDocument()->toString().toAscii());
    //compressedTmpArray = qCompress(tmpArray);
    compressedTmpArray = tmpArray;
    projectFile.append(fileArray.key(item) + "\\\\@" + QString::number(compressedTmpArray.size()) + "\\\\@");
    projectFile.append(compressedTmpArray);
  }
  qDebug() << "Writing:" << projectFile.size();
  return projectFile;
}

QByteArray ProjectManager2::readFile(QIODevice *device) {
  int fileVersion,fileLength;
  QString tmpName;
  QByteArray projectFile,tmpArray;
  device->seek(0);
  while(!device->atEnd()) 
    projectFile += device->read(1);
  
  
  if (QString(projectFile.left(3)) != "WKP") {
    QMessageBox::information(window(), tr("DOM Bookmarks"),tr("This is not a Kimberlite project file"));
    qDebug() << "Invalid file";
    return NULL;
  }
  tmpArray = projectFile.left(6);
  projectFile.remove(0,6);
  fileVersion = tmpArray.right(2).toInt();
  while (projectFile.size()) {
    tmpName = projectFile.left(projectFile.indexOf("\\\\@"));
    projectFile.remove(0,projectFile.indexOf("\\\\@") + 3);
    fileLength = projectFile.left(projectFile.indexOf("\\\\@")).toInt();
    projectFile.remove(0,projectFile.indexOf("\\\\@") + 3);
    tmpArray.clear();
    tmpArray = projectFile.left(fileLength);
    projectFile.remove(0,fileLength);
    //fileArray[tmpName] = qUncompress(tmpArray);
    fileArray[tmpName] = tmpArray;
  }
  if (fileArray.find("KimberliteInternalData") == fileArray.end())
    return NULL; //ERROR!!!
  return fileArray["KimberliteInternalData"];
}

void  ProjectManager2::exportProject(QString path,QTreeWidgetItem* parent) {
  QTreeWidgetItem* topItem;
  if (!parent)
    topItem = topLevelItem(0);
  else
    topItem = parent;

  QDir aQDir;
  aQDir.mkpath(path);
  
  for (int i = 0; i < topItem->childCount();i++) {
    if (domElementForItem[topItem->child(i)].tagName() ==  "folder")
      exportProject(path + domElementForItem[topItem->child(i)].attribute("title") + "/",topItem->child(i));
    else if (domElementForItem[topItem->child(i)].tagName() ==  "style") {
      qDebug() << "I am in style";
      QFile aFile(path+"StyleSheet.css");
      aFile.open(QIODevice::WriteOnly);
      aFile.write(CssParser::cssFile.toStdString().c_str());
      aFile.close();
    }
    else if (domElementForItem[topItem->child(i)].tagName() ==  "html") {
      exportProject(path,topItem->child(i));
    }
    else if (domElementForItem[topItem->child(i)].tagName() ==  "script") {
      exportProject(path + "scripts/",topItem->child(i));
    }
    else if (domElementForItem[topItem->child(i)].tagName() ==  "page") {
      QFile aFile(path+domElementForItem[topItem->child(i)].attribute("name"));
      aFile.open(QIODevice::WriteOnly);
      aFile.write(HtmlParser::getParsedHtml(toHTML(domElementForItem[topItem->child(i)].text())).toStdString().c_str());
      aFile.close();
    }
    else if (domElementForItem[topItem->child(i)].tagName() ==  "file") {
      QFile aFile(path+domElementForItem[topItem->child(i)].attribute("name"));
      aFile.open(QIODevice::WriteOnly);
      aFile.write(HtmlParser::getParsedHtml(toHTML(domElementForItem[topItem->child(i)].text())).toStdString().c_str());
      aFile.close();
    }
  }
}

void ProjectManager2::addScript(QString name, QString language, QString foldeName) {
  QDomElement anElement = domDocument.createElement("file");
  anElement.setAttribute("language", language);
  anElement.setAttribute("name", name);
  QDomElement newContentElement = domDocument.createElement("content");
  QDomText newContentText;
  newContentText= domDocument.createTextNode("");
  newContentElement.appendChild(newContentText);
  anElement.appendChild(newContentElement);
  QDomElement element2 = domElementForItem.value(getFolder(foldeName));
  element2.appendChild(anElement);
  TreeItem* anItem = createItem(anElement,getFolder(foldeName));
  anItem->setText(0,name);
  anItem->setIcon(0, KIcon("application-javascript"));
  setCurrentItem(anItem);
}
