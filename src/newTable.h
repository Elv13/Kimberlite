#ifndef DESIGNERR20503_H
#define DESIGNERR20503_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include <KDialog>

QT_BEGIN_NAMESPACE

class NewTable : public KDialog {
  public:
    NewTable(QWidget* parent);
    QGroupBox *grbNewTable;
    QGridLayout *gridLayout;
    QLabel *lblColumn;
    QSpinBox *spnColumn;
    QLabel *lblRow;
    QSpinBox *spnRow;
    QLabel *lblBorder;
    QComboBox *cbbBorder;
    QCheckBox *ckbAddHeader;
};

QT_END_NAMESPACE

#endif // DESIGNERR20503_H
