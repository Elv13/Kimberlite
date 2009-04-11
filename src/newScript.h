/********************************************************************************
** Form generated from reading ui file 'newScriptxg9921.ui'
**
** Created: Sat Apr 11 00:03:41 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef NEWSCRIPTXG9921_H
#define NEWSCRIPTXG9921_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewScript
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *grbFolder;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *tvFolder;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAddFolder;
    QSpacerItem *horizontalSpacer;
    QGroupBox *grbInfo;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblName;
    QLineEdit *txtName;
    QComboBox *cbbLanguage;
    QGroupBox *grbValidate;
    QGridLayout *gridLayout;
    QCheckBox *ckbOpera;
    QCheckBox *ckbSafari;
    QCheckBox *ckbKonqueror;
    QCheckBox *ckbIe;
    QCheckBox *ckBFirefox;
    QSpinBox *spcFirefox;
    QSpinBox *spnIe;
    QSpinBox *spcSafari;
    QSpinBox *spcOpera;
    QSpinBox *spnKonqueror;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QWidget *NewScript)
    {
        if (NewScript->objectName().isEmpty())
            NewScript->setObjectName(QString::fromUtf8("NewScript"));
        NewScript->resize(395, 568);
        verticalLayout_2 = new QVBoxLayout(NewScript);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        grbFolder = new QGroupBox(NewScript);
        grbFolder->setObjectName(QString::fromUtf8("grbFolder"));
        grbFolder->setMinimumSize(QSize(0, 210));
        grbFolder->setMaximumSize(QSize(16777215, 210));
        verticalLayout_3 = new QVBoxLayout(grbFolder);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tvFolder = new QTreeWidget(grbFolder);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tvFolder->setHeaderItem(__qtreewidgetitem);
        tvFolder->setObjectName(QString::fromUtf8("tvFolder"));
        tvFolder->header()->setVisible(false);

        verticalLayout_3->addWidget(tvFolder);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnAddFolder = new QPushButton(grbFolder);
        btnAddFolder->setObjectName(QString::fromUtf8("btnAddFolder"));
        btnAddFolder->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(btnAddFolder);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(grbFolder);

        grbInfo = new QGroupBox(NewScript);
        grbInfo->setObjectName(QString::fromUtf8("grbInfo"));
        verticalLayout = new QVBoxLayout(grbInfo);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lblName = new QLabel(grbInfo);
        lblName->setObjectName(QString::fromUtf8("lblName"));

        horizontalLayout_2->addWidget(lblName);

        txtName = new QLineEdit(grbInfo);
        txtName->setObjectName(QString::fromUtf8("txtName"));

        horizontalLayout_2->addWidget(txtName);


        verticalLayout->addLayout(horizontalLayout_2);

        cbbLanguage = new QComboBox(grbInfo);
        cbbLanguage->setObjectName(QString::fromUtf8("cbbLanguage"));

        verticalLayout->addWidget(cbbLanguage);


        verticalLayout_2->addWidget(grbInfo);

        grbValidate = new QGroupBox(NewScript);
        grbValidate->setObjectName(QString::fromUtf8("grbValidate"));
        gridLayout = new QGridLayout(grbValidate);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ckbOpera = new QCheckBox(grbValidate);
        ckbOpera->setObjectName(QString::fromUtf8("ckbOpera"));

        gridLayout->addWidget(ckbOpera, 4, 0, 1, 1);

        ckbSafari = new QCheckBox(grbValidate);
        ckbSafari->setObjectName(QString::fromUtf8("ckbSafari"));

        gridLayout->addWidget(ckbSafari, 3, 0, 1, 1);

        ckbKonqueror = new QCheckBox(grbValidate);
        ckbKonqueror->setObjectName(QString::fromUtf8("ckbKonqueror"));

        gridLayout->addWidget(ckbKonqueror, 5, 0, 1, 1);

        ckbIe = new QCheckBox(grbValidate);
        ckbIe->setObjectName(QString::fromUtf8("ckbIe"));

        gridLayout->addWidget(ckbIe, 2, 0, 1, 1);

        ckBFirefox = new QCheckBox(grbValidate);
        ckBFirefox->setObjectName(QString::fromUtf8("ckBFirefox"));
        ckBFirefox->setChecked(true);

        gridLayout->addWidget(ckBFirefox, 1, 0, 1, 1);

        spcFirefox = new QSpinBox(grbValidate);
        spcFirefox->setObjectName(QString::fromUtf8("spcFirefox"));
        spcFirefox->setMinimum(1);
        spcFirefox->setMaximum(4);
        spcFirefox->setValue(2);

        gridLayout->addWidget(spcFirefox, 1, 1, 1, 1);

        spnIe = new QSpinBox(grbValidate);
        spnIe->setObjectName(QString::fromUtf8("spnIe"));
        spnIe->setEnabled(false);
        spnIe->setMinimum(1);
        spnIe->setMaximum(8);

        gridLayout->addWidget(spnIe, 2, 1, 1, 1);

        spcSafari = new QSpinBox(grbValidate);
        spcSafari->setObjectName(QString::fromUtf8("spcSafari"));
        spcSafari->setEnabled(false);
        spcSafari->setMinimum(1);
        spcSafari->setMaximum(4);

        gridLayout->addWidget(spcSafari, 3, 1, 1, 1);

        spcOpera = new QSpinBox(grbValidate);
        spcOpera->setObjectName(QString::fromUtf8("spcOpera"));
        spcOpera->setEnabled(false);
        spcOpera->setMinimum(1);
        spcOpera->setMaximum(10);

        gridLayout->addWidget(spcOpera, 4, 1, 1, 1);

        spnKonqueror = new QSpinBox(grbValidate);
        spnKonqueror->setObjectName(QString::fromUtf8("spnKonqueror"));
        spnKonqueror->setEnabled(false);
        spnKonqueror->setMinimum(1);
        spnKonqueror->setMaximum(4);

        gridLayout->addWidget(spnKonqueror, 5, 1, 1, 1);

        label_2 = new QLabel(grbValidate);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(grbValidate);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 1, 1, 1);


        verticalLayout_2->addWidget(grbValidate);


        retranslateUi(NewScript);
        QObject::connect(ckBFirefox, SIGNAL(clicked(bool)), spcFirefox, SLOT(setEnabled(bool)));
        QObject::connect(ckbIe, SIGNAL(clicked(bool)), spnIe, SLOT(setEnabled(bool)));
        QObject::connect(ckbSafari, SIGNAL(clicked(bool)), spcSafari, SLOT(setEnabled(bool)));
        QObject::connect(ckbOpera, SIGNAL(clicked(bool)), spcOpera, SLOT(setEnabled(bool)));
        QObject::connect(ckbKonqueror, SIGNAL(clicked(bool)), spnKonqueror, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(NewScript);
    } // setupUi

    void retranslateUi(QWidget *NewScript)
    {
        NewScript->setWindowTitle(QApplication::translate("NewScript", "Form", 0, QApplication::UnicodeUTF8));
        grbFolder->setTitle(QApplication::translate("NewScript", "Folder", 0, QApplication::UnicodeUTF8));
        btnAddFolder->setText(QString());
        grbInfo->setTitle(QApplication::translate("NewScript", "Informations", 0, QApplication::UnicodeUTF8));
        lblName->setText(QApplication::translate("NewScript", "Name:", 0, QApplication::UnicodeUTF8));
        cbbLanguage->clear();
        cbbLanguage->insertItems(0, QStringList()
         << QApplication::translate("NewScript", "--Language (JavaScript)--", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewScript", "IE6 JavaScript", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewScript", "ECMAscript", 0, QApplication::UnicodeUTF8)
        );
        grbValidate->setTitle(QApplication::translate("NewScript", "Validate for", 0, QApplication::UnicodeUTF8));
        ckbOpera->setText(QApplication::translate("NewScript", "Opera", 0, QApplication::UnicodeUTF8));
        ckbSafari->setText(QApplication::translate("NewScript", "Safari/Chrome/WebKit", 0, QApplication::UnicodeUTF8));
        ckbKonqueror->setText(QApplication::translate("NewScript", "Konqueror", 0, QApplication::UnicodeUTF8));
        ckbIe->setText(QApplication::translate("NewScript", "InternetExplorer/Trident", 0, QApplication::UnicodeUTF8));
        ckBFirefox->setText(QApplication::translate("NewScript", "Firefox/Gecko", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("NewScript", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Browser</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("NewScript", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Version</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(NewScript);
    } // retranslateUi

};

namespace Ui {
    class NewScript: public Ui_NewScript {};
} // namespace Ui

QT_END_NAMESPACE

#endif // NEWSCRIPTXG9921_H

#ifndef NEW_SCRIPT
#define NEW SCRIPT
#include <KDialog>

class NewScript : public KDialog {
  Q_OBJECT
  public:
    NewScript(QWidget* parent, QTreeWidgetItem* rootItem);
  private:
    Ui_NewScript* aNewScript;
    QTreeWidgetItem* parentRoot;
    void setupFolder(QTreeWidget* aTreeWidget, QTreeWidgetItem* rootItem);
  private slots:
    void addFolder();
  signals:
    void addFolder(QString,QTreeWidgetItem*);
};

#endif