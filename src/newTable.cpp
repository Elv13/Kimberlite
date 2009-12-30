#include "newTable.h"

NewTable::NewTable(QWidget* parent) : KDialog(parent) {
  if (this->objectName().isEmpty())
      this->setObjectName(QString::fromUtf8("this"));
  grbNewTable = new QGroupBox(this);
  grbNewTable->setObjectName(QString::fromUtf8("grbNewTable"));
  grbNewTable->setGeometry(QRect(70, 30, 184, 171));
  setMainWidget(grbNewTable);

  gridLayout = new QGridLayout(grbNewTable);
  gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
  lblColumn = new QLabel(grbNewTable);
  lblColumn->setObjectName(QString::fromUtf8("lblColumn"));
  gridLayout->addWidget(lblColumn, 0, 0, 1, 1);

  spnColumn = new QSpinBox(grbNewTable);
  spnColumn->setObjectName(QString::fromUtf8("spnColumn"));
  gridLayout->addWidget(spnColumn, 0, 1, 1, 1);

  lblRow = new QLabel(grbNewTable);
  lblRow->setObjectName(QString::fromUtf8("lblRow"));
  gridLayout->addWidget(lblRow, 1, 0, 1, 1);

  spnRow = new QSpinBox(grbNewTable);
  spnRow->setObjectName(QString::fromUtf8("spnRow"));
  gridLayout->addWidget(spnRow, 1, 1, 1, 1);

  lblBorder = new QLabel(grbNewTable);
  lblBorder->setObjectName(QString::fromUtf8("lblBorder"));
  gridLayout->addWidget(lblBorder, 2, 0, 1, 1);

  cbbBorder = new QComboBox(grbNewTable);
  cbbBorder->setObjectName(QString::fromUtf8("cbbBorder"));
  gridLayout->addWidget(cbbBorder, 2, 1, 1, 1);
  
  cbbBorder->addItem("none");
  cbbBorder->addItem("solid");
  cbbBorder->addItem("double");
  cbbBorder->addItem("dotted");

  ckbAddHeader = new QCheckBox(grbNewTable);
  ckbAddHeader->setObjectName(QString::fromUtf8("ckbAddHeader"));
  gridLayout->addWidget(ckbAddHeader, 3, 0, 1, 2);

  QMetaObject::connectSlotsByName(this);

  this->setWindowTitle(QApplication::translate("NewTable", "New Table", 0, QApplication::UnicodeUTF8));
  grbNewTable->setTitle(QApplication::translate("NewTable", "New Table", 0, QApplication::UnicodeUTF8));
  lblColumn->setText(QApplication::translate("NewTable", "Column:", 0, QApplication::UnicodeUTF8));
  lblRow->setText(QApplication::translate("NewTable", "Row:", 0, QApplication::UnicodeUTF8));
  lblBorder->setText(QApplication::translate("NewTable", "Border:", 0, QApplication::UnicodeUTF8));
  ckbAddHeader->setText(QApplication::translate("NewTable", "Add header", 0, QApplication::UnicodeUTF8));
  
  connect(this,SIGNAL(okClicked()),this,SLOT(okClicked2()));
  Q_UNUSED(this);
  setModal(true);
}

void NewTable::okClicked2() {
  QString toReturn = "<table class=\"border-style:"+cbbBorder->currentText()+"\">";
  QString aRow,header;
  if (ckbAddHeader->isChecked()) {
    for (int i = 0; i < spnColumn->value(); i++) {
      header += "<th></th>";
    }
    toReturn += "<tr>"+header+"</tr><tbody>";
  }
  
  for (int i = 0; i < spnColumn->value(); i++) {
    aRow += "<td></td>";
  }
  
  for (int i = 0; i < spnRow->value(); i++) {
    toReturn += "<tr>"+aRow+"</tr>";
  }
  
  toReturn += QString((ckbAddHeader->isChecked())?"</tbody>":"")+"</table>";
  emit addTable(toReturn);
}