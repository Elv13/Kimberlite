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
#include "src/tagEditor.h"

MainWindow::MainWindow(QWidget* parent) : KMainWindow(parent),currentHTMLPage(NULL),currentScript(NULL),aProjectManager(NULL),isModified(false),previousCssMode(999),previousKimberliteMode(MODE_WYSIWYG) {
  actionCollection = new KActionCollection(this);
  setWindowTitle("Kimberlite");
  db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
  db->setDatabaseName( KStandardDirs::locate( "appdata", "kimberlite.db" ));
  qDebug() << (( db->open())? "Database corectly opened":"ERROR while opening the database, get ready for a crash");

  KTipDatabase* aTipDB = new KTipDatabase();
  KTipDialog* aTipDialog = new KTipDialog(aTipDB, this);
  aTipDialog->setShowOnStart(true);
  aTipDialog->showTip(this, QString(), true);
  
  configSkeleton.readConfig();

  aParser = new HtmlParser();
  tabWMenu = new KTabWidget(this);
  tabWMenu->setMinimumSize(QSize(0, 90));
  tabWMenu->setMaximumSize(QSize(9999999, 90));
  tabWMenu->setStyleSheet("/*Make the tab bar look less like a tab bar*/\
    QTabWidget {\
      border-right:0px;\
      margin-right:0px;\
      border-left:0px;\
      margin-left:0px;\
    }\
    QTabWidget::tab-bar {\
      left:0px;\
    }\
    QTabWidget::pane {\
      border: 0px solid #C2C7CB;\
    }\
    ");

  /***************************************************************
  
			File toolbar
  
  ***************************************************************/

  fileTB = new KToolBar(this);
  fileTB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  fileTB->setIconSize(QSize(22,22));
  tabWMenu->addTab(fileTB, "Files");
  QPalette aPalette;
  fileTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:transparent;");

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
  
  createAction("Export", "document-export", Qt::CTRL + Qt::Key_Q);
  connect(ashActions["Export"], SIGNAL(triggered(bool)),this, SLOT(exportProject()));
  fileTB->addAction(ashActions["Export"]);
  ashActions["Export"]->setDisabled(true);
  
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
  editTB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  editTB->setIconSize(QSize(22,22));
  editTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:transparent;");
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
  
  //cbbFont = new KFontComboBox(menuEdit);
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

  //cbbHeader = new HeaderComboBox(menuEdit);
  cbbHeader = new QComboBox(menuEdit);
  QStringList headerSize;
  headerSize << "H1" <<"H2"<<"H3"<<"H4"<<"H5";
  cbbHeader->addItems(headerSize);
  cbbHeader->setMaximumSize(QSize(175, 25));
  cbbHeader->setMinimumSize(QSize(175, 25));
  cbbHeader->setObjectName(QString::fromUtf8("cbbHeader"));
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
  connect(btnChar, SIGNAL(clicked()), this, SLOT(setOList()));
  hlTextAtributeButton->addWidget(btnChar);
  
  hlTextAtributeButton->addWidget(createSpacer());
  
  KPushButton* btnIndent = createToolButton(menuEdit,"format-indent-more","Indent selected text");
  connect(btnIndent, SIGNAL(clicked()), this, SLOT(addIndent()));
  hlTextAtributeButton->addWidget(btnIndent);
  
  KPushButton* btnUnIndent = createToolButton(menuEdit,"format-indent-less","Outdent selected text");
  connect(btnUnIndent, SIGNAL(clicked()), this, SLOT(addOutdent()));
  hlTextAtributeButton->addWidget(btnUnIndent);
  
  hlTextAtributeButton->addWidget(createSpacer());

  kcbbTextColor = new ColorComboBox(this);
  kcbbTextColor->setIcon(KIcon("format-text-color"));
  kcbbTextColor->setObjectName(QString::fromUtf8("kcbbTextColor"));
  connect(kcbbTextColor, SIGNAL(colorChanged(QColor)), this, SLOT(setTextColor(QColor)));
  hlTextAtributeButton->addWidget(kcbbTextColor);
  
  cbbHighlightColor = new ColorComboBox(menuEdit);
  cbbHighlightColor->setIcon(KIcon("format-stroke-color"));
  cbbHighlightColor->setObjectName(QString::fromUtf8("cbbHighlightColor"));
  connect(cbbHighlightColor, SIGNAL(colorChanged(QColor)), this, SLOT(setHighlightColor(QColor)));
  hlTextAtributeButton->addWidget(cbbHighlightColor);

  cbbBackgroundColor = new ColorComboBox(menuEdit);
  cbbBackgroundColor->setIcon(KIcon("fill-color"));
  cbbBackgroundColor->setObjectName(QString::fromUtf8("cbbBackgroundColor"));
  connect(cbbBackgroundColor, SIGNAL(colorChanged(QColor)), this, SLOT(setBackgroundColor(QColor)));
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
  viewTB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  viewTB->setIconSize(QSize(22,22));
  tabWMenu->addTab(viewTB, "View");
  viewTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:transparent;");

  createAction("Project", KStandardDirs::locate("appdata", "pixmap/showProject.png"), NULL,true);
  viewTB->addAction(ashActions["Project"]);

  createAction("CSS Class", KStandardDirs::locate("appdata", "pixmap/showCSS.png"), NULL, true);
  viewTB->addAction(ashActions["CSS Class"]);

  createAction("HTML Tree", KStandardDirs::locate("appdata", "pixmap/showHTML.png"), NULL, true);
  viewTB->addAction(ashActions["HTML Tree"]);
  
  createAction("Attributes", KStandardDirs::locate("appdata", "pixmap/showHTML.png"), NULL, true);
  viewTB->addAction(ashActions["Attributes"]);

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
  insertTB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  insertTB->setIconSize(QSize(22,22));
  insertTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:transparent;");
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

  createAction("Add Anchor", KStandardDirs::locate("appdata", "pixmap/anchor.png"), NULL);
  connect(ashActions["Add Anchor"], SIGNAL(triggered(bool)), this, SLOT(addAnchor()));
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

  btnNewLine = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/br.png"),"Add line");
  connect(btnNewLine, SIGNAL(clicked()), this, SLOT(addNewLine()));
  hlInsert->addWidget(btnNewLine,0,1);

  btnNewTab = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/tab.png"),"Add tab");
  connect(btnNewTab, SIGNAL(clicked()), this, SLOT(addNewTab()));
  hlInsert->addWidget(btnNewTab,1,0);

  btnNewSpace = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/space.png"),"Add space");
  connect(btnNewSpace, SIGNAL(clicked()), this, SLOT(addNewSpace()));
  hlInsert->addWidget(btnNewSpace,0,0);
  
  btnHr = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/hr.png"),"Add horizontal line");
  connect(btnHr, SIGNAL(clicked()), this, SLOT(addAddHr()));
  hlInsert->addWidget(btnHr,1,1);
  
  QFrame* aline = new QFrame(menuEdit);
  aline->setObjectName(QString::fromUtf8("line"));
  aline->setFrameShape(QFrame::VLine);
  aline->setFrameShadow(QFrame::Sunken);
  aline->setMinimumSize(6,0);
  aline->setStyleSheet("margin:3px;padding:3px;width:20px;");
  hlInsert->addWidget(aline,0,2,2,1);

  btnTextLine = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/textline.png"),"Add a text field");
  connect(btnTextLine, SIGNAL(clicked()), this, SLOT(addTextLine()));
  hlInsert->addWidget(btnTextLine,0,3);
  
  btnPassword = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/password.png"),"Add a password field");
  connect(btnPassword, SIGNAL(clicked()), this, SLOT(addPasswordLine()));
  hlInsert->addWidget(btnPassword,1,3);
  
  btnCheckBox = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/checkbox.png"),"Add a checkbox");
  connect(btnCheckBox, SIGNAL(clicked()), this, SLOT(addCheckBox()));
  hlInsert->addWidget(btnCheckBox,0,4);
  
  btnRadioButton = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/radio.png"),"Add a radio button");
  connect(btnRadioButton, SIGNAL(clicked()), this, SLOT(addRadioButton()));
  hlInsert->addWidget(btnRadioButton,1,4);
  
  btnSubmit = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/submit.png"),"Add a submit button");
  connect(btnSubmit, SIGNAL(clicked()), this, SLOT(addSubmitButton()));
  hlInsert->addWidget(btnSubmit,0,5);
  
  btnReset = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/reset.png"),"Add a reset button");
  connect(btnReset, SIGNAL(clicked()), this, SLOT(addResetButton()));
  hlInsert->addWidget(btnReset,1,5);
  
  btnUpload = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/upload.png"),"Add an upload button");
  connect(btnUpload, SIGNAL(clicked()), this, SLOT(addUploadButton()));
  hlInsert->addWidget(btnUpload,0,6);
  
  btnHidden = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/hidden.png"),"Add an hidden object");
  connect(btnHidden, SIGNAL(clicked()), this, SLOT(addHiddenField()));
  hlInsert->addWidget(btnHidden,1,6);
  
  btnButton = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/button.png"),"Add a simple button");
  connect(btnButton, SIGNAL(clicked()), this, SLOT(addButton()));
  hlInsert->addWidget(btnButton,0,7);
  
  btnTextAera = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/textarea.png"),"Add a text area");
  connect(btnTextAera, SIGNAL(clicked()), this, SLOT(addTextAera()));
  hlInsert->addWidget(btnTextAera,1,7);
  
  btnHtmlButton = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/button.png"),"Add a rich (html) button");
  connect(btnHtmlButton, SIGNAL(clicked()), this, SLOT(addHtmlButton()));
  hlInsert->addWidget(btnHtmlButton,0,8);
  
  btnSelect = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/combobox.png"),"Add a combo box");
  connect(btnSelect, SIGNAL(clicked()), this, SLOT(addSelect()));
  hlInsert->addWidget(btnSelect,1,8);
  
  btnList = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/list.png"),"Add a list");
  connect(btnList, SIGNAL(clicked()), this, SLOT(addListControl()));
  hlInsert->addWidget(btnList,0,9);
  
  btnLabel = createToolButton(menuInsert,KStandardDirs::locate("appdata", "pixmap/tags/label.png"),"Add a label");
  connect(btnLabel, SIGNAL(clicked()), this, SLOT(addLabel()));
  hlInsert->addWidget(btnLabel,1,9);

  /***************************************************************
  
			Tools toolbar
  
  ***************************************************************/
  
  toolsTB = new KToolBar(this);
  toolsTB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  toolsTB->setIconSize(QSize(22,22));
  tabWMenu->addTab(toolsTB, "Tools");
  toolsTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:transparent;");

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
  optToolBarLayout->setSpacing(2);
  
  optionsTB = new KToolBar(this);
  optionsTB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  optionsTB->setIconSize(QSize(22,22));
  optionsTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:transparent;");
  optToolBarLayout->addWidget(optionsTB,0,0,2,1);
  
  createAction("Shortcuts", "configure-shortcuts", NULL);
  connect(ashActions["Shortcuts"], SIGNAL(triggered(bool)), this, SLOT(editShortcut()));
  optionsTB->addAction(ashActions["Shortcuts"]);
  
  optToolBarLayout->addWidget(createSpacer(),0,1,2,1);
  
  QLabel* lblStandard = new QLabel(menuEdit);
  lblStandard->setText("<img src=\""+KStandardDirs::locate("appdata", "pixmap/browserLogo/w3c.png")+"\" height=16 style=\"padding-right:2px;margin-right:2px;\"> Standard");
  optToolBarLayout->addWidget(lblStandard,0,2);
  
  QComboBox* cbbLanguage = new QComboBox(this);
  cbbLanguage->setMaximumSize(9999,25);
  QStringList toAdd;
  toAdd << "HTML 4.01" << "xHTML 1.0.1" << "HTML 5.0";
  cbbLanguage->addItems(toAdd);
  optToolBarLayout->addWidget(cbbLanguage,1,2);
  
  optToolBarLayout->addWidget(createSpacer(),0,3,2,1);
  
  QLabel* lblFirefox = new QLabel(menuEdit);
  lblFirefox->setMaximumSize(20,20);
  KIcon icnFirefox(KStandardDirs::locate("appdata", "pixmap/browserLogo/FF.png"));
  lblFirefox->setPixmap(icnFirefox.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblFirefox,0,4);
  
  QLabel* lblFirefox2 = new QLabel(menuEdit);
  lblFirefox2->setText("Firefox");
  optToolBarLayout->addWidget(lblFirefox2,0,5);
  
  QComboBox* cbbFirefox = new QComboBox(this);
  cbbFirefox->setMaximumSize(9999,25);
  toAdd.clear();
  toAdd << "--None--" << "Version 1" << "Version 2" << "Version 3";
  cbbFirefox->addItems(toAdd);
  optToolBarLayout->addWidget(cbbFirefox,1,4,1,2);
  cbbFirefox->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,6,2,1);
  
  QLabel* lblInternetExplorer = new QLabel(menuEdit);
  KIcon icnInternetExplorer(KStandardDirs::locate("appdata", "pixmap/browserLogo/IE.png"));
  lblInternetExplorer->setPixmap(icnInternetExplorer.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblInternetExplorer,0,7);
  
  QLabel* lblInternetExplorer2 = new QLabel(menuEdit);
  lblInternetExplorer2->setText("Internet Explorer");
  optToolBarLayout->addWidget(lblInternetExplorer2,0,8);
  
  QComboBox* cbbInternetExplorer = new QComboBox(this);
  cbbInternetExplorer->setMaximumSize(9999,25);
  toAdd.clear();
  toAdd << "--None--" << "Version 6" << "Version 7" << "Version 8";
  cbbInternetExplorer->addItems(toAdd);
  optToolBarLayout->addWidget(cbbInternetExplorer,1,7,1,2);
  cbbInternetExplorer->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,9,2,1);
  
  QLabel* lblSafari = new QLabel(menuEdit);
  lblSafari->setMaximumSize(20,20);
  KIcon icnSafari(KStandardDirs::locate("appdata", "pixmap/browserLogo/SF.png"));
  lblSafari->setPixmap(icnSafari.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblSafari,0,10);
  
  QLabel* lblSafari2 = new QLabel(menuEdit);
  lblSafari2->setText("Safari");
  optToolBarLayout->addWidget(lblSafari2,0,11);
  
  QComboBox* cbbSafari = new QComboBox(this);
  cbbSafari->setMaximumSize(9999,25);
  toAdd.clear();
  toAdd << "--None--" << "Version 2" << "Version 3" << "Version 4";
  cbbSafari->addItems(toAdd);
  optToolBarLayout->addWidget(cbbSafari,1,10,1,2);
  cbbSafari->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,12,2,1);
  
  QLabel* lblOpera = new QLabel(menuEdit);
  lblOpera->setMaximumSize(20,20);
  KIcon icnOpera(KStandardDirs::locate("appdata", "pixmap/browserLogo/OP.png"));
  lblOpera->setPixmap(icnOpera.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblOpera,0,13);
  
  QLabel* lblOpera2 = new QLabel(menuEdit);
  lblOpera2->setText("Opera");
  optToolBarLayout->addWidget(lblOpera2,0,14);
  
  QComboBox* cbbOpera = new QComboBox(this);
  cbbOpera->setMaximumSize(9999,25);
  toAdd.clear();
  toAdd << "--None--" << "Version 7" << "Version 8" << "Version 9";
  cbbOpera->addItems(toAdd);
  optToolBarLayout->addWidget(cbbOpera,1,13,1,2);
  cbbOpera->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,15,2,1);
  
  QLabel* lblKonqueror = new QLabel(menuEdit);
  lblKonqueror->setMaximumSize(20,20);
  KIcon icnKonqueror(KStandardDirs::locate("appdata", "pixmap/browserLogo/KQ.png"));
  lblKonqueror->setPixmap(icnKonqueror.pixmap(20,QIcon::Normal,QIcon::On));
  optToolBarLayout->addWidget(lblKonqueror,0,16);
  
  QLabel* lblKonqueror2 = new QLabel(menuEdit);
  lblKonqueror2->setText("Konqueror");
  optToolBarLayout->addWidget(lblKonqueror2,0,17);
  
  QComboBox* cbbKonqueror = new QComboBox(this);
  cbbKonqueror->setMaximumSize(9999,25);
  toAdd.clear();
  toAdd << "--None--" << "Version 3" << "Version 4";
  cbbKonqueror->addItems(toAdd);
  optToolBarLayout->addWidget(cbbKonqueror,1,16,1,2);
  cbbKonqueror->setDisabled(true);
  
  optToolBarLayout->addWidget(createSpacer(),0,18,2,1);
  
  optToolBarLayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum),0,19);

  /***************************************************************
  
			Help tool bar
  
  ***************************************************************/

  helpTB = new KToolBar(this);
  helpTB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  helpTB->setIconSize(QSize(22,22));
  tabWMenu->addTab(helpTB, "Help");
  helpTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:transparent;");

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
  btnTreeAdd->setIcon(KIcon("list-add"));
  
  btnTreeRemove = new KPushButton(treeDockCentral);
  btnTreeRemove->setObjectName(QString::fromUtf8("btnTreeRemove"));
  verticalLayout_5->addWidget(btnTreeRemove,1,1);
  btnTreeRemove->setIcon(KIcon("list-remove"));
  
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
  
			Tag Editor Dock
  
  ***************************************************************/
  
  TagEditor* aTagEditor = new TagEditor(this);
  aTagEditor->setVisible(false);
  addDockWidget(Qt::RightDockWidgetArea, aTagEditor);
  connect(rtfHTMLEditor, SIGNAL(currentTagChanged(QString)),aTagEditor,SLOT(displayAttribute(QString)));
  connect(aTagEditor, SIGNAL(setAttribute(QString,QString)),rtfHTMLEditor,SLOT(setAttribute(QString, QString)));
  connect(ashActions["Attributes"], SIGNAL(triggered(bool)), aTagEditor, SLOT(setVisible(bool)));
  connect(aTagEditor, SIGNAL(visibilityChanged(bool)), ashActions["Attributes"] , SLOT(setChecked(bool)));
  
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
  verticalLayout_8->setContentsMargins(0,0,0,0);
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
  
  aHtmlThread = new ParserThread(this); 
  aHtmlThread->rtfHtml = rtfHTMLEditor;
  aHtmlThread->rtfCss = rtfCSSEditor;
  connect(rtfHTMLEditor, SIGNAL(textChanged()), aHtmlThread, SLOT(getReady()));
  connect(rtfCSSEditor, SIGNAL(textChanged()), aHtmlThread, SLOT(getReadyCss()));
  connect(aHtmlThread, SIGNAL(updateTree(IndexedTreeWidgetItem*,bool)), this, SLOT(updateHtmlTree(IndexedTreeWidgetItem*,bool)));
  connect(aHtmlThread, SIGNAL(updateCssTree(QTreeWidgetItem*,bool)), this, SLOT(updateCssTree(QTreeWidgetItem*,bool)));

  aHtmlThread->start();
  
  /***************************************************************
  
			StatusBar
  
  ***************************************************************/

  QWidget* statusWidget = new QWidget();
  QHBoxLayout* statusLayout = new QHBoxLayout(statusWidget);
  statusLayout->setContentsMargins(0,0,0,0);
  
  lblStatusBar3 = new KSqueezedTextLabel();
  lblStatusBar3->setMinimumSize(100,0);
  lblStatusBar3->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
  statusLayout->addWidget(lblStatusBar3);
  
  lblStatusBar1 = new QLabel();
  lblStatusBar1->setText("");
  lblStatusBar1->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum);
  lblStatusBar1->setFrameShape(QFrame::StyledPanel);
  lblStatusBar1->setFrameShadow(QFrame::Sunken);
  statusLayout->addWidget(lblStatusBar1);
  
  lblStatusBar2 = new QLabel();
  lblStatusBar2->setText("HTMLv4");
  lblStatusBar2->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum);
  lblStatusBar2->setFrameShape(QFrame::StyledPanel);
  lblStatusBar2->setFrameShadow(QFrame::Sunken);
  statusLayout->addWidget(lblStatusBar2);
  
  pbStatusBar = new QProgressBar();
  pbStatusBar->setMaximumSize(100,999);
  statusLayout->addWidget(pbStatusBar);
  
  tabWEditor->setCornerWidget(statusWidget);
  
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
  saveRecentProject(fileName, aProjectManager->getProjectName());
} //saveProjectAs
 
void MainWindow::saveProjectAs(){
  QString name = KFileDialog::getSaveFileName();
  if (name.size() < 5)
    name += ".wpk";
  else if (name.right(4).toLower() != ".wpk")
    name += ".wpk";
    
  if (QFile::exists(name)) {
    int result = KMessageBox::questionYesNo(this, name + " exist. Do you want to overwrite it?", "Overwrite?");
    if (result == KMessageBox::Yes)
      saveProjectAs(name);
  }
  else
    saveProjectAs(name);
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
    setupTmpDir(true);
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
    ashActions["Export"]->setDisabled(false);
    tableDock->setVisible(true);
    saveRecentProject(fileName, aProjectManager->getProjectName());
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
	webPreview->setHtml(HtmlParser::getParsedHtml(data),setupTmpDir());
	break;
      case MODE_HTML:
	rtfHTMLEditor->setPlainText(HtmlParser::getParsedHtml(data));
	break;
      default:
	if (!force)
	  tabWEditor->setCurrentIndex(1);
	rtfHTMLEditor->setPlainText(HtmlParser::getParsedHtml(data));
    }    
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
  connect(aScript, SIGNAL(addScript(QString,QString,QString)), aProjectManager, SLOT(addScript(QString,QString,QString)));
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
  updateCssFile(MODE_CSS,previousCssMode);
 /* if (previousCssMode == CSS_MODE_BEG) {
    aHtmlThread->updateClassTree();
  }
  else */if (previousCssMode == CSS_MODE_EXPERT) {
    clearCssBeg();
    if (treeWidget->currentItem() != NULL)
      fillCSSBegMode(getClassName(treeWidget->currentItem()));
  }
  
  qDebug() << CssParser::cssFile;
  
  if (((previousCssMode == CSS_MODE_BEG) || (previousCssMode == 999)) && (mode == CSS_MODE_EXPERT) && (treeWidget->currentItem() != NULL)) {
    //qDebug() << "I am suppose to enter here" << CssParser::cssFile;
    disconnect(rtfCSSEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
    rtfCSSEditor->setPlainText(CssParser::parseCSS());
    QString className = getClassName(treeWidget->currentItem());
    setCssCursor(className);
    connect(rtfCSSEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
  }
  previousCssMode = mode;
} //changeCssMode

void MainWindow::splitSubClass(QString name, QTreeWidgetItem* parent) {
  if (((name.indexOf(":") != -1) && (name.indexOf(" ") == -1)) && ((name.indexOf(":") != 0))) {
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
  aHtmlThread->updateClassTree(true);
} //loadCss

QString MainWindow::setupTmpDir(bool initial) {
  QDir aDir;
  if (initial) {
    if (aDir.exists(QDir::tempPath() + "/kimberlite/project/" + aProjectManager->getProjectName() + "/"))
      aDir.remove(QDir::tempPath() + "/kimberlite/project/" + aProjectManager->getProjectName() + "/");
  }
  QString path2;
  path2 = QDir::tempPath() + "/kimberlite/project/" + aProjectManager->getProjectName() + "/";
  aDir.mkpath(path2);
  QFile aFile(path2+"StyleSheet.css");
  aFile.open(QIODevice::WriteOnly);
  aFile.write(CssParser::parseCSS().toStdString().c_str());
  aFile.close();
  return path2;
}

void MainWindow::modeChanged(int index) {
  tableDock->setVisible(true);
  
  if (previousKimberliteMode == MODE_CSS)
    updateCssFile(MODE_CSS);
  else if (previousKimberliteMode == MODE_HTML)
    loadPage(currentHTMLPage,rtfHTMLEditor->toPlainText(),true);
  
  switch (index) {
    case MODE_WYSIWYG: { 
      qDebug() << "i am suppose to work";
      if (previousKimberliteMode == MODE_HTML)
	webPreview->setHtml(rtfHTMLEditor->toPlainText(),setupTmpDir());
      disableWysiwyg(false);
      ashActions["Zoom 1:1"]->setEnabled(true);
      treeDock->setVisible(false);
      dockHtmlTree->setVisible(false);
      pbStatusBar->setEnabled(true);
      break;
    }
    case MODE_HTML:
      if (previousKimberliteMode == MODE_WYSIWYG)
	rtfHTMLEditor->setPlainText(aParser->getParsedHtml(webPreview->page()->mainFrame()->toHtml()));
      disableWysiwyg(false);
      ashActions["Zoom 1:1"]->setEnabled(true);
      dockHtmlTree->setVisible(true);
      treeDock->setVisible(false);
      pbStatusBar->setEnabled(false);
      break;
    case MODE_SCRIPT:
      ashActions["Zoom 1:1"]->setEnabled(true);
      disableWysiwyg(true);
      dockHtmlTree->setVisible(false);
      treeDock->setVisible(false);
      pbStatusBar->setEnabled(false);
      break;
    case MODE_CSS:
      disableWysiwyg(true);
      ashActions["Zoom 1:1"]->setEnabled(false);
      dockHtmlTree->setVisible(false);
      tableDock->setVisible(false);
      treeDock->setVisible(true);
      pbStatusBar->setEnabled(false);
      if (!currentClassName.isEmpty())
	fillCSSBegMode(getClassName(getClassWidget(currentClassName)));
  }
  previousKimberliteMode = index;
} //modeChanged

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

void MainWindow::setTextColor(QColor aColor) {
  addTag("<font color=\""+aColor.name()+"\">","</font>","foreColor", aColor.name());
} //setTextColor

void MainWindow::setHighlightColor(QColor aColor) {
  addTag("<font style=\"background-color:"+aColor.name()+"\">","</font>","hiliteColor", aColor.name());
} //setHighlightColor

void MainWindow::setUList() {
  addTag("<ul>","</ul>","insertUnorderedList");
} //setUList

void MainWindow::addIndent() {
  addTag("<blockquote>","</blockquote>","indent");
} //addIndent

void MainWindow::addOutdent() {
  addTag("","","outdent");
} //addOutdent

void MainWindow::setOList() {
  addTag("<ol>","</ol>","insertOrderedList");
} //setOList

void MainWindow::addTextLine() {
  addTag("<input type=\"text\" name=\"\" />","","inserthtml","<input type=\\\"text\\\" name=\\\"\\\" value=\\\"test\\\" />");
} //addTextLine

void MainWindow::addPasswordLine() {
  addTag("<input type=\"password\" name=\"\" />","","inserthtml","<input type=\\\"password\\\" name=\\\"\\\" value=\\\"password\\\" />");
} //addPasswordLine

void MainWindow::addCheckBox() {
  addTag("<input type=\"checkbox\" name=\"\" />","","inserthtml","<input type=\\\"checkbox\\\" name=\\\"\\\" />");
} //addCheckBox

void MainWindow::addRadioButton() {
  addTag("<input type=\"radio\" name=\"\" />","","inserthtml","<input type=\\\"radio\\\" name=\\\"\\\" />");
} //addRadioButton

void MainWindow::addSubmitButton() {
  addTag("<input type=\"submit\" name=\"\" />","","inserthtml","<input type=\\\"submit\\\" name=\\\"\\\" />");
} //addSubmitButton

void MainWindow::addResetButton() {
  addTag("<input type=\"reset\" name=\"\" />","","inserthtml","<input type=\\\"reset\\\" name=\\\"\\\" />");
} //addResetButton

void MainWindow::addUploadButton() {
  addTag("<input type=\"file\" name=\"\" />","","inserthtml","<input type=\\\"file\\\" name=\\\"\\\" />");
} //addUploadButton

void MainWindow::addHiddenField() {
  addTag("<input type=\"hidden\" name=\"\" />","","inserthtml","<input type=\\\"hidden\\\" name=\\\"\\\" />");
} //addHiddenField

void MainWindow::addButton() {
  addTag("<input type=\"button\" name=\"\" />","","inserthtml","<input type=\\\"button\\\" name=\\\"\\\" value=\\\"Button\\\" />");
} //addButton

void MainWindow::addTextAera() {
  addTag("<textarea>","</textarea>","inserthtml","<textarea></textarea>");
} //addTextAera

void MainWindow::addHtmlButton() {
  addTag("<button>","</button>","inserthtml","<button></button>");
} //addHtmlButton

void MainWindow::addSelect() {
  addTag("<select>","</select>","inserthtml","<select><option>--SELECT--</option></select>");
} //addSelect

void MainWindow::addListControl() {
  addTag("<select multiple=\"yes\" size=5>","</select>","inserthtml","<select multiple=\"yes\" size=5><option>--SELECT--</option><option>--SELECT--</option></select>");
} //addListControl

void MainWindow::addLabel() {
  addTag("<label>","</label>","inserthtml","<label>Label</label>");
} //addLabel

void MainWindow::addNewLine() {
  addTag("<br />","","inserthtml","<br />");
} //addNewLine

void MainWindow::addNewTab() {
  addTag("<pre>	</pre>","","inserthtml","<pre>	</pre>"); //BUG Does not work
} //addNewTab

void MainWindow::addNewSpace() {
  addTag("&nbsp","","inserthtml","&nbsp");
} //addNewSpace

void MainWindow::addAddHr() {
  addTag("<hr />","","inserthtml","<hr />");
} //addAddHr

void MainWindow::setBackgroundColor(QColor aColor) {
  if (KIMBERLITE_MODE == MODE_WYSIWYG)
    rtfHTMLEditor->setPlainText(webPreview->page()->mainFrame()->toHtml());
  HtmlData pageData = aParser->getHtmlData(rtfHTMLEditor->toPlainText());
  HtmlParser::setAttribute(pageData, "body", 0, "bgcolor", aColor.name());
  rtfHTMLEditor->setPlainText(aParser->getParsedHtml(pageData));
  if (KIMBERLITE_MODE == MODE_WYSIWYG)
    webPreview->setHtml(rtfHTMLEditor->toPlainText(),setupTmpDir());
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
  aList << cbbFont << cbbFontSize << btnBold << btnUnderline << btnItalic << btnAlignLeft << btnAlignCenter << btnAlignRight  << btnJustify << cbbHeader /* <<btnLink*/ << btnChar /*<< btnTable*/ << btnList << kcbbTextColor  << cbbHighlightColor << cbbBackgroundColor << btnNewLine  << btnNewTab << btnNewSpace /*<< btnUnIndent << btnIndent*/ << btnHr << btnTextLine << btnPassword << btnCheckBox << btnRadioButton << btnSubmit << btnReset << btnUpload << btnHidden << btnButton << btnTextAera << btnHtmlButton << btnSelect << btnLabel;
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

void MainWindow::saveRecentProject(QString filePath, QString projectTitle) {
  QList<QStringList> recentProjects;
  bool done = false;
  recentProjects << configSkeleton.recentP1 << configSkeleton.recentP2 << configSkeleton.recentP3 << configSkeleton.recentP4 << configSkeleton.recentP5;
  for (int i=0;i<recentProjects.size();i++) {
    if (recentProjects[i][1] == filePath)
      break; //Dirty hack to prevent a bug, need rewriting, some day
    else if (recentProjects[i][1].isEmpty()) {
      recentProjects[i][0] = projectTitle;
      recentProjects[i][1] = filePath;
      done = true;
      break;
    }
  }
  if (!done)
    for (int i=0;i<recentProjects.size();i++) {
      if (recentProjects[i][1] == filePath) {
	recentProjects.removeAt(i);
	recentProjects.push_back(QStringList() << projectTitle << filePath);
	done = true;
	break;
      }
    }
  if (!done) {
    recentProjects.removeAt(0);
    recentProjects.push_back(QStringList() << projectTitle << filePath);
  }  
  configSkeleton.recentP1 = recentProjects[0];
  configSkeleton.recentP2 = recentProjects[1];
  configSkeleton.recentP3 = recentProjects[2];
  configSkeleton.recentP4 = recentProjects[3];
  configSkeleton.recentP5 = recentProjects[4];
  configSkeleton.writeConfig();
} //saveRecentProject

void MainWindow::loadDefaultPage() {
  KIconLoader *iconloader = KIconLoader::global();
  QString new_path = iconloader->iconPath("document-new", KIconLoader::Desktop );
  QString open_path = iconloader->iconPath("document-open", KIconLoader::Desktop );
  QString path = KStandardDirs::locate("appdata", "data/home.htm");//KStandardDirs::locate("data", "konqueror/about/launch.html");
  QFile file2(path);
  file2.open(QIODevice::ReadOnly);
  QString page;
  page = file2.readAll();
  page = page.arg( KStandardDirs::locate( "appdata", "pixmap/about/" ) );
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
  page = page.arg(configSkeleton.recentP1[1]).arg(configSkeleton.recentP1[0]);
  page = page.arg(configSkeleton.recentP2[1]).arg(configSkeleton.recentP2[0]);
  page = page.arg(configSkeleton.recentP3[1]).arg(configSkeleton.recentP3[0]);
  page = page.arg(configSkeleton.recentP4[1]).arg(configSkeleton.recentP4[0]);
  page = page.arg(configSkeleton.recentP5[1]).arg(configSkeleton.recentP5[0]);
  page = page.arg("Kimberlite 0.1 realised (2009/06/12)<br>The first official version of Kimberlite is now availible. Test, use it, crash it and report bugs!<br>Good Luck ;)");
  page = page.arg(KStandardDirs::locate("appdata", "pixmap/logo.png"));

  file2.close();
  webPreview->setHtml(page);
  qDebug() << page;
  //sleep(30);
  
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
  else if (rtfCSSEditor == getCurrentEditor()) {
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
    else {
      rtfCSSEditor->append(text+" {}");
      return;
    }
    CssParser::cssFile += text+" {}";
    currentClassName = text;
    aHtmlThread->updateClassTree(true);
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
  aButton->setMinimumSize(QSize(28, 26));
  aButton->setMaximumSize(QSize(28, 26));
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
  aline->setMinimumSize(5,0);
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

void MainWindow::updateHtmlTree(IndexedTreeWidgetItem* topItem,bool clear) {
  if (clear)
    treeHtml->clear();
  if ((!topItem->text(0).isEmpty()) || ((topItem->child(0)) && (topItem->text(0).isEmpty())))
    treeHtml->addTopLevelItem(topItem);
  treeHtml->expandAll();
} //updateHtmlTree

void MainWindow::exportProject() {
  QString path = KFileDialog::getExistingDirectory(QUrl(""),this);
  aProjectManager->exportProject(path + "/" + aProjectManager->getProjectName() + "/");
} //exportProject

void MainWindow::addAnchor() {
  bool ok;
  QString text = QInputDialog::getText(this, "Add anchor", "Set the name of the anchor", QLineEdit::Normal, "", &ok);
  if (ok && !text.isEmpty()) {
    addTag("<a name=\""+text+"\"></a>","","inserthtml","<a name=\""+text+"\"></a>");
    KMessageBox::error(this,"<b>Copy and paste this text in the html code where you want the link to be:</b><br><pre>&it;a href=\"\"&gt;\\</a\\></pre>");
  }
} //addAnchor

void MainWindow::updateCssTree(QTreeWidgetItem* topItem, bool clear) {
  if (clear) {
    treeWidget->clear();
    styleSheetName = topItem;
  }
  treeWidget->addTopLevelItem(topItem);

  if (currentClassName.isEmpty()) {
    QStringList classList = CssParser::getClassList();
    if (!classList.isEmpty())
      currentClassName = classList[0];
  }
  QTreeWidgetItem* toSelect = getClassWidget(currentClassName);
  if (toSelect) {
    fillCSSBegMode(getClassName(getClassWidget(currentClassName)));
    treeWidget->setCurrentItem(toSelect);
  }
  treeWidget->expandAll();
} //updateCssTree

void MainWindow::updateCssFile(int forceMode, int forceCssMode) {
  int lastMode((forceMode != 99)?forceMode:KIMBERLITE_MODE), lastCssMode((forceCssMode != 99)?forceCssMode:CSS_MODE);
  switch (lastMode) {
    case MODE_CSS:
      if ((lastCssMode == CSS_MODE_BEG) || (lastCssMode == 999))
	CssParser::cssFile = CssParser::setClass(currentClassName, clearCssBeg());
      else /*CSS_MODE_EXPERT*/
	CssParser::cssFile = rtfCSSEditor->toPlainText();
      break;
  }
} //updateCssFile