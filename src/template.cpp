#include "template.h"

StringConverter::StringConverter(QWidget* parent) : QDialog(parent) {
    resize(631, 404);
    verticalLayout_4 = new QVBoxLayout(this);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    verticalLayout_2 = new QVBoxLayout();
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    label_5 = new QLabel(this);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    verticalLayout_2->addWidget(label_5);

    label_2 = new QLabel(this);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    verticalLayout_2->addWidget(label_2);

    plainTextEdit_2 = new QPlainTextEdit(this);
    plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));

    verticalLayout_2->addWidget(plainTextEdit_2);


    verticalLayout_4->addLayout(verticalLayout_2);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    label_3 = new QLabel(this);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    verticalLayout->addWidget(label_3);

    plainTextEdit = new QPlainTextEdit(this);
    plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

    verticalLayout->addWidget(plainTextEdit);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_6 = new QLabel(this);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
    label_6->setSizePolicy(sizePolicy);

    horizontalLayout_2->addWidget(label_6);

    sonnet__dictionarycombobox = new Sonnet::DictionaryComboBox(this);
    sonnet__dictionarycombobox->setObjectName(QString::fromUtf8("sonnet__dictionarycombobox"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(sonnet__dictionarycombobox->sizePolicy().hasHeightForWidth());
    sonnet__dictionarycombobox->setSizePolicy(sizePolicy1);

    horizontalLayout_2->addWidget(sonnet__dictionarycombobox);


    verticalLayout->addLayout(horizontalLayout_2);


    verticalLayout_4->addLayout(verticalLayout);

    verticalLayout_3 = new QVBoxLayout();
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    label_4 = new QLabel(this);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    verticalLayout_3->addWidget(label_4);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    verticalLayout_3->addWidget(lineEdit);


    verticalLayout_4->addLayout(verticalLayout_3);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout->addWidget(label);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    pushButton_3 = new QPushButton(this);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

    horizontalLayout->addWidget(pushButton_3);

    pushButton_2 = new QPushButton(this);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    horizontalLayout->addWidget(pushButton_2);

    pushButton = new QPushButton(this);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    horizontalLayout->addWidget(pushButton);


    verticalLayout_4->addLayout(horizontalLayout);

    QMetaObject::connectSlotsByName(this);
    setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("Dialog", "//IMAGE//", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Dialog", "Current text:", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Dialog", "Replace with:", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("Dialog", "Spell checker:", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("Dialog", "Tag name:", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Dialog", "1/200", 0, QApplication::UnicodeUTF8));
    pushButton_3->setText(QApplication::translate("Dialog", "Ignore", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("Dialog", "Previous", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("Dialog", "Next", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(this);
}

QString StringConverter::translate(QString file) {
  show();
  HtmlParser aParser;
  HtmlData pageData = aParser.getHtmlData(file);
  for (int i=0;i < pageData.tagList.size();i++) {
    if (pageData.tagList[i][0] != '<') {
      printf("I need to translate \n");
    }
  }
}

QString StringConverter::toTemplate(QString file) {
  show();
  HtmlParser aParser;
  HtmlData pageData = aParser.getHtmlData(file);
  QString list = "<!-- \n";
  for (int i=0;i < pageData.tagList.size();i++) {
    if (pageData.tagList[i][0] != '<') {
      printf("I need to translate \n");
    }
  }
}