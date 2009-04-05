#include "ressourcesManager.h"

RessrourceManager::RessrourceManager(QWidget* parent) : KDialog(parent) {
  this->resize(634, 553);
  //verticalLayout = new QVBoxLayout(this);
  //verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
  //lblDescription = new QLabel(this);
  //lblDescription->setObjectName(QString::fromUtf8("lblDescription"));

  //verticalLayout->addWidget(lblDescription);

  grbRessources = new QGroupBox(this);
  grbRessources->setObjectName(QString::fromUtf8("grbRessources"));
  setMainWidget(grbRessources);
  verticalLayout_2 = new QVBoxLayout(grbRessources);
  verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

  rbCurerent = new QRadioButton(grbRessources);
  rbCurerent->setObjectName(QString::fromUtf8("rbCurerent"));
  rbCurerent->setChecked(true);

  verticalLayout_2->addWidget(rbCurerent);

  tvCurrentRessources = new QTreeWidget(grbRessources);
  tvCurrentRessources->setObjectName(QString::fromUtf8("tvCurrentRessources"));

  verticalLayout_2->addWidget(tvCurrentRessources);

  rbNew = new QRadioButton(grbRessources);
  rbNew->setObjectName(QString::fromUtf8("rbNew"));

  verticalLayout_2->addWidget(rbNew);

  txtNewRessources = new KUrlRequester(grbRessources);
  txtNewRessources->setObjectName(QString::fromUtf8("txtNewRessources"));
  txtNewRessources->setEnabled(false);

  verticalLayout_2->addWidget(txtNewRessources);

  rbUrl = new QRadioButton(grbRessources);
  rbUrl->setObjectName(QString::fromUtf8("rbUrl"));

  verticalLayout_2->addWidget(rbUrl);

  txtUrl = new KLineEdit(grbRessources);
  txtUrl->setObjectName(QString::fromUtf8("txtUrl"));
  txtUrl->setEnabled(false);

  verticalLayout_2->addWidget(txtUrl);


  //verticalLayout->addWidget(grbRessources);


  QObject::connect(rbCurerent, SIGNAL(toggled(bool)), tvCurrentRessources, SLOT(setEnabled(bool)));
  QObject::connect(rbNew, SIGNAL(toggled(bool)), txtNewRessources, SLOT(setEnabled(bool)));
  QObject::connect(rbUrl, SIGNAL(toggled(bool)), txtUrl, SLOT(setEnabled(bool)));

  QMetaObject::connectSlotsByName(this);

  setWindowTitle(QApplication::translate("RessrourceManager", "Ressrource Manager", 0, QApplication::UnicodeUTF8));
  //lblDescription->setText(QApplication::translate("RessrourceManager", "Select a ressource:", 0, QApplication::UnicodeUTF8));
  grbRessources->setTitle(QApplication::translate("RessrourceManager", "Ressources", 0, QApplication::UnicodeUTF8));
  rbCurerent->setText(QApplication::translate("RessrourceManager", "Current ressources", 0, QApplication::UnicodeUTF8));
  QTreeWidgetItem *___qtreewidgetitem = tvCurrentRessources->headerItem();
  ___qtreewidgetitem->setText(1, QApplication::translate("RessrourceManager", "Type", 0, QApplication::UnicodeUTF8));
  ___qtreewidgetitem->setText(0, QApplication::translate("RessrourceManager", "Ressources", 0, QApplication::UnicodeUTF8));
  rbNew->setText(QApplication::translate("RessrourceManager", "New ressources", 0, QApplication::UnicodeUTF8));
  rbUrl->setText(QApplication::translate("RessrourceManager", "Url", 0, QApplication::UnicodeUTF8));
}