#include "template.h"
#include <KMessageBox>

StringConverter::StringConverter(QWidget* parent) : QDialog(parent),index(0) {
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
    pushButton_2->setDisabled(true);
    connect(pushButton_2, SIGNAL(clicked()),
    this, SLOT(previousTag()));

    horizontalLayout->addWidget(pushButton_2);

    pushButton = new QPushButton(this);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    connect(pushButton, SIGNAL(clicked()),
    this, SLOT(nextTag()));
    horizontalLayout->addWidget(pushButton);
     
    btnFinish = new QPushButton(this);
    btnFinish->setObjectName(QString::fromUtf8("btnFinish"));
    btnFinish->setHidden(true);
    horizontalLayout->addWidget(btnFinish);
    connect(btnFinish, SIGNAL(clicked()),
    this, SLOT(finish()));
   


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
    btnFinish->setText(QApplication::translate("Dialog", "Finish", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(this);
}

QString StringConverter::translate(QString file) {
  mode = eTranslate;
  label_4->setHidden(true);
  lineEdit->setHidden(true);
  sonnet__dictionarycombobox->setHidden(true);
  HtmlParser aParser;
  HtmlData pageData = aParser.getHtmlData(file);
  QString list = "<!-- \n";
  for (int i=0;i < pageData.tagList.size();i++) {
    if (pageData.tagList[i].count() > 0)
      if ((pageData.tagList[i][0] != '<') && (HtmlParser::getTag(pageData.tagList[(i!=0)?i-1:0]) != "SCRIPT")) {
	qDebug() << "I need to translate" << pageData.tagList[i] << "previous :" << pageData.tagList[(i!=0)?i-1:0];
	StringInfo* aString = new StringInfo;
	aString->index = i;
	aString->originalText = pageData.tagList[i];
	stringVector.push_back(*aString);
      }
  }
  
  if (stringVector.size() > 0) {
    plainTextEdit_2->setPlainText(stringVector[0].originalText);
    label->setText("1/" + QString::number(stringVector.size()));
    show();
  }
  return QString();
}

QString StringConverter::toTemplate(QString file) {
  mode = eTemplate;
  plainTextEdit->setHidden(true);
  label_3->setHidden(true);
  label_6->setHidden(true);
  sonnet__dictionarycombobox->setHidden(true);
  HtmlParser aParser;
  HtmlData pageData = aParser.getHtmlData(file);
  QString list = "<!-- \n";
  for (int i=0;i < pageData.tagList.size();i++) {
    if (pageData.tagList[i].count() > 0)
      if ((pageData.tagList[i][0] != '<') && (HtmlParser::getTag(pageData.tagList[(i!=0)?i-1:0]) != "SCRIPT")) {
	qDebug() << "I need to translate" << pageData.tagList[i] << "previous :" << pageData.tagList[(i!=0)?i-1:0];
	StringInfo* aString = new StringInfo;
	aString->index = i;
	aString->originalText = pageData.tagList[i];
	stringVector.push_back(*aString);
      }
  }
  
  if (stringVector.size() > 0) {
    plainTextEdit_2->setPlainText(stringVector[0].originalText);
    label->setText("1/" + QString::number(stringVector.size()));
    show();
  }
  return QString();
}

void StringConverter::nextTag() {
  if (mode == eTranslate) {
    stringVector[index].newText = plainTextEdit->toPlainText();
    stringVector[index].originalText = plainTextEdit_2->toPlainText();
  }
  else {
    if (lineEdit->text().isEmpty() == true) {
      KMessageBox::error(this,"Please enter a name for this string");
      return;
    }
    if (findDuplicate(lineEdit->text())) {
      KMessageBox::error(this,"A tag with the same title already exist");
      return;
    }
    stringVector[index].newText = lineEdit->text();
    stringVector[index].originalText = plainTextEdit_2->toPlainText();
  }
  index++;
  if (mode == eTranslate) {
    plainTextEdit->setPlainText(stringVector[index].newText);
    plainTextEdit_2->setPlainText(stringVector[index].originalText);
  }
  else {
    lineEdit->setText(stringVector[index].newText);
    plainTextEdit_2->setPlainText(stringVector[index].originalText);
  }
  pushButton_2->setDisabled(false);
  label->setText(QString::number(index+1) + "/" + QString::number(stringVector.size()));
  if (index == stringVector.size() -1) {
    pushButton->setVisible(false);
    btnFinish->setVisible(true);
  }
}

void StringConverter::previousTag() {
  if (mode == eTranslate) {
    stringVector[index].newText = plainTextEdit->toPlainText();
    stringVector[index].originalText = plainTextEdit_2->toPlainText();
  }
  else {
    stringVector[index].newText = lineEdit->text();
    stringVector[index].originalText = plainTextEdit_2->toPlainText();
  }
  index--;
  if (mode == eTranslate) {
    plainTextEdit->setPlainText(stringVector[index].newText);
    plainTextEdit_2->setPlainText(stringVector[index].originalText);
  }
  else {
    lineEdit->setText(stringVector[index].newText);
    plainTextEdit_2->setPlainText(stringVector[index].originalText);
  }
  
  if (index == 0)
    pushButton_2->setDisabled(true);
  label->setText(QString::number(index+1) + "/" + QString::number(stringVector.size()));
  btnFinish->setVisible(false);
  pushButton->setVisible(true);
}

void StringConverter::finish() {
  
}

bool StringConverter::findDuplicate(QString name) {
  for (int i =0; i < stringVector.size(); i++) {
    if ((stringVector[i].newText == name) && (index != i))
      return true;
  }
  return false;
}