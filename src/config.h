#ifndef DEF_CONFIG
#define DEF_CONFIG

#include <KConfigDialog>
#include <KPageWidgetItem>
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHeaderView>
#include <KPushButton>
#include <KTextEdit>
#include <KLineEdit>
#include <QCheckBox>
#include <KIcon>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QListWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QSpacerItem>
#include "src/configSkeleton.h"

QT_BEGIN_NAMESPACE

class Config : public KConfigDialog {
  Q_OBJECT

  public:
    Config(QWidget* parent, KimberliteConfigSkeleton* configSkeleton);
    ~Config(){}
    
  private:
    KimberliteConfigSkeleton* configSkeleton;
    QComboBox* cbbServerType;
    QLineEdit* txtServerLocation;
    QLineEdit* txtServerUrl;
    QLineEdit* txtServerPort;
    QCheckBox* ckbUseServer;
  private slots:
    void saveConfig();
  private:
    
};
#endif
