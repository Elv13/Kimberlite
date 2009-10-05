#include "config.h"

Config::Config(QWidget* parent, KimberliteConfigSkeleton* configSkeleton) : KConfigDialog(parent, QString("settings"), configSkeleton) {
  this->configSkeleton = configSkeleton;
  KPageWidgetItem* pwiGeneral = addPage(new QWidget(), "General" );
  pwiGeneral->setIcon( KIcon( "configure" ) );
  
  QGroupBox* grpServer = new QGroupBox(this);
  
  QRadioButton* rbDidablePhp = new QRadioButton(this);
  rbDidablePhp->setText("Test");
  
} 