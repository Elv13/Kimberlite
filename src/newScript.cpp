#include "newScript.h"
#include <QInputDialog>
#include <KMessageBox>

NewScript::NewScript(QWidget* parent, QTreeWidgetItem* rootItem) : KDialog(parent),parentRoot(rootItem) {
  QWidget* mainWidget =  new QWidget(this);
  setMainWidget(mainWidget);
  aNewScript = new Ui_NewScript();
  aNewScript->setupUi(mainWidget);
  setModal(true);
  setupFolder(aNewScript->tvFolder,rootItem);
  connect(aNewScript->btnAddFolder,SIGNAL(clicked()),this,SLOT(addFolder()));
  connect(this,SIGNAL(okClicked()),this,SLOT(okClicked2()));
}

void NewScript::setupFolder(QTreeWidget* aTreeWidget, QTreeWidgetItem* rootItem) {
  QTreeWidgetItem* root = new QTreeWidgetItem(aTreeWidget);
  aTreeWidget->setCurrentItem(root);
  (*root) = (*rootItem);
  for (int i =0; i < rootItem->childCount();i++) {
    if (rootItem->child(i)->child(0) != NULL) {
      QTreeWidgetItem* child = new QTreeWidgetItem(root);
      (*child) = (*rootItem->child(i));
    }
  }
  root->setExpanded(true);
}

void NewScript::addFolder() {
  bool ok;
  QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("Folder name:"), QLineEdit::Normal,"", &ok);
  if (ok && !text.isEmpty()) {
      emit addFolder(text,parentRoot);
  }
}

void NewScript::okClicked2() {
  if (aNewScript->txtName->text().isEmpty()) {
    KMessageBox::error(this,"Invalid File name");
    return;
  }
  QString fileName = aNewScript->txtName->text();
  if (fileName.right(2).toLower() != ".js")
    fileName += ".js";
  if (aNewScript->tvFolder->currentItem() != aNewScript->tvFolder->topLevelItem(0))
    emit addScript(fileName, aNewScript->cbbLanguage->currentText(), aNewScript->tvFolder->currentItem()->text(0));
  else
    emit addScript(fileName, aNewScript->cbbLanguage->currentText(), "@@@ROOTJS");
}