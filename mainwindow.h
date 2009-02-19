/**     @file mainwindow.h

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

#ifndef UI_MAINWINDOW_20__2D__20_UNTITLED_H
#define UI_MAINWINDOW_20__2D__20_UNTITLED_H

#include "src/htmlParser.h"

#include <QtCore/QVariant>
#include <KXmlGuiWindow>
#include <QtGui/QAction>
#include <KApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>
#include <QDomDocument>
#include "kcolorcombo.h"
#include "kcombobox.h"
#include "knuminput.h"
#include "kpushbutton.h"
#include "ktabwidget.h"
#include "ktextedit.h"
#include <ktoolbar.h>
#include <kaction.h>
#include <KActionCollection>
#include <KLocale>
#include <QSqlDatabase>
#include <QSyntaxHighlighter>
#include <QtSql>
#include <QListWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QCompleter>

#include "src/CSSbeginnerWidget.h"
#include "src/rtfCssEditor.h"
#include "src/rtfHtmlEditor.h"
#include "src/ProjectManager.h"
#include "src/ProjectManager_v2.h"

QT_BEGIN_NAMESPACE

class MainWindow  : public KXmlGuiWindow {
   Q_OBJECT
public:
    MainWindow(QWidget* parent);
    void retranslateUi();
    QString readCSSFile(QString path);
    QString parseCSS();
    QStringList getClass(QString className);
    QStringList getClassList();
    static QString getPropriety(QString line);
    static QString getValue(QString line);
    static QString getUnit(QString line);
    void fillCSSBegMode(QString className);
    void fillCSSAdvMode();
    ProjectManager* aProject;
    QWidget *centralwidget;
    //QVBoxLayout *verticalLayout;
    KTabWidget *tabWMenu;
    QWidget *menufile;
    QWidget *menuEdit;
    QWidget *menuView;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *vlTextAtribute;
    QHBoxLayout *hlFont;
    KComboBox *cbbFont;
    KIntSpinBox *cbbFontSize;
    QHBoxLayout *hlTextAtributeButton;
    KPushButton *btnBold;
    KPushButton *btnUnderline;
    KPushButton *btnItalic;
    QFrame *line;
    KPushButton *btnAlignLeft;
    KPushButton *btnAlignCenter;
    KPushButton *btnAlignRight;
    KPushButton *btnJustify;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *vlOther;
    KComboBox *cbbHeader;
    QHBoxLayout *hlOther;
    KPushButton *btnLink;
    KPushButton *btnChar;
    KPushButton *btnTable;
    KPushButton *btnList;
    QVBoxLayout *vlColor;
    QHBoxLayout *hlTextColor;
    QLabel *lblTextColor;
    KColorCombo *kcbbTextColor;
    QHBoxLayout *hlHighlightColor;
    QLabel *lblHighlightColor;
    KColorCombo *cbbHighlightColor;
    QHBoxLayout *hlBackgroundColor;
    QLabel *lblBackgroundColor;
    KColorCombo *cbbBackgroundColor;
    QWidget *menuInsert;
    QHBoxLayout* hlInsert;
    KPushButton* btnNewLine;
    KPushButton* btnNewTab;
    KPushButton* btnNewSpace;
    QVBoxLayout* vlSpacing;
    QWidget *menuTools;
    QWidget *menuOptions;
    QWidget *menuHelp;
    QHBoxLayout *mainLayout;
    KTabWidget *ktabwidget_2;
    QWidget *tab_2;
    QDockWidget *tableDock;
    QWidget *tableDockCentral;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *tableView;
    QHBoxLayout *hlButton;
    KPushButton *btnTableAdd;
    KPushButton *btnTableRemove;
    QDockWidget *treeDock;
    QWidget *treeDockCentral;
    QVBoxLayout *verticalLayout_5;
    QTreeWidget *treeWidget;
    QHBoxLayout *hlButton2;
    KPushButton *btnTreeAdd;
    KPushButton *btnTreeRemove;
    QWidget *centralWidget2;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWEditor;
    QWidget *tabPreview;
    QHBoxLayout *horizontalLayout_2;
    QWebView *webPreview;
    QWidget *tabHTML;
    QVBoxLayout *verticalLayout_3;
    RtfHtmlEditor* rtfHTMLEditor;
    QPushButton *btnParse;
    QDockWidget *dockDebug;
    QListWidget* lstDebug;
    QVBoxLayout *verticalLayout_99;
    QWidget *dockDebugContents;
    QWidget *tabScripts;
    QVBoxLayout *verticalLayout_7;
    KTextEdit *rtfScriptEditor;
    QWidget *tabCSS;
    QVBoxLayout *verticalLayout_6;
    QTabWidget *tabWCSSLevel;
    QWidget *tabBeginner;
    QVBoxLayout *verticalLayout_8;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_10;
    QGroupBox *grbSize;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *grbText;
    QVBoxLayout *verticalLayout_11;
    QGroupBox *grbBackground;
    QVBoxLayout *verticalLayout_12;
    QGroupBox *grbBorder;
    QVBoxLayout *verticalLayout_13;
    QGroupBox *grbLayout;
    QVBoxLayout *verticalLayout_14;
    QGroupBox *grbOther;
    QVBoxLayout *verticalLayout_15;
    QWidget *tabAdvanced;
    QVBoxLayout *verticalLayout_18;
    QTableWidget *tblCSSPage;
    QHBoxLayout *hbButton2;
    QPushButton *btnaddClass;
    QPushButton *btnAddPClass;
    QPushButton *btnRemoveClass;
    QSpacerItem *horizontalSpacer;
    QWidget *tabexpert;
    QVBoxLayout *verticalLayout_17;
    RtfCssEditor *rtfCSSEditor;
    QCompleter* cssCompleter;
    //QCompleter* htmlCompleter;
    QWidget *tabValidator;
    QVBoxLayout *verticalLayout_16;
    QWebView *webValidator;
    QTreeWidgetItem* styleSheetName;
    QStatusBar *statusbar;
    QTextEdit* txtOtherTags;
    QDockWidget* dockHtmlTree;
    QTreeWidget* treeHtml;
    KToolBar* fileTB;
    KToolBar* editTB;
    KToolBar* viewTB;
    KToolBar* insertTB;
    KToolBar* toolsTB;
    KToolBar* optionsTB;
    KToolBar* helpTB;
    QSqlDatabase* db;
    CSSBeginnerWidget* cssHeight;
    CSSBeginnerWidget* cssWidth;
    CSSBeginnerWidget* cssText_align;
    CSSBeginnerWidget* cssText_transform;
    CSSBeginnerWidget* cssColor;
    CSSBeginnerWidget* cssFont_family;
    CSSBeginnerWidget* cssFont_size;
    CSSBeginnerWidget* cssFont_style;
    CSSBeginnerWidget* cssFont_weight;
    CSSBeginnerWidget* cssBackground_image;
    CSSBeginnerWidget* cssBackground_color;
    CSSBeginnerWidget* cssBackground_repeat;
    CSSBeginnerWidget* cssBorder_width;
    CSSBeginnerWidget* cssBorder_color;
    CSSBeginnerWidget* cssBorder_style;
    CSSBeginnerWidget* cssFloat;
    CSSBeginnerWidget* cssPosition;
    CSSBeginnerWidget* cssZ_index;
    CSSBeginnerWidget* cssMargin_top;
    CSSBeginnerWidget* cssMargin_bottom;
    CSSBeginnerWidget* cssMargin_left;
    CSSBeginnerWidget* cssMargin_right;
    CSSBeginnerWidget* cssPadding_top;
    CSSBeginnerWidget* cssPadding_bottom;
    CSSBeginnerWidget* cssPadding_left;
    CSSBeginnerWidget* cssPadding_right;
    CSSBeginnerWidget* cssList_style;
    CSSBeginnerWidget* cssCursor;
    QString cssFile;
    ProjectManager2* aProjectManager;
    QTreeWidgetItem* currentScript;

  private:
    HtmlParser* aParser;
    QString fileName;
    bool isModified;
    QString pageName;
    QString currentClassName;
    QString setClass(QString className, QString content);
    QString clearCssBeg();
    void disableWidget(bool value);
    void splitSubClass(QString name, QTreeWidgetItem* parent);
    KIcon getRightIcon(QString text);
    void updateHtmlTree(QString file);
    int previousCssMode;
    QTreeWidgetItem* currentHTMLPage;


  private slots:
    void setupToolTip();
    void reParse();
    void templaterize(bool check);
    void translate();
    void newProject(); 
    void openProject();
    void saveProject();
    void saveProjectAs();
    void saveProjectAs(const QString &outputFileName, QString input);
    void saveFile();
    void loadCSSClass(QTreeWidgetItem* anItem);
    void loadPage(QTableWidgetItem* anItem);
    void setModified();
    void changeCssMode(int mode);
    void updateClassTree();
    
    void showPageList(bool);
    void showCSS(bool);
    void showDebugger(bool);
    void addHtmlPage();
    void modeChanged(int index);
  public slots:
    void loadPage(QTreeWidgetItem* item, QString text);
    void loadScript(QTreeWidgetItem* anItem, QString text);
    void loadCss(QString text);

};


QT_END_NAMESPACE

#endif // UI_MAINWINDOW_20__2D__20_UNTITLED_H
