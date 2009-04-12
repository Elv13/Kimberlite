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

#ifndef UI_MAINWINDOW
#define UI_MAINWINDOW

#include "src/htmlParser.h"
#include "src/cssParser.h"

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
#include <QFontComboBox>
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
#include <QHash>
#include <QAction>
#include <KActionCollection>

#include "src/CSSbeginnerWidget.h"
#include "src/rtfCssEditor.h"
#include "src/rtfHtmlEditor.h"
#include "src/ProjectManager_v2.h"

QT_BEGIN_NAMESPACE

class IndexedTreeWidgetItem : public QTreeWidgetItem {
  public:
    IndexedTreeWidgetItem(QTreeWidget* parent, uint id) :QTreeWidgetItem(parent),index(id) {}
    IndexedTreeWidgetItem(QTreeWidgetItem* parent, const QStringList &strings, uint id) :QTreeWidgetItem(parent,strings),index(id){}
    IndexedTreeWidgetItem(QTreeWidget* parent, const QStringList &strings, uint id) :QTreeWidgetItem(parent,strings),index(id){}
    IndexedTreeWidgetItem(const QStringList &strings, uint id) :QTreeWidgetItem (strings),index(id) {}
    uint index;
};

class MainWindow  : public KMainWindow {
   Q_OBJECT
public:
    MainWindow(QWidget* parent);
    ~MainWindow();
    void retranslateUi();
    QString readCSSFile(QString path);
    void fillCSSBegMode(QString className);
    void fillCSSAdvMode();
    KActionCollection* actionCollection;
    QWidget *centralwidget;
    KTabWidget *tabWMenu;
    QWidget *menufile;
    QWidget *menuEdit;
    QWidget *menuView;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *vlTextAtribute;
    QHBoxLayout *hlFont;
    QComboBox *cbbFont;
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
    QGridLayout *vlColor;
    //QHBoxLayout *hlTextColor;
    QLabel *lblTextColor;
    KColorCombo *kcbbTextColor;
    //QHBoxLayout *hlHighlightColor;
    QLabel *lblHighlightColor;
    KColorCombo *cbbHighlightColor;
    //QHBoxLayout *hlBackgroundColor;
    QLabel *lblBackgroundColor;
    KColorCombo *cbbBackgroundColor;
    QWidget *menuInsert;
    QGridLayout* hlInsert;
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
    //QTableWidget *tableView;
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
    //QWidget *tabValidator;
    //QVBoxLayout *verticalLayout_16;
    //QWebView *webValidator;
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
    QStringList cssBegTagList;
    QHash<QString, CSSBeginnerWidget*> ashCssBeg;
    QHash<QString, QAction*> ashActions;
    ProjectManager2* aProjectManager;
    QTreeWidgetItem* currentScript;

  private:
    HtmlParser* aParser;
    QString fileName;
    bool isModified;
    QString pageName;
    QString currentClassName;
    //QString setClass(QString className, QString content);
    QString clearCssBeg();
    void disableWidget(bool value);
    void splitSubClass(QString name, QTreeWidgetItem* parent);
    KIcon getRightIcon(QString text);
    void updateHtmlTree(QString &file);
    void updateHtmlTree(HtmlData &pageData);
    int previousCssMode;
    QTreeWidgetItem* currentHTMLPage;
    void disableWysiwyg(bool value);
    KAction* createAction(QString name, QString icon, QKeySequence shortcut, bool checkable = false);

  private slots:
    void setupToolTip();
    void reParse();
    void templaterize();
    void translate();
    void newProject(); 
    void openProject();
    void saveProject();
    void saveProjectAs();
    void saveProjectAs(const QString &outputFileName, QString input);
    void saveFile();
    void loadCSSClass(QTreeWidgetItem* anItem);
    void setHtmlCursor(QTreeWidgetItem* item, int column);
    void setModified();
    void changeCssMode(int mode);
    void updateClassTree();
    void debugHtml();
    
    void showPageList(bool);
    void showCSS(bool);
    void showHtml(bool state);
    void showDebugger(bool);
    void showInspector(bool state);
    void addHtmlPage();
    void addScript();
    void zoomIn();
    void zoomOut();
    void zoomDefault();
    void modeChanged(int index);
    
    void setBold();
    void setItalic();
    void setUnderline();
    void setAlignCenter();
    void setAlignRight();
    void setAlignLeft();
    void setJustify();
    void execCommand(const QString &cmd, const QString &arg);
    void addTag(QString prefix, QString suffix);
    void addTag(QString prefix, QString suffix, QString cmd, QString arg);
    void setHeader(QString text);
    void setFont(QString text);
    void setFontSize(int size);
    void setBackgroundColor();
    void setHighlightColor();
    void setTextColor();
    void setUList();
    void insertImage();
    void insertTable();
    void insertLink();
    void insertChar();
    
    void editToolbar();
    void editShortcut();
    void aboutKimberlite();
    void reportBug();
    
  public slots:
    void loadPage(QTreeWidgetItem* item, QString text);
    void loadScript(QTreeWidgetItem* anItem, QString text);
    void loadCss(QString text);

};


QT_END_NAMESPACE

#endif // UI_MAINWINDOW
