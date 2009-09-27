#ifndef CSS_PREVIEW
#define CSS_PREVIEW
#include <QWebView>
#include <QWidget>
#include <QVBoxLayout>

class CssPreviewWidget : public QWidget {
  Q_OBJECT
  public:
    CssPreviewWidget(QWidget* parent);
    QWebView* webPreview;
  private:

  public slots:
    void diaplayObject(QString name);
};
#endif