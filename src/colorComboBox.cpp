#include "colorComboBox.h"
#include <QPoint>
#include <QDebug>
#include <QList>
#include <QHash>

ColorButton::ColorButton() : QPushButton() {
  setMaximumSize(12,12);
  setStyleSheet("border:0px;margin:0px;background-color:red;");
  connect(this, SIGNAL(clicked()), this, SLOT(pressed()));
}

void ColorButton::pressed() {
  emit clicked(buttonColor);
}

void HidingFrame::focusOutEvent(QFocusEvent* event) {
  //printf("I am here12\n");
  //QWidget::focusOutEvent(event);
  //hide();
  QWidget::focusOutEvent(event);
}

ColorComboBox::ColorComboBox(QWidget* parent) : QComboBox(parent),currentButton(NULL) {
  frmColor = new HidingFrame(0,/*Qt::WindowStaysOnTopHint |*/Qt::ToolTip );
  frmColor->setMaximumSize(100,100);
  frmColor->setMinimumSize(100,100);
  setMaximumSize(60,25);
  addItem(KIcon("fill-color"),QString::fromUtf8("■"));
  setStyleSheet("QComboBox {\
		  color:red;\
		}\
		QComboBox QAbstractItemView {\
		  max-height:0px;\
		  max-width:0px;\
		}");
  
  QGridLayout* mainLayout = new QGridLayout(frmColor);
  mainLayout->setSpacing(1);
  mainLayout->setContentsMargins(0,0,0,0);
  
  QList<QColor> colorList;
  colorList << QColor("#FF0000") << QColor("#00FF00") << QColor("#0000FF") << QColor("#FF0000") << QColor("#00FF00") << QColor("#0000FF") << QColor("#FF0000") << QColor("#00FF00") << QColor("#0000FF") << QColor("#FF0000") << QColor("#00FF00") << QColor("#0000FF") << QColor("#FF0000") << QColor("#00FF00") << QColor("#0000FF") << QColor("#FF0000") << QColor("#00FF00") << QColor("#0000FF") << QColor("#FF0000") << QColor("#00FF00") << QColor("#0000FF");
  
  int maxPerLine = 6;
  
  int curRow =0;
  int curCol =0;
  for (int i =0;i < colorList.count();i++) {
    if (curCol == maxPerLine -1) {
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
  
}

void ColorComboBox::showPopup() {
  //QComboBox::showPopup();
  QPoint aPoint = mapToGlobal(QPoint(0,0));
  frmColor->move(aPoint.x(), aPoint.y()+this->height());
  frmColor->hide();
  frmColor->show();
  frmColor->setFocus(Qt::OtherFocusReason);
}

void ColorComboBox::hidePopup() {
  //QComboBox::hidePopup();
  qDebug() << "I am here";
  frmColor->hide();
}

void ColorComboBox::colorClicked(QColor aColor) {
  if (currentButton)
    currentButton->reset();
  currentButton = buttonHash[aColor.name()];
  currentButton->select();
  setStyleSheet("QComboBox {\
		  color:"+aColor.name()+";\
		  }\
		  QComboBox QAbstractItemView {\
		    max-height:0px;\
		    max-width:0px;\
		  }");
  emit colorChanged(aColor);
  frmColor->hide();
}