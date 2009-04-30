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

#include <KIcon>
#include <QtSql>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlDriver>
#include <KIO/NetAccess>
#include <KFileDialog>
#include <KMessageBox>
#include <KIO/NetAccess>
#include <KSaveFile>
#include <QTextStream>
#include <QInputDialog>
#include <kstandarddirs.h>
#include <QColor>
#include <QBrush>
#include <QPalette>
#include <QListWidgetItem>
#include <QWebFrame>
#include <QTextCursor>
#include <QDir>
#include <KCharSelect>
#include <KShortcutsEditor>
#include <KEditToolBar>
#include <KAboutApplicationDialog>
#include <KBugReport>
#include <KAboutData>
#include <KStatusBar>
#include <QProgressBar>
#include "src/CSSbeginnerWidget.h"
#include "src/addProprietyWidget.h"
#include "src/stringToTemplate.h"
#include "src/ProjectManager_v2.h"
#include "src/template.h"
#include "src/ressourcesManager.h"
#include "src/newTable.h"
#include "src/newPage.h"
#include "src/newScript.h"
#include "src/newProject.h"
#include "src/debugger.h"
#include <ktip.h>
#include <QPrintDialog>
#include <KPrintPreview>
#include <QPrinter>

MainWindow::MainWindow(QWidget* parent)  : KMainWindow(parent),currentHTMLPage(NULL),currentScript(NULL),aProjectManager(NULL) {
  isModified = false;
  actionCollection = new KActionCollection(this);
  setWindowTitle("Kimberlite");
  setupToolTip(); 
  qDebug() << "This: " <<   KStandardDirs::locate( "appdata", "kimberlite.db" ) << endl;
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName( KStandardDirs::locate( "appdata", "kimberlite.db" ));
    if ( db->open()) {
      printf("database corectly opened");
    }
    else {
      printf("ERROR while opening the database, get ready for a crash");
    }
    aParser = new HtmlParser();
    tabWMenu = new KTabWidget(this);
    tabWMenu->setMinimumSize(QSize(0, 90));
    tabWMenu->setMaximumSize(QSize(9999999, 90));

    fileTB = new KToolBar(this);
    tabWMenu->addTab(fileTB, "Files");
    QPalette aPalette;
    fileTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

    createAction("New Project", "document-new", Qt::CTRL + Qt::Key_W);
    connect(ashActions["New Project"], SIGNAL(triggered(bool)),this, SLOT(newProject()));
    fileTB->addAction(ashActions["New Project"]);

    createAction("Open Project", "document-open", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Open Project"], SIGNAL(triggered(bool)),this, SLOT(openProject()));
    fileTB->addAction(ashActions["Open Project"]);

    fileTB->addSeparator();

    createAction("Save", "document-save", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Save"], SIGNAL(triggered(bool)),this, SLOT(saveProject()));
    fileTB->addAction(ashActions["Save"]);

    createAction("Save As", "document-save-as", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Save As"], SIGNAL(triggered(bool)), this, SLOT(saveProjectAs()));
    fileTB->addAction(ashActions["Save As"]);

    fileTB->addSeparator();

    createAction("Print", "document-print", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Print"], SIGNAL(triggered(bool)),this, SLOT(print()));
    fileTB->addAction(ashActions["Print"]);

    createAction("Print Preview", "document-print-preview", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Print Preview"], SIGNAL(triggered(bool)), this, SLOT(printPreview()));
    fileTB->addAction(ashActions["Print Preview"]);

    fileTB->addSeparator();

    createAction("Quit", "application-exit", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Quit"], SIGNAL(triggered(bool)),this, SLOT(quit()));
    fileTB->addAction(ashActions["Quit"]);

    menuEdit = new QWidget();
    menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
    horizontalLayout_14 = new QHBoxLayout(menuEdit);
    horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
    horizontalLayout_14->setContentsMargins(0,0,0,0);
    editTB = new KToolBar(this);
    editTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");
    horizontalLayout_14->addWidget(editTB);

    createAction("Undo", "edit-undo", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Undo"], SIGNAL(triggered(bool)), this, SLOT(undo()));
    editTB->addAction(ashActions["Undo"]);

    createAction("Redo", "edit-redo", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Redo"], SIGNAL(triggered(bool)), this, SLOT(redo()));
    editTB->addAction(ashActions["Redo"]);

    editTB->addSeparator();

    createAction("Copy", "edit-copy", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Copy"], SIGNAL(triggered(bool)), this, SLOT(copy()));
    editTB->addAction(ashActions["Copy"]);

    createAction("Cut", "edit-cut", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Cut"], SIGNAL(triggered(bool)), this, SLOT(cut()));
    editTB->addAction(ashActions["Cut"]);

    createAction("Paste", "edit-paste", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Paste"], SIGNAL(triggered(bool)), this, SLOT(paste()));
    editTB->addAction(ashActions["Paste"]);

    editTB->addSeparator();

    createAction("Find", "edit-find", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Find"], SIGNAL(triggered(bool)), this, SLOT(find()));
    editTB->addAction(ashActions["Find"]);

    editTB->addSeparator();

    vlTextAtribute = new QVBoxLayout();
    vlTextAtribute->setObjectName(QString::fromUtf8("vlTextAtribute"));
    vlTextAtribute->setContentsMargins(0,0,0,0);
    hlFont = new QHBoxLayout();
    hlFont->setContentsMargins(0,0,0,0);
    hlFont->setObjectName(QString::fromUtf8("hlFont"));
    hlFont->setSizeConstraint(QLayout::SetMinimumSize);
    cbbFont = new QComboBox(menuEdit);
    cbbFont->setObjectName(QString::fromUtf8("cbbFont"));
    cbbFont->setMinimumSize(QSize(225, 0));
    cbbFont->setMaximumSize(QSize(225, 20));
    cbbFont->addItems(QFontDatabase().families());
    connect(cbbFont, SIGNAL(currentIndexChanged (QString)),
    this, SLOT(setFont(QString)));

    hlFont->addWidget(cbbFont);

    cbbFontSize = new KIntSpinBox(menuEdit);
    
    cbbFontSize->setObjectName(QString::fromUtf8("cbbFontSize"));
    cbbFontSize->setMaximumSize(QSize(225, 20));
    cbbFontSize->setSuffix("px");
    connect(cbbFontSize, SIGNAL(valueChanged(int)),
    this, SLOT(setFontSize(int)));

    hlFont->addWidget(cbbFontSize);


    vlTextAtribute->addLayout(hlFont);

    hlTextAtributeButton = new QHBoxLayout();
    hlTextAtributeButton->setObjectName(QString::fromUtf8("hlTextAtributeButton"));
    btnBold = new KPushButton(menuEdit);
    btnBold->setObjectName(QString::fromUtf8("btnBold"));
    btnBold->setMinimumSize(QSize(30, 30));
    btnBold->setMaximumSize(QSize(30, 30));
    KIcon* icnBold = new KIcon("format-text-bold");
    btnBold->setIcon(*icnBold);
    connect(btnBold, SIGNAL(clicked()),
    this, SLOT(setBold()));

    hlTextAtributeButton->addWidget(btnBold);

    btnUnderline = new KPushButton(menuEdit);
    btnUnderline->setObjectName(QString::fromUtf8("btnUnderline"));
    btnUnderline->setMinimumSize(QSize(30, 30));
    btnUnderline->setMaximumSize(QSize(30, 16777215));
    KIcon* icnUnderLine = new KIcon("format-text-underline");
    btnUnderline->setIcon(*icnUnderLine);
    connect(btnUnderline, SIGNAL(clicked()),
    this, SLOT(setUnderline()));

    hlTextAtributeButton->addWidget(btnUnderline);

    btnItalic = new KPushButton(menuEdit);
    btnItalic->setObjectName(QString::fromUtf8("btnItalic"));
    btnItalic->setMinimumSize(QSize(30, 30));
    btnItalic->setMaximumSize(QSize(30, 16777215));
    KIcon* icnItalic = new KIcon("format-text-italic");
    btnItalic->setIcon(*icnItalic);
    connect(btnItalic, SIGNAL(clicked()),
    this, SLOT(setItalic()));

    hlTextAtributeButton->addWidget(btnItalic);

    line = new QFrame(menuEdit);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    hlTextAtributeButton->addWidget(line);

    btnAlignLeft = new KPushButton(menuEdit);
    btnAlignLeft->setObjectName(QString::fromUtf8("btnAlignLeft"));
    btnAlignLeft->setMinimumSize(QSize(30, 30));
    btnAlignLeft->setMaximumSize(QSize(30, 16777215));
    KIcon* icnAlignLeft = new KIcon("format-justify-left");
    btnAlignLeft->setIcon(*icnAlignLeft);
    connect(btnAlignLeft, SIGNAL(clicked()),
    this, SLOT(setAlignLeft()));

    hlTextAtributeButton->addWidget(btnAlignLeft);

    btnAlignCenter = new KPushButton(menuEdit);
    btnAlignCenter->setObjectName(QString::fromUtf8("btnAlignCenter"));
    btnAlignCenter->setMinimumSize(QSize(30, 30));
    btnAlignCenter->setMaximumSize(QSize(30, 16777215));
    KIcon* icnAlignCenter = new KIcon("format-justify-center");
    btnAlignCenter->setIcon(*icnAlignCenter);
    connect(btnAlignCenter, SIGNAL(clicked()),
    this, SLOT(setAlignCenter()));

    hlTextAtributeButton->addWidget(btnAlignCenter);

    btnAlignRight = new KPushButton(menuEdit);
    btnAlignRight->setObjectName(QString::fromUtf8("btnAlignRight"));
    btnAlignRight->setMinimumSize(QSize(30, 30));
    btnAlignRight->setMaximumSize(QSize(30, 16777215));
    KIcon* icnAlignRight = new KIcon("format-justify-right");
    btnAlignRight->setIcon(*icnAlignRight);
    connect(btnAlignRight, SIGNAL(clicked()),
    this, SLOT(setAlignRight()));

    hlTextAtributeButton->addWidget(btnAlignRight);

    btnJustify = new KPushButton(menuEdit);
    btnJustify->setObjectName(QString::fromUtf8("btnJustify"));
    btnJustify->setMinimumSize(QSize(30, 30));
    btnJustify->setMaximumSize(QSize(30, 16777215));
    KIcon* icnJustify = new KIcon("format-justify-fill");
    btnJustify->setIcon(*icnJustify);
    connect(btnJustify, SIGNAL(clicked()),
    this, SLOT(setJustify()));

    hlTextAtributeButton->addWidget(btnJustify);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

    hlTextAtributeButton->addItem(horizontalSpacer_2);

    vlTextAtribute->addLayout(hlTextAtributeButton);

    horizontalLayout_14->addLayout(vlTextAtribute);

    vlOther = new QVBoxLayout();
    vlOther->setObjectName(QString::fromUtf8("vlOther"));
    cbbHeader = new KComboBox(menuEdit);
    cbbHeader->setMaximumSize(QSize(225, 20));
    cbbHeader->setObjectName(QString::fromUtf8("cbbHeader"));
    cbbHeader->addItem("H1");
    cbbHeader->addItem("H2");
    cbbHeader->addItem("H3");
    cbbHeader->addItem("H4");
    cbbHeader->addItem("H6");
    connect(cbbHeader, SIGNAL(currentIndexChanged (QString)),
    this, SLOT(setHeader(QString)));

    vlOther->addWidget(cbbHeader);

    hlOther = new QHBoxLayout();
    hlOther->setObjectName(QString::fromUtf8("hlOther"));
    btnLink = new KPushButton(menuEdit);
    btnLink->setObjectName(QString::fromUtf8("btnLink"));
    btnLink->setIcon(KIcon("insert-link"));
    btnLink->setMinimumSize(QSize(30, 30));
    btnLink->setMaximumSize(QSize(30, 16777215));

    hlOther->addWidget(btnLink);

    btnChar = new KPushButton(menuEdit);
    btnChar->setObjectName(QString::fromUtf8("btnChar"));
    btnChar->setIcon(KIcon("draw-text"));
    btnChar->setMinimumSize(QSize(30, 30));
    btnChar->setMaximumSize(QSize(30, 16777215));

    hlOther->addWidget(btnChar);

    btnTable = new KPushButton(menuEdit);
    btnTable->setObjectName(QString::fromUtf8("btnTable"));
    btnTable->setIcon(KIcon("insert-table"));
    btnTable->setMinimumSize(QSize(30, 30));
    btnTable->setMaximumSize(QSize(30, 16777215));

    hlOther->addWidget(btnTable);

    btnList = new KPushButton(menuEdit);
    btnList->setObjectName(QString::fromUtf8("btnList"));
    btnList->setIcon(KIcon("format-list-unordered"));
    btnList->setMinimumSize(QSize(30, 30));
    btnList->setMaximumSize(QSize(30, 16777215));
    connect(btnList, SIGNAL(clicked()),
    this, SLOT(setUList()));
    
    hlOther->addWidget(btnList);

    vlOther->addLayout(hlOther);

    horizontalLayout_14->addLayout(vlOther);

    vlColor = new QGridLayout();
    vlColor->setSpacing(0);
    vlColor->setObjectName(QString::fromUtf8("vlColor"));
    
    lblTextColor = new QLabel(menuEdit);
    lblTextColor->setObjectName(QString::fromUtf8("lblTextColor"));
    KIcon* icnTextColor = new KIcon("format-text-color");
    lblTextColor->setPixmap(icnTextColor->pixmap(16,QIcon::Normal,QIcon::On));
    vlColor->addWidget(lblTextColor,0,0);

    kcbbTextColor = new KColorCombo(menuEdit);
    kcbbTextColor->setMaximumSize(QSize(40, 15));
    kcbbTextColor->setObjectName(QString::fromUtf8("kcbbTextColor"));
    connect(kcbbTextColor, SIGNAL(currentIndexChanged(int)),
    this, SLOT(setTextColor()));
    vlColor->addWidget(kcbbTextColor,0,1);

    lblHighlightColor = new QLabel(menuEdit);
    lblHighlightColor->setObjectName(QString::fromUtf8("lblHighlightColor"));
    KIcon* icnHighlightColor = new KIcon("format-stroke-color");
    lblHighlightColor->setPixmap(icnHighlightColor->pixmap(16,QIcon::Normal,QIcon::On));
    vlColor->addWidget(lblHighlightColor,1,0);

    cbbHighlightColor = new KColorCombo(menuEdit);
    cbbHighlightColor->setMaximumSize(QSize(40, 15));
    cbbHighlightColor->setObjectName(QString::fromUtf8("cbbHighlightColor"));
    connect(cbbHighlightColor, SIGNAL(currentIndexChanged(int)),
    this, SLOT(setHighlightColor()));
    vlColor->addWidget(cbbHighlightColor,1,1);

    lblBackgroundColor = new QLabel(menuEdit);
    lblBackgroundColor->setObjectName(QString::fromUtf8("lblBackgroundColor"));
    KIcon* icnBackgroundColor = new KIcon("fill-color");
    lblBackgroundColor->setPixmap(icnBackgroundColor->pixmap(16,QIcon::Normal,QIcon::On));
    vlColor->addWidget(lblBackgroundColor,2,0);

    cbbBackgroundColor = new KColorCombo(menuEdit);
    cbbBackgroundColor->setMaximumSize(QSize(40, 15));
    cbbBackgroundColor->setObjectName(QString::fromUtf8("cbbBackgroundColor"));
    connect(cbbBackgroundColor, SIGNAL(currentIndexChanged(int)),
    this, SLOT(setBackgroundColor()));
    vlColor->addWidget(cbbBackgroundColor,2,1);


    horizontalLayout_14->addLayout(vlColor);

    tabWMenu->addTab(menuEdit, "Edit");
    
    

    viewTB = new KToolBar(this);
    tabWMenu->addTab(viewTB, "File");
    viewTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

    createAction("Project", "text-x-katefilelist", Qt::CTRL + Qt::Key_W,true);
    connect(ashActions["Project"], SIGNAL(triggered(bool)), this, SLOT(showPageList(bool)));
    viewTB->addAction(ashActions["Project"]);

    createAction("CSS Class", "text-x-katefilelist", Qt::CTRL + Qt::Key_W, true);
    connect(ashActions["CSS Class"], SIGNAL(triggered(bool)), this, SLOT(showCSS(bool)));
    viewTB->addAction(ashActions["CSS Class"]);

    createAction("HTML Tree", "text-x-katefilelist", Qt::CTRL + Qt::Key_W, true);
    connect(ashActions["HTML Tree"], SIGNAL(triggered(bool)), this, SLOT(showHtml(bool)));
    viewTB->addAction(ashActions["HTML Tree"]);

    createAction("Debugger", "text-x-katefilelist", Qt::CTRL + Qt::Key_W, true);
    connect(ashActions["Debugger"], SIGNAL(triggered(bool)), this, SLOT(showDebugger(bool)));
    viewTB->addAction(ashActions["Debugger"]);
    
    createAction("Inspector", "text-x-katefilelist", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Inspector"], SIGNAL(triggered(bool)), this, SLOT(showInspector(bool)));
    viewTB->addAction(ashActions["Inspector"]);
    
    viewTB->addSeparator();
 
    createAction("Zoom In", "zoom-in", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Zoom In"], SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
    viewTB->addAction(ashActions["Zoom In"]);

    createAction("Zoom Out", "zoom-out", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Zoom Out"], SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
    viewTB->addAction(ashActions["Zoom Out"]);

    createAction("Zoom 1:1", "zoom-original", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Zoom 1:1"], SIGNAL(triggered(bool)), this, SLOT(zoomDefault()));
    viewTB->addAction(ashActions["Zoom 1:1"]);

    menuInsert = new QWidget();
    menuInsert->setStyleSheet("margin:0px;spacing:0px;padding:0px;");
    menuInsert->setObjectName(QString::fromUtf8("menuInsert"));
    tabWMenu->addTab(menuInsert, "Insert");
    hlInsert = new QGridLayout(menuInsert);
    hlInsert->setContentsMargins(0,0,0,0);
    hlInsert->setObjectName(QString::fromUtf8("hlInsert"));
    hlInsert->setSpacing(0);
    horizontalSpacer_3 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_14->addItem(horizontalSpacer_3);
    horizontalLayout_14->setContentsMargins(0,0,0,0);

    insertTB = new KToolBar(this);
    insertTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");
    hlInsert->addWidget(insertTB,0,0,0,3);

    createAction("New Page", "document-new", Qt::CTRL + Qt::Key_W);
    connect(ashActions["New Page"], SIGNAL(triggered(bool)), this, SLOT(addHtmlPage()));
    insertTB->addAction(ashActions["New Page"]);

    createAction("New Script", "application-add", Qt::CTRL + Qt::Key_W);
    connect(ashActions["New Script"], SIGNAL(triggered(bool)), this, SLOT(addScript()));
    insertTB->addAction(ashActions["New Script"]);

    insertTB->addSeparator();

    createAction("Add Image", "insert-image", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Add Image"], SIGNAL(triggered(bool)), this, SLOT(insertImage()));
    insertTB->addAction(ashActions["Add Image"]);

    createAction("Add Table", "insert-table", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Add Table"], SIGNAL(triggered(bool)), this, SLOT(insertTable()));
    insertTB->addAction(ashActions["Add Table"]);

    createAction("Add Link", "insert-link", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Add Link"], SIGNAL(triggered(bool)), this, SLOT(insertLink()));
    insertTB->addAction(ashActions["Add Link"]);

    createAction("Add Text", "insert-text", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Add Text"], SIGNAL(triggered(bool)), this, SLOT(quit()));
    insertTB->addAction(ashActions["Add Text"]);
    ashActions["Add Text"]->setDisabled(true);

    insertTB->addSeparator();

    createAction("Special Character", "list-add-font", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Special Character"], SIGNAL(triggered(bool)), this, SLOT(insertChar()));
    insertTB->addAction(ashActions["Special Character"]);
    //ashActions["Special Character"]->setDisabled(true);

    createAction("Get Color Code", "fill-color", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Get Color Code"], SIGNAL(triggered(bool)), this, SLOT(quit()));
    insertTB->addAction(ashActions["Get Color Code"]);
    ashActions["Get Color Code"]->setDisabled(true);

    QSpacerItem* horizontalSpacer8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlInsert->addItem(horizontalSpacer8,0,1);

    btnNewLine = new KPushButton(menuInsert);
    btnNewLine->setObjectName(QString::fromUtf8("btnNewLine"));
    btnNewLine->setText("New Line");
    //sizePolicy1.setHeightForWidth(btnChar->sizePolicy().hasHeightForWidth());
    //btnNewLine->setSizePolicy(sizePolicy1);
    btnNewLine->setMinimumSize(QSize(100, 16));
    btnNewLine->setMaximumSize(QSize(100, 16));

    hlInsert->addWidget(btnNewLine,2,2);

    btnNewTab = new KPushButton(menuInsert);
    btnNewTab->setObjectName(QString::fromUtf8("btnNewTab"));
    btnNewTab->setText("New Tab");
    //sizePolicy1.setHeightForWidth(btnChar->sizePolicy().hasHeightForWidth());
    //btnNewTab->setSizePolicy(sizePolicy1);
    btnNewTab->setMinimumSize(QSize(100, 16));
    btnNewTab->setMaximumSize(QSize(100, 16));

    hlInsert->addWidget(btnNewTab,1,2);

    btnNewSpace = new KPushButton(menuInsert);
    btnNewSpace->setObjectName(QString::fromUtf8("btnNewSpace"));
    btnNewSpace->setText("New Space");
    //sizePolicy1.setHeightForWidth(btnChar->sizePolicy().hasHeightForWidth());
    //btnNewSpace->setSizePolicy(sizePolicy1);
    btnNewSpace->setMinimumSize(QSize(100, 16));
    btnNewSpace->setMaximumSize(QSize(100, 16));
    hlInsert->addWidget(btnNewSpace,0,2);
    //hlInsert->addLayout(vlSpacing);

    
    toolsTB = new KToolBar(this);
    tabWMenu->addTab(toolsTB, "Tools");
    toolsTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

    createAction("Parse", "format-indent-more", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Parse"], SIGNAL(triggered(bool)), this, SLOT(reParse()));
    toolsTB->addAction(ashActions["Parse"]);

    createAction("Templaterize", "view-pim-tasks", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Templaterize"], SIGNAL(triggered(bool)), this, SLOT(templaterize()));
    toolsTB->addAction(ashActions["Templaterize"]);

    createAction("Translate", "application-x-marble", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Translate"], SIGNAL(triggered(bool)), this, SLOT(translate()));
    toolsTB->addAction(ashActions["Translate"]);

    createAction("Debug", "tools-report-bug", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Debug"], SIGNAL(triggered(bool)), this, SLOT(debugHtml()));
    toolsTB->addAction(ashActions["Debug"]);

    toolsTB->addSeparator();

    createAction("Match Tag", "application-xml", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Match Tag"], SIGNAL(triggered(bool)), this, SLOT(quit()));
    toolsTB->addAction(ashActions["Match Tag"]);
    ashActions["Match Tag"]->setDisabled(true);

    createAction("Spelling", "tools-check-spelling", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Spelling"], SIGNAL(triggered(bool)), this, SLOT(quit()));
    toolsTB->addAction(ashActions["Spelling"]);
    ashActions["Spelling"]->setDisabled(true);

    createAction("Check Link", "network-connect", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Check Link"], SIGNAL(triggered(bool)), this, SLOT(quit()));
    toolsTB->addAction(ashActions["Check Link"]);
    ashActions["Check Link"]->setDisabled(true);


    

    optionsTB = new KToolBar(this);
    tabWMenu->addTab(optionsTB, "Settings");
    optionsTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

    createAction("Configure WebKreator", "configure", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Configure WebKreator"], SIGNAL(triggered(bool)), this, SLOT(quit()));
    optionsTB->addAction(ashActions["Configure WebKreator"]);
    ashActions["Configure WebKreator"]->setDisabled(true);

    createAction("Configure ToolBars", "configure-toolbars", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Configure ToolBars"], SIGNAL(triggered(bool)), this, SLOT(editToolbar()));
    optionsTB->addAction(ashActions["Configure ToolBars"]);

    createAction("Configure Shortcuts", "configure-shortcuts", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Configure Shortcuts"], SIGNAL(triggered(bool)), this, SLOT(editShortcut()));
    optionsTB->addAction(ashActions["Configure Shortcuts"]);

    

    helpTB = new KToolBar(this);
    tabWMenu->addTab(helpTB, "Help");
    helpTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

    createAction("Manual", "help-contents", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Manual"], SIGNAL(triggered(bool)), this, SLOT(quit()));
    helpTB->addAction(ashActions["Manual"]);
    ashActions["Manual"]->setDisabled(true);

    createAction("About", "help-about", Qt::CTRL + Qt::Key_W);
    connect(ashActions["About"], SIGNAL(triggered(bool)), this, SLOT(aboutKimberlite()));
    helpTB->addAction(ashActions["About"]);

    helpTB->addSeparator();

    createAction("Report A Bug", "tools-report-bug", Qt::CTRL + Qt::Key_W);
    connect(ashActions["Report A Bug"], SIGNAL(triggered(bool)), this, SLOT(reportBug()));
    helpTB->addAction(ashActions["Report A Bug"]);

    setMenuWidget(tabWMenu); //BUG

    dockHtmlTree = new QDockWidget(this);
    dockHtmlTree->setWindowTitle("Tag tree");
    treeHtml = new QTreeWidget(this);
    treeHtml->setHeaderHidden(true);
    dockHtmlTree->setWidget(treeHtml);
    addDockWidget(Qt::LeftDockWidgetArea, dockHtmlTree);
    connect(treeHtml, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
    this, SLOT(setHtmlCursor(QTreeWidgetItem*, int)));
    dockHtmlTree->setVisible(false);
 
    tableDock = new QDockWidget(this);
    tableDock->setWindowTitle("Project");
    tableDock->setObjectName(QString::fromUtf8("tableDock"));

    aProjectManager = new ProjectManager2(0);
    
    
    tableDockCentral = new QWidget();
    tableDockCentral->setObjectName(QString::fromUtf8("tableDockCentral"));
    verticalLayout_4 = new QVBoxLayout(tableDockCentral);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

    addDockWidget(Qt::LeftDockWidgetArea, tableDock);
    tableDock->setVisible(false);
    hlButton = new QHBoxLayout();
    hlButton->setObjectName(QString::fromUtf8("hlButton"));
    btnTableAdd = new KPushButton(tableDockCentral);
    btnTableAdd->setObjectName(QString::fromUtf8("btnTableAdd"));

    hlButton->addWidget(btnTableAdd);

    btnTableRemove = new KPushButton(tableDockCentral);
    btnTableRemove->setObjectName(QString::fromUtf8("btnTableRemove"));

    hlButton->addWidget(btnTableRemove);


    verticalLayout_4->addLayout(hlButton);

    //tableDock->setWidget(tableDockCentral);

    //mainLayout->addWidget(tableDock);

    treeDock = new QDockWidget(this);
    treeDock->setWindowTitle("CSS file");
    treeDock->setObjectName(QString::fromUtf8("treeDock"));
    treeDockCentral = new QWidget();
    treeDockCentral->setObjectName(QString::fromUtf8("treeDockCentral"));

    verticalLayout_5 = new QVBoxLayout(treeDockCentral);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    verticalLayout_5->setContentsMargins(0,0,0,0);
    treeWidget = new QTreeWidget(treeDockCentral);
    treeWidget->setHeaderHidden(true);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
    treeWidget->setIconSize(QSize(32,32));
    addDockWidget(Qt::LeftDockWidgetArea,treeDock );
    treeDock->setVisible(false);
    

    verticalLayout_5->addWidget(treeWidget);

    hlButton2 = new QHBoxLayout();
    hlButton2->setObjectName(QString::fromUtf8("hlButton2"));
    btnTreeAdd = new KPushButton(treeDockCentral);
    btnTreeAdd->setObjectName(QString::fromUtf8("btnTreeAdd"));
    connect(btnTreeAdd, SIGNAL(clicked()), this, SLOT(addClasses()));

    hlButton2->addWidget(btnTreeAdd);

    btnTreeRemove = new KPushButton(treeDockCentral);
    btnTreeRemove->setObjectName(QString::fromUtf8("btnTreeRemove"));

    hlButton2->addWidget(btnTreeRemove);


    verticalLayout_5->addLayout(hlButton2);

    treeDock->setWidget(treeDockCentral);
    tabWEditor = new QTabWidget(this);
    setCentralWidget(tabWEditor);
    tabWEditor->setObjectName(QString::fromUtf8("tabWEditor"));
    tabWEditor->setTabPosition(QTabWidget::South);
    tabPreview = new QWidget();
    tabPreview->setObjectName(QString::fromUtf8("tabPreview"));
    horizontalLayout_2 = new QHBoxLayout(tabPreview);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    webPreview = new QWebView(tabPreview);
    webPreview->setObjectName(QString::fromUtf8("webPreview"));
    //webPreview->setUrl(QUrl("about:blank"));
    loadDefaultPage();
    webPreview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(webPreview,SIGNAL(linkClicked(QUrl)), this, SLOT(defaultPageLinkClicked(QUrl)));
    connect(webPreview->page(),SIGNAL(loadProgress(int)), this, SLOT(loading(int)));
    connect(webPreview->page(),SIGNAL(linkHovered(QString,QString,QString)), this, SLOT(linkHovered(QString)));

    horizontalLayout_2->addWidget(webPreview);

    tabWEditor->addTab(tabPreview, QString());
    tabHTML = new QWidget();
    tabHTML->setObjectName(QString::fromUtf8("tabHTML"));
    verticalLayout_3 = new QVBoxLayout(tabHTML);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    rtfHTMLEditor = new RtfHtmlEditor(tabHTML);
    rtfHTMLEditor->setObjectName(QString::fromUtf8("rtfHTMLEditor"));
    rtfHTMLEditor->setAcceptRichText(false);
    new HtmlSyntaxHighlighter(rtfHTMLEditor);
    rtfHTMLEditor->setLineWrapMode(QTextEdit::NoWrap);
    rtfHTMLEditor->setWordWrapMode(QTextOption::NoWrap);
    rtfHTMLEditor->setWordWrapMode(QTextOption::NoWrap);
    connect(rtfHTMLEditor, SIGNAL(textChanged()), this, SLOT(setModified()));
    connect(rtfHTMLEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));

    verticalLayout_3->addWidget(rtfHTMLEditor);

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
    verticalLayout_99 = new QVBoxLayout(dockDebugContents);
    verticalLayout_99->setObjectName(QString::fromUtf8("verticalLayout_99"));
    lstDebug = new QListWidget(dockDebugContents);
    verticalLayout_99->addWidget(lstDebug);

    verticalLayout_3->addWidget(dockDebug);

    tabWEditor->addTab(tabHTML, QString());
    tabScripts = new QWidget();
    tabScripts->setObjectName(QString::fromUtf8("tabScripts"));
    verticalLayout_7 = new QVBoxLayout(tabScripts);
    verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
    rtfScriptEditor = new KTextEdit(tabScripts);
    rtfScriptEditor->setObjectName(QString::fromUtf8("rtfScriptEditor"));
    connect(rtfScriptEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));

    verticalLayout_7->addWidget(rtfScriptEditor);

    tabWEditor->addTab(tabScripts, QString());
    tabCSS = new QWidget();
    tabCSS->setObjectName(QString::fromUtf8("tabCSS"));
    verticalLayout_6 = new QVBoxLayout(tabCSS);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    tabWCSSLevel = new QTabWidget(tabCSS);
    connect(tabWCSSLevel, SIGNAL(currentChanged(int)), this, SLOT(changeCssMode(int)));
    tabWCSSLevel->setObjectName(QString::fromUtf8("tabWCSSLevel"));
    /*tabWCSSLevel->setStyleSheet(QString::fromUtf8("QTabWidget::tab-bar {\n"
"	border-radius: 5px;\n"
"    alignment: center;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"	background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                               stop:0 darkgray, stop:1 lightgray);\n"
"	border-radius: 5px;\n"
"	margin-top:5px;\n"
"	margin-bottom:5px;\n"
"}\n"
"\n"
"QTabBar {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                         stop: 0 #111111, stop: 0.4 #444444,\n"
"                                         stop: 0.5 #444444, stop: 1.0 #111111);\n"
"border-radius: 5px;\n"
"padding-left:15px;\n"
"padding-right:15px;\n"
"margin-left:10px;\n"
"margin-right:10px;\n"
"border: 2px solid grey;\n"
"width:100%;\n"
"}\n"
"\n"
"QTabBar:tab {\n"
"border-top:0px;\n"
"border-bottom:0px;\n"
"margin-left:20px;\n"
"margin-right:20px;\n"
"padding-left:10px;\n"
"padding-right:10px;\n"
"}\n"
"\n"
"QTabWidget::pane {\n"
"	border:0px;\n"
"	spacing:0px;\n"
"	margin:0px;\n"
"	padding:0px\n"
"}"));*/
    tabWCSSLevel->setTabShape(QTabWidget::Triangular);
    
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
    //scrollAreaWidgetContents->setGeometry(QRect(0, 0, 567, 508));
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
    //aQVBoxLayout->setContentsMargins(0,0,0,0);
    
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

    tabWCSSLevel->addTab(tabBeginner, QString());
    
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
    //QTableWidgetItem *__tableItem = new QTableWidgetItem();
    //tblCSSPage->setItem(0, 0, __tableItem);
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
    tabexpert = new QWidget();
    tabexpert->setObjectName(QString::fromUtf8("tabexpert"));
    verticalLayout_17 = new QVBoxLayout(tabexpert);
    verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
    rtfCSSEditor = new RtfCssEditor(tabexpert);
    rtfCSSEditor->setObjectName(QString::fromUtf8("rtfCSSEditor"));
    new CssSyntaxHighlighter(rtfCSSEditor);
    connect(rtfCSSEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
    
    QStringList wordList;
    QSqlQuery query22;
    query22.exec("SELECT TITLE FROM TCSS_TAG");
    
    while (query22.next()) {
      wordList <<  query22.value(0).toString();
    }

    cssCompleter = new QCompleter(wordList, tabAdvanced);
    cssCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    //cssCompleter->setWidget(rtfCSSEditor);
    rtfCSSEditor->setCompleter(cssCompleter);

    verticalLayout_17->addWidget(rtfCSSEditor);

    tabWCSSLevel->addTab(tabexpert, QString());

    verticalLayout_6->addWidget(tabWCSSLevel);

    tabWEditor->addTab(tabCSS, QString());
    /*tabValidator = new QWidget();
    tabValidator->setObjectName(QString::fromUtf8("tabValidator"));
    verticalLayout_16 = new QVBoxLayout(tabValidator);
    verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
    webValidator = new QWebView(tabValidator);
    webValidator->setObjectName(QString::fromUtf8("webValidator"));
    webValidator->setUrl(QUrl("about:blank"));

    verticalLayout_16->addWidget(webValidator);*/
    
    tableDock->setWidget(aProjectManager);
    connect(aProjectManager, SIGNAL(htmlPageChanged(QTreeWidgetItem*, QString)), this, SLOT(loadPage(QTreeWidgetItem*, QString)));
    connect(aProjectManager, SIGNAL(javaScriptChanged(QTreeWidgetItem*, QString)), this, SLOT(loadScript(QTreeWidgetItem*, QString)));
    connect(aProjectManager, SIGNAL(loadCss(QString)), this, SLOT(loadCss(QString)));
    connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem* , int)), this, SLOT(cssClassClicked(QTreeWidgetItem*)));
    connect(tabWEditor, SIGNAL(currentChanged(int)), this, SLOT(modeChanged(int)));

    //tabWEditor->addTab(tabValidator, QString());
    
    setCentralWidget(tabWEditor);

    statusbar = new KStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    //statusbar->setGeometry(QRect(0, 696, 1008, 21));
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
    
    //statusBar()->insertItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum),2);
    
    //this->setStandardToolBarMenuEnabled(false);
    //this->toolBarArea()->setHidden(true);
//toolBars()[0]->setHidden(true);
    tabWEditor->setTabEnabled(1,false);
    tabWEditor->setTabEnabled(2,false);
    tabWEditor->setTabEnabled(3,false);
    tabWMenu->setTabEnabled(1,false);
    tabWMenu->setTabEnabled(3,false);
    tabWMenu->setTabEnabled(2,false);
    tabWMenu->setTabEnabled(4,false);
    tabWMenu->setTabEnabled(5,false);
    tabWMenu->setTabEnabled(6,false);
    ashActions["Save"]->setDisabled(true);
    ashActions["Save As"]->setDisabled(true);
    ashActions["Print"]->setDisabled(true);
    ashActions["Print Preview"]->setDisabled(true);
    rtfHTMLEditor->clear();
    
    /*aHtmlThread = new ParserThread(this);
    aHtmlThread->rtfHtml = rtfHTMLEditor;
    aHtmlThread->treeHtml = treeHtml;
    connect(rtfHTMLEditor, SIGNAL(textChanged()), aHtmlThread, SLOT(getReady()));
    aHtmlThread->start();*/
    
    retranslateUi();
}

void MainWindow::retranslateUi() {
    setWindowTitle(QApplication::translate("this", "Kimberlite", 0, QApplication::UnicodeUTF8));
    tabWMenu->setTabText(tabWMenu->indexOf(menufile), QApplication::translate("this", "File", 0, QApplication::UnicodeUTF8));
    //lblTextColor->setText(QApplication::translate("this", "Text:", 0, QApplication::UnicodeUTF8));
    //lblHighlightColor->setText(QApplication::translate("this", "Highlight:", 0, QApplication::UnicodeUTF8));
    //lblBackgroundColor->setText(QApplication::translate("this", "Background:", 0, QApplication::UnicodeUTF8));
    btnTableAdd->setText(QApplication::translate("this", "Add", 0, QApplication::UnicodeUTF8));
    btnTableRemove->setText(QApplication::translate("this", "Remove", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(0, QApplication::translate("this", "1", 0, QApplication::UnicodeUTF8));
    btnTreeAdd->setText(QApplication::translate("this", "Add", 0, QApplication::UnicodeUTF8));
    btnTreeRemove->setText(QApplication::translate("this", "Remove", 0, QApplication::UnicodeUTF8));
    tabWEditor->setTabText(tabWEditor->indexOf(tabPreview), QApplication::translate("this", "WYSIWYG", 0, QApplication::UnicodeUTF8));
    //btnParse->setText(QApplication::translate("this", "PushButton", 0, QApplication::UnicodeUTF8));
    tabWEditor->setTabText(tabWEditor->indexOf(tabHTML), QApplication::translate("this", "HTML", 0, QApplication::UnicodeUTF8));
    tabWEditor->setTabText(tabWEditor->indexOf(tabScripts), QApplication::translate("this", "Scripts", 0, QApplication::UnicodeUTF8));
    grbSize->setTitle(QApplication::translate("this", "Size", 0, QApplication::UnicodeUTF8));
    grbText->setTitle(QApplication::translate("this", "Text", 0, QApplication::UnicodeUTF8));
    grbBackground->setTitle(QApplication::translate("this", "Background", 0, QApplication::UnicodeUTF8));
    grbBorder->setTitle(QApplication::translate("this", "Border", 0, QApplication::UnicodeUTF8));
    grbLayout->setTitle(QApplication::translate("this", "Layout", 0, QApplication::UnicodeUTF8));
    grbOther->setTitle(QApplication::translate("this", "Other", 0, QApplication::UnicodeUTF8));
    tabWCSSLevel->setTabText(tabWCSSLevel->indexOf(tabBeginner), QApplication::translate("this", "Beginner", 0, QApplication::UnicodeUTF8));
    /*tblCSSPage->horizontalHeaderItem(0)->setText(QApplication::translate("this", "name", 0, QApplication::UnicodeUTF8));
    tblCSSPage->horizontalHeaderItem(1)->setText(QApplication::translate("this", "content", 0, QApplication::UnicodeUTF8));
    tblCSSPage->horizontalHeaderItem(2)->setText(QApplication::translate("this", "unit", 0, QApplication::UnicodeUTF8));
    tblCSSPage->horizontalHeaderItem(3)->setText(QApplication::translate("this", "comment", 0, QApplication::UnicodeUTF8));
    tblCSSPage->horizontalHeaderItem(4)->setText(QApplication::translate("this", "remove", 0, QApplication::UnicodeUTF8));
    tblCSSPage->verticalHeaderItem(0)->setText(QApplication::translate("this", "New Row", 0, QApplication::UnicodeUTF8));*/

    //const bool __sortingEnabled = tblCSSPage->isSortingEnabled();
    tblCSSPage->setSortingEnabled(false);
    //tblCSSPage->item(0, 0)->setText(QApplication::translate("this", "fdtgdfgdgdfgdgdfgdfgdfgdfgdgg", 0, QApplication::UnicodeUTF8));

    //tblCSSPage->setSortingEnabled(__sortingEnabled);
    btnaddClass->setText(QApplication::translate("this", "Add class", 0, QApplication::UnicodeUTF8));
    btnAddPClass->setText(QApplication::translate("this", "Add pseudo class", 0, QApplication::UnicodeUTF8));
    btnRemoveClass->setText(QApplication::translate("this", "Remove class", 0, QApplication::UnicodeUTF8));
    tabWCSSLevel->setTabText(tabWCSSLevel->indexOf(tabAdvanced), QApplication::translate("this", "Advanced", 0, QApplication::UnicodeUTF8));
    tabWCSSLevel->setTabEnabled(1,false);
    tabWCSSLevel->setTabText(tabWCSSLevel->indexOf(tabexpert), QApplication::translate("this", "Expert", 0, QApplication::UnicodeUTF8));
    tabWEditor->setTabText(tabWEditor->indexOf(tabCSS), QApplication::translate("this", "CSS", 0, QApplication::UnicodeUTF8));
    //tabWEditor->setTabText(tabWEditor->indexOf(tabValidator), QApplication::translate("this", "Validator", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(this);
} // retranslateUi

// QString MainWindow::readCSSFile(QString path) {
//     QString inputFileName = path;
//     QString tmpFile;
//     //path = "/home/lepagee/dev/webkreator/StyleSheet.css";
//     //if(KIO::NetAccess::download(inputFileName, tmpFile, this)) {
//       QString line;
//       bool inComment = false;
//       QFile file(path);
//       file.open(QIODevice::ReadOnly);
//       while (!file.atEnd()) {
// 	line = QString(file.readLine()).toAscii();
// 	printf("%s",line.toStdString().c_str());
// 	if (line.indexOf("/*") != -1) {
// 	  inComment = true;
// 	}
// 	if (line.indexOf("*/") != -1) {
// 	  inComment = false;
// 	}
// 
// 	if (inComment == false) {
// 	  //printf("\n This is what you are looking for: %d,%d",line.right(2).left(1).toStdString().c_str(),line.right(1).toStdString().c_str());
// 	  //line = line.remove(line.count() -2,2); //Remove UNICODE linefeed
// 	}
// 
// 	CssParser::cssFile += line;
//       }
// 
//       KIO::NetAccess::removeTempFile(tmpFile);
//     /*}
//     else
//     {
//       KMessageBox::error(this,
//       KIO::NetAccess::lastErrorString());
//     }*/
// 
// 
//     while (CssParser::cssFile.indexOf("\n") != -1) {
//       CssParser::cssFile.remove(CssParser::cssFile.indexOf("\n"),1);
//     }
//   printf("%s",CssParser::cssFile.toStdString().c_str());
//   return CssParser::cssFile;
// }

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
}
MainWindow::~MainWindow() {
  QDir aDir;
  aDir.rmdir(QDir::tempPath()+"/kimberlite");
}

void MainWindow::reParse() {
  HtmlData data = HtmlParser::getHtmlData(rtfHTMLEditor->toPlainText());
  updateHtmlTree(data);
  rtfHTMLEditor->setPlainText(aParser->getParsedHtml(data));
}

void MainWindow::templaterize() {
  StringConverter* aStringConverter = new StringConverter(this);
  aStringConverter->toTemplate(rtfHTMLEditor->toPlainText());
}

void MainWindow::translate() {
  StringConverter* aStringConverter = new StringConverter(this);
  aStringConverter->translate(rtfHTMLEditor->toPlainText());
}

void MainWindow::newProject(){
  NewProject* aNewProject = new NewProject(this);
  aNewProject->show();
  /*openProject("/home/lepagee/dev/myproject/kimberlite/template/default.wkp");
  fileName.clear();*/
  connect(aNewProject,SIGNAL(createProject(QString, QString)),this,SLOT(newProject(QString,QString)));
}

void MainWindow::newProject(QString name, QString filePath) {
  openProject(filePath);
  fileName.clear();
  aProjectManager->setProjectName(name);
}
 
void MainWindow::saveProjectAs(const QString &outputFileName){
  changeCssMode(tabWCSSLevel->currentIndex());
  aProjectManager->saveCss();
  KSaveFile file(outputFileName);
  file.open();
  QByteArray outputByteArray;
  outputByteArray.append(aProjectManager->getDomDocument()->toString());
  file.write(outputByteArray);
  file.finalize();
  file.close();

  fileName = outputFileName;
  saveRecentProject(fileName);
}
 
void MainWindow::saveProjectAs(){
  saveProjectAs(KFileDialog::getSaveFileName());
}
 
void MainWindow::saveProject(){
  if(!fileName.isEmpty()) 
    saveProjectAs(fileName);
  else 
    saveProjectAs();
}

void MainWindow::saveFile(){ 
  if(!pageName.isEmpty()) 
    saveProjectAs(pageName);
  else
    saveProjectAs();
}


void MainWindow::openProject() {
  QString fileName = KFileDialog::getOpenFileName();
  openProject(fileName);
}
 
void MainWindow::openProject(QString fileName) {
  qDebug() << "Load: " << fileName;
  if (!fileName.isEmpty()) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) 
      QMessageBox::warning(this, tr("SAX Bookmarks"),tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
    else 
      aProjectManager->read(&file);
    aProjectManager->expandAll();
    setWindowTitle("Kimberlite - "+aProjectManager->projectTitle);
    //tabPreview->setDisabled(true);
    #if QT_VERSION >= 0x040500
    webPreview->page()->setContentEditable(true);
    #endif
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
}

void MainWindow::showPageList(bool state) {
  if (state == true ) 
    tableDock->setHidden(false);
  else 
    tableDock->setHidden(true);
}
void MainWindow::showCSS(bool state) {
  if (state == true ) 
    treeDock->setHidden(false);
  else
    treeDock->setHidden(true);
}

void MainWindow::showDebugger(bool state) {
  if (state == true )
    dockDebug->setHidden(false);
  else
    dockDebug->setHidden(true);
}
void MainWindow::showInspector(bool state) {
  QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
  webPreview->triggerPageAction(QWebPage::InspectElement);
}

void MainWindow::showHtml(bool state) {
  if (state == true )
    dockHtmlTree->setHidden(false);
  else 
    dockHtmlTree->setHidden(true);
}

void MainWindow::zoomIn() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      webPreview->setZoomFactor(webPreview->zoomFactor()+1);
      break;
    case 1:
      rtfHTMLEditor->zoomIn(1);
  }
}

void MainWindow::zoomOut() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      webPreview->setZoomFactor(webPreview->zoomFactor()-1);
      break;
    case 1:
      rtfHTMLEditor->zoomOut(1);
  }
}

void MainWindow::zoomDefault() {
  webPreview->setZoomFactor(1);
}

void MainWindow::setupToolTip() {

   KTipDatabase* aTipDB = new KTipDatabase();

   KTipDialog* aTipDialog = new KTipDialog(aTipDB, this);
   aTipDialog->setShowOnStart(true);
   aTipDialog->showTip(this, QString(), true);

}

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
}

QTreeWidgetItem* MainWindow::getClassWidget(QString className) {
  QTreeWidgetItem* toReturn;
  QTreeWidgetItem* currentTop(styleSheetName);
  QString partialName;
  while (currentTop->childCount() > 0) {
    for (int i =0; i < currentTop->childCount();i++) {
      QString toAdd;
      if ((currentTop->child(i)->text(0)[0] != ':') && (partialName.count()))
	    toAdd += " ";
      QString tmpStr = partialName + toAdd + (currentTop->child(i)->text(0));
      if (tmpStr == className.left(tmpStr.count()))
	if (getClassName(currentTop->child(i)) == className)
	  return currentTop->child(i);
	else {
	  if ((currentTop->child(i)->text(0)[0] != ':') && (partialName.count()))
	    partialName += " ";
	  partialName += currentTop->child(i)->text(0);
	  currentTop = currentTop->child(i);
	  i = currentTop->childCount();
	}
      if (i == currentTop->childCount() -1)
	return NULL;
    }
  }
  return NULL;
}

void MainWindow::cssClassClicked(QTreeWidgetItem* anItem) {
  if (tabWCSSLevel->currentIndex() == 0)
    loadCSSClass(anItem);
  else if (tabWCSSLevel->currentIndex() == 2) {
    QString className = getClassName(anItem);
    setCssCursor(className);
  }
}

void MainWindow::loadCSSClass(QTreeWidgetItem* anItem) {
  QString newStyle = CssParser::setClass(currentClassName, clearCssBeg());
  QString className = getClassName(anItem);
  currentClassName = className;
  fillCSSBegMode(currentClassName);
  qDebug() << newStyle;
  CssParser::cssFile = newStyle;
  //saveProjectAs(aProject->cssPage , newStyle);
  rtfCSSEditor->setText(CssParser::parseCSS());
}

void MainWindow::setCssCursor(QString className) {
  uint counter =0;
  QStringList lineList = rtfCSSEditor->toPlainText().split('\n');
  for (int i=0;i < lineList.count();i++) {
    if (lineList[i].indexOf('{') != -1) {
      if (lineList[i]== className + " {") {
	QTextCursor tc = rtfCSSEditor->textCursor();
	tc.setPosition(counter);
	rtfCSSEditor->setTextCursor(tc);
	qDebug() << "Cursor position:" << counter;
	rtfCSSEditor->setFocus();
	return;
      }
    }
    counter += lineList[i].count() +1;
  }
}

void MainWindow::loadPage(QTreeWidgetItem* item, QString text) {
  if (item != currentHTMLPage) {
    isModified = false;
    if (currentHTMLPage != NULL) {
      switch (tabWEditor->currentIndex()) {
	case 0:
	  aProjectManager->updateDomElement(currentHTMLPage,webPreview->page()->mainFrame()->toHtml());
	  break;
	case 1:
	  aProjectManager->updateDomElement(currentHTMLPage,rtfHTMLEditor->toPlainText());
	  break;
      }
    }
    currentHTMLPage = item;
    HtmlData data = HtmlParser::getHtmlData(text);
    switch (tabWEditor->currentIndex()) {
      case 0:
	webPreview->setHtml(HtmlParser::getParsedHtml(data));
	break;
      case 1:
	rtfHTMLEditor->setPlainText(HtmlParser::getParsedHtml(data));
	break;
      default:
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
}

void MainWindow::setModified() {
  isModified = true;
}

void MainWindow::addHtmlPage() {
  NewWebPage* aDialog = new NewWebPage(this,aProjectManager->htmlPage);
  aDialog->show();
  connect(aDialog, SIGNAL(addFolder(QString,QTreeWidgetItem*)), aProjectManager, SLOT(addFolder(QString,QTreeWidgetItem*)));
  connect(aDialog, SIGNAL(addHtmlPage(QString,QString,QString,QString)), aProjectManager, SLOT(addHtmlPage(QString,QString,QString,QString)));
}

void MainWindow::addScript() {
  NewScript* aScript = new NewScript(this,aProjectManager->script);
  aScript->show();
}

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
}

void MainWindow::disableWidget(bool value) {
    tabWEditor->setEnabled(value);
    editTB->setEnabled(value);
    viewTB->setEnabled(value);
    insertTB->setEnabled(value);
    toolsTB->setEnabled(value);
    optionsTB->setEnabled(value);
    helpTB->setEnabled(value);
}

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
  else if (mode == 1) {
    
  }
  else if (mode == 2) {
    //setCssCursor(currentClassName);
  }
  previousCssMode = mode;
}

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
}

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
  /*else if (":") {
    
  }
  else if (",") {
    
  }*/
}

KIcon MainWindow::getRightIcon(QString text) {
  KIcon* anIcon = 0;
  if (text[0] == ' ') {
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/tag.png"));
  }
  else if (text[0] == ':') {
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/state.png"));
  }
  else if (text[0] == '#') {
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/id.png"));
  }
  else if (text[0] == '.') {
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/class.png"));
  }
  else {
    anIcon = new KIcon(KStandardDirs::locate("appdata", "pixmap/tag.png"));
  }
  //anIcon->setPixmap(anIcon->pixmap(QSize(16,32)));
  return *anIcon;
}

void MainWindow::loadScript(QTreeWidgetItem* anItem, QString text) {
  if (anItem != currentScript) {
    if (currentScript != NULL) {
      aProjectManager->updateDomElement(currentHTMLPage,rtfHTMLEditor->toPlainText());
      isModified = false;
    }
    rtfScriptEditor->setPlainText(text.trimmed());
    currentScript = anItem;
  }
}

void MainWindow::loadCss(QString text) {
  CssParser::cssFile = text;
  rtfCSSEditor->setPlainText(CssParser::parseCSS());
  styleSheetName = new  QTreeWidgetItem(treeWidget);
  styleSheetName->setText(0,"Style");
  updateClassTree();
  ////fillCSSAdvMode();
}

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
      #if QT_VERSION >= 0x040500
      //webPreview->page()->setContentEditable(true);
      #endif
      disableWysiwyg(false);
      ashActions["Zoom 1:1"]->setEnabled(true);
      treeDock->setVisible(false);
      dockHtmlTree->setVisible(false);
      pbStatusBar->setEnabled(true);
      break;
    }
    case 1:
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
}

void MainWindow::updateHtmlTree(QString &file) {
  HtmlData pageData = aParser->getHtmlData(file);
  updateHtmlTree(pageData);
}

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
}

void MainWindow::setHtmlCursor(QTreeWidgetItem* item, int column) {
  QTextCursor tc = rtfHTMLEditor->textCursor();
  tc.setPosition(((IndexedTreeWidgetItem*) item)->index);
  rtfHTMLEditor->setTextCursor(tc);
  rtfHTMLEditor->setFocus();
}

void MainWindow::debugHtml() {
  showDebugger(true);
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
}

void MainWindow::setBold() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      #if QT_VERSION >= 0x040500
      webPreview->page()->triggerAction(QWebPage::ToggleBold,true);
      #endif
      break;
    case 1:
      addTag("<b>","</b>");
      break;
  }
}

void MainWindow::setItalic() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      #if QT_VERSION >= 0x040500
      webPreview->page()->triggerAction(QWebPage::ToggleItalic,true);
      #endif
      break;
    case 1:
      addTag("<i>","</i>");
      break;
  }
}

void MainWindow::setUnderline() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      #if QT_VERSION >= 0x040500
      webPreview->page()->triggerAction(QWebPage::ToggleUnderline,true);
      #endif
      break;
    case 1:
      addTag("<u>","</u>");
      break;
  }
}

void MainWindow::execCommand(const QString &cmd, const QString &arg = "") {
  QWebFrame *frame = webPreview->page()->mainFrame();
  QString js;
  if (arg != "")
    js = QString("document.execCommand(\"%1\", false, \"%2\")").arg(cmd).arg(arg);
  else
    js = QString("document.execCommand(\"%1\", false, null)").arg(cmd);
  frame->evaluateJavaScript(js);
}

void MainWindow::addTag(QString prefix, QString suffix) {
  QTextCursor tc = rtfHTMLEditor->textCursor();
  QString text = tc.selectedText();
  tc.removeSelectedText();
  tc.insertText(prefix + text + suffix);
}

void MainWindow::addTag(QString prefix, QString suffix, QString cmd, QString arg = "") {
  switch (tabWEditor->currentIndex()) {
    case 0:
      #if QT_VERSION >= 0x040500
      execCommand(cmd,arg);
      #endif
      break;
    case 1:
      addTag(prefix,suffix);
      break;
  }
}

void MainWindow::setAlignCenter() {
  addTag("<center>","</center>","justifyCenter");
}

void MainWindow::setAlignLeft() {
  addTag("<font style=\"text-align:left\">","</font>","justifyLeft");
}

void MainWindow::setAlignRight() {
  addTag("<font style=\"text-align:right\">","</font>","justifyRight");
}

void MainWindow::setJustify() {
  addTag("<font style=\"text-align:justify\">","</font>","justifyFull");
}

void MainWindow::setHeader(QString text) {
  addTag("<"+text+">","</"+text+">","formatBlock",text.toLower());
}

void MainWindow::setFont(QString text) {
  addTag("<font face=\""+text+"\">","</font>","fontName", text.toLower());
}

void MainWindow::setFontSize(int size) {
  QString number = QString::number(size);
  addTag("<font size=\""+number+"\">","</font>","fontSize",number);
}

void MainWindow::setTextColor() {
  QString color = kcbbTextColor->color().name();
  addTag("<font color=\""+color+"\">","</font>","foreColor", color);
}

void MainWindow::setHighlightColor() {
  QString color = cbbHighlightColor->color().name();
  addTag("<font style=\"background-color:"+color+"\">","</font>","hiliteColor", color);
}

void MainWindow::setUList() {
  addTag("<ul>","</ul>","insertUnorderedList");
}

void MainWindow::setBackgroundColor() {
  if (tabWEditor->currentIndex() == 0)
    rtfHTMLEditor->setPlainText(webPreview->page()->mainFrame()->toHtml());
  QString color = cbbBackgroundColor->color().name();
  HtmlData pageData = aParser->getHtmlData(rtfHTMLEditor->toPlainText());
  HtmlParser::setAttribute(pageData, "body", 0, "bgcolor", color);
  rtfHTMLEditor->setPlainText(aParser->getParsedHtml(pageData));
  if (tabWEditor->currentIndex() == 0)
    webPreview->setHtml(rtfHTMLEditor->toPlainText());
}

void MainWindow::insertImage() {
  /*QString filters;
  filters += tr("Common Graphics (*.png *.jpg *.jpeg *.gif);;");
  filters += tr("Portable Network Graphics (PNG) (*.png);;");
  filters += tr("JPEG (*.jpg *.jpeg);;");
  filters += tr("Graphics Interchange Format (*.gif);;");
  filters += tr("All Files (*)");*/
  QString fn = KFileDialog::getOpenFileName();
  if (fn.isEmpty())
    return;
  if (!QFile::exists(fn))
    return;
  QUrl url = QUrl::fromLocalFile(fn);
  addTag("<img src=\""+url.toString()+"\" \\>","","insertImage", url.toString());
}

void MainWindow::insertTable() {
  NewTable* aNewTable = new NewTable(this);
  aNewTable->show();
  QWebFrame *frame = webPreview->page()->mainFrame();
  QString js;
  js = QString("document.write(\"%1\")").arg("<b><table><tr><td>test</td><td>test2</td></tr><tr><td>test3</td><td>test4</td></tr></table></b>");
  /*js = QString("\
  newdiv = document.createElement(\"table\");\
  newdiv.innerHTML(\"<tr><td>test4</td></tr>\");\
  document.appendChild(newdiv);");*/
  frame->evaluateJavaScript(js);
}

void MainWindow::insertLink() {
  RessrourceManager* aRessounrceManager = new RessrourceManager(this);
  aRessounrceManager->show();
}

void MainWindow::insertChar() {
  KDialog* aDialog = new KDialog(this);
  aDialog->setMainWidget(new KCharSelect(aDialog));
  aDialog->show();
}

void MainWindow::editShortcut() {
  KDialog* aDialog = new KDialog(this);
  aDialog->setMainWidget(new KShortcutsEditor(actionCollection,aDialog));
  aDialog->show();
}

void MainWindow::editToolbar() {
  KEditToolBar* aToolbarEditor = new KEditToolBar(actionCollection);
  aToolbarEditor->show();
}

void MainWindow::aboutKimberlite() {
  KAboutData* aboutData = new KAboutData( "kimberlite", "kimberlite",
  ki18n("Kimberlite"), "0.0.9",
  ki18n("A complete environemnt to develop, manage and execute Unix scripts"),
  KAboutData::License_GPL,
  ki18n("Copyright (c) 2008 Emmanuel Lepage Vallee") );
  KAboutApplicationDialog* aDialog = new KAboutApplicationDialog(aboutData,this);
  aDialog->show();
}

void MainWindow::reportBug() {
  KBugReport* aDialog = new KBugReport();
  aDialog->show();
}

void MainWindow::disableWysiwyg(bool value) {
  cbbFont->setDisabled(value);
  cbbFontSize->setDisabled(value);
  btnBold->setDisabled(value);
  btnUnderline->setDisabled(value);
  btnItalic->setDisabled(value);
  btnAlignLeft->setDisabled(value);
  btnAlignCenter->setDisabled(value);
  btnAlignRight->setDisabled(value);
  btnJustify->setDisabled(value);
  cbbHeader->setDisabled(value);
  btnLink->setDisabled(value);
  btnChar->setDisabled(value);
  btnTable->setDisabled(value);
  btnList->setDisabled(value);
  lblTextColor->setDisabled(value);
  kcbbTextColor->setDisabled(value);
  lblHighlightColor->setDisabled(value);
  cbbHighlightColor->setDisabled(value);
  lblBackgroundColor->setDisabled(value);
  cbbBackgroundColor->setDisabled(value);
  btnNewLine->setDisabled(value);
  btnNewTab->setDisabled(value);
  btnNewSpace->setDisabled(value);
  ashActions["Add Image"]->setDisabled(value);
  ashActions["Add Table"]->setDisabled(value);
  ashActions["Add Link"]->setDisabled(value);
  ashActions["Special Character"]->setDisabled(value);
}

KAction* MainWindow::createAction(QString name, QString icon, QKeySequence shortcut, bool checkable) {
  KAction* newAction = new KAction(this);
  newAction->setText(i18n(name.toStdString().c_str()));
  newAction->setIcon(KIcon(icon));
  newAction->setShortcut(shortcut);
  newAction->setCheckable(checkable);
  actionCollection->addAction(name, newAction);
  ashActions[name] = newAction;
  return newAction;
}

void MainWindow::quit() {
  exit(33);
}

void MainWindow::print() {
  QPrintDialog* aDialog = new QPrintDialog(this);
  if (aDialog->exec() == QDialog::Accepted) {
    switch (tabWEditor->currentIndex()) {
      case 0:
	webPreview->print(aDialog->printer());
	break;
      case 1:
	rtfHTMLEditor->print(aDialog->printer());
	break;
      case 2:
	rtfScriptEditor->print(aDialog->printer());
	break;
      case 3:
	rtfCSSEditor->print(aDialog->printer());
    }
  }
}

void MainWindow::printPreview() { //BUG Linking error
  /*QPrinter* aPrinter = new QPrinter();
  KPrintPreview var(aPrinter,this);
  KPrintPreview* aDialog = new KPrintPreview(aPrinter,this);
  aDialog->show();*/
}

void MainWindow::undo() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      webPreview->page()->triggerAction(QWebPage::Undo,true);
      break;
    case 1:
      rtfHTMLEditor->undo();
      break;
    case 2:
      rtfScriptEditor->undo();
      break;
    case 3:
      rtfCSSEditor->undo();
  }
}

void MainWindow::redo() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      webPreview->page()->triggerAction(QWebPage::Redo,true);
      break;
    case 1:
      rtfHTMLEditor->redo();
      break;
    case 2:
      rtfScriptEditor->redo();
      break;
    case 3:
      rtfCSSEditor->redo();
  }
}

void MainWindow::copy() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      webPreview->page()->triggerAction(QWebPage::Copy,true);
      break;
    case 1:
      rtfHTMLEditor->copy();
      break;
    case 2:
      rtfScriptEditor->copy();
      break;
    case 3:
      rtfCSSEditor->copy();
  }
}

void MainWindow::cut() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      webPreview->page()->triggerAction(QWebPage::Cut,true);
      break;
    case 1:
      rtfHTMLEditor->cut();
      break;
    case 2:
      rtfScriptEditor->cut();
      break;
    case 3:
      rtfCSSEditor->cut();
  }
}

void MainWindow::paste() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      webPreview->page()->triggerAction(QWebPage::Paste,true);
      break;
    case 1:
      rtfHTMLEditor->paste();
      break;
    case 2:
      rtfScriptEditor->paste();
      break;
    case 3:
      rtfCSSEditor->paste();
  }
}

void MainWindow::find() {
  switch (tabWEditor->currentIndex()) {
    case 0:
      //webPreview->page()->triggerAction(QWebPage::Paste,true);
      break;
    case 1:
      rtfHTMLEditor->findText();
      break;
    case 2:
      //rtfScriptEditor->paste();
      break;
    case 3:
      //rtfCSSEditor->paste();
      break;
  }
}

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
}

void MainWindow::saveRecentProject(QString filePath) {
  QStringList recentProjectList;
  QString path2 = KStandardDirs::locateLocal( "appdata", "recent.txt" );
  QFile file2(path2);
  file2.open(QIODevice::ReadOnly);
  while (!file2.atEnd()) {
    recentProjectList << QString(file2.readLine()).toAscii();
  }
  file2.close();
  
  if (recentProjectList.indexOf(filePath+"\n") != -1)
    return;
  
  if (recentProjectList.count() >= 5)
    recentProjectList.removeAt(0);
  
  recentProjectList << filePath;
  
  QString toSave;
  for (int i =0; i < recentProjectList.count();i++) {
    toSave += recentProjectList[i].trimmed() + "\n";
  }
  
  QString path = KStandardDirs::locateLocal( "appdata", "recent.txt" );
  KSaveFile file(path);
  file.open();
  QByteArray outputByteArray;
  outputByteArray.append(toSave);
  file.write(outputByteArray);
  file.finalize();
  file.close();
}

void MainWindow::loadDefaultPage() {
  KIconLoader *iconloader = KIconLoader::global();
  //int iconSize = iconloader->currentSize(KIconLoader::Desktop);
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
}

void MainWindow::defaultPageLinkClicked(const QUrl & url) {
  if (url.toString() == "new")
    newProject();
  else if (url.toString() == "open")
    openProject();
  else if (url.toString().left(5) == "load:")
    openProject(url.toString().remove(0,5));
}

void MainWindow::cursorChanged() {
  QTextCursor tc;
  switch (tabWEditor->currentIndex()) {
    case 0:
      //N/A
      break;
    case 1:
      tc = rtfHTMLEditor->textCursor();
      break;
    case 2:
      tc = rtfScriptEditor->textCursor();
      break;
    case 3:
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
	if ((anItem) && (tabWCSSLevel->currentIndex() != 0)) {
	  currentClassName = currentText.left(currentText.indexOf('{')).trimmed();
	  treeWidget->setCurrentItem(anItem);
	}
      }
      break;
  }
  lblStatusBar1->setText("Line: " + QString::number(tc.blockNumber()));
}

void MainWindow::addClasses() {
  bool ok;
  QString text = QInputDialog::getText(this, "Add class", "Insert the name of the new class", QLineEdit::Normal, "", &ok);
  if (ok && !text.isEmpty()) {
    if (tabWCSSLevel->currentIndex() == 0) {
      CssParser::cssFile = CssParser::setClass(currentClassName, clearCssBeg());
    }
    CssParser::cssFile += text+" {}";
    currentClassName = text;
    updateClassTree();
    fillCSSBegMode(currentClassName);
  }
}

void MainWindow::loading(int value) {
  pbStatusBar->setValue(value);
}

void MainWindow::linkHovered(QString link) {
  lblStatusBar3->setText(link);
}