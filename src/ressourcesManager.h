#ifndef DESIGNERM20503_H
#define DESIGNERM20503_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <KDialog>
#include "klineedit.h"
#include "kurlrequester.h"

QT_BEGIN_NAMESPACE

class RessrourceManager : public KDialog {
public:
    RessrourceManager(QWidget* parent);
    QVBoxLayout *verticalLayout;
    QLabel *lblDescription;
    QGroupBox *grbRessources;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *rbCurerent;
    QTreeWidget *tvCurrentRessources;
    QRadioButton *rbNew;
    KUrlRequester *txtNewRessources;
    QRadioButton *rbUrl;
    KLineEdit *txtUrl;
};
QT_END_NAMESPACE

#endif // DESIGNERM20503_H
