#ifndef CONFIG_SKELETON
#define CONFIG_SKELETON

#include <kconfigskeleton.h>
#include <QStringList>

class KimberliteConfigSkeleton : public KConfigSkeleton {
  public:
    KimberliteConfigSkeleton() {
    setCurrentGroup("RecentProject");
    addItemStringList("recentP1", recentP1, QStringList() << "" << "");
    addItemStringList("recentP2", recentP2, QStringList() << "" << "");
    addItemStringList("recentP3", recentP3, QStringList() << "" << "");
    addItemStringList("recentP4", recentP4, QStringList() << "" << "");
    addItemStringList("recentP5", recentP5, QStringList() << "" << "");
    
    setCurrentGroup("Server");
    addItemString("serverTypes", serverTypes, "Apache");
    addItemString("serverLocation", serverLocation, "/var/www/");
    addItemString("serverUrl", serverUrl, "http://localhost/");
    addItemString("serverPort", serverPort, "80");
    addItemBool("useServer", useServer, true);
  }
  //RecentProject
  QStringList recentP1;
  QStringList recentP2;
  QStringList recentP3;
  QStringList recentP4;
  QStringList recentP5;
  
  //Server
  QString serverTypes;
  QString serverLocation;
  QString serverUrl;
  QString serverPort;
  bool useServer;
};

#endif
