#include "newPage.h"
#include <KIcon>
#include <QInputDialog>
#include <KMessageBox>
#include <KIO/NetAccess>
#include <QTextStream>
#include <QDebug>
#include "miniClasses.h"

NewWebPage::NewWebPage(QWidget* parent, TreeItem* root) : KPageDialog(parent),parentRoot(root) {
  setWindowTitle("New Page");
  
  QWidget* wdgNewPage = new QWidget(this);
  newPage = new Ui_NewPage(/*this*/);
  newPage->setupUi(wdgNewPage);
  pageEmpty = addPage(wdgNewPage,"Empty");
  pageEmpty->setIcon(KIcon("document-new"));
  setupFolder(newPage->tvFolder,root);
  connect(newPage->btnAddFolder,SIGNAL(clicked()),this,SLOT(addFolder()));
  
  QWidget* wdgNewTemplate = new QWidget(this);
  newTemple = new Ui_NewPageFromTemplate(/*this*/);
  newTemple->setupUi(wdgNewTemplate);
  pageTemplate = addPage(wdgNewTemplate,"Template");
  pageTemplate->setIcon(KIcon("zoom-fit-best"));
  setupFolder(newTemple->tvFolder,root);
  connect(newTemple->btnAddFolder,SIGNAL(clicked()),this,SLOT(addFolder()));
  
  QWidget* wdgImport = new QWidget(this);
  newImport = new Ui_ImportFile(/*this*/);
  newImport->setupUi(wdgImport);
  pageImport = addPage(wdgImport,"Import");
  pageImport->setIcon(KIcon("document-open-folder"));
  resize(750,400);
  setModal(true);
  setupFolder(newImport->tvFolder,root);
  connect(newImport->btnAddFolder,SIGNAL(clicked()),this,SLOT(addFolder()));
  
  connect(this,SIGNAL(okClicked()),this,SLOT(addHtmlPage()));
}

void NewWebPage::setupFolder(QTreeWidget* aTreeWidget, TreeItem* rootItem) {
  QTreeWidgetItem* root = new QTreeWidgetItem(aTreeWidget);
  aTreeWidget->setCurrentItem(root);
  (*root) = (*rootItem);
  for (int i =0; i < rootItem->childCount();i++) {
    qDebug() << "setupFolder" << rootItem->child(i)->text(0);
    if (((TreeItem*) rootItem->child(i))->type == 1) {
      QTreeWidgetItem* child = new QTreeWidgetItem(root);
      (*child) = (*rootItem->child(i));
    }
  }
  root->setExpanded(true);
}

void NewWebPage::addFolder() {
  bool ok;
  QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("Folder name:"), QLineEdit::Normal,"", &ok);
  if (ok && !text.isEmpty()) {
    emit addFolder(text,parentRoot);
    QTreeWidgetItem* child;
    if (currentPage() == pageEmpty) 
      child = new QTreeWidgetItem(newPage->tvFolder->topLevelItem(0));
    else if (currentPage() == pageTemplate) 
      child = new QTreeWidgetItem(newTemple->tvFolder->topLevelItem(0));
    else 
      child = new QTreeWidgetItem(newImport->tvFolder->topLevelItem(0));
    child->setText(0,text);
    child->setIcon(0,KIcon("document-open-folder"));
  }
}
  
void NewWebPage::addHtmlPage() {
  if (currentPage() == pageEmpty) {
    if (newPage->txtFileName->text().isEmpty()) {
      KMessageBox::error(this,"Invalid File name");
      return;
    }
    if (newPage->txtPageTitle->text().isEmpty()) {
      KMessageBox::error(this,"Invalid Page title");
      return;
    }
    if (newPage->tvFolder->currentItem() == NULL) {
      KMessageBox::error(this,"No destination have been selected");
      return;
    }
    QString fileName = newPage->txtFileName->text();
    if ((fileName.right(4).toLower() != ".htm") || (fileName.right(5).toLower() != ".html"))
      fileName += ".htm";
    if (newPage->tvFolder->currentItem() != newPage->tvFolder->topLevelItem(0))
      emit addHtmlPage(newPage->txtPageTitle->text(), fileName, "", newPage->tvFolder->currentItem()->text(0));
    else
      emit addHtmlPage("", fileName, "", "@@@ROOT");
  }
  else if (currentPage() == pageTemplate) {
    //TODO Incoming feature
  }
  else {
    QString content;
    if (newImport->tvFolder->currentItem() == NULL) {
      KMessageBox::error(this,"No destination have been selected");
      return;
    }
    QString tmpFile;
    if(KIO::NetAccess::download(newImport->txtCurrentPath->text(), tmpFile,this)) {
      QFile file(tmpFile);
      file.open(QIODevice::ReadOnly);
      content = QTextStream(&file).readAll();
      KIO::NetAccess::removeTempFile(tmpFile);
    }
    else {
      KMessageBox::error(this,KIO::NetAccess::lastErrorString());
      return;
    }
    QString fileName = newImport->txtFileName->text();
    if ((fileName.right(4).toLower() != ".htm") || (fileName.right(5).toLower() != ".html"))
      fileName += ".htm";
    if (newImport->tvFolder->currentItem() != newImport->tvFolder->topLevelItem(0))
      emit addHtmlPage("", fileName, content, newImport->tvFolder->currentItem()->text(0));
    else
      emit addHtmlPage("", fileName, content, "@@@ROOT");
  }
}
