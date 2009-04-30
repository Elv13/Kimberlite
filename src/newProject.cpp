#include "newProject.h"
#include <KStandardDirs>

NewProject::NewProject(QWidget* parent):KDialog(parent) {
  mainUi = new Ui_NewProject();

  QWidget* mainWidget = new QWidget(this);
  mainUi->setupUi(mainWidget);
  this->setMainWidget(mainWidget);
  connect(this,SIGNAL(okClicked()),this,SLOT(newProject()));
}

void NewProject::newProject() {
  emit createProject(mainUi->txtName->text(),KStandardDirs::locate("appdata", "template/default.wkp"));
}