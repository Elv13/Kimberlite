/********************************************************************************
** Form generated from reading ui file 'newProjectJ19664.ui'
**
** Created: Sat Apr 18 21:18:37 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef NEWPROJECTJ19664_H
#define NEWPROJECTJ19664_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QWidget>
#include <KDialog>

QT_BEGIN_NAMESPACE

class Ui_NewProject
{
public:
    QGridLayout *gridLayout;
    QLineEdit *txtName;
    QLabel *lblType;
    QListWidget *lstTemplate;
    QLabel *lblName;

    void setupUi(QWidget *NewProject)
    {
        if (NewProject->objectName().isEmpty())
            NewProject->setObjectName(QString::fromUtf8("NewProject"));
        NewProject->resize(418, 364);
        gridLayout = new QGridLayout(NewProject);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        txtName = new QLineEdit(NewProject);
        txtName->setObjectName(QString::fromUtf8("txtName"));

        gridLayout->addWidget(txtName, 0, 1, 1, 1);

        lblType = new QLabel(NewProject);
        lblType->setObjectName(QString::fromUtf8("lblType"));

        gridLayout->addWidget(lblType, 2, 0, 1, 1);

        lstTemplate = new QListWidget(NewProject);
        lstTemplate->setObjectName(QString::fromUtf8("lstTemplate"));

        gridLayout->addWidget(lstTemplate, 3, 0, 1, 2);

        lblName = new QLabel(NewProject);
        lblName->setObjectName(QString::fromUtf8("lblName"));

        gridLayout->addWidget(lblName, 0, 0, 1, 1);


        retranslateUi(NewProject);

        QMetaObject::connectSlotsByName(NewProject);
    } // setupUi

    void retranslateUi(QWidget *NewProject)
    {
        NewProject->setWindowTitle(QApplication::translate("NewProject", "Form", 0, QApplication::UnicodeUTF8));
        lblType->setText(QApplication::translate("NewProject", "Type:", 0, QApplication::UnicodeUTF8));
        lblName->setText(QApplication::translate("NewProject", "Porject name:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(NewProject);
    } // retranslateUi

};

class NewProject : public KDialog {
  Q_OBJECT
  public:
    NewProject(QWidget* parent);
  private:
    Ui_NewProject* mainUi;
  private slots:
    void newProject();
  signals:
    void createProject(QString, QString);
    
};

QT_END_NAMESPACE

#endif // NEWPROJECTJ19664_H
