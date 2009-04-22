#include "newProject.h"

NewProject::NewProject(QWidget* parent):KDialog(parent) {
  mainUi = new Ui_NewProject();

  QWidget* mainWidget = new QWidget(this);
  mainUi->setupUi(mainWidget);
  this->setMainWidget(mainWidget);
  connect(this,SIGNAL(okClicked()),this,SLOT(newProject()));
}

void NewProject::newProject() {
  emit createProject(mainUi->txtName->text(),"/home/lepagee/dev/myproject/kimberlite/template/default.wkp");
}