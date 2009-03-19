#ifndef TRANSLATEE11004_H
#define TRANSLATEE11004_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QDialog>
#include <QVector>
#include <sonnet/dictionarycombobox.h>
#include "htmlParser.h"

struct StringInfo {
  int index;
  QString originalText;
  QString newText;
};

class StringConverter : public QDialog {
public:
    StringConverter(QWidget* parent);
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_5;
    QLabel *label_2;
    QPlainTextEdit *plainTextEdit_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QPlainTextEdit *plainTextEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    Sonnet::DictionaryComboBox *sonnet__dictionarycombobox;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QString translate(QString file);
    QString toTemplate(QString file);
    QVector<StringInfo> stringVector;
    int index;
};

#endif // TRANSLATEE11004_H