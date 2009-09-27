#include "cssPreviewWidget.h"

CssPreviewWidget::CssPreviewWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  webPreview = new QWebView(this);
  mainLayout->addWidget(webPreview);
  webPreview->setHtml("\
    <html>\
      <head>\
	  <link rel=\"stylesheet\" href=\"./StyleSheet.css\" type=\"text/css\">\
      </head>\
      <body style=\"word-wrap: break-word; -webkit-nbsp-mode: space; -webkit-line-break: after-white-space; \">\
	  Welcome to your new website!\
      </body>\
    </html>\
  ");
}

void CssPreviewWidget::diaplayObject(QString name) {
  QString type;
  if (name.trimmed()[0] == '#')
    type = "identifier";
  else if (name.trimmed()[0] == '.')
    type = "class";
  else
    type = "tag";
    
  webPreview->setHtml("\
    <html>\
      <head>\
	  <link rel=\"stylesheet\" href=\"./StyleSheet.css\" type=\"text/css\">\
      </head>\
      <body style=\"word-wrap: break-word; -webkit-nbsp-mode: space; -webkit-line-break: after-white-space; \">\
	  "+type+"\
	  <br>\
	  "+name+"\
      </body>\
    </html>\
  ");
}