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
#include "src/phpParser.h"

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
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>
#include <QDomDocument>
#include <QFontComboBox>
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
#include <QProgressBar>
#include <KIO/NetAccess>
#include <KFileDialog>
#include <KMessageBox>
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
#include <ktip.h>
#include <QPrintDialog>
#include <KPrintPreview>
#include <QPrinter>
#include <KFontComboBox>
#include <KSqueezedTextLabel>

#include "src/CSSbeginnerWidget.h"
#include "src/rtfCssEditor.h"
#include "src/rtfHtmlEditor.h"
#include "src/ProjectManager_v2.h"
#include "src/htmlThread.h"
#include "src/colorComboBox.h"
#include "src/headerComboBox.h"
#include "src/configSkeleton.h"
#include "src/tagEditor.h"
#include "src/cssPreviewWidget.h"
#include "src/config.h"

#define KIMBERLITE_MODE tabWEditor->currentIndex()
#define CSS_MODE tabWCSSLevel->currentIndex()
#define CSS_MODE_BEG 0 
#define CSS_MODE_EXPERT 1
#define MODE_WYSIWYG 0
#define MODE_HTML 1
#define MODE_SCRIPT 2
#define MODE_CSS 3

QT_BEGIN_NAMESPACE


class MainWindow  : public KMainWindow {
   Q_OBJECT
public:
    MainWindow(QWidget* parent);
    ~MainWindow();
    void retranslateUi();
    void fillCSSBegMode(QString className);
    static KIcon getRightIcon(QString text);
    static void splitSubClass(QString name, QTreeWidgetItem* parent);
    KActionCollection* actionCollection;
    QWidget *centralwidget;
    KTabWidget *tabWMenu;
    QWidget *menuEdit;
    QGridLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *vlTextAtribute;
    QHBoxLayout *hlFont;
    //KFontComboBox *cbbFont;
    QComboBox* cbbFont;
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
    //HeaderComboBox *cbbHeader;
    QComboBox* cbbHeader;
    QHBoxLayout *hlOther;
    KPushButton *btnLink;
    KPushButton *btnChar;
    KPushButton *btnTable;
    //KPushButton *btnList;
    QGridLayout *vlColor;
    QLabel *lblTextColor;
    ColorComboBox *kcbbTextColor;
    QLabel *lblHighlightColor;
    ColorComboBox *cbbHighlightColor;
    QLabel *lblBackgroundColor;
    ColorComboBox *cbbBackgroundColor;
    QWidget *menuInsert;
    QGridLayout* hlInsert;
    KPushButton* btnIndent;
    KPushButton* btnUnIndent;
    KPushButton* btnNewLine;
    KPushButton* btnNewTab;
    KPushButton* btnNewSpace;
    KPushButton* btnHr;
    KPushButton* btnTextLine;
    KPushButton* btnPassword;
    KPushButton* btnCheckBox;
    KPushButton* btnRadioButton;
    KPushButton* btnSubmit;
    KPushButton* btnReset;
    KPushButton* btnUpload;
    KPushButton* btnHidden;
    KPushButton* btnButton;
    KPushButton* btnTextAera;
    KPushButton* btnHtmlButton;
    KPushButton* btnSelect;
    KPushButton* btnList;
    KPushButton* btnLabel;
    QWidget *menuTools;
    KTabWidget *ktabwidget_2;
    QWidget *tab_2;
    QDockWidget *tableDock;
    QHBoxLayout *hlButton;
    KPushButton *btnTableAdd;
    KPushButton *btnTableRemove;
    QDockWidget *treeDock;
    QWidget *treeDockCentral;
    QGridLayout *verticalLayout_5;
    QTreeWidget *treeWidget;
    QHBoxLayout *hlButton2;
    KPushButton *btnTreeAdd;
    KPushButton *btnTreeRemove;
    QTabWidget *tabWEditor;
    QWebView *webPreview;
    QWidget *tabHTML;
    QVBoxLayout *verticalLayout_3;
    RtfHtmlEditor* rtfHTMLEditor;
    int intRtfHtmlPos;
    QPushButton *btnParse;
    TagEditor* aTagEditor;
    QDockWidget *dockDebug;
    QListWidget* lstDebug;
    QGridLayout *verticalLayout_99;
    QWidget *dockDebugContents;
    KTextEdit *rtfScriptEditor;
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
    //QWidget *tabexpert;
    RtfCssEditor *rtfCSSEditor;
    QCompleter* cssCompleter;
    QTreeWidgetItem* styleSheetName;
    QTextEdit* txtOtherTags;
    QDockWidget* dockHtmlTree;
    QTreeWidget* treeHtml;
    CssPreviewWidget* cssPreview;
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
    QTreeWidgetItem* currentScript;

  private:
    HtmlParser* aParser;
    QString fileName;
    bool isModified;
    QString pageName;
    QString currentClassName;
    ParserThread* aHtmlThread;
    int previousCssMode;
    int previousKimberliteMode;
    QTreeWidgetItem* currentHTMLPage;
    QLabel* lblStatusBar1;
    QLabel* lblStatusBar2;
    KSqueezedTextLabel* lblStatusBar3;
    QProgressBar* pbStatusBar;
    QString clearCssBeg();
    void disableWidget(bool value);
    QString setupTmpDir(bool initial=false);
    void disableWysiwyg(bool value);
    QString getClassName(QTreeWidgetItem* anItem);
    QTreeWidgetItem* getClassWidget(QString className);
    void setCssCursor(QString className);
    void loadCSSClass(QTreeWidgetItem* anItem);
    void saveRecentProject(QString filePath, QString projectTitle);
    void loadDefaultPage();
    KPushButton* createToolButton(QWidget* parent, QString icon, QString toolTip="", bool checkable=false);
    QFrame* createSpacer();
    KAction* createAction(QString name, QString icon, QKeySequence shortcut, bool checkable = false);
    QTextEdit* getCurrentEditor();
    KimberliteConfigSkeleton configSkeleton;
    
  private slots:
    void defaultPageLinkClicked(const QUrl& url);
    void quit();
    void print();
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    void find();
    void printPreview();
    void reParse();
    void templaterize();
    void translate();
    void replaceHtml(QString newHtml);
    void newProject(); 
    void newProject(QString name, QString filePath);
    void openProject();
    void saveProject();
    void saveProjectAs();
    void saveProjectAs(const QString &outputFileName);
    void saveFile();
    void exportProject();
    
    void cssClassClicked(QTreeWidgetItem* anItem);    
    void addClasses();
    void setHtmlCursor(QTreeWidgetItem* item);
    void setModified();
    void changeCssMode(int mode);
    //void updateClassTree();
    void debugHtml();
    
    void showInspector(bool state);
    void addHtmlPage();
    void addScript();
    void zoomIn();
    void zoomOut();
    void zoomDefault();
    void modeChanged(int index);
    void loading(int value);
    void linkHovered(QString link);
    void webPageCursorMoved();
    
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
    void setBackgroundColor(QColor aColor = QColor());
    void setHighlightColor(QColor aColor = QColor());
    void setTextColor(QColor aColor = QColor());
    void setUList();
    void setOList();
    void insertImage(QString path = "");
    void insertTable(QString html =  "");
    void insertLink(QString path = "");
    void insertChar();
    void addAnchor();
    void addTextLine();
    void addPasswordLine();
    void addCheckBox();
    void addRadioButton();
    void addSubmitButton();
    void addResetButton();
    void addUploadButton();
    void addHiddenField();
    void addButton();
    void addTextAera();
    void addHtmlButton();
    void addSelect();
    void addListControl();
    void addLabel();
    void addNewLine();
    void addNewTab();
    void addNewSpace();
    void addAddHr();
    void addIndent();
    void addOutdent();
    
    void editToolbar();
    void editShortcut();
    void editPref();
    void aboutKimberlite();
    void reportBug();
    void cursorChanged();
    void updateHtmlTree(IndexedTreeWidgetItem* topItem,bool clear);
    void updateCssTree(QTreeWidgetItem* topItem, bool clear);
    void updateCssFile(int forceMode = 99, int forceCssMode = 99);
    
  public slots:
    void loadPage(QTreeWidgetItem* item, QString text, bool force = false);
    void loadScript(QTreeWidgetItem* anItem, QString text);
    void loadCss(QString text);
    void openProject(QString fileName);
    
  private:
    /*enum KimberliteMode2 { //TODO make it work
      WYSIWYG =0,
      HTML = 1,
      SCRIPT=2,
      CSS = 4
    };*/
    //static const KimberliteMode2 KimberliteMode;
};


QT_END_NAMESPACE

#endif // UI_MAINWINDOW
