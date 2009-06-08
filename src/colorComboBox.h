#ifndef COLOR_COMBOBOX
#define COLOR_COMBOBOX
#include <QComboBox>
#include <KIcon>
#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QColor>
#include <QListWidget>

class ColorButton : public QPushButton {
  Q_OBJECT
  public:
    ColorButton();
    void setColor(QColor aColor) {
      buttonColor = aColor;
      setStyleSheet("border:0px;margin:0px;background-color:"+aColor.name()+";");
    }
    void select() {
      setStyleSheet("border:2px;border-color:white;border-style:solid;margin:0px;background-color:"+buttonColor.name()+";");
    }
    void reset() {
      setStyleSheet("border:0px;margin:0px;background-color:"+buttonColor.name()+";");
    }
  signals:
    void clicked(QColor);
  private:
    QColor buttonColor;
  private slots:
    void pressed();
};

class ColorComboBox : public QComboBox {
  Q_OBJECT
  public:
    ColorComboBox(QWidget* parent);
    virtual void showPopup();
    virtual void hidePopup();
    void setIcon(QIcon anIcon) {
      ((QListWidget*)view())->item(0)->setIcon(anIcon);
    }
    
  private:
    QFrame* frmColor;
    ColorButton* currentButton;
    QHash<QString,ColorButton *> buttonHash;
    QGridLayout* mainLayout;
    int maxPerLine;
    int curRow;
    int curCol;

  private slots:
    void colorClicked(QColor);
    void addCustomColor();
  signals:
    void colorChanged(QColor);
};
#endif