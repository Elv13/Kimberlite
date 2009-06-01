/**     @file mainwindow.cpp

	This file is part of the Kimberlite project
	Copyright (C) 2008 Emmanuel Lepage Vallee <elv1313@gmail.com>
	
	This software is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 or 3 of the License, or (at your option) any later version.

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public License
	along with this library; see the file COPYING.LIB.  If not, write to
	the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
	Boston, MA 02111-1307, USA.


        @author Emmanuel Lepage Vallée
        @date 13 August 2008
        @version 0.1-pre-alpah0
*/
#include "mainwindow.h"

#include "src/CSSbeginnerWidget.h"
#include "src/addProprietyWidget.h"
#include "src/ProjectManager_v2.h"
#include "src/template.h"
#include "src/ressourcesManager.h"
#include "src/newTable.h"
#include "src/newPage.h"
#include "src/newScript.h"
#include "src/newProject.h"
#include "src/debugger.h"

MainWindow::MainWindow(QWidget* parent)  : KMainWindow(parent),currentHTMLPage(NULL),currentScript(NULL),aProjectManager(NULL),isModified(false) {
  actionCollection = new KActionCollection(this);
  setWindowTitle("Kimberlite");
  db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
  db->setDatabaseName( KStandardDirs::locate( "appdata", "kimberlite.db" ));
  qDebug() << (( db->open())? "Database corectly opened":"ERROR while opening the database, get ready for a crash");

  aParser = new HtmlParser();
  tabWMenu = new KTabWidget(this);
  tabWMenu->setMinimumSize(QSize(0, 90));
  tabWMenu->setMaximumSize(QSize(9999999, 90));
  
  /***************************************************************
  
			File toolbar
  
  ***************************************************************/

  fileTB = new KToolBar(this);
  tabWMenu->addTab(fileTB, "Files");
  QPalette aPalette;
  fileTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

  createAction("New Project", "document-new", Qt::CTRL + Qt::Key_N);
  connect(ashActions["New Project"], SIGNAL(triggered(bool)),this, SLOT(newProject()));
  fileTB->addAction(ashActions["New Project"]);

  createAction("Open Project", "document-open", Qt::CTRL + Qt::Key_O);
  connect(ashActions["Open Project"], SIGNAL(triggered(bool)),this, SLOT(openProject()));
  fileTB->addAction(ashActions["Open Project"]);

  fileTB->addSeparator();

  createAction("Save", "document-save", Qt::CTRL + Qt::Key_S);
  connect(ashActions["Save"], SIGNAL(triggered(bool)),this, SLOT(saveProject()));
  fileTB->addAction(ashActions["Save"]);

  createAction("Save As", "document-save-as", Qt::CTRL + Qt::SHIFT + Qt::Key_S);
  connect(ashActions["Save As"], SIGNAL(triggered(bool)), this, SLOT(saveProjectAs()));
  fileTB->addAction(ashActions["Save As"]);

  fileTB->addSeparator();

  createAction("Print", "document-print", Qt::CTRL + Qt::Key_P);
  connect(ashActions["Print"], SIGNAL(triggered(bool)),this, SLOT(print()));
  fileTB->addAction(ashActions["Print"]);

  createAction("Print Preview", "document-print-preview", NULL);
  connect(ashActions["Print Preview"], SIGNAL(triggered(bool)), this, SLOT(printPreview()));
  fileTB->addAction(ashActions["Print Preview"]);

  fileTB->addSeparator();

  createAction("Quit", "application-exit", Qt::CTRL + Qt::Key_Q);
  connect(ashActions["Quit"], SIGNAL(triggered(bool)),this, SLOT(quit()));
  fileTB->addAction(ashActions["Quit"]);
  
  /***************************************************************
  
			Edit toolbar
  
  ***************************************************************/

  menuEdit = new QWidget();
  menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
  horizontalLayout_14 = new QGridLayout(menuEdit);
  horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
  horizontalLayout_14->setContentsMargins(0,0,0,0);
  horizontalLayout_14->setSpacing(0);
  editTB = new KToolBar(this);
  editTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");
  horizontalLayout_14->addWidget(editTB,0,0,2,1);

  createAction("Undo", "edit-undo", Qt::CTRL + Qt::Key_Z);
  connect(ashActions["Undo"], SIGNAL(triggered(bool)), this, SLOT(undo()));
  editTB->addAction(ashActions["Undo"]);

  createAction("Redo", "edit-redo", Qt::CTRL + Qt::SHIFT + Qt::Key_Z);
  connect(ashActions["Redo"], SIGNAL(triggered(bool)), this, SLOT(redo()));
  editTB->addAction(ashActions["Redo"]);

  editTB->addSeparator();

  createAction("Copy", "edit-copy", Qt::CTRL + Qt::Key_C);
  connect(ashActions["Copy"], SIGNAL(triggered(bool)), this, SLOT(copy()));
  editTB->addAction(ashActions["Copy"]);

  createAction("Cut", "edit-cut", Qt::CTRL + Qt::Key_X);
  connect(ashActions["Cut"], SIGNAL(triggered(bool)), this, SLOT(cut()));
  editTB->addAction(ashActions["Cut"]);

  createAction("Paste", "edit-paste", Qt::CTRL + Qt::Key_V);
  connect(ashActions["Paste"], SIGNAL(triggered(bool)), this, SLOT(paste()));
  editTB->addAction(ashActions["Paste"]);

  editTB->addSeparator();

  createAction("Find", "edit-find", Qt::CTRL + Qt::Key_F);
  connect(ashActions["Find"], SIGNAL(triggered(bool)), this, SLOT(find()));
  editTB->addAction(ashActions["Find"]);

  editTB->addSeparator();

  hlFont = new QHBoxLayout();
  hlFont->setContentsMargins(0,0,0,0);
  hlFont->setSpacing(0);
  
  cbbFont = new QComboBox(menuEdit);
  cbbFont->setObjectName(QString::fromUtf8("cbbFont"));
  cbbFont->setMinimumSize(QSize(225, 0));
  cbbFont->setMaximumSize(QSize(225, 25));
  cbbFont->addItems(QFontDatabase().families());
  connect(cbbFont, SIGNAL(currentIndexChanged (QString)), this, SLOT(setFont(QString)));
  hlFont->addWidget(cbbFont);

  cbbFontSize = new KIntSpinBox(menuEdit);
  cbbFontSize->setObjectName(QString::fromUtf8("cbbFontSize"));
  cbbFontSize->setMaximumSize(QSize(225, 25));
  cbbFontSize->setSuffix("px");
  connect(cbbFontSize, SIGNAL(valueChanged(int)), this, SLOT(setFontSize(int)));
  hlFont->addWidget(cbbFontSize);

  cbbHeader = new KComboBox(menuEdit);
  cbbHeader->setMaximumSize(QSize(225, 25));
  cbbHeader->setObjectName(QString::fromUtf8("cbbHeader"));
  QStringList headerSize;
  headerSize << "Header 1" << "Header 2" << "Header 3" << "Header 4" << "Header 5" << "Header 6";
  cbbHeader->addItems(headerSize);
  connect(cbbHeader, SIGNAL(currentIndexChanged (QString)), this, SLOT(setHeader(QString)));
  hlFont->addWidget(cbbHeader);

  hlTextAtributeButton = new QHBoxLayout();
  hlTextAtributeButton->setContentsMargins(0,0,0,0);
  hlTextAtributeButton->setSpacing(0);
  
  btnBold = createToolButton(menuEdit,"format-text-bold","Make selected bold",true);
  connect(btnBold, SIGNAL(clicked()), this, SLOT(setBold()));
  hlTextAtributeButton->addWidget(btnBold);

  btnUnderline =  createToolButton(menuEdit,"format-text-underline","Underline selected text",true);
  connect(btnUnderline, SIGNAL(clicked()), this, SLOT(setUnderline()));
  hlTextAtributeButton->addWidget(btnUnderline);

  btnItalic = createToolButton(menuEdit,"format-text-italic","Make selected text italic",true);
  connect(btnItalic, SIGNAL(clicked()), this, SLOT(setItalic()));
  hlTextAtributeButton->addWidget(btnItalic);

  hlTextAtributeButton->addWidget(createSpacer());

  btnAlignLeft = createToolButton(menuEdit,"format-justify-left","Align left selected text");
  connect(btnAlignLeft, SIGNAL(clicked()), this, SLOT(setAlignLeft()));
  hlTextAtributeButton->addWidget(btnAlignLeft);

  btnAlignCenter = createToolButton(menuEdit,"format-justify-center","Center the selected text");
  connect(btnAlignCenter, SIGNAL(clicked()), this, SLOT(setAlignCenter()));
  hlTextAtributeButton->addWidget(btnAlignCenter);

  btnAlignRight =  createToolButton(menuEdit,"format-justify-right","Align right the selected text");
  connect(btnAlignRight, SIGNAL(clicked()), this, SLOT(setAlignRight()));
  hlTextAtributeButton->addWidget(btnAlignRight);

  btnJustify = createToolButton(menuEdit,"format-justify-fill","Justify the selected text");
  connect(btnJustify, SIGNAL(clicked()), this, SLOT(setJustify()));
  hlTextAtributeButton->addWidget(btnJustify);
  
  hlTextAtributeButton->addWidget(createSpacer());
  
  btnList =  createToolButton(menuEdit,"format-list-unordered","Create a dot list");
  connect(btnList, SIGNAL(clicked()), this, SLOT(setUList()));
  hlTextAtributeButton->addWidget(btnList);
  
  btnChar = createToolButton(menuEdit,"format-list-ordered","Create an ordered list");
  hlTextAtributeButton->addWidget(btnChar);
  
  hlTextAtributeButton->addWidget(createSpacer());
  
  lblTextColor = new QLabel(menuEdit);
  lblTextColor->setObjectName(QString::fromUtf8("lblTextColor"));
  lblTextColor->setMaximumSize(QSize(25, 24));
  KIcon* icnTextColor = new KIcon("format-text-color");
  lblTextColor->setPixmap(icnTextColor->pixmap(20,QIcon::Normal,QIcon::On));
  hlTextAtributeButton->addWidget(lblTextColor);

  kcbbTextColor = new KColorCombo(menuEdit);
  kcbbTextColor->setMaximumSize(QSize(25, 24));
  kcbbTextColor->setObjectName(QString::fromUtf8("kcbbTextColor"));
  connect(kcbbTextColor, SIGNAL(currentIndexChanged(int)), this, SLOT(setTextColor()));
  hlTextAtributeButton->addWidget(kcbbTextColor);

  lblHighlightColor = new QLabel(menuEdit);
  lblHighlightColor->setObjectName(QString::fromUtf8("lblHighlightColor"));
  lblHighlightColor->setMaximumSize(QSize(25, 24));
  KIcon* icnHighlightColor = new KIcon("format-stroke-color");
  lblHighlightColor->setPixmap(icnHighlightColor->pixmap(18,QIcon::Normal,QIcon::On));
  hlTextAtributeButton->addWidget(lblHighlightColor);

  cbbHighlightColor = new KColorCombo(menuEdit);
  cbbHighlightColor->setMaximumSize(QSize(25, 24));
  cbbHighlightColor->setObjectName(QString::fromUtf8("cbbHighlightColor"));
  //cbbHighlightColor->setStyleSheet("QComboBox QAbstractItemView { min-width:50px;}");
  connect(cbbHighlightColor, SIGNAL(currentIndexChanged(int)), this, SLOT(setHighlightColor()));
  hlTextAtributeButton->addWidget(cbbHighlightColor);

  lblBackgroundColor = new QLabel(menuEdit);
  lblBackgroundColor->setMaximumSize(QSize(25, 24));
  lblBackgroundColor->setObjectName(QString::fromUtf8("lblBackgroundColor"));
  KIcon* icnBackgroundColor = new KIcon("fill-color");
  lblBackgroundColor->setPixmap(icnBackgroundColor->pixmap(18,QIcon::Normal,QIcon::On));
  hlTextAtributeButton->addWidget(lblBackgroundColor);

  cbbBackgroundColor = new KColorCombo(menuEdit);
  cbbBackgroundColor->setMaximumSize(QSize(25, 24));
  cbbBackgroundColor->setObjectName(QString::fromUtf8("cbbBackgroundColor"));
  connect(cbbBackgroundColor, SIGNAL(currentIndexChanged(int)), this, SLOT(setBackgroundColor()));
  hlTextAtributeButton->addWidget(cbbBackgroundColor);
  
  hlTextAtributeButton->addItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

  horizontalLayout_14->addLayout(hlFont,0,1);
  horizontalLayout_14->addLayout(hlTextAtributeButton,1,1);
  horizontalLayout_14->addItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum),0,2);

  tabWMenu->addTab(menuEdit, "Edit");
  
  /***************************************************************
  
			View toolbar
  
  ***************************************************************/

  viewTB = new KToolBar(this);
  tabWMenu->addTab(viewTB, "View");
  viewTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

  createAction("Project", KStandardDirs::locate("appdata", "pixmap/showProject.png"), NULL,true);
  viewTB->addAction(ashActions["Project"]);

  createAction("CSS Class", KStandardDirs::locate("appdata", "pixmap/showCSS.png"), NULL, true);
  viewTB->addAction(ashActions["CSS Class"]);

  createAction("HTML Tree", KStandardDirs::locate("appdata", "pixmap/showHTML.png"), NULL, true);
  viewTB->addAction(ashActions["HTML Tree"]);

  createAction("Debugger", KStandardDirs::locate("appdata", "pixmap/showDebugger.png"), NULL, true);
  viewTB->addAction(ashActions["Debugger"]);
  
  createAction("Inspector", KStandardDirs::locate("appdata", "pixmap/showInspector.png"), NULL);
  connect(ashActions["Inspector"], SIGNAL(triggered(bool)), this, SLOT(showInspector(bool)));
  viewTB->addAction(ashActions["Inspector"]);
  
  viewTB->addSeparator();

  createAction("Zoom In", "zoom-in", Qt::CTRL + Qt::Key_Equal);
  connect(ashActions["Zoom In"], SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
  viewTB->addAction(ashActions["Zoom In"]);

  createAction("Zoom Out", "zoom-out", Qt::CTRL + Qt::Key_Minus);
  connect(ashActions["Zoom Out"], SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
  viewTB->addAction(ashActions["Zoom Out"]);

  createAction("Zoom 1:1", "zoom-original", NULL);
  connect(ashActions["Zoom 1:1"], SIGNAL(triggered(bool)), this, SLOT(zoomDefault()));
  viewTB->addAction(ashActions["Zoom 1:1"]);

  /***************************************************************
  
			Insert toolbar
  
  ***************************************************************/
  
  menuInsert = new QWidget();
  menuInsert->setStyleSheet("margin:0px;spacing:0px;padding:0px;");
  menuInsert->setObjectName(QString::fromUtf8("menuInsert"));
  tabWMenu->addTab(menuInsert, "Insert");
  hlInsert = new QGridLayout(menuInsert);
  hlInsert->setContentsMargins(0,0,0,0);
  hlInsert->setObjectName(QString::fromUtf8("hlInsert"));
  hlInsert->setSpacing(0);

  insertTB = new KToolBar(this);
  insertTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");
  hlInsert->addWidget(insertTB,0,11,0,2);

  createAction("New Page", "document-new", Qt::CTRL + Qt::ALT + Qt::Key_N);
  connect(ashActions["New Page"], SIGNAL(triggered(bool)), this, SLOT(addHtmlPage()));
  insertTB->addAction(ashActions["New Page"]);

  createAction("New Script", "application-add", Qt::CTRL + Qt::SHIFT + Qt::Key_N);
  connect(ashActions["New Script"], SIGNAL(triggered(bool)), this, SLOT(addScript()));
  insertTB->addAction(ashActions["New Script"]);

  insertTB->addSeparator();

  createAction("Add Image", "insert-image", Qt::CTRL + Qt::Key_I);
  connect(ashActions["Add Image"], SIGNAL(triggered(bool)), this, SLOT(insertImage()));
  insertTB->addAction(ashActions["Add Image"]);

  createAction("Add Table", "insert-table", Qt::CTRL + Qt::Key_T);
  connect(ashActions["Add Table"], SIGNAL(triggered(bool)), this, SLOT(insertTable()));
  insertTB->addAction(ashActions["Add Table"]);

  createAction("Add Link", "insert-link", Qt::CTRL + Qt::Key_L);
  connect(ashActions["Add Link"], SIGNAL(triggered(bool)), this, SLOT(insertLink()));
  insertTB->addAction(ashActions["Add Link"]);

  createAction("Add Anchor", "view-refresh", NULL);
  connect(ashActions["Add Anchor"], SIGNAL(triggered(bool)), this, SLOT(quit()));
  insertTB->addAction(ashActions["Add Anchor"]);

  insertTB->addSeparator();

  createAction("Character", "list-add-font", NULL);
  connect(ashActions["Character"], SIGNAL(triggered(bool)), this, SLOT(insertChar()));
  insertTB->addAction(ashActions["Character"]);

  QSpacerItem* horizontalSpacer8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hlInsert->addItem(horizontalSpacer8,0,12);
  
  QFrame* aline2 = new QFrame(menuEdit);
  aline2->setObjectName(QString::fromUtf8("line"));
  aline2->setFrameShape(QFrame::VLine);
  aline2->setFrameShadow(QFrame::Sunken);
  aline2->setMinimumSize(6,0);
  aline2->setStyleSheet("margin:3px;padding:3px;width:20px;");
  hlInsert->addWidget(aline2,0,10,2,1);

  btnNewLine = createToolButton(menuInsert,"view-refresh","Add line");
  hlInsert->addWidget(btnNewLine,0,1);

  btnNewTab = createToolButton(menuInsert,"view-refresh","Add tab");
  hlInsert->addWidget(btnNewTab,1,0);

  btnNewSpace = createToolButton(menuInsert,"view-refresh","Add space");
  hlInsert->addWidget(btnNewSpace,0,0);
  
  KPushButton* btnHr = createToolButton(menuInsert,"view-refresh","Add horizontal line");
  hlInsert->addWidget(btnHr,1,1);
  
  QFrame* aline = new QFrame(menuEdit);
  aline->setObjectName(QString::fromUtf8("line"));
  aline->setFrameShape(QFrame::VLine);
  aline->setFrameShadow(QFrame::Sunken);
  aline->setMinimumSize(6,0);
  aline->setStyleSheet("margin:3px;padding:3px;width:20px;");
  hlInsert->addWidget(aline,0,2,2,1);
  
  KPushButton* btnTextLine = createToolButton(menuInsert,"view-refresh","Add a text field");
  hlInsert->addWidget(btnTextLine,0,3);
  
  KPushButton* btnPassword = createToolButton(menuInsert,"view-refresh","Add a password field");
  hlInsert->addWidget(btnPassword,1,3);
  
  KPushButton* btnCheckBox = createToolButton(menuInsert,"view-refresh","Add a checkbox");
  hlInsert->addWidget(btnCheckBox,0,4);
  
  KPushButton* btnRadioButton = createToolButton(menuInsert,"view-refresh","Add a radio button");
  hlInsert->addWidget(btnRadioButton,1,4);
  
  KPushButton* btnSubmit = createToolButton(menuInsert,"view-refresh","Add a submit button");
  hlInsert->addWidget(btnSubmit,0,5);
  
  KPushButton* btnReset = createToolButton(menuInsert,"view-refresh","Add a reset button");
  hlInsert->addWidget(btnReset,1,5);
  
  KPushButton* btnUpload = createToolButton(menuInsert,"view-refresh","Add an upload button");
  hlInsert->addWidget(btnUpload,0,6);
  
  KPushButton* btnHidden = createToolButton(menuInsert,"view-refresh","Add an hidden object");
  hlInsert->addWidget(btnHidden,1,6);
  
  KPushButton* btnButton = createToolButton(menuInsert,"view-refresh","Add a simple button");
  hlInsert->addWidget(btnButton,0,7);
  
  KPushButton* btnTextAera = createToolButton(menuInsert,"view-refresh","Add a text area");
  hlInsert->addWidget(btnTextAera,1,7);
  
  KPushButton* btnHtmlButton = createToolButton(menuInsert,"view-refresh","Add a rich (html) button");
  hlInsert->addWidget(btnHtmlButton,0,8);
  
  KPushButton* btnSelect = createToolButton(menuInsert,"view-refresh","Add a combo box");
  hlInsert->addWidget(btnSelect,1,8);
  
  KPushButton* btnList = createToolButton(menuInsert,"view-refresh","Add a list");
  hlInsert->addWidget(btnList,0,9);
  
  KPushButton* btnLabel = createToolButton(menuInsert,"view-refresh","Add a label");
  hlInsert->addWidget(btnLabel,1,9);

  /***************************************************************
  
			Tools toolbar
  
  ***************************************************************/
  
  toolsTB = new KToolBar(this);
  tabWMenu->addTab(toolsTB, "Tools");
  toolsTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

  createAction("Parse", "format-indent-more", Qt::CTRL + Qt::Key_P);
  connect(ashActions["Parse"], SIGNAL(triggered(bool)), this, SLOT(reParse()));
  toolsTB->addAction(ashActions["Parse"]);

  createAction("Templaterize", "view-pim-tasks", NULL);
  connect(ashActions["Templaterize"], SIGNAL(triggered(bool)), this, SLOT(templaterize()));
  toolsTB->addAction(ashActions["Templaterize"]);

  createAction("Translate", "application-x-marble", NULL);
  connect(ashActions["Translate"], SIGNAL(triggered(bool)), this, SLOT(translate()));
  toolsTB->addAction(ashActions["Translate"]);

  createAction("Debug", "tools-report-bug", Qt::CTRL + Qt::Key_D);
  connect(ashActions["Debug"], SIGNAL(triggered(bool)), this, SLOT(debugHtml()));
  toolsTB->addAction(ashActions["Debug"]);

  toolsTB->addSeparator();

  createAction("Match Tag", "application-xml", NULL);
  connect(ashActions["Match Tag"], SIGNAL(triggered(bool)), this, SLOT(quit()));
  toolsTB->addAction(ashActions["Match Tag"]);
  ashActions["Match Tag"]->setDisabled(true);

  createAction("Spelling", "tools-check-spelling", NULL);
  connect(ashActions["Spelling"], SIGNAL(triggered(bool)), this, SLOT(quit()));
  toolsTB->addAction(ashActions["Spelling"]);
  ashActions["Spelling"]->setDisabled(true);

  createAction("Check Link", "network-connect", NULL);
  connect(ashActions["Check Link"], SIGNAL(triggered(bool)), this, SLOT(quit()));
  toolsTB->addAction(ashActions["Check Link"]);
  ashActions["Check Link"]->setDisabled(true);

  /***************************************************************
  
			Option toolbar
  
  ***************************************************************/

  QWidget* optionToolbarCentral = new QWidget(this);
  tabWMenu->addTab(optionToolbarCentral, "Settings");
  
  QGridLayout* optToolBarLayout = new QGridLayout(optionToolbarCentral);
  optToolBarLayout->setContentsMargins(0,0,0,0);
  optToolBarLayout->setSpacing(0);
  
  optionsTB = new KToolBar(this);
  optionsTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");
  optToolBarLayout->addWidget(optionsTB,0,0);
  
  createAction("Configure Shortcuts", "configure-shortcuts", NULL);
  connect(ashActions["Configure Shortcuts"], SIGNAL(triggered(bool)), this, SLOT(editShortcut()));
  optionsTB->addAction(ashActions["Configure Shortcuts"]);
  
  optToolBarLayout->addWidget(createSpacer(),0,1);
  
  QComboBox* cbbLanguage = new QComboBox(this);
  QStringList toAdd;
  toAdd << "HTML 4.01" << "xHTML 1.0.1" << "HTML 5.0";
  cbbLanguage->addItems(toAdd);
  optToolBarLayout->addWidget(cbbLanguage,0,2);
  
  optToolBarLayout->addWidget(createSpacer(),0,3);
  
  QLabel* lblFirefox = new QLabel(menuEdit);
  KIcon icnFirefox(KStandardDirs::locate("appdata", "pixmap/browserLogo/FF.png"));
  lblFirefox->setPixmap(icnFirefox.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblFirefox,0,4);
  lblFirefox->setDisabled(true);
  
  QComboBox* cbbFirefox = new QComboBox(this);
  toAdd.clear();
  toAdd << "FF1" << "FF2" << "FF3";
  cbbFirefox->addItems(toAdd);
  optToolBarLayout->addWidget(cbbFirefox,0,5);
  cbbFirefox->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,6);
  
  QLabel* lblInternetExplorer = new QLabel(menuEdit);
  KIcon icnInternetExplorer(KStandardDirs::locate("appdata", "pixmap/browserLogo/IE.png"));
  lblInternetExplorer->setPixmap(icnInternetExplorer.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblInternetExplorer,0,7);
  lblInternetExplorer->setDisabled(true);
  
  QComboBox* cbbInternetExplorer = new QComboBox(this);
  toAdd.clear();
  toAdd << "IE6" << "IE7" << "IE8";
  cbbInternetExplorer->addItems(toAdd);
  optToolBarLayout->addWidget(cbbInternetExplorer,0,8);
  cbbInternetExplorer->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,9);
  
  QLabel* lblSafari = new QLabel(menuEdit);
  KIcon icnSafari(KStandardDirs::locate("appdata", "pixmap/browserLogo/SF.png"));
  lblSafari->setPixmap(icnSafari.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblSafari,0,10);
  lblSafari->setDisabled(true);
  
  QComboBox* cbbSafari = new QComboBox(this);
  toAdd.clear();
  toAdd << "SF2" << "SF3" << "SF4";
  cbbSafari->addItems(toAdd);
  optToolBarLayout->addWidget(cbbSafari,0,11);
  cbbSafari->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,12);
  
  QLabel* lblOpera = new QLabel(menuEdit);
  KIcon icnOpera(KStandardDirs::locate("appdata", "pixmap/browserLogo/OP.png"));
  lblOpera->setPixmap(icnOpera.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblOpera,0,13);
  lblOpera->setDisabled(true);
  
  QComboBox* cbbOpera = new QComboBox(this);
  toAdd.clear();
  toAdd << "OP7" << "OP8" << "OP9";
  cbbOpera->addItems(toAdd);
  optToolBarLayout->addWidget(cbbOpera,0,14);
  cbbOpera->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,15);
  
  QLabel* lblKonqueror = new QLabel(menuEdit);
  KIcon icnKonqueror(KStandardDirs::locate("appdata", "pixmap/browserLogo/KQ.png"));
  lblKonqueror->setPixmap(icnKonqueror.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblKonqueror,0,16);
  lblKonqueror->setDisabled(true);
  
  QComboBox* cbbKonqueror = new QComboBox(this);
  toAdd.clear();
  toAdd << "Konq3" << "Konq4";
  cbbKonqueror->addItems(toAdd);
  optToolBarLayout->addWidget(cbbKonqueror,0,17);
  cbbKonqueror->setDisabled(true);
  
  optToolBarLayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum),0,18);

  /***************************************************************
  
			Help tool bar
  
  ***************************************************************/

  helpTB = new KToolBar(this);
  tabWMenu->addTab(helpTB, "Help");
  helpTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

  createAction("Manual", "help-contents", NULL);
  connect(ashActions["Manual"], SIGNAL(triggered(bool)), this, SLOT(quit()));
  helpTB->addAction(ashActions["Manual"]);
  ashActions["Manual"]->setDisabled(true);

  createAction("About", "help-about", NULL);
  connect(ashActions["About"], SIGNAL(triggered(bool)), this, SLOT(aboutKimberlite()));
  helpTB->addAction(ashActions["About"]);

  helpTB->addSeparator();

  createAction("Report A Bug", "tools-report-bug", NULL);
  connect(ashActions["Report A Bug"], SIGNAL(triggered(bool)), this, SLOT(reportBug()));
  helpTB->addAction(ashActions["Report A Bug"]);

  setMenuWidget(tabWMenu);
  
  /***************************************************************
  
			HTML tag Dock
  
  ***************************************************************/

  dockHtmlTree = new QDockWidget(this);
  dockHtmlTree->setWindowTitle("Tag tree");
  treeHtml = new QTreeWidget(this);
  treeHtml->setHeaderHidden(true);
  dockHtmlTree->setWidget(treeHtml);
  addDockWidget(Qt::LeftDockWidgetArea, dockHtmlTree);
  dockHtmlTree->setVisible(false);
  
  connect(treeHtml, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(setHtmlCursor(QTreeWidgetItem*)));
  connect(ashActions["HTML Tree"], SIGNAL(triggered(bool)), dockHtmlTree, SLOT(setVisible(bool)));
  connect(dockHtmlTree, SIGNAL(visibilityChanged(bool)), ashActions["HTML Tree"] , SLOT(setChecked(bool)));

  
  /*aHtmlThread = new ParserThread(this); //TODO make it work
  aHtmlThread->rtfHtml = rtfHTMLEditor;
  aHtmlThread->treeHtml = treeHtml;
  connect(rtfHTMLEditor, SIGNAL(textChanged()), aHtmlThread, SLOT(getReady()));
  aHtmlThread->start();*/

  /***************************************************************
  
			Project Dock
  
  ***************************************************************/

  aProjectManager = new ProjectManager2(0);
  
  tableDock = new QDockWidget(this);
  tableDock->setWindowTitle("Project");
  tableDock->setObjectName(QString::fromUtf8("tableDock"));
  tableDock->setWidget(aProjectManager);
  addDockWidget(Qt::LeftDockWidgetArea, tableDock);
  tableDock->setVisible(false);
  
  connect(aProjectManager, SIGNAL(htmlPageChanged(QTreeWidgetItem*, QString)), this, SLOT(loadPage(QTreeWidgetItem*, QString)));
  connect(aProjectManager, SIGNAL(javaScriptChanged(QTreeWidgetItem*, QString)), this, SLOT(loadScript(QTreeWidgetItem*, QString)));
  connect(aProjectManager, SIGNAL(loadCss(QString)), this, SLOT(loadCss(QString)));
  connect(ashActions["Project"], SIGNAL(triggered(bool)), tableDock , SLOT(setVisible(bool)));
  connect(tableDock, SIGNAL(visibilityChanged(bool)), ashActions["Project"] , SLOT(setChecked(bool)));

 
  /***************************************************************
  
			Class Dock
  
  ***************************************************************/

  treeDock = new QDockWidget(this);
  treeDock->setWindowTitle("CSS file");
  treeDock->setObjectName(QString::fromUtf8("treeDock"));
  treeDockCentral = new QWidget();
  treeDockCentral->setObjectName(QString::fromUtf8("treeDockCentral"));

  verticalLayout_5 = new QGridLayout(treeDockCentral);
  verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
  verticalLayout_5->setContentsMargins(0,0,0,0);
  
  treeWidget = new QTreeWidget(treeDockCentral);
  treeWidget->setHeaderHidden(true);
  treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
  treeWidget->setIconSize(QSize(18,18));
  treeWidget->setToolTip(QString("<img src=\"%1\" height=22 width=22> Class <br><img src=\"%2\" height=22 width=22> Identifier (ID)<br><img src=\"%3\" height=22 width=22> Tag<br><img src=\"%4\" height=22 width=22> State").arg(KStandardDirs::locate("appdata", "pixmap/class.png")).arg(KStandardDirs::locate("appdata", "pixmap/id.png")).arg(KStandardDirs::locate("appdata", "pixmap/tag.png")).arg(KStandardDirs::locate("appdata", "pixmap/state.png")));
  
  addDockWidget(Qt::LeftDockWidgetArea,treeDock );
  treeDock->setVisible(false);
  verticalLayout_5->addWidget(treeWidget,0,0,1,2);
  treeDock->setWidget(treeDockCentral);
  
  btnTreeAdd = new KPushButton(treeDockCentral);
  btnTreeAdd->setObjectName(QString::fromUtf8("btnTreeAdd"));
  verticalLayout_5->addWidget(btnTreeAdd,1,0);
  
  btnTreeRemove = new KPushButton(treeDockCentral);
  btnTreeRemove->setObjectName(QString::fromUtf8("btnTreeRemove"));
  verticalLayout_5->addWidget(btnTreeRemove,1,1);
  
  connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem* , int)), this, SLOT(cssClassClicked(QTreeWidgetItem*)));
  connect(btnTreeAdd, SIGNAL(clicked()), this, SLOT(addClasses()));
  connect(ashActions["CSS Class"], SIGNAL(triggered(bool)), treeDock, SLOT(setVisible(bool)));
  connect(treeDock, SIGNAL(visibilityChanged(bool)), ashActions["CSS Class"] , SLOT(setChecked(bool)));

  /***************************************************************
  
			Editing MODE
  
  ***************************************************************/
  
  tabWEditor = new QTabWidget(this);
  setCentralWidget(tabWEditor);
  tabWEditor->setObjectName(QString::fromUtf8("tabWEditor"));
  tabWEditor->setTabPosition(QTabWidget::South);
  setCentralWidget(tabWEditor);

  /***************************************************************
  
			WYSIWYG MODE
  
  ***************************************************************/

  webPreview = new QWebView(this);
  webPreview->setObjectName(QString::fromUtf8("webPreview"));
  loadDefaultPage();
  webPreview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  tabWEditor->addTab(webPreview, QString());
  
  connect(webPreview,SIGNAL(linkClicked(QUrl)), this, SLOT(defaultPageLinkClicked(QUrl)));
  connect(webPreview->page(),SIGNAL(loadProgress(int)), this, SLOT(loading(int)));
  connect(webPreview->page(),SIGNAL(linkHovered(QString,QString,QString)), this, SLOT(linkHovered(QString)));
  connect(webPreview->page(),SIGNAL(selectionChanged()), this, SLOT(webPageCursorMoved()));
  
  /***************************************************************
  
			HTML mode
  
  ***************************************************************/
  
  tabHTML = new QWidget();
  tabHTML->setObjectName(QString::fromUtf8("tabHTML"));
  verticalLayout_3 = new QVBoxLayout(tabHTML);
  verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
  verticalLayout_3->setContentsMargins(0,0,0,0);
  rtfHTMLEditor = new RtfHtmlEditor(tabHTML);
  rtfHTMLEditor->setObjectName(QString::fromUtf8("rtfHTMLEditor"));
  rtfHTMLEditor->setAcceptRichText(false);
  new HtmlSyntaxHighlighter(rtfHTMLEditor);
  rtfHTMLEditor->setLineWrapMode(QTextEdit::NoWrap);
  rtfHTMLEditor->setWordWrapMode(QTextOption::NoWrap);
  rtfHTMLEditor->setWordWrapMode(QTextOption::NoWrap);
  verticalLayout_3->addWidget(rtfHTMLEditor);
  tabWEditor->addTab(tabHTML, QString());
  
  connect(rtfHTMLEditor, SIGNAL(textChanged()), this, SLOT(setModified()));
  connect(rtfHTMLEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));

  /***************************************************************
  
			HTML Debugger
  
  ***************************************************************/
  
  dockDebug = new QDockWidget(tabHTML);
  dockDebug->setWindowTitle("Debugger");
  dockDebug->setObjectName(QString::fromUtf8("dockDebug"));
  dockDebug->setHidden(true);
  QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
  sizePolicy3.setHorizontalStretch(0);
  sizePolicy3.setVerticalStretch(0);
  sizePolicy3.setHeightForWidth(dockDebug->sizePolicy().hasHeightForWidth());
  dockDebug->setSizePolicy(sizePolicy3);
  dockDebug->setMinimumSize(QSize(0, 150));
  dockDebugContents = new QWidget();
  dockDebugContents->setObjectName(QString::fromUtf8("dockDebugContents"));
  dockDebug->setWidget(dockDebugContents);
  verticalLayout_99 = new QGridLayout(dockDebugContents);
  verticalLayout_99->setObjectName(QString::fromUtf8("verticalLayout_99"));
  verticalLayout_99->setContentsMargins(0,0,0,0);
  lstDebug = new QListWidget(dockDebugContents);
  verticalLayout_99->addWidget(lstDebug,0,0,1,19);
  verticalLayout_3->addWidget(dockDebug);
  
  KPushButton* btnDebugOptions = new KPushButton(this);
  btnDebugOptions->setText("Options");
  btnDebugOptions->setIcon(KIcon("configure"));
  verticalLayout_99->addWidget(btnDebugOptions,1,0);
  
  connect(ashActions["Debugger"], SIGNAL(triggered(bool)), dockDebug, SLOT(setVisible(bool)));
  connect(dockDebug, SIGNAL(visibilityChanged(bool)), ashActions["Debugger"] , SLOT(setChecked(bool)));
  
  /***************************************************************
  
			Script mode
  
  ***************************************************************/
  
  rtfScriptEditor = new KTextEdit(this);
  rtfScriptEditor->setObjectName(QString::fromUtf8("rtfScriptEditor"));
  tabWEditor->addTab(rtfScriptEditor, "Script");
  
  connect(rtfScriptEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
  
  /***************************************************************
  
			CSS MODE
  
  ***************************************************************/

  tabWCSSLevel = new QTabWidget(this);
  tabWCSSLevel->setObjectName(QString::fromUtf8("tabWCSSLevel"));
  tabWCSSLevel->setTabShape(QTabWidget::Triangular);
  
  connect(tabWCSSLevel, SIGNAL(currentChanged(int)), this, SLOT(changeCssMode(int)));
  
  /***************************************************************
  
			CSS begginermode
  
  ***************************************************************/
  
  tabBeginner = new QWidget();
  tabBeginner->setObjectName(QString::fromUtf8("tabBeginner"));
  verticalLayout_8 = new QVBoxLayout(tabBeginner);
  verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
  scrollArea = new QScrollArea(tabBeginner);
  scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
  scrollArea->setWidgetResizable(true);
  scrollAreaWidgetContents = new QWidget();
  scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
  verticalLayout_10 = new QVBoxLayout(scrollAreaWidgetContents);
  verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
  
  cssBegTagList << "height" << "width" << "text-align" << "text-transform" << "color" << "font-family" << "font-size" << "font-style" << "font-weight" << "background-image" << "background-color" << "background-repeat" << "border-width" << "border-color" << "border-style" << "float" << "position" << "z-index" << "margin-top" << "margin-bottom" << "margin-left" << "margin-right" << "padding-top" << "padding-bottom" << "padding-left" << "padding-right" << "list-style" << "cursor";
  
  grbSize = new QGroupBox(scrollAreaWidgetContents);
  grbSize->setObjectName(QString::fromUtf8("grbSize"));
  verticalLayout_9 = new QVBoxLayout(grbSize);
  verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
  QStringList tmpTagList;
  tmpTagList.clear();
  tmpTagList << "height" << "width";
  foreach (QString tag, tmpTagList) {
    ashCssBeg[tag] = new CSSBeginnerWidget(tabBeginner,tag);
    verticalLayout_9->addWidget(ashCssBeg[tag]);
  }
  verticalLayout_10->addWidget(grbSize);

  grbText = new QGroupBox(scrollAreaWidgetContents);
  grbText->setObjectName(QString::fromUtf8("grbText"));
  verticalLayout_11 = new QVBoxLayout(grbText);
  verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
  
  tmpTagList.clear();
  tmpTagList << "text-align" << "text-transform" << "color" << "font-family" << "font-size" << "font-style" << "font-weight";
  foreach (QString tag, tmpTagList) {
    ashCssBeg[tag] = new CSSBeginnerWidget(tabBeginner,tag);
    verticalLayout_11->addWidget(ashCssBeg[tag]);
  }
  verticalLayout_10->addWidget(grbText);

  grbBackground = new QGroupBox(scrollAreaWidgetContents);
  grbBackground->setObjectName(QString::fromUtf8("grbBackground"));
  verticalLayout_12 = new QVBoxLayout(grbBackground);
  verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));

  tmpTagList.clear();
  tmpTagList << "background-image"<< "background-color"<< "background-repeat";
  foreach (QString tag, tmpTagList) {
    ashCssBeg[tag] = new CSSBeginnerWidget(tabBeginner,tag);
    verticalLayout_12->addWidget(ashCssBeg[tag]);
  }
  verticalLayout_10->addWidget(grbBackground);

  grbBorder = new QGroupBox(scrollAreaWidgetContents);
  grbBorder->setObjectName(QString::fromUtf8("grbBorder"));
  verticalLayout_13 = new QVBoxLayout(grbBorder);
  verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));

  tmpTagList.clear();
  tmpTagList << "border-width"<< "border-color"<< "border-style";
  foreach (QString tag, tmpTagList) {
    ashCssBeg[tag] = new CSSBeginnerWidget(tabBeginner,tag);
    verticalLayout_13->addWidget(ashCssBeg[tag]);
  }
  verticalLayout_10->addWidget(grbBorder);

  grbLayout = new QGroupBox(scrollAreaWidgetContents);
  grbLayout->setObjectName(QString::fromUtf8("grbLayout"));
  grbLayout->setMinimumSize(QSize(0, 0));
  verticalLayout_14 = new QVBoxLayout(grbLayout);
  verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));

  tmpTagList.clear();
  tmpTagList << "float"<< "position"<< "z-index"<< "margin-top"<< "margin-bottom"<< "margin-left"<< "margin-right"<< "padding-top"<< "padding-bottom"<< "padding-left"<< "padding-right";
  foreach (QString tag, tmpTagList) {
    ashCssBeg[tag] = new CSSBeginnerWidget(tabBeginner,tag);
    verticalLayout_14->addWidget(ashCssBeg[tag]);
  }
  verticalLayout_10->addWidget(grbLayout);

  grbOther = new QGroupBox(scrollAreaWidgetContents);
  grbOther->setObjectName(QString::fromUtf8("grbOther"));
  QVBoxLayout* aQVBoxLayout = new QVBoxLayout(grbOther);
  
  verticalLayout_15 = new QVBoxLayout();
  verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
  aQVBoxLayout->addLayout(verticalLayout_15);
  
  tmpTagList.clear();
  tmpTagList << "list-style"<< "cursor";
  foreach (QString tag, tmpTagList) {
    ashCssBeg[tag] = new CSSBeginnerWidget(tabBeginner,tag);
    verticalLayout_15->addWidget(ashCssBeg[tag]);
  }

  txtOtherTags = new QTextEdit();
  txtOtherTags->setMaximumSize(9999,125);
  txtOtherTags->setMinimumSize(0,125);
  aQVBoxLayout->addWidget(txtOtherTags);
  verticalLayout_10->addWidget(grbOther);
  scrollArea->setWidget(scrollAreaWidgetContents);
  verticalLayout_8->addWidget(scrollArea);
  tabWCSSLevel->addTab(tabBeginner, "Begginer");
  
  /***************************************************************
  
			CSS EXPERT mode
  
  ***************************************************************/

  rtfCSSEditor = new RtfCssEditor(this);
  rtfCSSEditor->setObjectName(QString::fromUtf8("rtfCSSEditor"));
  new CssSyntaxHighlighter(rtfCSSEditor);

  tabWCSSLevel->addTab(rtfCSSEditor, "Expert");
  tabWEditor->addTab(tabWCSSLevel, QString());
    
  connect(rtfCSSEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
  
  /***************************************************************
  
			StatusBar
  
  ***************************************************************/

  statusbar = new KStatusBar(this);
  statusbar->setObjectName(QString::fromUtf8("statusbar"));
  this->setStatusBar(statusbar);
  
  lblStatusBar3 = new QLabel();
  lblStatusBar3->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
  statusBar()->addWidget(lblStatusBar3,100);
  
  lblStatusBar1 = new QLabel();
  lblStatusBar1->setText("");
  lblStatusBar1->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum);
  lblStatusBar1->setFrameShape(QFrame::StyledPanel);
  lblStatusBar1->setFrameShadow(QFrame::Sunken);
  statusBar()->addWidget(lblStatusBar1);
  
  lblStatusBar2 = new QLabel();
  lblStatusBar2->setText("HTMLv4");
  lblStatusBar2->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum);
  lblStatusBar2->setFrameShape(QFrame::StyledPanel);
  lblStatusBar2->setFrameShadow(QFrame::Sunken);
  statusBar()->addWidget(lblStatusBar2);
  
  pbStatusBar = new QProgressBar();
  pbStatusBar->setMaximumSize(100,999);
  statusBar()->addWidget(pbStatusBar);
  
  /***************************************************************
  
			Default disabled widget
  
  ***************************************************************/

  for (int i =1;i <= 3;i++)
    tabWEditor->setTabEnabled(i,false);
  for (int i =1;i <= 6;i++)
    tabWMenu->setTabEnabled(i,false);

  ashActions["Save"]->setDisabled(true);
  ashActions["Save As"]->setDisabled(true);
  ashActions["Print"]->setDisabled(true);
  ashActions["Print Preview"]->setDisabled(true);
  rtfHTMLEditor->clear();

  connect(tabWEditor, SIGNAL(currentChanged(int)), this, SLOT(modeChanged(int))); //DO NOT move
  retranslateUi();
}

void MainWindow::retranslateUi() {
  setWindowTitle(QApplication::translate("this", "Kimberlite", 0, QApplication::UnicodeUTF8));
  btnTreeAdd->setText(QApplication::translate("this", "Add", 0, QApplication::UnicodeUTF8));
  btnTreeRemove->setText(QApplication::translate("this", "Remove", 0, QApplication::UnicodeUTF8));
  tabWEditor->setTabText(tabWEditor->indexOf(webPreview), QApplication::translate("this", "WYSIWYG", 0, QApplication::UnicodeUTF8));
  tabWEditor->setTabText(tabWEditor->indexOf(tabHTML), QApplication::translate("this", "HTML", 0, QApplication::UnicodeUTF8));
  tabWEditor->setTabText(tabWEditor->indexOf(rtfScriptEditor), QApplication::translate("this", "Scripts", 0, QApplication::UnicodeUTF8));
  grbSize->setTitle(QApplication::translate("this", "Size", 0, QApplication::UnicodeUTF8));
  grbText->setTitle(QApplication::translate("this", "Text", 0, QApplication::UnicodeUTF8));
  grbBackground->setTitle(QApplication::translate("this", "Background", 0, QApplication::UnicodeUTF8));
  grbBorder->setTitle(QApplication::translate("this", "Border", 0, QApplication::UnicodeUTF8));
  grbLayout->setTitle(QApplication::translate("this", "Layout", 0, QApplication::UnicodeUTF8));
  grbOther->setTitle(QApplication::translate("this", "Other", 0, QApplication::UnicodeUTF8));
  //tabWCSSLevel->setTabEnabled(1,false);
  tabWEditor->setTabText(tabWEditor->indexOf(tabWCSSLevel), QApplication::translate("this", "CSS", 0, QApplication::UnicodeUTF8));
  Q_UNUSED(this);
} // retranslateUi

void MainWindow::fillCSSBegMode(QString className) {
  QStringList aClass = CssParser::getClass(className);
  foreach (QString propriety, aClass) {
    if (cssBegTagList.indexOf(CssParser::getPropriety(propriety.toLower())) != -1)
      ashCssBeg[CssParser::getPropriety(propriety.toLower())]->fillMe(propriety);
    else {
      cssBegTagList << CssParser::getPropriety(propriety.toLower());
      ashCssBeg[CssParser::getPropriety(propriety.toLower())] = new CSSBeginnerWidget(tabBeginner,CssParser::getPropriety(propriety.toLower()));
      verticalLayout_15->addWidget(ashCssBeg[CssParser::getPropriety(propriety.toLower())]);
      ashCssBeg[CssParser::getPropriety(propriety.toLower())]->fillMe(propriety);
    }
  }
} //fillCSSBegMode

MainWindow::~MainWindow() {
  QDir aDir;
  aDir.rmdir(QDir::tempPath()+"/kimberlite");
} //~MainWindow

void MainWindow::reParse() {
  HtmlData data = HtmlParser::getHtmlData(rtfHTMLEditor->toPlainText());
  updateHtmlTree(data);
  rtfHTMLEditor->setPlainText(aParser->getParsedHtml(data));
} //reParse

void MainWindow::templaterize() {
  StringConverter* aStringConverter = new StringConverter(this);
  aStringConverter->toTemplate(rtfHTMLEditor->toPlainText());
  connect(aStringConverter, SIGNAL(over(QString)),this, SLOT(replaceHtml(QString)));
} //templaterize

void MainWindow::translate() {
  StringConverter* aStringConverter = new StringConverter(this);
  aStringConverter->translate(rtfHTMLEditor->toPlainText());
  connect(aStringConverter, SIGNAL(over(QString)),this, SLOT(replaceHtml(QString)));
} //translate

void MainWindow::replaceHtml(QString newHtml) {
  rtfHTMLEditor->setPlainText(newHtml);
} //replaceHtml

void MainWindow::newProject(){
  NewProject* aNewProject = new NewProject(this);
  aNewProject->show();
  connect(aNewProject,SIGNAL(createProject(QString, QString)),this,SLOT(newProject(QString,QString)));
} //newProject

void MainWindow::newProject(QString name, QString filePath) {
  openProject(filePath);
  fileName.clear();
  aProjectManager->setProjectName(name);
} //newProject
 
void MainWindow::saveProjectAs(const QString &outputFileName){
  if (CSS_MODE == CSS_MODE_BEG)
    changeCssMode(CSS_MODE_BEG);
  else
    CssParser::cssFile = rtfCSSEditor->toPlainText();
  aProjectManager->saveCss();
  loadPage(currentHTMLPage,rtfHTMLEditor->toPlainText(),true);
  KSaveFile file(outputFileName);
  file.open();
  QByteArray outputByteArray;
  outputByteArray.append(aProjectManager->createFile());
  file.write(outputByteArray);
  file.finalize();
  file.close();

  fileName = outputFileName;
  saveRecentProject(fileName);
} //saveProjectAs
 
void MainWindow::saveProjectAs(){
  saveProjectAs(KFileDialog::getSaveFileName());
} //saveProjectAs
 
void MainWindow::saveProject(){
  if(!fileName.isEmpty()) 
    saveProjectAs(fileName);
  else 
    saveProjectAs();
} //saveProject

void MainWindow::saveFile(){ 
  if(!pageName.isEmpty()) 
    saveProjectAs(pageName);
  else
    saveProjectAs();
} //saveFile


void MainWindow::openProject() {
  QString fileName = KFileDialog::getOpenFileName(KUrl("$HOME"),"*.wkp|Kimberlite projects (*.wkp)",this,"Open Project");
  openProject(fileName);
} //openProject
 
void MainWindow::openProject(QString fileName) {
  qDebug() << "Loading project" << fileName;
  if (!fileName.isEmpty()) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) 
      QMessageBox::warning(this, tr("SAX Bookmarks"),tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
    else 
      aProjectManager->read(&file);
    aProjectManager->expandAll();
    setWindowTitle("Kimberlite - "+aProjectManager->projectTitle);
    webPreview->page()->setContentEditable(true);
    tabWEditor->setTabEnabled(1,true);
    tabWEditor->setTabEnabled(2,true);
    tabWEditor->setTabEnabled(3,true);
    tabWMenu->setTabEnabled(1,true);
    tabWMenu->setTabEnabled(3,true);
    tabWMenu->setTabEnabled(2,true);
    tabWMenu->setTabEnabled(4,true);
    tabWMenu->setTabEnabled(5,true);
    tabWMenu->setTabEnabled(6,true);
    ashActions["Save"]->setDisabled(false);
    ashActions["Save As"]->setDisabled(false);
    ashActions["Print"]->setDisabled(false);
    ashActions["Print Preview"]->setDisabled(false);
    tableDock->setVisible(true);
    saveRecentProject(fileName);
    this->fileName = fileName;
  }
} //openProject

void MainWindow::showInspector(bool state) {
  QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
  webPreview->triggerPageAction(QWebPage::InspectElement);
} //showInspector

void MainWindow::zoomIn() {
  if (KIMBERLITE_MODE == MODE_WYSIWYG)
    webPreview->setZoomFactor(webPreview->zoomFactor()+1);
  else
    getCurrentEditor()->zoomIn(1);
} //zoomIn

void MainWindow::zoomOut() {
  if (KIMBERLITE_MODE == MODE_WYSIWYG) 
    webPreview->setZoomFactor(webPreview->zoomFactor()-1);
  else
    getCurrentEditor()->zoomOut(1);
} //zoomOut

void MainWindow::zoomDefault() {
  webPreview->setZoomFactor(1);
} //zoomDefault

QString MainWindow::getClassName(QTreeWidgetItem* anItem) {
  QString className;
  QTreeWidgetItem* currentLevel = anItem;
  while (currentLevel != styleSheetName) {
    if (className[0] != ':') 
      className.insert(0, " ");
    className.insert(0, currentLevel->text(0)); //BUG the : dont need space
    currentLevel = currentLevel->parent();
  }
  className = className.trimmed();
  return className;
} //getClassName

QTreeWidgetItem* MainWindow::getClassWidget(QString className) {
  QTreeWidgetItem* currentTop(styleSheetName);
  QString partialName;
  while (currentTop->childCount() > 0) {
    for (int i =0; i < currentTop->childCount();i++) {
      QString toAdd;
      if ((currentTop->child(i)->text(0)[0] != ':') && (partialName.count()))
	    toAdd += " ";
      QString tmpStr = partialName + toAdd + (currentTop->child(i)->text(0));
      if (tmpStr == className.left(tmpStr.count())) {
	if (getClassName(currentTop->child(i)) == className)
	  return currentTop->child(i);
	else {
	  if ((currentTop->child(i)->text(0)[0] != ':') && (partialName.count()))
	    partialName += " ";
	  partialName += currentTop->child(i)->text(0);
	  currentTop = currentTop->child(i);
	  i = currentTop->childCount();
	}
      }
      if (i == currentTop->childCount() -1)
	return NULL;
    }
  }
  return NULL;
} //getClassWidget

void MainWindow::cssClassClicked(QTreeWidgetItem* anItem) {
  if (CSS_MODE == CSS_MODE_BEG)
    loadCSSClass(anItem);
  else if (CSS_MODE == CSS_MODE_EXPERT) {
    QString className = getClassName(anItem);
    setCssCursor(className);
  }
} //cssClassClicked

void MainWindow::loadCSSClass(QTreeWidgetItem* anItem) {
  QString newStyle = CssParser::setClass(currentClassName, clearCssBeg());
  QString className = getClassName(anItem);
  currentClassName = className;
  fillCSSBegMode(currentClassName);
  CssParser::cssFile = newStyle;
  rtfCSSEditor->setText(CssParser::parseCSS());
} //loadCSSClass

void MainWindow::setCssCursor(QString className) {
  uint counter =0;
  QStringList lineList = rtfCSSEditor->toPlainText().split('\n');
  for (int i=0;i < lineList.count();i++) {
    if (lineList[i].indexOf('{') != -1) {
      if (lineList[i]== className + " {") {
	QTextCursor tc = rtfCSSEditor->textCursor();
	tc.setPosition(counter);
	rtfCSSEditor->setTextCursor(tc);
	rtfCSSEditor->setFocus();
	return;
      }
    }
    counter += lineList[i].count() +1;
  }
} //setCssCursor

void MainWindow::loadPage(QTreeWidgetItem* item, QString text, bool force) {
  if ((item != currentHTMLPage) || (force)) {
    isModified = false;
    if (currentHTMLPage != NULL) {
      switch (KIMBERLITE_MODE) {
	case MODE_WYSIWYG:
	  aProjectManager->updateDomElement(currentHTMLPage,webPreview->page()->mainFrame()->toHtml());
	  break;
	case MODE_HTML:
	  aProjectManager->updateDomElement(currentHTMLPage,rtfHTMLEditor->toPlainText());
	  break;
      }
    }
    currentHTMLPage = item;
    HtmlData data = HtmlParser::getHtmlData(text);
    switch (KIMBERLITE_MODE) {
      case MODE_WYSIWYG:
	webPreview->setHtml(HtmlParser::getParsedHtml(data));
	break;
      case MODE_HTML:
	rtfHTMLEditor->setPlainText(HtmlParser::getParsedHtml(data));
	break;
      default:
	if (!force)
	  tabWEditor->setCurrentIndex(1);
	rtfHTMLEditor->setPlainText(HtmlParser::getParsedHtml(data));
    }
    updateHtmlTree(data);
    
    QString completeName;
    QTreeWidgetItem* parent = item->parent();
    completeName += parent->text(0) + "/";
    while (parent->parent() != NULL) {
      parent = parent->parent();
      completeName.insert(0,parent->text(0) + "/");
    }
    setWindowTitle("Kimberlite - "+aProjectManager->projectTitle + "  (" + completeName + item->text(0) + ")");
  }
} //loadPage

void MainWindow::setModified() {
  isModified = true;
} //setModified

void MainWindow::addHtmlPage() {
  NewWebPage* aDialog = new NewWebPage(this,aProjectManager->htmlPage);
  aDialog->show();
  connect(aDialog, SIGNAL(addFolder(QString,QTreeWidgetItem*)), aProjectManager, SLOT(addFolder(QString,QTreeWidgetItem*)));
  connect(aDialog, SIGNAL(addHtmlPage(QString,QString,QString,QString)), aProjectManager, SLOT(addHtmlPage(QString,QString,QString,QString)));
} //addHtmlPage

void MainWindow::addScript() {
  NewScript* aScript = new NewScript(this,aProjectManager->script);
  aScript->show();
} //addScript

QString MainWindow::clearCssBeg() {
  QString currentClass;
  foreach (QString tag, cssBegTagList) {
    if (ashCssBeg[tag]->cbbValue->isEnabled()) {
      currentClass += "  " + tag +": "+ ashCssBeg[tag]->cbbValue->currentText();
      if (ashCssBeg[tag]->isEnabled())
        currentClass += ashCssBeg[tag]->cbbUnit->currentText();
      currentClass += ";\n";
    }
    ashCssBeg[tag]->clear();
  }
  currentClass += txtOtherTags->toPlainText();
  txtOtherTags->clear();
  return currentClass;
} //clearCssBeg

void MainWindow::disableWidget(bool value) {
  tabWEditor->setEnabled(value);
  editTB->setEnabled(value);
  viewTB->setEnabled(value);
  insertTB->setEnabled(value);
  toolsTB->setEnabled(value);
  optionsTB->setEnabled(value);
  helpTB->setEnabled(value);
} //disableWidget

void MainWindow::changeCssMode(int mode) {
  if (previousCssMode == 2) {
    CssParser::cssFile = rtfCSSEditor->toPlainText();
    updateClassTree();
  }
  else if (previousCssMode == 0)
    CssParser::cssFile = CssParser::setClass(currentClassName, clearCssBeg());
  
  if ((previousCssMode == 0) && (mode ==2) && (treeWidget->currentItem() != NULL)) {
    rtfCSSEditor->setPlainText(CssParser::parseCSS());
    setCssCursor(currentClassName);
  }
  else if (mode == 1) {}
  else if (mode == 2) {
    //setCssCursor(currentClassName);
  }
  previousCssMode = mode;
} //changeCssMode

void MainWindow::updateClassTree() {
  QTreeWidgetItem* anItem =  new QTreeWidgetItem();
  (*anItem) = (*styleSheetName);
  treeWidget->clear();
  treeWidget->addTopLevelItem(anItem);
  styleSheetName = anItem;
  
  QStringList classList = CssParser::getClassList();

  for (int j = 0; j < classList.count(); j++) {
    splitSubClass(classList[j], styleSheetName);
  }
  treeWidget->expandAll();
  
  if (classList.count() > 0) {
    if (currentClassName.isEmpty())
      currentClassName = classList[0];
    fillCSSBegMode(currentClassName);
    QTreeWidgetItem* toSelect = getClassWidget(currentClassName);
    if (toSelect)
      treeWidget->setCurrentItem(toSelect);
  }
} //updateClassTree

void MainWindow::splitSubClass(QString name, QTreeWidgetItem* parent) {
  if (((name.indexOf(":") != -1) && (name.indexOf(" ") == -1)) /*|| (((name.indexOf(":") < (name.indexOf(" ")) && ((name.indexOf(" ") != -1)))))*/ && ((name.indexOf(":") != 0))) {
    bool found = false;
    for (int i =0; i < parent->childCount(); i++) {
      if (parent->child(i)->text(0) ==  name.left(name.indexOf(":"))) {
        splitSubClass(":" + name.right(name.count() - name.indexOf(":") -1), parent->child(i));
        found = true;
      }
    }
    
    if (found == false) {
      QTreeWidgetItem* aTreeViewWidgetItem = new  QTreeWidgetItem(parent);
      aTreeViewWidgetItem->setText(0,name.left(name.indexOf(":")));
      aTreeViewWidgetItem->setIcon(0,getRightIcon(name.left(name.indexOf(":"))));
      //aTreeViewWidgetItem->setIconSize(0,QSize(22,22));
      splitSubClass( ":" + name.right(name.count() - name.indexOf(":") -1), aTreeViewWidgetItem);
    }
  }
  else if (name.indexOf(":") == 0) { //BUG
    QTreeWidgetItem* aTreeViewWidgetItem = new  QTreeWidgetItem(parent);
    aTreeViewWidgetItem->setText(0,name);
    aTreeViewWidgetItem->setIcon(0,getRightIcon(name));
  }
  else if ((name.indexOf(" ") != -1) && (name[name.indexOf(" ")-1] != ',')) {
    bool found = false;
    for (int i =0; i < parent->childCount(); i++) {
      if (parent->child(i)->text(0) ==  name.left(name.indexOf(" "))) {
        splitSubClass(name.right(name.count() - name.indexOf(" ") -1), parent->child(i));
        found = true;
      }
    }
    
    if (found == false) {
      QTreeWidgetItem* aTreeViewWidgetItem = new  QTreeWidgetItem(parent);
      aTreeViewWidgetItem->setText(0,name.left(name.indexOf(" ")));
      aTreeViewWidgetItem->setIcon(0,getRightIcon(name.left(name.indexOf(" "))));
      splitSubClass(name.right(name.count() - name.indexOf(" ") -1), aTreeViewWidgetItem);
    }
    
  }
  else {
    QTreeWidgetItem* aTreeViewWidgetItem = new  QTreeWidgetItem(parent);
    aTreeViewWidgetItem->setIcon(0,getRightIcon(name));
    aTreeViewWidgetItem->setText(0,name);
  }
} //splitSubClass

KIcon MainWindow::getRightIcon(QString text) {
  KIcon* anIcon = 0;
  if (text[0] == ' ') 
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/tag.png"));
  else if (text[0] == ':') 
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/state.png"));
  else if (text[0] == '#') 
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/id.png"));
  else if (text[0] == '.') 
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/class.png"));
  else 
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/tag.png"));
  return *anIcon;
} //getRightIcon

void MainWindow::loadScript(QTreeWidgetItem* anItem, QString text) {
  if (anItem != currentScript) {
    if (currentScript != NULL) {
      aProjectManager->updateDomElement(currentHTMLPage,rtfHTMLEditor->toPlainText());
      isModified = false;
    }
    rtfScriptEditor->setPlainText(text.trimmed());
    currentScript = anItem;
  }
} //loadScript

void MainWindow::loadCss(QString text) {
  CssParser::cssFile = text;
  rtfCSSEditor->setPlainText(CssParser::parseCSS());
  styleSheetName = new  QTreeWidgetItem(treeWidget);
  styleSheetName->setText(0,"Style");
  updateClassTree();
} //loadCss

void MainWindow::modeChanged(int index) {
  tableDock->setVisible(true);
  switch (index) {
    case 0: { //Don't ask me about this {, I don't even understant it myself
      QString path2;
      QDir aDir;
      path2 = QDir::tempPath() + "/kimberlite/project/";
      aDir.mkpath(path2);
      QFile aFile(path2+"StyleSheet.css");
      aFile.open(QIODevice::WriteOnly);
      aFile.write(CssParser::parseCSS().toStdString().c_str());
      aFile.close();
      webPreview->setHtml(rtfHTMLEditor->toPlainText(),path2);
      disableWysiwyg(false);
      ashActions["Zoom 1:1"]->setEnabled(true);
      treeDock->setVisible(false);
      dockHtmlTree->setVisible(false);
      pbStatusBar->setEnabled(true);
      break;
    }
    case 1:
      //if (webPreview->isModified()) //Too dangerous
      rtfHTMLEditor->setPlainText(aParser->getParsedHtml(webPreview->page()->mainFrame()->toHtml()));
      disableWysiwyg(false);
      ashActions["Zoom 1:1"]->setEnabled(true);
      dockHtmlTree->setVisible(true);
      treeDock->setVisible(false);
      pbStatusBar->setEnabled(false);
      break;
    case 2:
      ashActions["Zoom 1:1"]->setEnabled(true);
      disableWysiwyg(true);
      dockHtmlTree->setVisible(false);
      treeDock->setVisible(false);
      pbStatusBar->setEnabled(false);
      break;
    case 3:
      disableWysiwyg(true);
      ashActions["Zoom 1:1"]->setEnabled(false);
      dockHtmlTree->setVisible(false);
      tableDock->setVisible(false);
      treeDock->setVisible(true);
      pbStatusBar->setEnabled(false);
  }
} //modeChanged

void MainWindow::updateHtmlTree(QString &file) {
  HtmlData pageData = aParser->getHtmlData(file);
  updateHtmlTree(pageData);
} //updateHtmlTree

void MainWindow::updateHtmlTree(HtmlData &pageData) {
  IndexedTreeWidgetItem* previousNode(NULL);
  IndexedTreeWidgetItem* aNode(NULL);
  int index(0), size(pageData.tagList.size());
  treeHtml->clear();
  for (int j=0; j < size;j++) {
    if (pageData.levelList[j] == 0)
      aNode = new IndexedTreeWidgetItem(treeHtml,QStringList(pageData.tagList[j]),index);
    else if (pageData.levelList[j] > pageData.levelList[(j > 0)?j-1:0]) 
      aNode = new IndexedTreeWidgetItem(previousNode,QStringList(pageData.tagList[j]),index);
    else if (pageData.levelList[j] == pageData.levelList[(j > 0)?j-1:0]) 
      aNode = new IndexedTreeWidgetItem(previousNode->parent(),QStringList(pageData.tagList[j]),index);
    else 
      aNode = new IndexedTreeWidgetItem(previousNode->parent()->parent(),QStringList(pageData.tagList[j]),index);
    previousNode = aNode;
    index += pageData.tagList[j].count()+1 + (3*(pageData.levelList[j+((size-1!=j)?1:0)]));
  }
  treeHtml->expandAll();
} //updateHtmlTree

void MainWindow::setHtmlCursor(QTreeWidgetItem* item) {
  QTextCursor tc = rtfHTMLEditor->textCursor();
  tc.setPosition(((IndexedTreeWidgetItem*) item)->index);
  rtfHTMLEditor->setTextCursor(tc);
  rtfHTMLEditor->setFocus();
} //setHtmlCursor

void MainWindow::debugHtml() {
  dockDebug->setVisible(true);
  lstDebug->clear();
  HtmlData pageData = HtmlParser::getHtmlData(rtfHTMLEditor->toPlainText());
  updateHtmlTree(pageData);
  QVector<debugItem> debugVector = HtmlDebugger::debug(pageData);
  for (int i =0; i < debugVector.size(); i++) {
    QListWidgetItem* anItem = new QListWidgetItem("["+QString::number(debugVector[i].line)+"] "+debugVector[i].message);
    if (debugVector[i].icon == 0)
      anItem->setIcon(KIcon("dialog-warning"));
    else if (debugVector[i].icon == 1)
      anItem->setIcon(KIcon("dialog-error"));
    else
      anItem->setIcon(KIcon("dialog-information"));
    lstDebug->addItem(anItem);
  }
} //debugHtml

void MainWindow::setBold() {
  switch (KIMBERLITE_MODE) {
    case MODE_WYSIWYG:
      webPreview->page()->triggerAction(QWebPage::ToggleBold,true);
      break;
    case MODE_HTML:
      addTag("<b>","</b>");
      break;
  }
} //setBold

void MainWindow::setItalic() {
  switch (KIMBERLITE_MODE) {
    case MODE_WYSIWYG:
      webPreview->page()->triggerAction(QWebPage::ToggleItalic,true);
      break;
    case MODE_HTML:
      addTag("<i>","</i>");
      break;
  }
} //setItalic

void MainWindow::setUnderline() {
  switch (KIMBERLITE_MODE) {
    case MODE_WYSIWYG:
      webPreview->page()->triggerAction(QWebPage::ToggleUnderline,true);
      break;
    case MODE_HTML:
      addTag("<u>","</u>");
      break;
  }
} //setUnderline

void MainWindow::execCommand(const QString &cmd, const QString &arg = "") {
  QWebFrame *frame = webPreview->page()->mainFrame();
  QString js;
  if (arg != "")
    js = QString("document.execCommand(\"%1\", false, \"%2\")").arg(cmd).arg(arg);
  else
    js = QString("document.execCommand(\"%1\", false, null)").arg(cmd);
  frame->evaluateJavaScript(js);
} //execCommand

void MainWindow::addTag(QString prefix, QString suffix) {
  QTextCursor tc = rtfHTMLEditor->textCursor();
  QString text = tc.selectedText();
  tc.removeSelectedText();
  tc.insertText(prefix + text + suffix);
} //addTag

void MainWindow::addTag(QString prefix, QString suffix, QString cmd, QString arg = "") {
  switch (KIMBERLITE_MODE) {
    case MODE_WYSIWYG:
      execCommand(cmd,arg);
      break;
    case MODE_HTML:
      addTag(prefix,suffix);
      break;
  }
} //addTag

void MainWindow::setAlignCenter() {
  addTag("<center>","</center>","justifyCenter");
} //setAlignCenter

void MainWindow::setAlignLeft() {
  addTag("<font style=\"text-align:left\">","</font>","justifyLeft");
} //setAlignLeft

void MainWindow::setAlignRight() {
  addTag("<font style=\"text-align:right\">","</font>","justifyRight");
} //setAlignRight

void MainWindow::setJustify() {
  addTag("<font style=\"text-align:justify\">","</font>","justifyFull");
} //setJustify

void MainWindow::setHeader(QString text) {
  addTag("<h"+QString::number(cbbHeader->currentIndex()+1)+">","</h"+QString::number(cbbHeader->currentIndex()+1)+">","formatBlock","h"+QString::number(cbbHeader->currentIndex()+1));
} //setHeader

void MainWindow::setFont(QString text) {
  addTag("<font face=\""+text+"\">","</font>","fontName", text.toLower());
} //setFont

void MainWindow::setFontSize(int size) {
  QString number = QString::number(size);
  addTag("<font size=\""+number+"\">","</font>","fontSize",number);
} //setFontSize

void MainWindow::setTextColor() {
  QString color = kcbbTextColor->color().name();
  addTag("<font color=\""+color+"\">","</font>","foreColor", color);
} //setTextColor

void MainWindow::setHighlightColor() {
  QString color = cbbHighlightColor->color().name();
  addTag("<font style=\"background-color:"+color+"\">","</font>","hiliteColor", color);
} //setHighlightColor

void MainWindow::setUList() {
  addTag("<ul>","</ul>","insertUnorderedList");
} //setUList

void MainWindow::setBackgroundColor() {
  if (KIMBERLITE_MODE == MODE_WYSIWYG)
    rtfHTMLEditor->setPlainText(webPreview->page()->mainFrame()->toHtml());
  QString color = cbbBackgroundColor->color().name();
  HtmlData pageData = aParser->getHtmlData(rtfHTMLEditor->toPlainText());
  HtmlParser::setAttribute(pageData, "body", 0, "bgcolor", color);
  rtfHTMLEditor->setPlainText(aParser->getParsedHtml(pageData));
  if (KIMBERLITE_MODE == MODE_WYSIWYG)
    webPreview->setHtml(rtfHTMLEditor->toPlainText());
} //setBackgroundColor

void MainWindow::insertImage(QString path) {
  if (path.isEmpty()) {
    RessrourceManager* aRessounrceManager = new RessrourceManager(this);
    connect(aRessounrceManager, SIGNAL(finish(QString)), this, SLOT(insertImage(QString)));
    aRessounrceManager->show();
  }
  else
    addTag("<img src=\""+path+"\" \\>","","insertImage", path);
} //insertImage

void MainWindow::insertTable() {
  NewTable* aNewTable = new NewTable(this);
  aNewTable->show();
  addTag("<a href=\"\">","</a>","inserthtml","<b><table><tr><td>test</td><td>test2</td></tr><tr><td>test3</td><td>test4</td></tr></table></b>");
} //insertTable

void MainWindow::insertLink(QString path) {
  if (path.isEmpty()) {
    RessrourceManager* aRessounrceManager = new RessrourceManager(this);
    connect(aRessounrceManager, SIGNAL(finish(QString)), this, SLOT(insertLink(QString)));
    aRessounrceManager->show();
  }
  else {
    addTag("<a href=\""+path+"\">","</a>","CreateLink",path);
  }
} //insertLink

void MainWindow::insertChar() {
  KDialog* aDialog = new KDialog(this);
  aDialog->setMainWidget(new KCharSelect(aDialog));
  aDialog->show();
} //insertChar

void MainWindow::editShortcut() {
  KDialog* aDialog = new KDialog(this);
  aDialog->setMainWidget(new KShortcutsEditor(actionCollection,aDialog));
  aDialog->show();
} //editShortcut

void MainWindow::editToolbar() {
  KEditToolBar* aToolbarEditor = new KEditToolBar(actionCollection);
  aToolbarEditor->show();
} //editToolbar

void MainWindow::aboutKimberlite() {
  KAboutData* aboutData = new KAboutData( "kimberlite", "kimberlite",
  ki18n("Kimberlite"), "0.0.9",
  ki18n("A complete environemnt to develop, manage and execute Unix scripts"),
  KAboutData::License_GPL,
  ki18n("Copyright (c) 2008 Emmanuel Lepage Vallee") );
  KAboutApplicationDialog* aDialog = new KAboutApplicationDialog(aboutData,this);
  aDialog->show();
} //aboutKimberlite

void MainWindow::reportBug() {
  KBugReport* aDialog = new KBugReport();
  aDialog->show();
} //reportBug

void MainWindow::disableWysiwyg(bool value) {
  QList<QWidget*> aList;
  aList << cbbFont << cbbFontSize << btnBold << btnUnderline << btnItalic << btnAlignLeft << btnAlignCenter << btnAlignRight  << btnJustify << cbbHeader /* <<btnLink*/ << btnChar /*<< btnTable*/ << btnList << lblTextColor << kcbbTextColor << lblHighlightColor << cbbHighlightColor << lblBackgroundColor << cbbBackgroundColor << btnNewLine  << btnNewTab << btnNewSpace;
  for (int i =0; i< aList.size();i++)
    aList[i]->setDisabled(value);
  ashActions["Add Image"]->setDisabled(value);
  ashActions["Add Table"]->setDisabled(value);
  ashActions["Add Link"]->setDisabled(value);
  ashActions["Character"]->setDisabled(value);
} //disableWysiwyg

void MainWindow::quit() {
  exit(33);
} //quit

QTextEdit* MainWindow::getCurrentEditor() {
  if (KIMBERLITE_MODE == MODE_HTML)
    return rtfHTMLEditor;
  else if (KIMBERLITE_MODE == MODE_SCRIPT)
    return rtfScriptEditor;
  else if(KIMBERLITE_MODE == MODE_CSS)
    return rtfCSSEditor;
}

void MainWindow::print() {
  QPrintDialog* aDialog = new QPrintDialog(this);
  if (aDialog->exec() == QDialog::Accepted) {
    if (KIMBERLITE_MODE == MODE_WYSIWYG)
      webPreview->print(aDialog->printer());
    else
      getCurrentEditor()->print(aDialog->printer());
  }
} //print

void MainWindow::printPreview() { //BUG Linking error
  /*QPrinter* aPrinter = new QPrinter();
  KPrintPreview var(aPrinter,this);
  KPrintPreview* aDialog = new KPrintPreview(aPrinter,this);
  aDialog->show();*/
} //printPreview

void MainWindow::undo() {
  if (KIMBERLITE_MODE == MODE_WYSIWYG) 
    webPreview->page()->triggerAction(QWebPage::Undo,true);
  else
    getCurrentEditor()->undo();
} //undo

void MainWindow::redo() {
  if (KIMBERLITE_MODE == MODE_WYSIWYG)
    webPreview->page()->triggerAction(QWebPage::Redo,true);
  else
    getCurrentEditor()->redo();
} //redo

void MainWindow::copy() {
  if (KIMBERLITE_MODE == MODE_WYSIWYG)
    webPreview->page()->triggerAction(QWebPage::Copy,true);
  else
    getCurrentEditor()->copy();
} //copy

void MainWindow::cut() {
  if (KIMBERLITE_MODE == MODE_WYSIWYG) 
    webPreview->page()->triggerAction(QWebPage::Cut,true);
  else
    getCurrentEditor()->cut();
} //cut

void MainWindow::paste() {
  if (KIMBERLITE_MODE == MODE_WYSIWYG) 
      webPreview->page()->triggerAction(QWebPage::Paste,true);
  else
    getCurrentEditor()->paste();
} //paste

void MainWindow::find() {
  switch (KIMBERLITE_MODE) {
    case MODE_WYSIWYG:
      //webPreview->page()->triggerAction(QWebPage::Paste,true);
      break;
    case MODE_HTML:
      rtfHTMLEditor->findText();
      break;
    case MODE_SCRIPT:
      //rtfScriptEditor->findText();
      break;
    case MODE_CSS:
      //rtfCSSEditor->findText();
      break;
  }
} //find

QStringList MainWindow::loadRecentProjectList() {
  QStringList toReturn;
  QString path = KStandardDirs::locateLocal( "appdata", "recent.txt" );
  QFile file(path);
  file.open(QIODevice::ReadOnly);
  while (!file.atEnd()) {
    QString line = file.readLine();
    toReturn << "<a href=\"load:" + line + "\">" + line + "</a>";
  }
  while (toReturn.count() < 5)
    toReturn << "";
  file.close();
  return toReturn;
} //loadRecentProjectList

void MainWindow::saveRecentProject(QString filePath) {
  QStringList recentProjectList;
  QString path2 = KStandardDirs::locateLocal( "appdata", "recent.txt" );
  QFile file2(path2);
  file2.open(QIODevice::ReadOnly);
  while (!file2.atEnd())
    recentProjectList << QString(file2.readLine()).toAscii();
  file2.close();
  if (recentProjectList.indexOf(filePath+"\n") != -1)
    return;
  if (recentProjectList.count() >= 5)
    recentProjectList.removeAt(0);
  recentProjectList << filePath;
  
  QString toSave;
  for (int i =0; i < recentProjectList.count();i++) 
    toSave += recentProjectList[i].trimmed() + "\n";
  
  QString path = KStandardDirs::locateLocal( "appdata", "recent.txt" );
  KSaveFile file(path);
  file.open();
  QByteArray outputByteArray;
  outputByteArray.append(toSave);
  file.write(outputByteArray);
  file.finalize();
  file.close();
} //saveRecentProject

void MainWindow::loadDefaultPage() {
  KIconLoader *iconloader = KIconLoader::global();
  QString new_path = iconloader->iconPath("document-new", KIconLoader::Desktop );
  QString open_path = iconloader->iconPath("document-open", KIconLoader::Desktop );
  QStringList recentProject = loadRecentProjectList();
  QString path = KStandardDirs::locate("appdata", "data/home.htm");//KStandardDirs::locate("data", "konqueror/about/launch.html");
  QFile file2(path);
  file2.open(QIODevice::ReadOnly);
  QString page;
  page = file2.readAll();
  page = page.arg( KStandardDirs::locate( "data", "kdeui/about/" ) );
  page = page.arg(new_path);
  page = page.arg(open_path);
  page = page.arg("Kimberlite");
  page = page.arg("Master the web");
  page = page.arg("Kimberlite is a WYSIWYG HTML editor for KDE based on WebKit");
  page = page.arg("To use kimberlite, you have to load a project. You can either open an existing one or create a new one from a template or from scratch:");
  page = page.arg("Create a new project");
  page = page.arg("Open an existing project");
  page = page.arg("News:");
  page = page.arg("Recent project:");
  page = page.arg(recentProject[0]);
  page = page.arg(recentProject[1]);
  page = page.arg(recentProject[2]);
  page = page.arg(recentProject[3]);
  page = page.arg(recentProject[4]);
  page = page.arg("Kimberlite 0.1 realised (2009/06/12)<br>The first official version of Kimberlite is now availible. Test, use it, crash it and report bugs!<br>Good Luck ;)");
  page = page.arg(KStandardDirs::locate("appdata", "pixmap/logo.png"));

  file2.close();
  webPreview->setHtml(page);
} //loadDefaultPage

void MainWindow::defaultPageLinkClicked(const QUrl & url) {
  if (url.toString() == "new")
    newProject();
  else if (url.toString() == "open")
    openProject();
  else if (url.toString().left(5) == "load:")
    openProject(url.toString().remove(0,5));
} //defaultPageLinkClicked

void MainWindow::cursorChanged() {
  QTextCursor tc;
  if ((KIMBERLITE_MODE != MODE_WYSIWYG) && (getCurrentEditor() != rtfCSSEditor))
    tc = getCurrentEditor()->textCursor();
  else if (rtfScriptEditor == getCurrentEditor()) {
    tc = rtfCSSEditor->textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    QString currentText = tc.selectedText();
    while ((currentText.indexOf('{') == -1) && (tc.blockNumber() > 0)) {
      tc.movePosition(QTextCursor::Up);
      tc.select(QTextCursor::LineUnderCursor);
      currentText = tc.selectedText();
    }
    if (currentText.indexOf('{') != -1) {
      QTreeWidgetItem* anItem = getClassWidget(currentText.left(currentText.indexOf('{')).trimmed());
      if ((anItem) && (CSS_MODE != CSS_MODE_BEG)) {
	currentClassName = currentText.left(currentText.indexOf('{')).trimmed();
	treeWidget->setCurrentItem(anItem);
      }
    }
  }
  lblStatusBar1->setText("Line: " + QString::number(tc.blockNumber()));
} //cursorChanged

void MainWindow::addClasses() {
  bool ok;
  QString text = QInputDialog::getText(this, "Add class", "Insert the name of the new class", QLineEdit::Normal, "", &ok);
  if (ok && !text.isEmpty()) {
    if (CSS_MODE == CSS_MODE_BEG) 
      CssParser::cssFile = CssParser::setClass(currentClassName, clearCssBeg());
    CssParser::cssFile += text+" {}";
    currentClassName = text;
    updateClassTree();
    fillCSSBegMode(currentClassName);
  }
} //addClasses

void MainWindow::loading(int value) {
  pbStatusBar->setValue(value);
} //loading

void MainWindow::linkHovered(QString link) {
  lblStatusBar3->setText(link);
} //linkHovered

void MainWindow::webPageCursorMoved() {
  QAction* action = webPreview->pageAction(QWebPage::ToggleBold);
  btnBold->setChecked(action->isChecked());
  action = webPreview->pageAction(QWebPage::ToggleItalic);
  btnItalic->setChecked(action->isChecked());
  action = webPreview->pageAction(QWebPage::ToggleUnderline);
  btnUnderline->setChecked(action->isChecked());
  //lblStatusBar1->setText(QString::number(webPreview->page()->totalBytes()/1024) + "KB");
} //webPageCursorMoved

KPushButton* MainWindow::createToolButton(QWidget* parent, QString icon, QString toolTip, bool checkable) {
  KPushButton* aButton = new KPushButton(parent);
  aButton->setMinimumSize(QSize(26, 26));
  aButton->setMaximumSize(QSize(26, 26));
  aButton->setIcon(KIcon(icon));
  aButton->setIconSize(QSize(16,16));
  aButton->setToolTip(toolTip);
  aButton->setCheckable(checkable);
  aButton->setStyleSheet("margin-left:1px;margin-right:1px;spacing:0px;padding:px;");
  return aButton;
} //createToolButton

QFrame* MainWindow::createSpacer() {
  QFrame* aline = new QFrame(menuEdit);
  aline->setObjectName(QString::fromUtf8("line"));
  aline->setFrameShape(QFrame::VLine);
  aline->setFrameShadow(QFrame::Sunken);
  aline->setStyleSheet("margin:3px;padding:3px;width:7px;");
  return aline;
} //createSpacer()

KAction* MainWindow::createAction(QString name, QString icon, QKeySequence shortcut, bool checkable) {
  KAction* newAction = new KAction(this);
  newAction->setText(i18n(name.toStdString().c_str()));
  newAction->setIcon(KIcon(icon));
  newAction->setShortcut(shortcut);
  newAction->setCheckable(checkable);
  actionCollection->addAction(name, newAction);
  ashActions[name] = newAction;
  return newAction;
} //createAction