#include "ProjectManager_v2.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QTextStream>
#include <KIcon>

ProjectManager2::ProjectManager2(QWidget *parent) : QTreeWidget(parent) {
    QStringList labels;
    labels << tr("Project");
    this->setColumnCount(1);

    header()->setResizeMode(QHeaderView::Stretch);
    setHeaderLabels(labels);

    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
    connect(this, SIGNAL(htmlPageChanged(QTreeWidgetItem*, QString)), this, SLOT(test(QTreeWidgetItem*, QString)));
}

bool ProjectManager2::read(QIODevice *device) {
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "xbel") {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL version 1.0 "
                                    "file."));
        return false;
    }

    clear();

    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
               this, SLOT(updateDomElement(QTreeWidgetItem *, int)));

    QDomElement child = root.firstChildElement("project");
    while (!child.isNull()) {
        parseProjectElement(child);
        child = child.nextSiblingElement("project");
    }

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
            this, SLOT(updateDomElement(QTreeWidgetItem *, int)));

    return true;

}

bool ProjectManager2::write(QIODevice *device) {
    const int IndentSize = 4;

    QTextStream out(device);
    domDocument.save(out, IndentSize);
    return true;
}

void ProjectManager2::updateDomElement(QTreeWidgetItem *item, int column) {
    QDomElement element = domElementForItem.value(item);
    if (!element.isNull()) {
        if (column == 0) {
            QDomElement oldTitleElement = element.firstChildElement("title");
            QDomElement newTitleElement = domDocument.createElement("title");

            QDomText newTitleText = domDocument.createTextNode(item->text(0));
            newTitleElement.appendChild(newTitleText);

            element.replaceChild(newTitleElement, oldTitleElement);
        } else {
            if (element.tagName() == "bookmark")
                element.setAttribute("href", item->text(1));
        }
    }
}

void ProjectManager2::parseProjectElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
    QTreeWidgetItem *item = createItem(element, parentItem);

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
        if (child.tagName() == "folder") {
            parseFolderElement(child, item);
        } 
        else if (child.tagName() == "html") {
            parseHtmlElement(child, item);
        }  
        else if (child.tagName() == "style") {
            styleSheethName = child.firstChildElement("file").attribute("name");
        }  
        else if (child.tagName() == "script") {
            parseScriptElement(child, item);
        }   
        else if (child.tagName() == "ressources") {
            parseRessourcesElement(child, item);
        }
        child = child.nextSiblingElement();
    }
}

void ProjectManager2::parseFolderElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
    QTreeWidgetItem *item = createItem(element, parentItem);

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
        if (child.tagName() == "folder") {
            parseFolderElement(child, item);
        } 
        else if (child.tagName() == "page") {
          QTreeWidgetItem *item2 = createItem(element, item);
          item2->setIcon(0,KIcon("application-xhtml+xml"));
          item2->setText(0, child.attribute("name"));
        }
        else if (child.tagName() == "image") {
            QTreeWidgetItem *item2 = createItem(element, item);
            item2->setText(0, child.attribute("name"));
            item2->setIcon(0,KIcon("image-gif"));
        } 
        else if (child.tagName() == "swf") {
            QTreeWidgetItem *item2 = createItem(element, item);
            item2->setText(0, child.attribute("name"));
            item2->setIcon(0,KIcon("x-kde-nsplugin-generated"));
        }
        else if (child.tagName() == "flv") {
           QTreeWidgetItem *item2 = createItem(element, item);
           item2->setText(0, child.attribute("name"));
           item2->setIcon(0,KIcon("video-mpeg"));
        }
        else if (child.tagName() == "file") {
            QTreeWidgetItem *item2 = createItem(element, item);
            item2->setText(0, child.attribute("name"));
            item2->setIcon(0,KIcon("application-javascript"));
        }
        
        child = child.nextSiblingElement();
    }
}

void ProjectManager2::parseHtmlElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
    QTreeWidgetItem *item = createItem(element, parentItem);

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
        if (child.tagName() == "folder") {
            parseFolderElement(child, item);
        } 
        else if (child.tagName() == "page") {
            QTreeWidgetItem* item2 = createItem(element, item);
            item2->setText(0, child.attribute("name"));
            item2->setIcon(0,KIcon("application-xhtml+xml"));
            if (child.attribute("name") == "page1.htm") {
              emit htmlPageChanged(item2, toHTML(child.text()));
              printf("\nThis is it: %s \n", toHTML(child.text()).toStdString().c_str());
            }
        } 
        child = child.nextSiblingElement();
    }
}

void ProjectManager2::test(QTreeWidgetItem* item, QString text) {
  printf("\nWork %s\n",text.toStdString().c_str());
}

void ProjectManager2::parseScriptElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
    QTreeWidgetItem *item = createItem(element, parentItem);

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
        if (child.tagName() == "folder") {
            parseFolderElement(child, item);
        } 
        else if (child.tagName() == "file") {
            QTreeWidgetItem *item2 = createItem(element, item);
            item2->setText(0, child.attribute("name"));
            item2->setIcon(0,KIcon("application-javascript"));
        } 
        child = child.nextSiblingElement();
    }
}

void ProjectManager2::parseRessourcesElement(const QDomElement &element, QTreeWidgetItem *parentItem) {
    QTreeWidgetItem *item = createItem(element, parentItem);

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
        if (child.tagName() == "folder") {
            parseFolderElement(child, item);
        } 
        else if (child.tagName() == "image") {
            QTreeWidgetItem *item2 = createItem(element, item);
            item2->setText(0, child.attribute("name"));
            item2->setIcon(0,KIcon("image-gif"));
        } 
        else if (child.tagName() == "swf") {
            QTreeWidgetItem *item2 = createItem(element, item);
            item2->setText(0, child.attribute("name"));
            item2->setIcon(0,KIcon("x-kde-nsplugin-generated"));
        }
        else if (child.tagName() == "flv") {
           QTreeWidgetItem *item2 = createItem(element, item);
           item2->setText(0, child.attribute("name"));
           item2->setIcon(0,KIcon("video-mpeg"));
        }
        else if (child.tagName() == "file") {
            QTreeWidgetItem *item2 = createItem(element, item);
            item2->setText(0, child.attribute("name"));
        }
        child = child.nextSiblingElement();
    }
}

QTreeWidgetItem * ProjectManager2::createItem(const QDomElement &element, QTreeWidgetItem *parentItem) {
    QTreeWidgetItem *item;
    if (parentItem) {
        item = new QTreeWidgetItem(parentItem);
    } else {
        item = new QTreeWidgetItem(this);
    }
    domElementForItem.insert(item, element);
    return item;
}

QString ProjectManager2::toHTML(QString input) {
  while (input.indexOf("[[@O@]]") != -1) {
    input = input.replace(input.indexOf("[[@O@]]"),7,"<");
  }
  while (input.indexOf("[[@C@]]") != -1) {
    input = input.replace(input.indexOf("[[@C@]]"),7,">");
  }
  return input;
}
 
QString ProjectManager2::fromHTML(QString input) {
  while (input.indexOf("<") != -1) {
    input = input.replace(input.indexOf("<"),1,"[[@O@]]");
  }
  while (input.indexOf(">") != -1) {
    input = input.replace(input.indexOf(">"),1,"[[@C@]]");
  }
  return input;
}
