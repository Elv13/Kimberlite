#include "config.h"

Config::Config(QWidget* parent, KimberliteConfigSkeleton* configSkeleton) : KConfigDialog(parent, QString("settings"), configSkeleton) {
  resize(900,500);
  
  this->configSkeleton = configSkeleton;
  KPageWidgetItem* pwiGeneral = addPage(new QWidget(), "General" );
  pwiGeneral->setIcon( KIcon( "configure" ) );
  
  QWidget* wdgServer = new QWidget(this);
  QVBoxLayout* serverLayout = new QVBoxLayout(wdgServer);
  wdgServer->setLayout(serverLayout);
  
  KPageWidgetItem* pwiServer = addPage(wdgServer, "Server" );
  pwiServer->setIcon( KIcon( "configure" ) );
  
  QGroupBox* grbServer = new QGroupBox(this);
  grbServer->setTitle("Server");
  QGridLayout* serverTypeLayout = new QGridLayout(grbServer);
  serverLayout->addWidget(grbServer);
  
  QLabel* lblServerType = new QLabel("Server type:");
  serverTypeLayout->addWidget(lblServerType,0,0);
  
  cbbServerType = new QComboBox();
  serverTypeLayout->addWidget(cbbServerType,0,1);
  
  QStringList serverTypes;
  serverTypes << "Apache" << "LigHTTPD";
  cbbServerType->addItems(serverTypes);
  
  QLabel* lblServerLocation = new QLabel("Pages location:");
  serverTypeLayout->addWidget(lblServerLocation,1,0);
  
  txtServerLocation = new QLineEdit();
  txtServerLocation->setText(configSkeleton->serverLocation);
  serverTypeLayout->addWidget(txtServerLocation,1,1);
  
  QLabel* lblServerUrl = new QLabel("URL:");
  serverTypeLayout->addWidget(lblServerUrl,2,0);
  
  txtServerUrl = new QLineEdit();
  txtServerUrl->setText(configSkeleton->serverUrl);
  serverTypeLayout->addWidget(txtServerUrl,2,1);
  
  QLabel* lblServerPort = new QLabel("Port:");
  serverTypeLayout->addWidget(lblServerPort,3,0);
  
  txtServerPort = new QLineEdit();
  txtServerPort->setText(configSkeleton->serverPort);
  serverTypeLayout->addWidget(txtServerPort,3,1);
  
  ckbUseServer = new QCheckBox("Use this server for PHP projects");
  ckbUseServer->setChecked(configSkeleton->useServer);
  serverTypeLayout->addWidget(ckbUseServer,4,0,1,2);
  
  serverLayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
  
  KIconLoader *iconloader = KIconLoader::global();
  QString new_path = iconloader->iconPath("help-about", KIconLoader::Desktop );
  
  QLabel* lblServerInfo = new QLabel("<img src=\""+new_path+"\" height=22 width=22> You need write permissions to the pages folder, otherwise, Kimberlite may crash.");
  lblServerInfo->setWordWrap(true);
  serverLayout->addWidget(lblServerInfo); 
  connect( this, SIGNAL( okClicked() ), this, SLOT( saveConfig() ) );
} 

void Config::saveConfig() {
  configSkeleton->serverLocation = txtServerLocation->text();
  configSkeleton->serverUrl = txtServerUrl->text();
  configSkeleton->serverPort = txtServerPort->text();
  configSkeleton->useServer = ckbUseServer->isChecked();
  
  configSkeleton->writeConfig();
}