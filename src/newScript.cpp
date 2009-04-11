#include "newScript.h"
#include <QInputDialog>

NewScript::NewScript(QWidget* parent, QTreeWidgetItem* rootItem) : KDialog(parent),parentRoot(rootItem) {
  QWidget* mainWidget =  new QWidget(this);
  setMainWidget(mainWidget);
  aNewScript = new Ui_NewScript();
  aNewScript->setupUi(mainWidget);
  setModal(true);
  setupFolder(aNewScript->tvFolder,rootItem);
  connect(aNewScript->btnAddFolder,SIGNAL(clicked()),this,SLOT(addFolder()));
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