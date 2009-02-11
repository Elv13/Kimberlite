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

//Useless
#include <vector>
#include <iostream>

#include <KIcon>
#include <QtSql>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlDriver>
#include <KIO/NetAccess>
#include <KMessageBox>
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
#include "src/CSSbeginnerWidget.h"
#include "src/addProprietyWidget.h"
#include "src/stringToTemplate.h"
#include "src/ProjectManager_v2.h"
#include <ktip.h>

MainWindow::MainWindow(QWidget* parent)  : KXmlGuiWindow(parent),currentHTMLPage(NULL),currentScript(NULL) {
  isModified = false;
  //test code
  aProject = new ProjectManager("test.wkp");
  cout << aProject->projectName.toStdString() << endl;
  setWindowTitle(QApplication::translate("this", "Kimberlite - ", 0, QApplication::UnicodeUTF8) + aProject->projectName);
  for (int i =0;i<aProject->htmlPage.count();i++) {
    cout << aProject->htmlPage[i].toStdString() << endl;
  }
  //exit(33);
  //end of test code

  setupToolTip(); 
  cout << "This: " <<   KStandardDirs::locate( "appdata", "kimberlite.db" ).toStdString() << endl;
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName( KStandardDirs::locate( "appdata", "kimberlite.db" ));
    if ( db->open()) {
      printf("database corectly opened");
    }
    else {
      printf("ERROR while opening the database, get ready for a crash");
    }
    aParser = new HtmlParser();
/*if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("this"));
    this->resize(1008, 717);*/
    /*centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    centralwidget->setGeometry(QRect(0, 0, 1008, 696));*/
    //verticalLayout = new QVBoxLayout(0);
    //verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    tabWMenu = new KTabWidget(this);
    //tabWMenu->setObjectName(QString::fromUtf8("tabWMenu"));
    //QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    /*sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(tabWMenu->sizePolicy().hasHeightForWidth());*/
    //tabWMenu->setSizePolicy(sizePolicy);
    //tabWMenu->tabBar()->setSizePolicy(sizePolicy);
    tabWMenu->setMinimumSize(QSize(0, 87));
    tabWMenu->setMaximumSize(QSize(9999999, 87));
    tabWMenu->setStyleSheet(QString::fromUtf8("QTabWidget::tab-bar {\n"
"	border-radius: 5px;\n"
"}\n"
"QTabWidget::pane {\n"
"	margin-top:2px;"
"	border:0px;\n"
"	margin-bottom:0px;"
"	padding-bottom:0px;"
"	spacing:0px;"
"}\n"
"\n"
"QTabWidget {\n"
"	margin-bottom:0px;"
"	padding-bottom:0px;"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"	background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                               stop:0 darkgray, stop:1 lightgray);\n"
"	border-radius: 5px;\n"
"	margin-top:5px;\n"
"	margin-bottom:5px;\n"
"	color:black;\n"
"}\n"
"\n"
"QTabBar {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                         stop: 0 #111111, stop: 0.4 #444444,\n"
"                                         stop: 0.5 #444444, stop: 1.0 #111111);\n"
"border-radius: 5px;\n"
"padding-left:15px;\n"
"padding-right:15px;\n"
"margin-bottom:3px;\n"
"margin-left:2px;\n"
"border: 2px solid grey;\n"
"width:100%;\n"
"}\n"
"\n"
"QTabBar:tab {\n"
"border-top:0px;\n"
"border-bottom:0px;\n"
"padding-left:4px;\n"
"padding-right:4px;\n"
"margin-left:5px;\n"
"margin-right:5px;\n"
"color:#B4B4B4;\n"
"}"));
    menufile = new QWidget();
    menufile->setObjectName(QString::fromUtf8("menufile"));
    menufile->setGeometry(QRect(0, 0, 1000, 81));
    tabWMenu->addTab(menufile, QString());

    fileTB = new KToolBar(menufile);
    QPalette aPalette;
    fileTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");


    KAction* newProjectAction = new KAction(this);
    newProjectAction->setText(i18n("New Project"));
    newProjectAction->setIcon(KIcon("document-new"));
    newProjectAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("newProject", newProjectAction);
    connect(newProjectAction, SIGNAL(triggered(bool)),
    this, SLOT(newProject()));
    fileTB->addAction(newProjectAction);

    KAction* openProjectAction = new KAction(this);
    openProjectAction->setText(i18n("Open Project"));
    openProjectAction->setIcon(KIcon("document-open"));
    openProjectAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("openProject", newProjectAction);
    connect(openProjectAction, SIGNAL(triggered(bool)),
    this, SLOT(openProject()));
    fileTB->addAction(openProjectAction);

    fileTB->addSeparator();

    KAction* saveProjectAction = new KAction(this);
    saveProjectAction->setText(i18n("Save"));
    saveProjectAction->setIcon(KIcon("document-save"));
    saveProjectAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("saveProject", saveProjectAction);
    connect(saveProjectAction, SIGNAL(triggered(bool)),
    this, SLOT(saveProject()));
    fileTB->addAction(saveProjectAction);

    KAction* saveasProjectAction = new KAction(this);
    saveasProjectAction->setText(i18n("Save As"));
    saveasProjectAction->setIcon(KIcon("document-save-as"));
    saveasProjectAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("saveasProject", saveasProjectAction);
    connect(saveasProjectAction, SIGNAL(triggered(bool)),
    this, SLOT(saveProjectAs()));
    fileTB->addAction(saveasProjectAction);

    fileTB->addSeparator();

    KAction* printProjectAction = new KAction(this);
    printProjectAction->setText(i18n("Print"));
    printProjectAction->setIcon(KIcon("document-print"));
    printProjectAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("saveasProject", printProjectAction);
    connect(printProjectAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    fileTB->addAction(printProjectAction);

    KAction* printPreviewProjectAction = new KAction(this);
    printPreviewProjectAction->setText(i18n("Print Preview"));
    printPreviewProjectAction->setIcon(KIcon("document-print-preview"));
    printPreviewProjectAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("saveasProject", printPreviewProjectAction);
    connect(printPreviewProjectAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    fileTB->addAction(printPreviewProjectAction);

    fileTB->addSeparator();

    KAction* quitAction = new KAction(this);
    quitAction->setText(i18n("Quit"));
    quitAction->setIcon(KIcon("application-exit"));
    quitAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("quit", quitAction);
    connect(quitAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    fileTB->addAction(quitAction);
    
    

    menuEdit = new QWidget();
    //menuEdit->setStyleSheet("margin:0px;spacing:0px;padding:0px;");
    menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
    //menuEdit->setGeometry(QRect(0, 0, 1000, 81));
    horizontalLayout_14 = new QHBoxLayout(menuEdit);
    horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
    horizontalLayout_14->setContentsMargins(0,0,0,0);
    editTB = new KToolBar(menuEdit);
    editTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");
    horizontalLayout_14->addWidget(editTB);
    
    KAction* undoAction = new KAction(this);
    undoAction->setText(i18n("Undo"));
    undoAction->setIcon(KIcon("edit-undo"));
    undoAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("Undo", undoAction);
    connect(undoAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(undoAction);

    KAction* redoAction = new KAction(this);
    redoAction->setText(i18n("Redo"));
    redoAction->setIcon(KIcon("edit-redo"));
    redoAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("redo", redoAction);
    connect(redoAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(redoAction);

    editTB->addSeparator();

    KAction* copyAction = new KAction(this);
    copyAction->setText(i18n("Copy"));
    copyAction->setIcon(KIcon("edit-copy"));
    copyAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("copy", copyAction);
    connect(copyAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(copyAction);

    KAction* cutAction = new KAction(this);
    cutAction->setText(i18n("Cut"));
    cutAction->setIcon(KIcon("edit-cut"));
    cutAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("cut", cutAction);
    connect(cutAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(cutAction);

    KAction* pasteAction = new KAction(this);
    pasteAction->setText(i18n("Paste"));
    pasteAction->setIcon(KIcon("edit-paste"));
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("Paste", pasteAction);
    connect(pasteAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(pasteAction);

    editTB->addSeparator();

    KAction* findAction = new KAction(this);
    findAction->setText(i18n("Find"));
    findAction->setIcon(KIcon("edit-find"));
    findAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("find", findAction);
    connect(findAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(findAction);

    KAction* zoomInAction = new KAction(this);
    zoomInAction->setText(i18n("Zoom In"));
    zoomInAction->setIcon(KIcon("zoom-in"));
    zoomInAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("zoomin", zoomInAction);
    connect(zoomInAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(zoomInAction);

    KAction* zoomOutAction = new KAction(this);
    zoomOutAction->setText(i18n("Zoom Out"));
    zoomOutAction->setIcon(KIcon("zoom-out"));
    zoomOutAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("zoomout", zoomOutAction);
    connect(zoomOutAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(zoomOutAction);

    KAction* zoomOriAction = new KAction(this);
    zoomOriAction->setText(i18n("Zoom 1:1"));
    zoomOriAction->setIcon(KIcon("zoom-original"));
    zoomOriAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("zoomnormal", zoomOriAction);
    connect(zoomOutAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    editTB->addAction(zoomOriAction);

    editTB->addSeparator();

    vlTextAtribute = new QVBoxLayout();
    vlTextAtribute->setObjectName(QString::fromUtf8("vlTextAtribute"));
    vlTextAtribute->setContentsMargins(0,0,0,0);
    hlFont = new QHBoxLayout();
    hlFont->setContentsMargins(0,0,0,0);
    hlFont->setObjectName(QString::fromUtf8("hlFont"));
    hlFont->setSizeConstraint(QLayout::SetMinimumSize);
    cbbFont = new KComboBox(menuEdit);
    cbbFont->setObjectName(QString::fromUtf8("cbbFont"));
    cbbFont->setMinimumSize(QSize(225, 0));
    cbbFont->setMaximumSize(QSize(225, 16777215));

    hlFont->addWidget(cbbFont);

    cbbFontSize = new KIntSpinBox(menuEdit);
    cbbFontSize->setObjectName(QString::fromUtf8("cbbFontSize"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(cbbFontSize->sizePolicy().hasHeightForWidth());
    cbbFontSize->setSizePolicy(sizePolicy1);

    hlFont->addWidget(cbbFontSize);


    vlTextAtribute->addLayout(hlFont);

    hlTextAtributeButton = new QHBoxLayout();
    hlTextAtributeButton->setObjectName(QString::fromUtf8("hlTextAtributeButton"));
    btnBold = new KPushButton(menuEdit);
    btnBold->setObjectName(QString::fromUtf8("btnBold"));
    sizePolicy1.setHeightForWidth(btnBold->sizePolicy().hasHeightForWidth());
    btnBold->setSizePolicy(sizePolicy1);
    btnBold->setMinimumSize(QSize(30, 30));
    btnBold->setMaximumSize(QSize(30, 30));
    KIcon* icnBold = new KIcon("format-text-bold");
    btnBold->setIcon(*icnBold);

    hlTextAtributeButton->addWidget(btnBold);

    btnUnderline = new KPushButton(menuEdit);
    btnUnderline->setObjectName(QString::fromUtf8("btnUnderline"));
    sizePolicy1.setHeightForWidth(btnUnderline->sizePolicy().hasHeightForWidth());
    btnUnderline->setSizePolicy(sizePolicy1);
    btnUnderline->setMinimumSize(QSize(30, 30));
    btnUnderline->setMaximumSize(QSize(30, 16777215));
    KIcon* icnUnderLine = new KIcon("format-text-underline");
    btnUnderline->setIcon(*icnUnderLine);

    hlTextAtributeButton->addWidget(btnUnderline);

    btnItalic = new KPushButton(menuEdit);
    btnItalic->setObjectName(QString::fromUtf8("btnItalic"));
    sizePolicy1.setHeightForWidth(btnItalic->sizePolicy().hasHeightForWidth());
    btnItalic->setSizePolicy(sizePolicy1);
    btnItalic->setMinimumSize(QSize(30, 30));
    btnItalic->setMaximumSize(QSize(30, 16777215));
    KIcon* icnItalic = new KIcon("format-text-italic");
    btnItalic->setIcon(*icnItalic);

    hlTextAtributeButton->addWidget(btnItalic);

    line = new QFrame(menuEdit);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    hlTextAtributeButton->addWidget(line);

    btnAlignLeft = new KPushButton(menuEdit);
    btnAlignLeft->setObjectName(QString::fromUtf8("btnAlignLeft"));
    sizePolicy1.setHeightForWidth(btnAlignLeft->sizePolicy().hasHeightForWidth());
    btnAlignLeft->setSizePolicy(sizePolicy1);
    btnAlignLeft->setMinimumSize(QSize(30, 30));
    btnAlignLeft->setMaximumSize(QSize(30, 16777215));
    KIcon* icnAlignLeft = new KIcon("format-justify-left");
    btnAlignLeft->setIcon(*icnAlignLeft);

    hlTextAtributeButton->addWidget(btnAlignLeft);

    btnAlignCenter = new KPushButton(menuEdit);
    btnAlignCenter->setObjectName(QString::fromUtf8("btnAlignCenter"));
    sizePolicy1.setHeightForWidth(btnAlignCenter->sizePolicy().hasHeightForWidth());
    btnAlignCenter->setSizePolicy(sizePolicy1);
    btnAlignCenter->setMinimumSize(QSize(30, 30));
    btnAlignCenter->setMaximumSize(QSize(30, 16777215));
    KIcon* icnAlignCenter = new KIcon("format-justify-center");
    btnAlignCenter->setIcon(*icnAlignCenter);

    hlTextAtributeButton->addWidget(btnAlignCenter);

    btnAlignRight = new KPushButton(menuEdit);
    btnAlignRight->setObjectName(QString::fromUtf8("btnAlignRight"));
    sizePolicy1.setHeightForWidth(btnAlignRight->sizePolicy().hasHeightForWidth());
    btnAlignRight->setSizePolicy(sizePolicy1);
    btnAlignRight->setMinimumSize(QSize(30, 30));
    btnAlignRight->setMaximumSize(QSize(30, 16777215));
    KIcon* icnAlignRight = new KIcon("format-justify-right");
    btnAlignRight->setIcon(*icnAlignRight);

    hlTextAtributeButton->addWidget(btnAlignRight);

    btnJustify = new KPushButton(menuEdit);
    btnJustify->setObjectName(QString::fromUtf8("btnJustify"));
    sizePolicy1.setHeightForWidth(btnJustify->sizePolicy().hasHeightForWidth());
    btnJustify->setSizePolicy(sizePolicy1);
    btnJustify->setMinimumSize(QSize(30, 30));
    btnJustify->setMaximumSize(QSize(30, 16777215));
    KIcon* icnJustify = new KIcon("format-justify-fill");
    btnJustify->setIcon(*icnJustify);

    hlTextAtributeButton->addWidget(btnJustify);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

    hlTextAtributeButton->addItem(horizontalSpacer_2);


    vlTextAtribute->addLayout(hlTextAtributeButton);


    horizontalLayout_14->addLayout(vlTextAtribute);

    vlOther = new QVBoxLayout();
    vlOther->setObjectName(QString::fromUtf8("vlOther"));
    cbbHeader = new KComboBox(menuEdit);
    cbbHeader->setObjectName(QString::fromUtf8("cbbHeader"));

    vlOther->addWidget(cbbHeader);

    hlOther = new QHBoxLayout();
    hlOther->setObjectName(QString::fromUtf8("hlOther"));
    btnLink = new KPushButton(menuEdit);
    btnLink->setObjectName(QString::fromUtf8("btnLink"));
    sizePolicy1.setHeightForWidth(btnLink->sizePolicy().hasHeightForWidth());
    btnLink->setSizePolicy(sizePolicy1);
    btnLink->setMinimumSize(QSize(30, 30));
    btnLink->setMaximumSize(QSize(30, 16777215));

    hlOther->addWidget(btnLink);

    btnChar = new KPushButton(menuEdit);
    btnChar->setObjectName(QString::fromUtf8("btnChar"));
    sizePolicy1.setHeightForWidth(btnChar->sizePolicy().hasHeightForWidth());
    btnChar->setSizePolicy(sizePolicy1);
    btnChar->setMinimumSize(QSize(30, 30));
    btnChar->setMaximumSize(QSize(30, 16777215));

    hlOther->addWidget(btnChar);

    btnTable = new KPushButton(menuEdit);
    btnTable->setObjectName(QString::fromUtf8("btnTable"));
    sizePolicy1.setHeightForWidth(btnTable->sizePolicy().hasHeightForWidth());
    btnTable->setSizePolicy(sizePolicy1);
    btnTable->setMinimumSize(QSize(30, 30));
    btnTable->setMaximumSize(QSize(30, 16777215));

    hlOther->addWidget(btnTable);

    btnList = new KPushButton(menuEdit);
    btnList->setObjectName(QString::fromUtf8("btnList"));
    sizePolicy1.setHeightForWidth(btnList->sizePolicy().hasHeightForWidth());
    btnList->setSizePolicy(sizePolicy1);
    btnList->setMinimumSize(QSize(30, 30));
    btnList->setMaximumSize(QSize(30, 16777215));

    hlOther->addWidget(btnList);


    vlOther->addLayout(hlOther);


    horizontalLayout_14->addLayout(vlOther);

    vlColor = new QVBoxLayout();
    vlColor->setSpacing(0);
    vlColor->setObjectName(QString::fromUtf8("vlColor"));
    hlTextColor = new QHBoxLayout();
    hlTextColor->setSpacing(0);
    hlTextColor->setObjectName(QString::fromUtf8("hlTextColor"));
    hlTextColor->setSizeConstraint(QLayout::SetMinimumSize);
    lblTextColor = new QLabel(menuEdit);
    lblTextColor->setObjectName(QString::fromUtf8("lblTextColor"));

    hlTextColor->addWidget(lblTextColor);

    kcbbTextColor = new KColorCombo(menuEdit);
    kcbbTextColor->setObjectName(QString::fromUtf8("kcbbTextColor"));

    hlTextColor->addWidget(kcbbTextColor);


    vlColor->addLayout(hlTextColor);

    hlHighlightColor = new QHBoxLayout();
    hlHighlightColor->setSpacing(0);
    hlHighlightColor->setObjectName(QString::fromUtf8("hlHighlightColor"));
    lblHighlightColor = new QLabel(menuEdit);
    lblHighlightColor->setObjectName(QString::fromUtf8("lblHighlightColor"));

    hlHighlightColor->addWidget(lblHighlightColor);

    cbbHighlightColor = new KColorCombo(menuEdit);
    cbbHighlightColor->setObjectName(QString::fromUtf8("cbbHighlightColor"));

    hlHighlightColor->addWidget(cbbHighlightColor);


    vlColor->addLayout(hlHighlightColor);

    hlBackgroundColor = new QHBoxLayout();
    hlBackgroundColor->setSpacing(0);
    hlBackgroundColor->setObjectName(QString::fromUtf8("hlBackgroundColor"));
    lblBackgroundColor = new QLabel(menuEdit);
    lblBackgroundColor->setObjectName(QString::fromUtf8("lblBackgroundColor"));

    hlBackgroundColor->addWidget(lblBackgroundColor);

    cbbBackgroundColor = new KColorCombo(menuEdit);
    cbbBackgroundColor->setObjectName(QString::fromUtf8("cbbBackgroundColor"));

    hlBackgroundColor->addWidget(cbbBackgroundColor);


    vlColor->addLayout(hlBackgroundColor);


    horizontalLayout_14->addLayout(vlColor);

    tabWMenu->addTab(menuEdit, QString());
    
    
    menuView = new QWidget();
    menuView->setObjectName(QString::fromUtf8("menuview"));
    //menuView->setGeometry(QRect(0, 0, 1000, 81));
    tabWMenu->addTab(menuView, QString());

    viewTB = new KToolBar(menuView);
    viewTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");


    KAction* viewPageList = new KAction(this);
    viewPageList->setCheckable(true);
    viewPageList->setText(i18n("Page List"));
    viewPageList->setIcon(KIcon("text-x-katefilelist"));
    viewPageList->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("viewPageList", viewPageList);
    connect(viewPageList, SIGNAL(triggered(bool)),
    this, SLOT(showPageList(bool)));
    viewTB->addAction(viewPageList);
    
    KAction* viewCSSClass = new KAction(this);
    viewCSSClass->setCheckable(true);
    viewCSSClass->setText(i18n("CSS Class"));
    viewCSSClass->setIcon(KIcon("text-x-katefilelist"));
    viewCSSClass->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("viewCSSClass", viewPageList);
    connect(viewCSSClass, SIGNAL(triggered(bool)),
    this, SLOT(showCSS(bool)));
    viewTB->addAction(viewCSSClass);
    
    KAction* viewDebugger = new KAction(this);
    viewDebugger->setCheckable(true);
    viewDebugger->setText(i18n("Debugger"));
    viewDebugger->setIcon(KIcon("text-x-katefilelist"));
    viewDebugger->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("viewDebugger", viewPageList);
    connect(viewDebugger, SIGNAL(triggered(bool)),
    this, SLOT(showDebugger(bool)));
    viewTB->addAction(viewDebugger);
    
    
    menuInsert = new QWidget();
    menuInsert->setStyleSheet("margin:0px;spacing:0px;padding:0px;");
    menuInsert->setObjectName(QString::fromUtf8("menuInsert"));
    //menuInsert->setGeometry(QRect(0, 0, 1000, 81));
    tabWMenu->addTab(menuInsert, QString());
    hlInsert = new QHBoxLayout(menuInsert);
    hlInsert->setContentsMargins(0,0,0,0);
    hlInsert->setObjectName(QString::fromUtf8("hlInsert"));
    hlInsert->setSpacing(0);
    //hlInsert->setSizeConstraint(QLayout::SetMinimumSize);
    horizontalSpacer_3 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_14->addItem(horizontalSpacer_3);
    horizontalLayout_14->setContentsMargins(0,0,0,0);

    insertTB = new KToolBar(menuInsert);
    insertTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");
    hlInsert->addWidget(insertTB);

    KAction* addPageAction = new KAction(this);
    addPageAction->setText(i18n("New Page"));
    addPageAction->setIcon(KIcon("document-new"));
    addPageAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("addpage", addPageAction);
    connect(addPageAction, SIGNAL(triggered(bool)),
    this, SLOT(addHtmlPage()));
    insertTB->addAction(addPageAction);

    KAction* addScriptAction = new KAction(this);
    addScriptAction->setText(i18n("New Script"));
    addScriptAction->setIcon(KIcon("application-add"));
    addScriptAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("addscript", addScriptAction);
    connect(addScriptAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    insertTB->addAction(addScriptAction);

    insertTB->addSeparator();

    KAction* addImageAction = new KAction(this);
    addImageAction->setText(i18n("Add Image"));
    addImageAction->setIcon(KIcon("insert-image"));
    addImageAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("addimage", addImageAction);
    connect(addImageAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    insertTB->addAction(addImageAction);

    KAction* addTableAction = new KAction(this);
    addTableAction->setText(i18n("Add Table"));
    addTableAction->setIcon(KIcon("insert-table"));
    addTableAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("addtable", addTableAction);
    connect(addTableAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    insertTB->addAction(addTableAction);

    KAction* addLinkAction = new KAction(this);
    addLinkAction->setText(i18n("Add Link"));
    addLinkAction->setIcon(KIcon("insert-link"));
    addLinkAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("addlink", addLinkAction);
    connect(addLinkAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    insertTB->addAction(addLinkAction);

    KAction* addTextAction = new KAction(this);
    addTextAction->setText(i18n("Add Text"));
    addTextAction->setIcon(KIcon("insert-text"));
    addTextAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("addtext", addTextAction);
    connect(addTextAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    insertTB->addAction(addTextAction);

    insertTB->addSeparator();

    KAction* addCharAction = new KAction(this);
    addCharAction->setText(i18n("Insert Special Character"));
    addCharAction->setIcon(KIcon("list-add-font"));
    addCharAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("addchar", addCharAction);
    connect(addCharAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    insertTB->addAction(addCharAction);

    KAction* addColorAction = new KAction(this);
    addColorAction->setText(i18n("Get Color Code"));
    addColorAction->setIcon(KIcon("fill-color"));
    addColorAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("addcolor", addColorAction);
    connect(addColorAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    insertTB->addAction(addColorAction);

    QSpacerItem* horizontalSpacer8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlInsert->addItem(horizontalSpacer8);

    vlSpacing = new QVBoxLayout();
    vlSpacing->setObjectName(QString::fromUtf8("vlSpacing"));
    vlSpacing->setSpacing(0);
    vlSpacing->setSizeConstraint(QLayout::SetMinimumSize);
    btnNewLine = new KPushButton(menuInsert);
    btnNewLine->setObjectName(QString::fromUtf8("btnNewLine"));
    btnNewLine->setText("New Line");
    sizePolicy1.setHeightForWidth(btnChar->sizePolicy().hasHeightForWidth());
    btnNewLine->setSizePolicy(sizePolicy1);
    btnNewLine->setMinimumSize(QSize(100, 23));
    btnNewLine->setMaximumSize(QSize(100, 23));

    vlSpacing->addWidget(btnNewLine);

    btnNewTab = new KPushButton(menuInsert);
    btnNewTab->setObjectName(QString::fromUtf8("btnNewTab"));
    btnNewTab->setText("New Tab");
    sizePolicy1.setHeightForWidth(btnChar->sizePolicy().hasHeightForWidth());
    btnNewTab->setSizePolicy(sizePolicy1);
    btnNewTab->setMinimumSize(QSize(100, 23));
    btnNewTab->setMaximumSize(QSize(100, 23));

    vlSpacing->addWidget(btnNewTab);

    btnNewSpace = new KPushButton(menuInsert);
    btnNewSpace->setObjectName(QString::fromUtf8("btnNewSpace"));
    btnNewSpace->setText("New Space");
    sizePolicy1.setHeightForWidth(btnChar->sizePolicy().hasHeightForWidth());
    btnNewSpace->setSizePolicy(sizePolicy1);
    btnNewSpace->setMinimumSize(QSize(100, 23));
    btnNewSpace->setMaximumSize(QSize(100, 23));
    vlSpacing->addWidget(btnNewSpace);
    hlInsert->addLayout(vlSpacing);

    menuTools = new QWidget();
    menuTools->setObjectName(QString::fromUtf8("MenuTools"));
    //menuTools->setGeometry(QRect(0, 0, 1000, 81));
    tabWMenu->addTab(menuTools, QString());

    toolsTB = new KToolBar(menuTools);
    toolsTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");
    //menuTools->addWidget(toolsTB);

    KAction* parseAction = new KAction(this);
    parseAction->setText(i18n("Parse"));
    parseAction->setIcon(KIcon("format-indent-more"));
    parseAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("parse", parseAction);
    connect(parseAction, SIGNAL(triggered(bool)),
    this, SLOT(reParse()));
    toolsTB->addAction(parseAction);

    KAction* templaterizeAction = new KAction(this);
    templaterizeAction->setText(i18n("Templaterize"));
    templaterizeAction->setIcon(KIcon("view-pim-tasks"));
    templaterizeAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("templaterize", templaterizeAction);
    connect(templaterizeAction, SIGNAL(triggered(bool)),
    this, SLOT(templaterize(bool)));
    toolsTB->addAction(templaterizeAction);

    KAction* translateAction = new KAction(this);
    translateAction->setText(i18n("Translate"));
    translateAction->setIcon(KIcon("application-x-marble"));
    translateAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("translate", translateAction);
    connect(templaterizeAction, SIGNAL(triggered(bool)),
    this, SLOT(translate()));
    toolsTB->addAction(translateAction);

    KAction* debugAction = new KAction(this);
    debugAction->setText(i18n("Debug"));
    debugAction->setIcon(KIcon("tools-report-bug"));
    debugAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("debug", debugAction);
    connect(debugAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    toolsTB->addAction(debugAction);

    toolsTB->addSeparator();

    KAction* matchTagAction = new KAction(this);
    matchTagAction->setText(i18n("Match Tag"));
    matchTagAction->setIcon(KIcon("application-xml"));
    matchTagAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("matchtag", matchTagAction);
    connect(matchTagAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    toolsTB->addAction(matchTagAction);

    KAction* spellCheckAction = new KAction(this);
    spellCheckAction->setText(i18n("Spelling"));
    spellCheckAction->setIcon(KIcon("tools-check-spelling"));
    spellCheckAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("spellcheck", spellCheckAction);
    connect(spellCheckAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    toolsTB->addAction(spellCheckAction);

    KAction* checkLinkAction = new KAction(this);
    checkLinkAction->setText(i18n("Check Link"));
    checkLinkAction->setIcon(KIcon("network-connect"));
    checkLinkAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("checklink", checkLinkAction);
    connect(checkLinkAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    toolsTB->addAction(checkLinkAction);

    menuOptions = new QWidget();
    menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
    //menuOptions->setGeometry(QRect(0, 0, 1000, 81));
    tabWMenu->addTab(menuOptions, QString());

    optionsTB = new KToolBar(menuOptions);
    optionsTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

    KAction* configureWebKreatorAction = new KAction(this);
    configureWebKreatorAction->setText(i18n("Configure WebKreator"));
    configureWebKreatorAction->setIcon(KIcon("configure"));
    configureWebKreatorAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("configure", configureWebKreatorAction);
    connect(configureWebKreatorAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    optionsTB->addAction(configureWebKreatorAction);

    KAction* configureToolBarAction = new KAction(this);
    configureToolBarAction->setText(i18n("Configure ToolBars"));
    configureToolBarAction->setIcon(KIcon("configure-toolbars"));
    configureToolBarAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("configuretoolbar", configureToolBarAction);
    connect(configureToolBarAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    optionsTB->addAction(configureToolBarAction);

    KAction* configureShortcutsAction = new KAction(this);
    configureShortcutsAction->setText(i18n("Configure Shortcuts"));
    configureShortcutsAction->setIcon(KIcon("configure-shortcuts"));
    configureShortcutsAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("configuretoolbar", configureShortcutsAction);
    connect(configureShortcutsAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    optionsTB->addAction(configureShortcutsAction);

    menuHelp = new QWidget();
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    //menuHelp->setGeometry(QRect(0, 0, 1000, 81));
    tabWMenu->addTab(menuHelp, QString());

    helpTB = new KToolBar(menuHelp);
    helpTB->setStyleSheet("margin:0px;spacing:0px;padding:0px;background-color:" + aPalette.window().color().name () +";");

    KAction* manualAction = new KAction(this);
    manualAction->setText(i18n("Manual"));
    manualAction->setIcon(KIcon("help-contents"));
    manualAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("manual", manualAction);
    connect(manualAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    helpTB->addAction(manualAction);

    KAction* aboutAction = new KAction(this);
    aboutAction->setText(i18n("About"));
    aboutAction->setIcon(KIcon("help-about"));
    aboutAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("about", aboutAction);
    connect(aboutAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    helpTB->addAction(aboutAction);

    helpTB->addSeparator();

    KAction* reportBugAction = new KAction(this);
    reportBugAction->setText(i18n("Report A Bug"));
    reportBugAction->setIcon(KIcon("tools-report-bug"));
    reportBugAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("reportBug", reportBugAction);
    connect(reportBugAction, SIGNAL(triggered(bool)),
    this, SLOT(quit()));
    helpTB->addAction(reportBugAction);

    //verticalLayout->addWidget(tabWMenu);
    setMenuWidget(tabWMenu);
    
    /*mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));*/

    dockHtmlTree = new QDockWidget(this);
    treeHtml = new QTreeWidget(this);
    dockHtmlTree->setWidget(treeHtml);
    addDockWidget(Qt::LeftDockWidgetArea, dockHtmlTree);
 
    tableDock = new QDockWidget(this);
    tableDock->setObjectName(QString::fromUtf8("tableDock"));
    //tableDock->setHidden(true);
    
    aProjectManager = new ProjectManager2(0);
    
    
    tableDockCentral = new QWidget();
    tableDockCentral->setObjectName(QString::fromUtf8("tableDockCentral"));
    //tableDockCentral->setGeometry(QRect(4, 22, 166, 556));
    //tableDockCentral->setMaximumSize(200,9999);
    //tableDock->setMaximumSize(200,9999);
    verticalLayout_4 = new QVBoxLayout(tableDockCentral);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    /*tableView = new QTableWidget(tableDockCentral);
    tableView->setObjectName(QString::fromUtf8("tableView"));
    if (tableView->columnCount() < 2)
       tableView->setColumnCount(2);
   
    connect(tableView, SIGNAL(itemClicked(QTableWidgetItem*)),
    this, SLOT(loadPage(QTableWidgetItem*)));
    
    QTableWidgetItem *__colItem10 = new QTableWidgetItem("column 1");
    tableView->setHorizontalHeaderItem(0, __colItem10);

    verticalLayout_4->addWidget(tableView);*/
    addDockWidget(Qt::LeftDockWidgetArea, tableDock);
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
    treeDock->setObjectName(QString::fromUtf8("treeDock"));
    //treeDock->setHidden(true);
    treeDockCentral = new QWidget();
    treeDockCentral->setObjectName(QString::fromUtf8("treeDockCentral"));
    //treeDockCentral->setGeometry(QRect(4, 22, 166, 556));
    //treeDock->setMaximumSize(200,9999);
    verticalLayout_5 = new QVBoxLayout(treeDockCentral);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    treeWidget = new QTreeWidget(treeDockCentral);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
    treeWidget->setIconSize(QSize(32,32));
    addDockWidget(Qt::LeftDockWidgetArea,treeDock );
    

    verticalLayout_5->addWidget(treeWidget);

    hlButton2 = new QHBoxLayout();
    hlButton2->setObjectName(QString::fromUtf8("hlButton2"));
    btnTreeAdd = new KPushButton(treeDockCentral);
    btnTreeAdd->setObjectName(QString::fromUtf8("btnTreeAdd"));

    hlButton2->addWidget(btnTreeAdd);

    btnTreeRemove = new KPushButton(treeDockCentral);
    btnTreeRemove->setObjectName(QString::fromUtf8("btnTreeRemove"));

    hlButton2->addWidget(btnTreeRemove);


    verticalLayout_5->addLayout(hlButton2);

    treeDock->setWidget(treeDockCentral);

    //mainLayout->addWidget(treeDock);

    /*centralWidget2 = new QWidget(centralwidget);
    centralWidget2->setObjectName(QString::fromUtf8("centralWidget2"));
    verticalLayout_2 = new QVBoxLayout(centralWidget2);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));*/
    tabWEditor = new QTabWidget(this);
    setCentralWidget(tabWEditor);
    tabWEditor->setObjectName(QString::fromUtf8("tabWEditor"));
    /*tabWEditor->setStyleSheet(QString::fromUtf8("QTabWidget::pane {\n"
"	margin-top:0px;\n"
"	spacing:0px;\n"
"	padding-top:4px;\n"
"}"));*/
    tabWEditor->setTabPosition(QTabWidget::South);
    tabPreview = new QWidget();
    tabPreview->setObjectName(QString::fromUtf8("tabPreview"));
    horizontalLayout_2 = new QHBoxLayout(tabPreview);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    webPreview = new QWebView(tabPreview);
    webPreview->setObjectName(QString::fromUtf8("webPreview"));
    webPreview->setUrl(QUrl("about:blank"));

    horizontalLayout_2->addWidget(webPreview);

    tabWEditor->addTab(tabPreview, QString());
    tabHTML = new QWidget();
    tabHTML->setObjectName(QString::fromUtf8("tabHTML"));
    verticalLayout_3 = new QVBoxLayout(tabHTML);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    rtfHTMLEditor = new KTextEdit(tabHTML);
    rtfHTMLEditor->setObjectName(QString::fromUtf8("rtfHTMLEditor"));
    rtfHTMLEditor->setAcceptRichText(false);
    HtmlSyntaxHighlighter* htmlHighlighter = new HtmlSyntaxHighlighter(rtfHTMLEditor);
    rtfHTMLEditor->setLineWrapMode(QTextEdit::NoWrap);
    rtfHTMLEditor->setWordWrapMode(QTextOption::NoWrap);
    rtfHTMLEditor->setWordWrapMode(QTextOption::NoWrap);
    connect(rtfHTMLEditor, SIGNAL(textChanged()), this, SLOT(setModified()));

    /*aParser = new HtmlParser();
    std::string aFile = aParser->compressFile("/home/lepagee/dev/webkreator/test.htm");
    rtfHTMLEditor->setPlainText(QString::fromStdString(aParser->htmlParser(aFile,true, false)));*/

    verticalLayout_3->addWidget(rtfHTMLEditor);

    /*btnParse = new QPushButton(tabHTML);
    btnParse->setObjectName(QString::fromUtf8("btnParse"));
    sizePolicy.setHeightForWidth(btnParse->sizePolicy().hasHeightForWidth());
    //btnParse->setSizePolicy(sizePolicy);
    connect(btnParse, SIGNAL(clicked()), this, SLOT(reParse()));


    verticalLayout_3->addWidget(btnParse);*/

    dockDebug = new QDockWidget(tabHTML);
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
    //dockDebugContents->setGeometry(QRect(4, 22, 573, 131));
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

    verticalLayout_7->addWidget(rtfScriptEditor);

    tabWEditor->addTab(tabScripts, QString());
    tabCSS = new QWidget();
    tabCSS->setObjectName(QString::fromUtf8("tabCSS"));
    verticalLayout_6 = new QVBoxLayout(tabCSS);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    tabWCSSLevel = new QTabWidget(tabCSS);
    connect(tabWCSSLevel, SIGNAL(currentChanged(int)),
    this, SLOT(changeCssMode(int)));
    tabWCSSLevel->setObjectName(QString::fromUtf8("tabWCSSLevel"));
    tabWCSSLevel->setStyleSheet(QString::fromUtf8("QTabWidget::tab-bar {\n"
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
"}"));
    tabWCSSLevel->setTabShape(QTabWidget::Triangular);
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
    grbSize = new QGroupBox(scrollAreaWidgetContents);
    grbSize->setObjectName(QString::fromUtf8("grbSize"));
    verticalLayout_9 = new QVBoxLayout(grbSize);
    verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
    /*hlTxtTag = new QHBoxLayout();
    hlTxtTag->setObjectName(QString::fromUtf8("hlTxtTag"));
    ckbName2 = new QCheckBox(grbSize);
    ckbName2->setObjectName(QString::fromUtf8("ckbName2"));
    ckbName2->setMinimumSize(QSize(135, 0));

    hlTxtTag->addWidget(ckbName2);

    txtValue2 = new QLineEdit(grbSize);
    txtValue2->setObjectName(QString::fromUtf8("txtValue2"));

    hlTxtTag->addWidget(txtValue2);

    cbbUnit2 = new KComboBox(grbSize);
    cbbUnit2->setObjectName(QString::fromUtf8("cbbUnit2"));
    sizePolicy1.setHeightForWidth(cbbUnit2->sizePolicy().hasHeightForWidth());
    cbbUnit2->setSizePolicy(sizePolicy1);

    hlTxtTag->addWidget(cbbUnit2);


    verticalLayout_9->addLayout(hlTxtTag);*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////TODO//TODO
    cssHeight = new CSSBeginnerWidget(tabBeginner,"height");
    verticalLayout_9->addWidget(cssHeight);

    cssWidth = new CSSBeginnerWidget(tabBeginner,"width");
    verticalLayout_9->addWidget(cssWidth);


    verticalLayout_10->addWidget(grbSize);

    grbText = new QGroupBox(scrollAreaWidgetContents);
    grbText->setObjectName(QString::fromUtf8("grbText"));
    verticalLayout_11 = new QVBoxLayout(grbText);
    verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));

    cssText_align = new CSSBeginnerWidget(tabBeginner,"text-align");
    verticalLayout_11->addWidget(cssText_align);
    cssText_transform = new CSSBeginnerWidget(tabBeginner,"text-transform");
    verticalLayout_11->addWidget(cssText_transform);
    cssColor = new CSSBeginnerWidget(tabBeginner,"color");
    verticalLayout_11->addWidget(cssColor);
    cssFont_family = new CSSBeginnerWidget(tabBeginner,"font-family");
    verticalLayout_11->addWidget(cssFont_family);
    cssFont_size = new CSSBeginnerWidget(tabBeginner,"font-size");
    verticalLayout_11->addWidget(cssFont_size);
    cssFont_style = new CSSBeginnerWidget(tabBeginner,"font-style");
    verticalLayout_11->addWidget(cssFont_style);
    cssFont_weight = new CSSBeginnerWidget(tabBeginner,"font-weight");
    verticalLayout_11->addWidget(cssFont_weight);

    verticalLayout_10->addWidget(grbText);

    grbBackground = new QGroupBox(scrollAreaWidgetContents);
    grbBackground->setObjectName(QString::fromUtf8("grbBackground"));
    verticalLayout_12 = new QVBoxLayout(grbBackground);
    verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));

    cssBackground_image = new CSSBeginnerWidget(tabBeginner,"background-image");
    verticalLayout_12->addWidget(cssBackground_image);
    cssBackground_color = new CSSBeginnerWidget(tabBeginner,"background-color");
    verticalLayout_12->addWidget(cssBackground_color);
    cssBackground_repeat = new CSSBeginnerWidget(tabBeginner,"background-repeat");
    verticalLayout_12->addWidget(cssBackground_repeat);

    verticalLayout_10->addWidget(grbBackground);

    grbBorder = new QGroupBox(scrollAreaWidgetContents);
    grbBorder->setObjectName(QString::fromUtf8("grbBorder"));
    verticalLayout_13 = new QVBoxLayout(grbBorder);
    verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));

    cssBorder_width = new CSSBeginnerWidget(tabBeginner,"border-width");
    verticalLayout_13->addWidget(cssBorder_width);
    cssBorder_color = new CSSBeginnerWidget(tabBeginner,"border-color");
    verticalLayout_13->addWidget(cssBorder_color);
    cssBorder_style = new CSSBeginnerWidget(tabBeginner,"border-style");
    verticalLayout_13->addWidget(cssBorder_style);

    verticalLayout_10->addWidget(grbBorder);

    grbLayout = new QGroupBox(scrollAreaWidgetContents);
    grbLayout->setObjectName(QString::fromUtf8("grbLayout"));
    grbLayout->setMinimumSize(QSize(0, 0));
    verticalLayout_14 = new QVBoxLayout(grbLayout);
    verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));

    cssFloat = new CSSBeginnerWidget(tabBeginner,"float");
    verticalLayout_14->addWidget(cssFloat);
    cssPosition = new CSSBeginnerWidget(tabBeginner,"position");
    verticalLayout_14->addWidget(cssPosition);
    cssZ_index = new CSSBeginnerWidget(tabBeginner,"z-index");
    verticalLayout_14->addWidget(cssZ_index);
    cssMargin_top = new CSSBeginnerWidget(tabBeginner,"margin-top");
    verticalLayout_14->addWidget(cssMargin_top);
    cssMargin_bottom = new CSSBeginnerWidget(tabBeginner,"margin-bottom");
    verticalLayout_14->addWidget(cssMargin_bottom);
    cssMargin_left = new CSSBeginnerWidget(tabBeginner,"margin-left");
    verticalLayout_14->addWidget(cssMargin_left);
    cssMargin_right = new CSSBeginnerWidget(tabBeginner,"margin-right");
    verticalLayout_14->addWidget(cssMargin_right);
    cssPadding_top = new CSSBeginnerWidget(tabBeginner,"padding-top");
    verticalLayout_14->addWidget(cssPadding_top);
    cssPadding_bottom = new CSSBeginnerWidget(tabBeginner,"padding-bottom");
    verticalLayout_14->addWidget(cssPadding_bottom);
    cssPadding_left = new CSSBeginnerWidget(tabBeginner,"padding-left");
    verticalLayout_14->addWidget(cssPadding_left);
    cssPadding_right = new CSSBeginnerWidget(tabBeginner,"padding-right");
    verticalLayout_14->addWidget(cssPadding_right);

    verticalLayout_10->addWidget(grbLayout);

    grbOther = new QGroupBox(scrollAreaWidgetContents);
    grbOther->setObjectName(QString::fromUtf8("grbOther"));
    verticalLayout_15 = new QVBoxLayout(grbOther);
    verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
    cssList_style = new CSSBeginnerWidget(tabBeginner,"list-style");
    verticalLayout_15->addWidget(cssList_style);
    cssCursor = new CSSBeginnerWidget(tabBeginner,"cursor");
    verticalLayout_15->addWidget(cssCursor);
    txtOtherTags = new QTextEdit();
    txtOtherTags->setMaximumSize(9999,125);
    txtOtherTags->setMinimumSize(0,125);
    verticalLayout_15->addWidget(txtOtherTags);

    verticalLayout_10->addWidget(grbOther);

    scrollArea->setWidget(scrollAreaWidgetContents);

    verticalLayout_8->addWidget(scrollArea);

    tabWCSSLevel->addTab(tabBeginner, QString());
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
    rtfCSSEditor = new KTextEdit(tabexpert);
    rtfCSSEditor->setObjectName(QString::fromUtf8("rtfCSSEditor"));
    CssSyntaxHighlighter* cssHighlighter = new CssSyntaxHighlighter(rtfCSSEditor);

    verticalLayout_17->addWidget(rtfCSSEditor);

    tabWCSSLevel->addTab(tabexpert, QString());

    verticalLayout_6->addWidget(tabWCSSLevel);

    tabWEditor->addTab(tabCSS, QString());
    tabValidator = new QWidget();
    tabValidator->setObjectName(QString::fromUtf8("tabValidator"));
    verticalLayout_16 = new QVBoxLayout(tabValidator);
    verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
    webValidator = new QWebView(tabValidator);
    webValidator->setObjectName(QString::fromUtf8("webValidator"));
    webValidator->setUrl(QUrl("about:blank"));

    verticalLayout_16->addWidget(webValidator);
    
    tableDock->setWidget(aProjectManager);
     connect(aProjectManager, SIGNAL(htmlPageChanged(QTreeWidgetItem*, QString)), this, SLOT(loadPage(QTreeWidgetItem*, QString)));
    connect(aProjectManager, SIGNAL(javaScriptChanged(QTreeWidgetItem*, QString)), this, SLOT(loadScript(QTreeWidgetItem*, QString)));
    connect(aProjectManager, SIGNAL(loadCss(QString)), this, SLOT(loadCss(QString)));
    connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem* , int)), this, SLOT(loadCSSClass(QTreeWidgetItem*)));
    QString fileName = "/home/lepagee/dev/myproject/kimberlite/test2.wkp";
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    }
    else 
      aProjectManager->read(&file);
    aProjectManager->expandAll();
    
//     aProjectManager = new ProjectManager2(tabValidator);
//      connect(aProjectManager, SIGNAL(htmlPageChanged(QTreeWidgetItem*, QString)), this, SLOT(loadPage(QTreeWidgetItem*, QString)));
//     QString fileName = "/home/lepagee/dev/myproject/kimberlite/test2.wkp";
//     QFile file(fileName);
//     if (!file.open(QFile::ReadOnly | QFile::Text)) {
//         QMessageBox::warning(this, tr("SAX Bookmarks"),
//                              tr("Cannot read file %1:\n%2.")
//                              .arg(fileName)
//                              .arg(file.errorString()));
//     }
//     aProjectManager->read(&file);
    //verticalLayout_16->addWidget(aProjectManager);

    tabWEditor->addTab(tabValidator, QString());
    
    setCentralWidget(tabWEditor);

    //verticalLayout_2->addWidget(tabWEditor);


    //mainLayout->addWidget(centralWidget2);

    //centralwidget->setLayout(mainLayout);
    //verticalLayout->addLayout(mainLayout);

    //this->setCentralWidget(centralwidget);
    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    //statusbar->setGeometry(QRect(0, 696, 1008, 21));
    this->setStatusBar(statusbar);
    //this->setStandardToolBarMenuEnabled(false);
    //this->toolBarArea()->setHidden(true);
//toolBars()[0]->setHidden(true);
    
    
    retranslateUi();
    //disableWidget(false);
//rtfCSSEditor->setText(readCSSFile("/home"));
/*readCSSFile("/home");
rtfCSSEditor->setText(parseCSS());
getClass("#header");
fillCSSBegMode("#header");
QTreeWidgetItem* styleSheetName = new  QTreeWidgetItem(treeWidget);
styleSheetName->setText(0,"StyleSheet.css");
//treeWidget->append(new QTreeWidgetItem((QTreeWidget*)0, append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1"))));));
fillCSSAdvMode();
QStringList classList = getClassList();
for (int j =0; j < classList.count();j++) {
  QTreeWidgetItem* aTreeViewWidgetItem = new  QTreeWidgetItem(styleSheetName);
  aTreeViewWidgetItem->setText(0,classList[j]);
}*/

}

void MainWindow::retranslateUi()
    {
    //setWindowTitle(QApplication::translate("this", "Kimberlite", 0, QApplication::UnicodeUTF8));
    tabWMenu->setTabText(tabWMenu->indexOf(menufile), QApplication::translate("this", "File", 0, QApplication::UnicodeUTF8));
    lblTextColor->setText(QApplication::translate("this", "icn", 0, QApplication::UnicodeUTF8));
//     kcbbTextColor->insertItems(0, QStringList()
//      << QApplication::translate("this", "Custom...", 0, QApplication::UnicodeUTF8)
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//     );
    lblHighlightColor->setText(QApplication::translate("this", "icn", 0, QApplication::UnicodeUTF8));
//     cbbHighlightColor->insertItems(0, QStringList()
//      << QApplication::translate("this", "Custom...", 0, QApplication::UnicodeUTF8)
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//     );
    lblBackgroundColor->setText(QApplication::translate("this", "icn", 0, QApplication::UnicodeUTF8));
//     cbbBackgroundColor->insertItems(0, QStringList()
//      << QApplication::translate("this", "Custom...", 0, QApplication::UnicodeUTF8)
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//      << QString()
//     );
    tabWMenu->setTabText(tabWMenu->indexOf(menuEdit), QApplication::translate("this", "Edit", 0, QApplication::UnicodeUTF8));
    tabWMenu->setTabText(tabWMenu->indexOf(menuView), QApplication::translate("this", "View", 0, QApplication::UnicodeUTF8));
    tabWMenu->setTabText(tabWMenu->indexOf(menuInsert), QApplication::translate("this", "Insert", 0, QApplication::UnicodeUTF8));
    tabWMenu->setTabText(tabWMenu->indexOf(menuTools), QApplication::translate("this", "Tools", 0, QApplication::UnicodeUTF8));
    tabWMenu->setTabText(tabWMenu->indexOf(menuOptions), QApplication::translate("this", "Options", 0, QApplication::UnicodeUTF8));
    tabWMenu->setTabText(tabWMenu->indexOf(menuHelp), QApplication::translate("this", "Help", 0, QApplication::UnicodeUTF8));
    btnTableAdd->setText(QApplication::translate("this", "Add", 0, QApplication::UnicodeUTF8));
    btnTableRemove->setText(QApplication::translate("this", "Remove", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(0, QApplication::translate("this", "1", 0, QApplication::UnicodeUTF8));
    btnTreeAdd->setText(QApplication::translate("this", "Add", 0, QApplication::UnicodeUTF8));
    btnTreeRemove->setText(QApplication::translate("this", "Remove", 0, QApplication::UnicodeUTF8));
    tabWEditor->setTabText(tabWEditor->indexOf(tabPreview), QApplication::translate("this", "Preview", 0, QApplication::UnicodeUTF8));
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

    const bool __sortingEnabled = tblCSSPage->isSortingEnabled();
    tblCSSPage->setSortingEnabled(false);
    //tblCSSPage->item(0, 0)->setText(QApplication::translate("this", "fdtgdfgdgdfgdgdfgdfgdfgdfgdgg", 0, QApplication::UnicodeUTF8));

    //tblCSSPage->setSortingEnabled(__sortingEnabled);
    btnaddClass->setText(QApplication::translate("this", "Add class", 0, QApplication::UnicodeUTF8));
    btnAddPClass->setText(QApplication::translate("this", "Add pseudo class", 0, QApplication::UnicodeUTF8));
    btnRemoveClass->setText(QApplication::translate("this", "Remove class", 0, QApplication::UnicodeUTF8));
    tabWCSSLevel->setTabText(tabWCSSLevel->indexOf(tabAdvanced), QApplication::translate("this", "Advanced", 0, QApplication::UnicodeUTF8));
    tabWCSSLevel->setTabText(tabWCSSLevel->indexOf(tabexpert), QApplication::translate("this", "Expert", 0, QApplication::UnicodeUTF8));
    tabWEditor->setTabText(tabWEditor->indexOf(tabCSS), QApplication::translate("this", "CSS", 0, QApplication::UnicodeUTF8));
    tabWEditor->setTabText(tabWEditor->indexOf(tabValidator), QApplication::translate("this", "Validator", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(this);
    tabWEditor->setCurrentIndex(3);
    } // retranslateUi

QString MainWindow::readCSSFile(QString path) {
    QString inputFileName = path;
    QString tmpFile;
    //path = "/home/lepagee/dev/webkreator/StyleSheet.css";
    //if(KIO::NetAccess::download(inputFileName, tmpFile, this)) {
      QString line;
      bool inComment = false;
      QFile file(path);
      file.open(QIODevice::ReadOnly);
      while (!file.atEnd()) {
	  line = QString(file.readLine()).toAscii();
	  printf("%s",line.toStdString().c_str());
	  if (line.indexOf("/*") != -1) {
	    inComment = true;
	  }
	  if (line.indexOf("*/") != -1) {
	    inComment = false;
	  }

	  if (inComment == false) {
	    //printf("\n This is what you are looking for: %d,%d",line.right(2).left(1).toStdString().c_str(),line.right(1).toStdString().c_str());
	    //line = line.remove(line.count() -2,2); //Remove UNICODE linefeed
	  }

	  cssFile += line;
      }

      KIO::NetAccess::removeTempFile(tmpFile);
    /*}
    else
    {
      KMessageBox::error(this,
      KIO::NetAccess::lastErrorString());
    }*/


    while (cssFile.indexOf("\n") != -1) {
      cssFile.remove(cssFile.indexOf("\n"),1);
    }
  printf("%s",cssFile.toStdString().c_str());
  return cssFile;
}

QString MainWindow::parseCSS() {
  QString parsedCSS;
  QString tmpCSS = cssFile.trimmed();
  printf("\nThis is the complete text: %s \n\n", tmpCSS.toStdString().c_str());
  if (tmpCSS.isEmpty() == false) {
    while (tmpCSS.count() != 0) {
      if ((tmpCSS.indexOf("/*") != -1) && ((tmpCSS.indexOf("/*") < (tmpCSS.indexOf("{"))))) {
	parsedCSS += tmpCSS.left(tmpCSS.indexOf("*/")+1) + "\n";
	tmpCSS.remove(0,tmpCSS.indexOf("*/")+1);
      }
      else {
	parsedCSS += tmpCSS.left(tmpCSS.indexOf("{")).trimmed() + " {\n";
	tmpCSS.remove(0,tmpCSS.indexOf("{")+1);
	while ((tmpCSS.indexOf(";") < tmpCSS.indexOf("}")) && (tmpCSS.indexOf(";") != -1) && (tmpCSS.indexOf("}") != -1)) {
	    parsedCSS += "	" +tmpCSS.left(tmpCSS.indexOf(";")+1).trimmed() + "\n";
	    tmpCSS.remove(0,tmpCSS.indexOf(";")+1);
	}
      }
	parsedCSS += "}\n\n";
	tmpCSS.remove(0,tmpCSS.indexOf("}")+1);
        tmpCSS = tmpCSS.trimmed();
    }
  }
  printf("\nThis is the file:\n %s \n\n\n\n\n\n\n\n",parsedCSS.toStdString().c_str());
  return parsedCSS;
}

QStringList MainWindow::getClass(QString className) {
  QString parsedCSS = parseCSS();
  QStringList aClass;
  parsedCSS.remove(0,parsedCSS.indexOf(className + " {")+ className.count() +2 );
  parsedCSS.remove(parsedCSS.indexOf("\n}"), parsedCSS.count() - (parsedCSS.indexOf("\n}")-2)).trimmed();
  printf("La class : %s",parsedCSS.trimmed().toStdString().c_str());
  while (parsedCSS.count() != 0) {
    printf("file: %s[/] \n",parsedCSS.toStdString().c_str());
    if ((parsedCSS.indexOf("/*") != -1) && ((parsedCSS.indexOf("/*") < (parsedCSS.indexOf(";"))))) {
      aClass.push_back(parsedCSS.left(parsedCSS.indexOf("*/")+2).trimmed());
      printf("A Line: %s \n",parsedCSS.left(parsedCSS.indexOf("*/")+2).trimmed().toStdString().c_str());
      parsedCSS.remove(0,parsedCSS.indexOf("*/")+2);
    }
    else if (parsedCSS.indexOf(";") == -1)
      parsedCSS.clear();
    else {
      aClass.push_back(parsedCSS.left(parsedCSS.indexOf(";")+1).trimmed());
      printf("\nA Line: %s",parsedCSS.left(parsedCSS.indexOf(";")+1).trimmed().toStdString().c_str());
      parsedCSS.remove(0,parsedCSS.indexOf(";")+1);
    }
    parsedCSS = parsedCSS.trimmed();
  }
  //*printf("\nSA          :%d",aClass.count());
  return aClass;
}

QString MainWindow::getPropriety(QString line) {
  line = line.trimmed();
  line = line.left(line.indexOf(":"));
  line = line.trimmed().toLower();
  return line;
}

QString MainWindow::getValue(QString line) {
  line = line.trimmed();
  QString anUnit = getUnit(line);
  QString line2;
printf("%d",anUnit.count());
  if((anUnit.count() != 0) && (anUnit != "-1")) {
    line2 = line.right(line.count() - line.indexOf(":")-1);
    line2 = line2.trimmed();
    line2 = line2.left(line2.indexOf(anUnit));
  }
  else {
    line2 = line.right(line.count() - line.indexOf(":")-1);
    line2 = line2.trimmed();
    line2 = line2.left(line2.indexOf(";"));
  }
  line2 = line2.trimmed().toLower();
  return line2;
}

QString MainWindow::getUnit(QString line) {
  line = line.trimmed();
  QString anUnit = "-1";
  QSqlQuery query;
  query.exec("SELECT UNIT FROM TCSS_TAG WHERE TITLE = '"+ getPropriety(line) +"'");
  
  while (query.next()) {
    QStringList possibleValues = query.value(0).toString().split(";");
    for (int i =0;i < possibleValues.count(); i++) {
printf("\n Unit: %s  Line: %s %s \n",possibleValues[i].toStdString().c_str(),line.toStdString().c_str(),line.right(possibleValues[i].count()+1).toStdString().c_str());
      if ((possibleValues[i]+";") == line.right(possibleValues[i].count()+1)) {
	anUnit = possibleValues[i];
      }
    }
  }
  return anUnit;
}

void MainWindow::fillCSSBegMode(QString className) {
  QStringList aClass = getClass(className);
  for (int i =0; i < aClass.count(); i++) {
    if (getPropriety(aClass[i]) == "height") {
      cssHeight->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "width") {
      cssWidth->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "text-align") {
      cssText_align->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "text-transform") {
      cssText_transform->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "color") {
      cssColor->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "font-family") {
      cssFont_family->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "font-size") {
      cssFont_size->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "font-style") {
      cssFont_style->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "font-weight") {
      cssFont_weight->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "background-image") {
      cssBackground_image->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "background-color") {
      cssBackground_color->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "background-repeat") {
      cssBackground_repeat->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "border-width") {
      cssBorder_width->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "border-color") {
      cssBorder_color->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "border-style") {
      cssBorder_style->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "float") {
      cssFloat->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "position") {
      cssPosition->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "z-index") {
      cssZ_index->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "margin-top") {
      cssMargin_top->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "margin-bottom") {
      cssMargin_bottom->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "margin-left") {
      cssMargin_left->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "margin-right") {
      cssMargin_right->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "padding-top") {
      cssPadding_top->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "padding-bottom") {
      cssPadding_bottom->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "padding-left") {
      cssPadding_left->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "padding-right") {
      cssPadding_right->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "list-style") {
      cssList_style->fillMe(aClass[i]);
    }
    else if (getPropriety(aClass[i]) == "cursor") {
      cssCursor->fillMe(aClass[i]);
    }
    //else

  }
}

QString MainWindow::setClass(QString className, QString content) {
      QStringList classList = getClassList();
      QString newCSSStyle;
      for (int i =0; i < classList.count();i++) {
	    if (classList[i] != className) {
		  QStringList classPropriety =  getClass(classList[i]);
                  printf("Class = %s, nbpropr = %d, classNb = %d \n", (classList[i]).toStdString().c_str(), classPropriety.count(), classList.count());
		  QString classText = classList[i] + " {";
		  for (int j =0; j < classPropriety.count();j++) {
			classText += classPropriety[j];
		  }
		  classText += "}";
		  newCSSStyle += classText;
	    }
	    else {
		  newCSSStyle += classList[i] + " {\n" + content + "}";
	    }
      }
      
      printf("This is the new file: \n%s\n\n\n\n\n",newCSSStyle.toStdString().c_str());
      return newCSSStyle;
}

QStringList MainWindow::getClassList() {
  QStringList classList;
  QString cssFile = parseCSS();

  while (cssFile.indexOf("{") != -1) {
    while (cssFile.indexOf("{") > cssFile.indexOf("\n")) {
      cssFile.remove(0, cssFile.indexOf("\n")+1);
    }
     
    if (cssFile.at(0) == '	')
      classList.push_back(classList[classList.count()-1] + "^?" + cssFile.left(cssFile.indexOf(" {")).trimmed());
    else
      classList.push_back(cssFile.left(cssFile.indexOf(" {")).trimmed());
    cssFile.remove(0, cssFile.indexOf("\n")+1);
  }

  for (int j =0; j < classList.count(); j++) {
    printf("%s \n",classList[j].toStdString().c_str());
  }

  return classList;
}

void MainWindow::fillCSSAdvMode() {
  QString color1 = "background-color:#7AEEFF;";
  QString color2 ="background-color:#5BFF74;";
  QString color = color1;
  int nbRow =0;
  tblCSSPage->setRowCount(nbRow);
  QStringList classList = getClassList();
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

    QStringList proprietyList = getClass(classList[i]);
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
      lblProprietyName->setText("    "+getPropriety(proprietyList[j])+": ");
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
      query.exec("SELECT VALUE,UNIT FROM TCSS_TAG WHERE TITLE = '"+ getPropriety(proprietyList[j]) +"'");
      
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

void MainWindow::reParse() {
    aParser->debugVector.clear();
    QString aFile = aParser->compressString(rtfHTMLEditor->toPlainText());
    aParser->htmlParser(aFile,true,false,false,treeHtml);
    rtfHTMLEditor->setPlainText(aParser->htmlParser(aFile,false,false,true, NULL));
    lstDebug->clear();
    
    for (int i =0; i < aParser->debugVector.size(); i++) {
      QListWidgetItem* anItem = new QListWidgetItem("["+QString::number(aParser->debugVector[i].line)+"] "+aParser->debugVector[i].message);
      if (aParser->debugVector[i].icon == 0)
        anItem->setIcon(KIcon("dialog-warning"));
      else if (aParser->debugVector[i].icon == 1)
        anItem->setIcon(KIcon("dialog-error"));
      else
        anItem->setIcon(KIcon("dialog-information"));
      lstDebug->addItem(anItem);
    }
}

void MainWindow::templaterize(bool check) {
    if (check == true) {
    //aParser = new HtmlParser();
      std::string aFile = aParser->compressString(rtfHTMLEditor->toPlainText()).toStdString();
      rtfHTMLEditor->setPlainText(aParser->htmlParser(QString::fromStdString(aFile),true,true,true,NULL));
    }
}

void MainWindow::translate() {
    //aParser = new HtmlParser();
    QString aFile = aParser->compressString(rtfHTMLEditor->toPlainText());
    rtfHTMLEditor->setPlainText(aParser->htmlParser(aFile,true,true,true,NULL));
}

void MainWindow::newProject(){
      fileName.clear();
      rtfHTMLEditor->clear();
}
 
void MainWindow::saveProjectAs(const QString &outputFileName, QString input){
      KSaveFile file(outputFileName);
      file.open();
      QByteArray outputByteArray;
      outputByteArray.append(input.toUtf8());
      file.write(outputByteArray);
      file.finalize();
      file.close();

      fileName = outputFileName;
}
 
void MainWindow::saveProjectAs(){
      saveProjectAs(KFileDialog::getSaveFileName(), rtfHTMLEditor->toPlainText());
}
 
void MainWindow::saveProject(){
      if(!fileName.isEmpty()) {
	    saveProjectAs(fileName, rtfHTMLEditor->toPlainText());
      }
      else {
	    saveProjectAs();
      }
}

void MainWindow::saveFile(){ 
      if(!pageName.isEmpty()) {
            saveProjectAs(pageName, rtfHTMLEditor->toPlainText());
      }
      else {
            saveProjectAs();
      }
}

 
void MainWindow::openProject() {
      QString fileNameFromDialog = KFileDialog::getOpenFileName();
      
      QString tmpFile;
      if(KIO::NetAccess::download(fileNameFromDialog, tmpFile,this)) {
	    QFile file(tmpFile);
	    file.open(QIODevice::ReadOnly);
	    //rtfHTMLEditor->setPlainText(QTextStream(&file).readAll());
	    fileName = fileNameFromDialog;
	    
	    KIO::NetAccess::removeTempFile(tmpFile);
            delete aProject;
	    aProject =  new ProjectManager(fileName);
            tableView->setRowCount(aProject->htmlPage.count());
            for (int i = 0;i < aProject->htmlPage.count();i++) {
              QTableWidgetItem* aTableWidget = new QTableWidgetItem(aProject->htmlPage[i]);
              tableView->setItem(i, 0, aTableWidget);
              //tableView->setToolTip(thisJob.c_str());
            }
            QString aFile = aParser->compressFile(aProject->htmlPage[0]);
            pageName = aProject->htmlPage[0];
            rtfHTMLEditor->setPlainText(aParser->htmlParser(aFile,true, false,true,NULL));
            //cout << aProject->cssPage.toStdString(); exit(33);
            readCSSFile("/home/lepagee/dev/myproject/kimberlite/StyleSheet.css");
            //readCSSFile(aProject->cssPage);
            rtfCSSEditor->setText(parseCSS());
            styleSheetName = new  QTreeWidgetItem(treeWidget);
            styleSheetName->setText(0,aProject->cssPage);
            //treeWidget->append(new QTreeWidgetItem((QTreeWidget*)0, append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1"))));));
            fillCSSAdvMode();
            QStringList classList = getClassList();
            
            updateClassTree();
	    if (classList.count() > 0) {
	      //currentClassName = classList[0];
	      getClass(classList[0]);
	      fillCSSBegMode(classList[0]);
	    }
            /*for (int j = 0; j < classList.count(); j++) {
              QTreeWidgetItem* aTreeViewWidgetItem = new  QTreeWidgetItem(styleSheetName);
              aTreeViewWidgetItem->setText(0,classList[j]);
            }*/
            
	disableWidget(true);
      }
      else {
	    KMessageBox::error(this,KIO::NetAccess::lastErrorString());
      }
}

void MainWindow::showPageList(bool state) {
  if (state == true ) {
    tableDock->setHidden(false);
  }
  else {
    tableDock->setHidden(true);
  }
}
void MainWindow::showCSS(bool state) {
  if (state == true ) {
    treeDock->setHidden(false);
  }
  else {
    treeDock->setHidden(true);
  }
}

void MainWindow::showDebugger(bool state) {
  if (state == true ) {
    dockDebug->setHidden(false);
  }
  else {
    dockDebug->setHidden(true);
  }
}

void MainWindow::setupToolTip() {

   KTipDatabase* aTipDB = new KTipDatabase();

   KTipDialog* aTipDialog = new KTipDialog(aTipDB, this);
   aTipDialog->setShowOnStart(true);
   aTipDialog->showTip(this, QString(), true);

}

void MainWindow::loadCSSClass(QTreeWidgetItem* anItem) {
  QString newStyle = setClass(currentClassName, clearCssBeg());
  QString className;
  QTreeWidgetItem* currentLevel = anItem;
  while (currentLevel != styleSheetName) {
    if (currentLevel->text(0)[0] != ':')
      className.insert(0, " ");
    className.insert(0, currentLevel->text(0));
    currentLevel = currentLevel->parent();
  }
  cout << "/" << className.toStdString() << "/" << endl;
  className = className.trimmed();
  currentClassName = className;
  fillCSSBegMode(currentClassName);
  cout << newStyle.toStdString() <<endl;
  cssFile = newStyle;
  saveProjectAs(aProject->cssPage , newStyle);
  rtfCSSEditor->setText(parseCSS());
}

void MainWindow::loadPage(QTableWidgetItem* anItem) {
  cout << anItem->text().toStdString() << endl;
  if (isModified == true)
    saveFile();
  pageName = anItem->text();
  isModified = false;
  QString aFile = aParser->compressFile(pageName);
  rtfHTMLEditor->setPlainText(aParser->htmlParser(aFile,true, false,true,NULL));
}

void MainWindow::loadPage(QTreeWidgetItem* item, QString text) {
  printf("\nI am in mainwindow::loadPage text:%s \n",text.toStdString().c_str());
  /*if ((isModified == true) && (currentHTMLPage != NULL)) {
    
  }*/
  isModified = false;
  QString aFile = aParser->compressString(text.trimmed());
  rtfHTMLEditor->setPlainText(aParser->htmlParser(aFile,true, false,true,NULL));
  aParser->htmlParser(aFile,true,false,false,treeHtml);
}

void MainWindow::setModified() {
  isModified = true;
}

void MainWindow::addHtmlPage() {
     bool ok;
     QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                          tr("User name:"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);
     if (ok && !text.isEmpty()) {
         tableView->setRowCount(tableView->rowCount()+1);
	 QTableWidgetItem* aTableWidget = new QTableWidgetItem(text);
	 tableView->setItem(tableView->rowCount()-1, 0, aTableWidget);
     }
}


QString MainWindow::clearCssBeg() {
    QString currentClass;
    /*if (->isEnabled()) {
     currentClass += ->ckbName->text()+" "+ ->cbbValue->currentText();
     if (->isEnabled()) {
	    currentClass += ->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }*/

    if (cssHeight->cbbValue->isEnabled()) {
     currentClass += "	height: "+ cssHeight->cbbValue->currentText();
     if (cssHeight->isEnabled()) {
	    currentClass += cssHeight->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssHeight->clear();
    if (cssWidth->cbbValue->isEnabled()) {
     currentClass += "	width: "+ cssWidth->cbbValue->currentText().toAscii();
     if (cssWidth->cbbUnit->isEnabled()) {
	    currentClass += cssWidth->cbbUnit->currentText().toAscii();
     }
     currentClass += ";\n";
    }
    cssWidth->clear();
    if (cssText_align->cbbValue->isEnabled()) {
     currentClass += "	text-align: "+ cssText_align->cbbValue->currentText();
     if (cssText_align->cbbUnit->isEnabled()) {
	    currentClass += cssText_align->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssText_align->clear();
    if (cssText_transform->cbbValue->isEnabled()) {
     currentClass += "	text-transform: "+ cssText_transform->cbbValue->currentText();
     if (cssText_transform->cbbUnit->isEnabled()) {
	    currentClass += cssText_transform->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssText_transform->clear();
    if (cssColor->cbbValue->isEnabled()) {
     currentClass += "	color: "+ cssColor->cbbValue->currentText();
     if (cssColor->cbbUnit->isEnabled()) {
	    currentClass += cssColor->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssColor->clear();
    if (cssFont_family->cbbValue->isEnabled()) {
     currentClass += "	font-family: "+ cssFont_family->cbbValue->currentText();
     if (cssFont_family->cbbUnit->isEnabled()) {
	    currentClass += cssFont_family->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssFont_family->clear();
    if (cssFont_size->cbbValue->isEnabled()) {
     currentClass += "	font-size: "+ cssFont_size->cbbValue->currentText();
     if (cssFont_size->cbbUnit->isEnabled()) {
	    currentClass += cssFont_size->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssFont_size->clear();
    if (cssFont_style->cbbValue->isEnabled()) {
     currentClass += "	font-style: "+ cssFont_style->cbbValue->currentText();
     if (cssFont_style->cbbUnit->isEnabled()) {
	    currentClass += cssFont_style->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssFont_style->clear();
    if (cssFont_weight->cbbValue->isEnabled()) {
     currentClass += "	font-weight: "+ cssFont_weight->cbbValue->currentText();
     if (cssFont_weight->cbbUnit->isEnabled()) {
	    currentClass += cssFont_weight->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssFont_weight->clear();
    if (cssBackground_image->cbbValue->isEnabled()) {
     currentClass += "	background-image: "+ cssBackground_image->cbbValue->currentText();
     if (cssBackground_image->cbbUnit->isEnabled()) {
	    currentClass += cssBackground_image->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssBackground_image->clear();
    if (cssBackground_color->cbbValue->isEnabled()) {
     currentClass += "	background-color: "+ cssBackground_color->cbbValue->currentText();
     if (cssBackground_color->cbbUnit->isEnabled()) {
	    currentClass += cssBackground_color->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssBackground_color->clear();
    if (cssBackground_repeat->cbbValue->isEnabled()) {
     currentClass += "	background-repeat: "+ cssBackground_repeat->cbbValue->currentText();
     if (cssBackground_repeat->cbbUnit->isEnabled()) {
	    currentClass += cssBackground_repeat->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssBackground_repeat->clear();
    if (cssBorder_width->cbbValue->isEnabled()) {
     currentClass += "	border-width: "+ cssBorder_width->cbbValue->currentText();
     if (cssBorder_width->cbbUnit->isEnabled()) {
	    currentClass += cssBorder_width->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssBorder_width->clear();
    if (cssBorder_color->cbbValue->isEnabled()) {
     currentClass += "	border-color: "+ cssBorder_color->cbbValue->currentText();
     if (cssBorder_color->cbbUnit->isEnabled()) {
	    currentClass += cssBorder_color->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssBorder_color->clear();
    if (cssBorder_style->cbbValue->isEnabled()) {
     currentClass += "	border-style: "+ cssBorder_style->cbbValue->currentText();
     if (cssBorder_style->cbbUnit->isEnabled()) {
	    currentClass += cssBorder_style->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssBorder_style->clear();
    if (cssFloat->cbbValue->isEnabled()) {
     currentClass += "	float: "+ cssFloat->cbbValue->currentText();
     if (cssFloat->cbbUnit->isEnabled()) {
	    currentClass += cssFloat->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssFloat->clear();
    if (cssPosition->cbbValue->isEnabled()) {
     currentClass += "	position: "+ cssPosition->cbbValue->currentText();
     if (cssPosition->cbbUnit->isEnabled()) {
	    currentClass += cssPosition->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssPosition->clear();
    if (cssZ_index->cbbValue->isEnabled()) {
     currentClass += "	z-index: "+ cssZ_index->cbbValue->currentText();
     if (cssZ_index->cbbUnit->isEnabled()) {
	    currentClass += cssZ_index->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssZ_index->clear();
    if (cssMargin_top->cbbValue->isEnabled()) {
     currentClass += "	margin-top: "+ cssMargin_top->cbbValue->currentText();
     if (cssMargin_top->cbbUnit->isEnabled()) {
	    currentClass += cssMargin_top->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssMargin_top->clear();
    if (cssMargin_bottom->cbbValue->isEnabled()) {
     currentClass += "	margin-bottom: "+ cssMargin_bottom->cbbValue->currentText();
     if (cssMargin_bottom->cbbUnit->isEnabled()) {
	    currentClass += cssMargin_bottom->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssMargin_bottom->clear();
    if (cssMargin_left->cbbValue->isEnabled()) {
     currentClass += "	margin-left: "+ cssMargin_left->cbbValue->currentText();
     if (cssMargin_left->cbbUnit->isEnabled()) {
	    currentClass += cssMargin_left->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssMargin_left->clear();
    if (cssMargin_right->cbbValue->isEnabled()) {
     currentClass += "	margin-right: "+ cssMargin_right->cbbValue->currentText();
     if (cssMargin_right->cbbUnit->isEnabled()) {
	    currentClass += cssMargin_right->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssMargin_right->clear();
    if (cssPadding_top->cbbValue->isEnabled()) {
     currentClass += "	padding-top: "+ cssPadding_top->cbbValue->currentText();
     if (cssPadding_top->cbbUnit->isEnabled()) {
	    currentClass += cssPadding_top->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssPadding_top->clear();
    if (cssPadding_bottom->cbbValue->isEnabled()) {
     currentClass += "	padding-bottom: "+ cssPadding_bottom->cbbValue->currentText();
     if (cssPadding_bottom->cbbUnit->isEnabled()) {
	    currentClass += cssPadding_bottom->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssPadding_bottom->clear();
    if (cssPadding_left->cbbValue->isEnabled()) {
     currentClass += "	padding-left: "+ cssPadding_left->cbbValue->currentText();
     if (cssPadding_left->cbbUnit->isEnabled()) {
	    currentClass += cssPadding_left->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssPadding_left->clear();
    if (cssPadding_right->cbbValue->isEnabled()) {
     currentClass += "	padding-right: "+ cssPadding_right->cbbValue->currentText();
     if (cssPadding_right->cbbUnit->isEnabled()) {
	    currentClass += cssPadding_right->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssPadding_right->clear();
    if (cssList_style->cbbValue->isEnabled()) {
     currentClass += "	list-style: "+ cssList_style->cbbValue->currentText();
     if (cssList_style->cbbUnit->isEnabled()) {
	    currentClass += cssList_style->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssList_style->clear();
    if (cssCursor->cbbValue->isEnabled()) {
     currentClass += "	cursor: "+ cssCursor->cbbValue->currentText();
     if (cssCursor->cbbUnit->isEnabled()) {
	    currentClass += cssCursor->cbbUnit->currentText();
     }
     currentClass += ";\n";
    }
    cssCursor->clear();
    
    printf("Previous Class : %s \n",currentClass.toStdString().c_str());
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
  if ((previousCssMode == 0) && (mode ==2) && (treeWidget->currentItem() != NULL)) {
    loadCSSClass(treeWidget->currentItem());
  }
  else if (mode == 1) {
    
  }
}

void MainWindow::updateClassTree() {
  QStringList classList = getClassList();
  if (classList.count() > 0) {
    currentClassName = classList[0];
    getClass(classList[0]);
    fillCSSBegMode(classList[0]);
  }
  for (int j = 0; j < classList.count(); j++) {
    /*QTreeWidgetItem* aTreeViewWidgetItem = new  QTreeWidgetItem(styleSheetName);
    aTreeViewWidgetItem->setText(0,classList[j]);*/
    splitSubClass(classList[j], styleSheetName);
  }
  treeWidget->expandAll();
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
    anIcon = new KIcon("/home/lepagee/dev/myproject/kimberlite/pixmap/tag.png");
  }
  else if (text[0] == ':') {
    anIcon = new KIcon("/home/lepagee/dev/myproject/kimberlite/pixmap/state.png");
  }
  else if (text[0] == '#') {
    anIcon = new KIcon("/home/lepagee/dev/myproject/kimberlite/pixmap/id.png");
  }
  else if (text[0] == '.') {
    anIcon = new KIcon("/home/lepagee/dev/myproject/kimberlite/pixmap/class.png");
  }
  else {
    anIcon = new KIcon("/home/lepagee/dev/myproject/kimberlite/pixmap/tag.png");
  }
  //anIcon->setPixmap(anIcon->pixmap(QSize(16,32)));
  return *anIcon;
}

void MainWindow::loadScript(QTreeWidgetItem* anItem, QString text) {
  printf("Load script\n");
  if ((currentScript != NULL) && (anItem != currentScript)) {
    //aProjectManager->getDomElement(anItem).firstChildElement().setText(rtfScriptEditor->text());
    QDomText newTitleText = aProjectManager->getDomDocument()->createTextNode(rtfScriptEditor->toPlainText());
    //aProjectManager->getDomElement(anItem).dropFirstChildElement();
    aProjectManager->getDomElement(anItem).firstChildElement().text().clear();
    aProjectManager->getDomElement(anItem).firstChildElement().clear();
    //aProjectManager->getDomElement(anItem).removeChildElements();
    aProjectManager->getDomElement(anItem).firstChildElement().appendChild(newTitleText);
    rtfScriptEditor->setPlainText(text.trimmed());
  }
  currentScript = anItem;
  /*rtfScriptEditor->setPlainText(text.trimmed());
  currentScript = anItem;*/
}

void MainWindow::loadCss(QString text) {
  printf("Load css\n");
  cssFile = text;
  rtfCSSEditor->setPlainText(parseCSS());
  //fillCSSAdvMode();
  styleSheetName = new  QTreeWidgetItem(treeWidget);
  styleSheetName->setText(0,"Style");
  updateClassTree();
  //fillCSSAdvMode();
}