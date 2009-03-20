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

#define eTranslate 0
#define eTemplate 1

struct StringInfo {
  int index;
  QString originalText;
  QString newText;
};


class StringConverter : public QDialog {
  Q_OBJECT
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
    QPushButton *btnFinish;
    QString translate(QString file);
    QString toTemplate(QString file);
    QVector<StringInfo> stringVector;
    int index;
    bool mode;
  private slots:
    void nextTag();
    void previousTag();
    void finish();
  private:
    bool findDuplicate(QString name);
};

#endif // TRANSLATEE11004_H