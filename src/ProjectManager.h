#include <QString>
#include <QStringList>

class ProjectManager {

public:
    ProjectManager(QString path);
    ~ProjectManager() {}
    
      QStringList htmlPage;
      QString cssPage;
      QString projectName;
      QString projectFolder;
      QString line;
      QStringList images;
      QStringList flash;
      QStringList sounds;
      QStringList videos;
      QStringList scripts;

//private:


};