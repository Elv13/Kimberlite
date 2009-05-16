Constructor {
  /***************************************************************
  
			CSS ADVANCED mode
  
  ***************************************************************/
  
  tabAdvanced = new QWidget();
  tabAdvanced->setObjectName(QString::fromUtf8("tabAdvanced"));
  verticalLayout_18 = new QVBoxLayout(tabAdvanced);
  verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
  tblCSSPage = new QTableWidget(tabAdvanced);
  
  if (tblCSSPage->columnCount() < 5)
      tblCSSPage->setColumnCount(5);    

  QTableWidgetItem *__rowItem = new QTableWidgetItem();
  tblCSSPage->setVerticalHeaderItem(0, __rowItem);
  tblCSSPage->setObjectName(QString::fromUtf8("tblCSSPage"));
  tblCSSPage->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tblCSSPage->setAlternatingRowColors(true);
  tblCSSPage->setSelectionMode(QAbstractItemView::NoSelection);
  tblCSSPage->setWordWrap(false);
  tblCSSPage->setCornerButtonEnabled(false);

  verticalLayout_18->addWidget(tblCSSPage);

  hbButton2 = new QHBoxLayout();
  hbButton2->setObjectName(QString::fromUtf8("hbButton2"));
  btnaddClass = new QPushButton(tabAdvanced);
  btnaddClass->setObjectName(QString::fromUtf8("btnaddClass"));
  connect(btnaddClass, SIGNAL(clicked()), this, SLOT(addClasses()));

  hbButton2->addWidget(btnaddClass);

  btnAddPClass = new QPushButton(tabAdvanced);
  btnAddPClass->setObjectName(QString::fromUtf8("btnAddPClass"));
  hbButton2->addWidget(btnAddPClass);
  btnRemoveClass = new QPushButton(tabAdvanced);
  btnRemoveClass->setObjectName(QString::fromUtf8("btnRemoveClass"));
  hbButton2->addWidget(btnRemoveClass);
  horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hbButton2->addItem(horizontalSpacer);
  verticalLayout_18->addLayout(hbButton2);

  tabWCSSLevel->addTab(tabAdvanced, QString());
  
    //tblCSSPage->setSortingEnabled(false);
  //btnaddClass->setText(QApplication::translate("this", "Add class", 0, QApplication::UnicodeUTF8));
  //btnAddPClass->setText(QApplication::translate("this", "Add pseudo class", 0, QApplication::UnicodeUTF8));
  //btnRemoveClass->setText(QApplication::translate("this", "Remove class", 0, QApplication::UnicodeUTF8));
  //tabWCSSLevel->setTabText(tabWCSSLevel->indexOf(tabAdvanced), QApplication::translate("this", "Advanced", 0, QApplication::UnicodeUTF8));
 
}

void MainWindow::fillCSSAdvMode() {
  QString color1 = "background-color:#7AEEFF;";
  QString color2 ="background-color:#5BFF74;";
  QString color = color1;
  int nbRow =0;
  tblCSSPage->setRowCount(nbRow);
  QStringList classList = CssParser::getClassList();
  for (int i =0; i < classList.count();i++) {
    nbRow++;
    tblCSSPage->setRowCount(nbRow);
    QWidget* classNameBackground = new QWidget(tblCSSPage);
    classNameBackground->setStyleSheet(color);
    QLabel* lblClassName = new QLabel(classNameBackground);
    lblClassName->setText("<b>"+classList[i]+"  { </b>");
    tblCSSPage->setCellWidget((nbRow-1), 0, classNameBackground);

    QWidget* classNameBackground1 = new QWidget(tblCSSPage);
    classNameBackground1->setStyleSheet(color);
    tblCSSPage->setCellWidget((nbRow-1), 1, classNameBackground1);

    QWidget* classNameBackground2 = new QWidget(tblCSSPage);
    classNameBackground2->setStyleSheet(color);
    tblCSSPage->setCellWidget((nbRow-1), 2, classNameBackground2);

    QWidget* classNameBackground3 = new QWidget(tblCSSPage);
    classNameBackground3->setStyleSheet(color);
    tblCSSPage->setCellWidget((nbRow-1), 3, classNameBackground3);

    QWidget* classNameBackground4 = new QWidget(tblCSSPage);
    classNameBackground4->setStyleSheet(color);
    tblCSSPage->setCellWidget((nbRow-1), 4, classNameBackground4);

    QStringList proprietyList = CssParser::getClass(classList[i]);
    for (int j =0; j < proprietyList.count();j++) {
      QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
      sizePolicy.setHorizontalStretch(0);
      sizePolicy.setVerticalStretch(0);

      QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
      sizePolicy1.setHorizontalStretch(0);
      sizePolicy1.setVerticalStretch(0);

      //Title
      nbRow++;
      tblCSSPage->setRowCount(nbRow);
      //QTableWidgetItem *__tableItem2 = new QTableWidgetItem("   "+proprietyList[j]);
      //tblCSSPage->setItem((nbRow-1), 0, __tableItem2);
      QWidget* proprietyNameBackground = new QWidget(tblCSSPage);
      proprietyNameBackground->setStyleSheet(color);
      QLabel* lblProprietyName = new QLabel(proprietyNameBackground);
      lblProprietyName->setText("    "+CssParser::getPropriety(proprietyList[j])+": ");
      tblCSSPage->setCellWidget((nbRow-1), 0, proprietyNameBackground);


      //Value
      QWidget* valueBackground = new QWidget(tblCSSPage);
      valueBackground->setStyleSheet( color + "margin-top:1px;");
      QHBoxLayout* horizontalLayout_value = new QHBoxLayout(valueBackground);
      horizontalLayout_value->setObjectName(QString::fromUtf8("horizontalLayout_value"));
      QComboBox* cbbValue = new QComboBox(valueBackground);
      cbbValue->setObjectName(QString::fromUtf8("cbbValue"));
      cbbValue->setStyleSheet("background-color:default;");
      cbbValue->setMinimumSize(100,30);
      sizePolicy.setHeightForWidth(cbbValue->sizePolicy().hasHeightForWidth());
      tblCSSPage->setCellWidget((nbRow-1), 1, valueBackground);
      cbbValue->setSizePolicy(sizePolicy);
      horizontalLayout_value->addWidget(cbbValue);

      //Unit
      QWidget* unitBackground = new QWidget(tblCSSPage);
      QHBoxLayout* horizontalLayout_unit = new QHBoxLayout(unitBackground);
      unitBackground->setStyleSheet( color + "margin:0px;padding:0px;spacing:0px;");
      QComboBox* cbbUnit = new QComboBox(unitBackground);
      cbbUnit->setMinimumSize(50,30);
      cbbUnit->setStyleSheet("background-color:default;");
      tblCSSPage->setCellWidget((nbRow-1), 2, unitBackground);
      horizontalLayout_unit->addWidget(cbbUnit);
      cbbUnit->setSizePolicy(sizePolicy);

      //Add comment
      QWidget* commentBackground = new QWidget(tblCSSPage);
      commentBackground->setStyleSheet( color + "margin:0px;padding:0px;spacing:0px;");
      //QHBoxLayout* horizontalLayout_comment = new QHBoxLayout(commentBackground);
      QPushButton* btnAddComment = new QPushButton(commentBackground);
      btnAddComment->setObjectName(QString::fromUtf8("btnAddComment"));
      btnAddComment->setStyleSheet("background-color:default;");
      btnAddComment->setSizePolicy(sizePolicy1);
      btnAddComment->setMinimumSize(30,30);
      btnAddComment->setMaximumSize(30,30);
      //horizontalLayout_comment->addWidget(btnAddComment);
      tblCSSPage->setCellWidget((nbRow-1), 3, commentBackground);
      KIcon icnAddComment("edit-rename");
      btnAddComment->setIcon(icnAddComment);

      //Remove Line
      QWidget* removeLineBackground = new QWidget(tblCSSPage);
      removeLineBackground->setStyleSheet( color + "margin-top:1px;");
      //QHBoxLayout* horizontalLayout_removeLine = new QHBoxLayout(removeLineBackground);
      QPushButton* btnRemoveLine = new QPushButton(removeLineBackground);
      btnRemoveLine->setStyleSheet("background-color:default;");
      btnRemoveLine->setSizePolicy(sizePolicy1);
      btnRemoveLine->setMinimumSize(30,30);
      btnRemoveLine->setMaximumSize(30,30);
      //horizontalLayout_removeLine->addWidget(btnRemoveLine);
      tblCSSPage->setCellWidget((nbRow-1), 4, removeLineBackground);
      KIcon icnRemove("list-remove");
      btnRemoveLine->setIcon(icnRemove);

      QList<bool>* isEditable = new QList<bool>;
      QSqlQuery query;
      query.exec("SELECT VALUE,UNIT FROM TCSS_TAG WHERE TITLE = '"+ CssParser::getPropriety(proprietyList[j]) +"'");
      
      while (query.next()) {
	QStringList possibleValues = query.value(0).toString().split(";");
	for (int i =0;i < possibleValues.count(); i++) {
	  if (possibleValues[i].left(4) == "UNIT") {
	    cbbValue->addItem(possibleValues[i].remove(0,4));
	    //cbbValue->setEditable(true);
	    isEditable->push_back(true);
	  }
	  else {
	    cbbValue->addItem(possibleValues[i]);
	    isEditable->push_back(false);
	  }
	}
	QStringList possibleUnit = query.value(1).toString().split(";");
	for (int i =0;i < possibleUnit.count(); i++) {
	  if (possibleUnit[i].isEmpty() == false)
	    cbbUnit->addItem(possibleUnit[i]);
	}
	//cbbUnit->addItem(query.value(1).toString());
      }




    }

    AddProprietyWidget* anEndOfClass =  new AddProprietyWidget(tblCSSPage);
    anEndOfClass->setStyleSheet( color);
    nbRow++;
    tblCSSPage->setRowCount(nbRow);
    tblCSSPage->setCellWidget((nbRow-1), 0, anEndOfClass);

    QWidget* classEndBackground1 = new QWidget(tblCSSPage);
    classEndBackground1->setStyleSheet(color);
    QWidget* classEndBackground2 = new QWidget(tblCSSPage);
    classEndBackground2->setStyleSheet(color);
    QWidget* classEndBackground3 = new QWidget(tblCSSPage);
    classEndBackground3->setStyleSheet(color);
    QWidget* classEndBackground4 = new QWidget(tblCSSPage);
    classEndBackground4->setStyleSheet(color);
    tblCSSPage->setCellWidget((nbRow-1), 1, classEndBackground1);
    tblCSSPage->setCellWidget((nbRow-1), 2, classEndBackground2);
    tblCSSPage->setCellWidget((nbRow-1), 3, classEndBackground3);
    tblCSSPage->setCellWidget((nbRow-1), 4, classEndBackground4);

    nbRow++;
    tblCSSPage->setRowCount(nbRow);

    QWidget* betweenClassBackground0 = new QWidget(tblCSSPage);
    betweenClassBackground0->setStyleSheet(color);

    QWidget* betweenClassBackground1 = new QWidget(tblCSSPage);
    betweenClassBackground1->setStyleSheet(color);

    QWidget* betweenClassBackground2 = new QWidget(tblCSSPage);
    betweenClassBackground2->setStyleSheet(color);

    QWidget* betweenClassBackground3 = new QWidget(tblCSSPage);
    betweenClassBackground3->setStyleSheet(color);

    QWidget* betweenClassBackground4 = new QWidget(tblCSSPage);
    betweenClassBackground4->setStyleSheet(color);

    tblCSSPage->setCellWidget((nbRow-1), 0, betweenClassBackground0);
    tblCSSPage->setCellWidget((nbRow-1), 1, betweenClassBackground1);
    tblCSSPage->setCellWidget((nbRow-1), 2, betweenClassBackground2);
    tblCSSPage->setCellWidget((nbRow-1), 3, betweenClassBackground3);
    tblCSSPage->setCellWidget((nbRow-1), 4, betweenClassBackground4);


    //__tableItem->setText();
    if (color == color1) 
      color = color2;
    else
      color =color1;
  }
} //fillCSSAdvMode