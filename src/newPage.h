#ifndef NEW_PAGE2
#define NEW_PAGE2
#include <KPageDialog>
#include <KPageWidgetItem>
#include "miniClasses.h"

/********************************************************************************
** Form generated from reading ui file 'FromTemplatePs9921.ui'
**
** Created: Fri Apr 10 16:43:22 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/


#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <KIcon>

QT_BEGIN_NAMESPACE

class Ui_NewPageFromTemplate
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *grbFolder;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *tvFolder;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAddFolder;
    QSpacerItem *horizontalSpacer;
    QGroupBox *grbFile;
    QGridLayout *gridLayout;
    QLineEdit *txtFileName;
    QLabel *lblPageTitle;
    QLineEdit *txtPageTitle;
    QLabel *lblFileName;
    QGroupBox *grbTemplate;
    QVBoxLayout *verticalLayout;
    QComboBox *cbbLanguage;
    QListWidget *lstTemplate;

    void setupUi(QWidget *NewPageFromTemplate)
    {
        if (NewPageFromTemplate->objectName().isEmpty())
            NewPageFromTemplate->setObjectName(QString::fromUtf8("NewPageFromTemplate"));
        NewPageFromTemplate->resize(545, 527);
        verticalLayout_3 = new QVBoxLayout(NewPageFromTemplate);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        grbFolder = new QGroupBox(NewPageFromTemplate);
        grbFolder->setObjectName(QString::fromUtf8("grbFolder"));
        grbFolder->setMinimumSize(QSize(0, 210));
        grbFolder->setMaximumSize(QSize(16777215, 210));
        verticalLayout_2 = new QVBoxLayout(grbFolder);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tvFolder = new QTreeWidget(grbFolder);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tvFolder->setHeaderItem(__qtreewidgetitem);
        tvFolder->setObjectName(QString::fromUtf8("tvFolder"));
        tvFolder->header()->setVisible(false);

        verticalLayout_2->addWidget(tvFolder);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnAddFolder = new QPushButton(grbFolder);
        btnAddFolder->setObjectName(QString::fromUtf8("btnAddFolder"));
        //btnAddFolder->setMaximumSize(QSize(30, 30));
	btnAddFolder->setIcon(KIcon("menu-add"));

        horizontalLayout->addWidget(btnAddFolder);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(grbFolder);

        grbFile = new QGroupBox(NewPageFromTemplate);
        grbFile->setObjectName(QString::fromUtf8("grbFile"));
        gridLayout = new QGridLayout(grbFile);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        txtFileName = new QLineEdit(grbFile);
        txtFileName->setObjectName(QString::fromUtf8("txtFileName"));

        gridLayout->addWidget(txtFileName, 0, 1, 1, 1);

        lblPageTitle = new QLabel(grbFile);
        lblPageTitle->setObjectName(QString::fromUtf8("lblPageTitle"));

        gridLayout->addWidget(lblPageTitle, 2, 0, 1, 1);

        txtPageTitle = new QLineEdit(grbFile);
        txtPageTitle->setObjectName(QString::fromUtf8("txtPageTitle"));

        gridLayout->addWidget(txtPageTitle, 2, 1, 1, 1);

        lblFileName = new QLabel(grbFile);
        lblFileName->setObjectName(QString::fromUtf8("lblFileName"));

        gridLayout->addWidget(lblFileName, 0, 0, 1, 1);


        verticalLayout_3->addWidget(grbFile);

        grbTemplate = new QGroupBox(NewPageFromTemplate);
        grbTemplate->setObjectName(QString::fromUtf8("grbTemplate"));
        verticalLayout = new QVBoxLayout(grbTemplate);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        cbbLanguage = new QComboBox(grbTemplate);
        cbbLanguage->setObjectName(QString::fromUtf8("cbbLanguage"));

        verticalLayout->addWidget(cbbLanguage);

        lstTemplate = new QListWidget(grbTemplate);
        lstTemplate->setObjectName(QString::fromUtf8("lstTemplate"));

        verticalLayout->addWidget(lstTemplate);


        verticalLayout_3->addWidget(grbTemplate);


        retranslateUi(NewPageFromTemplate);

        QMetaObject::connectSlotsByName(NewPageFromTemplate);
    } // setupUi

    void retranslateUi(QWidget *NewPageFromTemplate)
    {
        NewPageFromTemplate->setWindowTitle(QApplication::translate("NewPageFromTemplate", "Form", 0, QApplication::UnicodeUTF8));
        grbFolder->setTitle(QApplication::translate("NewPageFromTemplate", "Folder", 0, QApplication::UnicodeUTF8));
        btnAddFolder->setText(QString());
        grbFile->setTitle(QApplication::translate("NewPageFromTemplate", "Informations", 0, QApplication::UnicodeUTF8));
        lblPageTitle->setText(QApplication::translate("NewPageFromTemplate", "Page title:", 0, QApplication::UnicodeUTF8));
        lblFileName->setText(QApplication::translate("NewPageFromTemplate", "File name:", 0, QApplication::UnicodeUTF8));
        grbTemplate->setTitle(QApplication::translate("NewPageFromTemplate", "Template", 0, QApplication::UnicodeUTF8));
        cbbLanguage->clear();
        cbbLanguage->insertItems(0, QStringList()
         << QApplication::translate("NewPageFromTemplate", "--Language (HTML 4.01)--", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPageFromTemplate", "HTML (4.01)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPageFromTemplate", "HTML (5)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPageFromTemplate", "xHTML (1.01)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPageFromTemplate", "PHP4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPageFromTemplate", "PHP5", 0, QApplication::UnicodeUTF8)
        );
        Q_UNUSED(NewPageFromTemplate);
    } // retranslateUi

};

namespace Ui {
    class NewPageFromTemplate: public Ui_NewPageFromTemplate {};
} // namespace Ui

QT_END_NAMESPACE



/********************************************************************************
** Form generated from reading ui file 'newPagezM9921.ui'
**
** Created: Fri Apr 10 16:43:42 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewPage
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *grbFolder;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *tvFolder;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAddFolder;
    QSpacerItem *horizontalSpacer;
    QGroupBox *grbFile;
    QGridLayout *gridLayout;
    QLineEdit *txtFileName;
    QLabel *lblPageTitle;
    QLineEdit *txtPageTitle;
    QLabel *lblFileName;
    QGroupBox *grbLanguage;
    QVBoxLayout *verticalLayout;
    QComboBox *cbbLanguage;
    QComboBox *comboBox_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *NewPage)
    {
        if (NewPage->objectName().isEmpty())
            NewPage->setObjectName(QString::fromUtf8("NewPage"));
        NewPage->resize(605, 429);
        verticalLayout_3 = new QVBoxLayout(NewPage);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        grbFolder = new QGroupBox(NewPage);
        grbFolder->setObjectName(QString::fromUtf8("grbFolder"));
        grbFolder->setMinimumSize(QSize(0, 210));
        grbFolder->setMaximumSize(QSize(16777215, 210));
        verticalLayout_2 = new QVBoxLayout(grbFolder);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tvFolder = new QTreeWidget(grbFolder);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tvFolder->setHeaderItem(__qtreewidgetitem);
        tvFolder->setObjectName(QString::fromUtf8("tvFolder"));
        tvFolder->header()->setVisible(false);

        verticalLayout_2->addWidget(tvFolder);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnAddFolder = new QPushButton(grbFolder);
        btnAddFolder->setObjectName(QString::fromUtf8("btnAddFolder"));
        //btnAddFolder->setMaximumSize(QSize(30, 30));
	btnAddFolder->setIcon(KIcon("menu-add"));

        horizontalLayout->addWidget(btnAddFolder);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(grbFolder);

        grbFile = new QGroupBox(NewPage);
        grbFile->setObjectName(QString::fromUtf8("grbFile"));
        gridLayout = new QGridLayout(grbFile);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        txtFileName = new QLineEdit(grbFile);
        txtFileName->setObjectName(QString::fromUtf8("txtFileName"));

        gridLayout->addWidget(txtFileName, 0, 1, 1, 1);

        lblPageTitle = new QLabel(grbFile);
        lblPageTitle->setObjectName(QString::fromUtf8("lblPageTitle"));

        gridLayout->addWidget(lblPageTitle, 2, 0, 1, 1);

        txtPageTitle = new QLineEdit(grbFile);
        txtPageTitle->setObjectName(QString::fromUtf8("txtPageTitle"));

        gridLayout->addWidget(txtPageTitle, 2, 1, 1, 1);

        lblFileName = new QLabel(grbFile);
        lblFileName->setObjectName(QString::fromUtf8("lblFileName"));

        gridLayout->addWidget(lblFileName, 0, 0, 1, 1);


        verticalLayout_3->addWidget(grbFile);

        grbLanguage = new QGroupBox(NewPage);
        grbLanguage->setObjectName(QString::fromUtf8("grbLanguage"));
        verticalLayout = new QVBoxLayout(grbLanguage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        cbbLanguage = new QComboBox(grbLanguage);
        cbbLanguage->setObjectName(QString::fromUtf8("cbbLanguage"));

        verticalLayout->addWidget(cbbLanguage);

        comboBox_2 = new QComboBox(grbLanguage);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        verticalLayout->addWidget(comboBox_2);


        verticalLayout_3->addWidget(grbLanguage);

        verticalSpacer = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        retranslateUi(NewPage);

        QMetaObject::connectSlotsByName(NewPage);
    } // setupUi

    void retranslateUi(QWidget *NewPage)
    {
        NewPage->setWindowTitle(QApplication::translate("NewPage", "Form", 0, QApplication::UnicodeUTF8));
        grbFolder->setTitle(QApplication::translate("NewPage", "Folder", 0, QApplication::UnicodeUTF8));
        btnAddFolder->setText(QString());
        grbFile->setTitle(QApplication::translate("NewPage", "Informations", 0, QApplication::UnicodeUTF8));
        lblPageTitle->setText(QApplication::translate("NewPage", "Page title:", 0, QApplication::UnicodeUTF8));
        lblFileName->setText(QApplication::translate("NewPage", "File name:", 0, QApplication::UnicodeUTF8));
        grbLanguage->setTitle(QApplication::translate("NewPage", "Type", 0, QApplication::UnicodeUTF8));
        cbbLanguage->clear();
        cbbLanguage->insertItems(0, QStringList()
         << QApplication::translate("NewPage", "--Language (HTML 4.01)--", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPage", "HTML (4.01)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPage", "HTML (5)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPage", "xHTML (1.01)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPage", "PHP4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewPage", "PHP5", 0, QApplication::UnicodeUTF8)
        );
        Q_UNUSED(NewPage);
    } // retranslateUi

};

namespace Ui {
    class NewPage: public Ui_NewPage {};
} // namespace Ui

QT_END_NAMESPACE

/********************************************************************************
** Form generated from reading ui file 'importFilelG9921.ui'
**
** Created: Fri Apr 10 23:20:07 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "kurlrequester.h"

QT_BEGIN_NAMESPACE

class Ui_ImportFile
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *grbFolder;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *tvFolder;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAddFolder;
    QSpacerItem *horizontalSpacer;
    QGroupBox *grbImport;
    QGridLayout *gridLayout;
    QLabel *label;
    KUrlRequester *txtCurrentPath;
    QLabel *lblFileName;
    QLineEdit *txtFileName;
    QComboBox *cbbLanguage;
    QSpacerItem *verticalSpacer;
    QLabel *lblWarning;

    void setupUi(QWidget *ImportFile)
    {
        if (ImportFile->objectName().isEmpty())
            ImportFile->setObjectName(QString::fromUtf8("ImportFile"));
        ImportFile->resize(518, 485);
        verticalLayout = new QVBoxLayout(ImportFile);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        grbFolder = new QGroupBox(ImportFile);
        grbFolder->setObjectName(QString::fromUtf8("grbFolder"));
        grbFolder->setMinimumSize(QSize(0, 210));
        grbFolder->setMaximumSize(QSize(16777215, 210));
        verticalLayout_2 = new QVBoxLayout(grbFolder);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tvFolder = new QTreeWidget(grbFolder);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tvFolder->setHeaderItem(__qtreewidgetitem);
        tvFolder->setObjectName(QString::fromUtf8("tvFolder"));
        tvFolder->header()->setVisible(false);

        verticalLayout_2->addWidget(tvFolder);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnAddFolder = new QPushButton(grbFolder);
        btnAddFolder->setObjectName(QString::fromUtf8("btnAddFolder"));
        //btnAddFolder->setMaximumSize(QSize(30, 30));
	btnAddFolder->setIcon(KIcon("menu-add"));

        horizontalLayout->addWidget(btnAddFolder);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(grbFolder);

        grbImport = new QGroupBox(ImportFile);
        grbImport->setObjectName(QString::fromUtf8("grbImport"));
        gridLayout = new QGridLayout(grbImport);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(grbImport);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        txtCurrentPath = new KUrlRequester(grbImport);
        txtCurrentPath->setObjectName(QString::fromUtf8("txtCurrentPath"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(txtCurrentPath->sizePolicy().hasHeightForWidth());
        txtCurrentPath->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(txtCurrentPath, 0, 1, 1, 2);

        lblFileName = new QLabel(grbImport);
        lblFileName->setObjectName(QString::fromUtf8("lblFileName"));
        sizePolicy.setHeightForWidth(lblFileName->sizePolicy().hasHeightForWidth());
        lblFileName->setSizePolicy(sizePolicy);

        gridLayout->addWidget(lblFileName, 1, 0, 2, 1);

        txtFileName = new QLineEdit(grbImport);
        txtFileName->setObjectName(QString::fromUtf8("txtFileName"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(txtFileName->sizePolicy().hasHeightForWidth());
        txtFileName->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(txtFileName, 1, 1, 2, 2);


        verticalLayout->addWidget(grbImport);

        cbbLanguage = new QComboBox(ImportFile);
        cbbLanguage->setObjectName(QString::fromUtf8("cbbLanguage"));

        verticalLayout->addWidget(cbbLanguage);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        lblWarning = new QLabel(ImportFile);
        lblWarning->setObjectName(QString::fromUtf8("lblWarning"));
        lblWarning->setWordWrap(true);

        verticalLayout->addWidget(lblWarning);


        retranslateUi(ImportFile);

        QMetaObject::connectSlotsByName(ImportFile);
    } // setupUi

    void retranslateUi(QWidget *ImportFile)
    {
        ImportFile->setWindowTitle(QApplication::translate("ImportFile", "Form", 0, QApplication::UnicodeUTF8));
        grbFolder->setTitle(QApplication::translate("ImportFile", "Folder", 0, QApplication::UnicodeUTF8));
        btnAddFolder->setText(QString());
        grbImport->setTitle(QApplication::translate("ImportFile", "Import existing file", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ImportFile", "Path:", 0, QApplication::UnicodeUTF8));
        lblFileName->setText(QApplication::translate("ImportFile", "File name:", 0, QApplication::UnicodeUTF8));
        cbbLanguage->clear();
        cbbLanguage->insertItems(0, QStringList()
         << QApplication::translate("ImportFile", "--Language (autodetect)--", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportFile", "HTML (4.01)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportFile", "HTML (5)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportFile", "xHTML (1.01)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportFile", "PHP4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportFile", "PHP5", 0, QApplication::UnicodeUTF8)
        );
        lblWarning->setText(QApplication::translate("ImportFile", "Please note that the corrent file will be copied into the project. Furter modification in the file will not affect the copy, nor the copy will afect the original file.", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ImportFile);
    } // retranslateUi

};

namespace Ui {
    class ImportFile: public Ui_ImportFile {};
} // namespace Ui

QT_END_NAMESPACE


class NewWebPage : public KPageDialog {
  Q_OBJECT
  public:
    NewWebPage(QWidget* parent, TreeItem* root);
  private:
    Ui_NewPageFromTemplate* newTemple;
    Ui_NewPage* newPage;
    Ui_ImportFile* newImport;
    KPageWidgetItem* pageEmpty;
    KPageWidgetItem* pageTemplate;
    KPageWidgetItem* pageImport;
    void setupFolder(QTreeWidget* aTreeWidget, TreeItem* rootItem);
    QTreeWidgetItem* parentRoot;
  private slots:
    void addFolder();
    void addHtmlPage();
  signals:
    void addFolder(QString,QTreeWidgetItem*);
    void addHtmlPage(QString title, QString name, QString body, QString folderName, int pageType);
};

#endif