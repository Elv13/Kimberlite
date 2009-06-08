#include "colorComboBox.h"
#include <QPoint>
#include <QDebug>
#include <QList>
#include <QHash>
#include <QSpacerItem>
#include <KColorDialog>
#include <QScrollArea>
#include <QVBoxLayout>

ColorButton::ColorButton() : QPushButton() {
  setMaximumSize(12,12);
  setStyleSheet("border:0px;margin:0px;background-color:red;");
  connect(this, SIGNAL(clicked()), this, SLOT(pressed()));
}

void ColorButton::pressed() {
  emit clicked(buttonColor);
}

ColorComboBox::ColorComboBox(QWidget* parent) : QComboBox(parent),currentButton(NULL) {
  frmColor = new QFrame();
  frmColor->setMaximumSize(115,135);
  frmColor->setMinimumSize(115,135);
  frmColor->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
  setMaximumSize(60,25);
  
  QVBoxLayout* frmLayout = new QVBoxLayout(frmColor);
  frmLayout->setSpacing(0);
  frmLayout->setContentsMargins(0,0,0,0);
  
  QScrollArea* scrollArea = new QScrollArea(this);
  scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
  scrollArea->setWidgetResizable(true);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  frmLayout->addWidget(scrollArea);
  
  QWidget* scrollWidget =  new QWidget(scrollArea);
  scrollArea->setWidget(scrollWidget);
  
  mainLayout = new QGridLayout(scrollWidget);
  mainLayout->setSpacing(3);
  mainLayout->setContentsMargins(0,0,0,0);
  
  QList<QColor> colorList;
  colorList << QColor("#000000") << QColor("#FFFFFF") << QColor("#808080") << QColor("#C0C0C0") << QColor("#FF0000") << QColor("#800000") << QColor("#FF8000") << QColor("#804000") << QColor("#FFFF00") << QColor("#808000") << QColor("#00FF00") << QColor("#008000") << QColor("#00FFFF") << QColor("#008080") << QColor("#0000FF") << QColor("#000080") << QColor("#FF00FF") << QColor("#800080") << QColor("#FF8080") << QColor("#8080FF") << QColor("#80FF80") << QColor("#FFFF80");
 
  maxPerLine = 6;
  
  curRow =0;
  curCol =0;
  for (int i =0;i < colorList.count();i++) {
    if (curCol == maxPerLine) {
      curRow++;
      curCol=0;
    }
    ColorButton* aColor = new ColorButton();
    buttonHash[colorList[i].name()] = aColor;
    aColor->setColor(colorList[i]);
    connect(aColor, SIGNAL(clicked(QColor)), this, SLOT(colorClicked(QColor)));
    mainLayout->addWidget(aColor,curRow,curCol);
    curCol++;
  }
  
  mainLayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Expanding),999,0);
  mainLayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum),999,maxPerLine+1);

  QPushButton* btnCustomColor = new QPushButton();
  btnCustomColor->setText("Custom...");
  btnCustomColor->setIcon(KIcon("fill-color"));
  frmLayout->addWidget(btnCustomColor);
  connect(btnCustomColor, SIGNAL(clicked()), this, SLOT(addCustomColor()));

  
  QListWidget* aList = new QListWidget();
  aList->setStyleSheet("background-color:transparent");
  aList->addItem(QString::fromUtf8("■"));
  aList->setItemWidget(aList->item(0),frmColor);
  setView(aList);
  setModel(aList->model());
  
  setStyleSheet("QComboBox {\
		  color:"+colorList[0].name()+";\
		}\
		QComboBox QAbstractItemView {\
		  min-height:135px;\
		  min-width:140px;\
		  padding-left:-20px;\
		}");
}

void ColorComboBox::showPopup() {
  QComboBox::showPopup();
}

void ColorComboBox::hidePopup() {
  QComboBox::hidePopup();
}

void ColorComboBox::colorClicked(QColor aColor) {
  if (currentButton)
    currentButton->reset();
  
  if (buttonHash.find(aColor.name()) != buttonHash.end()) {
    currentButton = buttonHash[aColor.name()];
    currentButton->select();
    setStyleSheet("QComboBox {\
		    color:"+aColor.name()+";\
		  }\
		  QComboBox QAbstractItemView {\
		    min-height:135px;\
		    min-width:140px;\
		    padding-left:-20px;\
		  }");
  }
  else {
    if (curCol == maxPerLine) {
      curRow++;
      curCol=0;
    }
    ColorButton* aColor2 = new ColorButton();
    buttonHash[aColor.name()] = aColor2;
    aColor2->setColor(aColor);
    connect(aColor2, SIGNAL(clicked(QColor)), this, SLOT(colorClicked(QColor)));
    mainLayout->addWidget(aColor2,curRow,curCol);
    curCol++;
  }
  emit colorChanged(aColor);
  hidePopup();
}

void ColorComboBox::addCustomColor() {
  KColorDialog aDialog(this,true);
  QColor aColor;
  int result = aDialog.getColor( aColor );
  if ( result == KColorDialog::Accepted )
    colorClicked(aColor);
}