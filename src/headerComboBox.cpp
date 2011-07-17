#include "headerComboBox.h"

HeaderComboBox::HeaderComboBox(QWidget* parent) : QComboBox(parent) {
  QStringList headerSize;
  headerSize << "Header 1" << "Header 2" << "Header 3" << "Header 4" << "Header 5" << "Header 6";
  addItems(headerSize);
  QListWidget* headerList = new QListWidget();
  headerList->setStyleSheet("QListView::item {\
			      height:35px;\
			      color:transparent;\
			    }\
			    QListView::item:selected {\
			      boder:0px;\
			      background-color:transparent;\
			      height:30px;\
			      min-height:30px;\
			      width:100%;\
			    }\
			    ");
  headerList->addItems(headerSize);
  QLabel* lbltestHesder = new QLabel("<h1>Header 1</h1>");
  lbltestHesder->setFocusPolicy(Qt::NoFocus);
  headerList->setItemWidget(headerList->item(0),lbltestHesder);
  headerList->setItemWidget(headerList->item(1),new QLabel("<h2>Header 2</h2>"));
  headerList->setItemWidget(headerList->item(2),new QLabel("<h3>Header 3</h3>"));
  headerList->setItemWidget(headerList->item(3),new QLabel("<h4>Header 4</h4>"));
  headerList->setItemWidget(headerList->item(4),new QLabel("<h5>Header 5</h5>"));
  headerList->setItemWidget(headerList->item(5),new QLabel("<h6>Header 6</h6>"));
  setView(headerList);
  setModel(headerList->model());
}