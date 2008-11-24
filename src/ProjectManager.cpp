#include "ProjectManager.h"
//#include <iostream> //useless
#include <KIO/NetAccess>
#include <KSaveFile>
#include <QTextStream>

ProjectManager::ProjectManager(QString path) {

      QFile file(path);
      file.open(QIODevice::ReadOnly);
      while (!file.atEnd()) {
          line = file.readLine();
          //std::cout << line.toStdString() << endl;
	  if (line.indexOf("<title>") != -1) {
            line = file.readLine();
            while (line.indexOf("</title>") == -1) {
              projectName = line.trimmed();
              line = file.readLine();
            }
	  }
	  else if (line.indexOf("<page>") != -1) {
            //std::cout << "I am alive" << endl;
            line = file.readLine();
            while (line.indexOf("</page>") == -1) {
              //std::cout << line.toStdString() << endl;
              htmlPage << line.trimmed();
              line = file.readLine();
            }
	  }
	  else if (line.indexOf("<media>") != -1) {
            line = file.readLine();
            while (line.indexOf("</media>") == -1) {
                if (line.indexOf("<image>") != -1) {
                  line = file.readLine();
                  while (line.indexOf("</image>") == -1) {
                    images << line.trimmed();
                    line = file.readLine();
                  }
                }
                else if (line.indexOf("<flash>") != -1) {
                  line = file.readLine();
                  while (line.indexOf("</flash>") == -1) {
                    flash << line.trimmed();
                    line = file.readLine();
                  }
                }
                else if (line.indexOf("<sounds>") != -1) {
                  line = file.readLine();
                  while (line.indexOf("</sounds>") == -1) {
                    sounds << line.trimmed();
                    line = file.readLine();
                  }
                }
                else if (line.indexOf("<video>") != -1) {
                  line = file.readLine();
                  while (line.indexOf("</video>") == -1) {
                    videos << line.trimmed();
                    line = file.readLine();
                  }
                }
              line = file.readLine();
            }
	  }
          else if (line.indexOf("<script>") != -1) {
            line = file.readLine();
            while (line.indexOf("</script>") == -1) {
              scripts << line.trimmed();
              line = file.readLine();
            }
          }
          else if (line.indexOf("<style>") != -1) {
            line = file.readLine();
            while (line.indexOf("</style>") == -1) {
              cssPage = line.trimmed();
              line = file.readLine();
            }
          }
      }
}