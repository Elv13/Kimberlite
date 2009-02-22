#include "cssParser.h"
QString CssParser::cssFile = QString(); 

QString CssParser::setClass(QString className, QString content) {
  QStringList classPropriety,classList(getClassList());
  QString newCSSStyle,classText;
  for (int i =0; i < classList.count();i++) {
    if (classList[i] != className) {
      classPropriety =  getClass(classList[i]);
      classText = classList[i] + " {";
      for (int j =0; j < classPropriety.count();j++)
        classText += classPropriety[j];
      classText += "}";
      newCSSStyle += classText;
    }
    else
      newCSSStyle += classList[i] + " {\n" + content + "}";
  }
  return newCSSStyle;
}

QStringList CssParser::getClassList() {
  QStringList classList;
  QString cssFile = parseCSS();
  while (cssFile.indexOf("{") != -1) {
    while (cssFile.indexOf("{") > cssFile.indexOf("\n"))
      cssFile.remove(0, cssFile.indexOf("\n")+1);
    if (cssFile.at(0) == '      ')
      classList.push_back(classList[classList.count()-1] + "^?" + cssFile.left(cssFile.indexOf(" {")).trimmed());
    else
      classList.push_back(cssFile.left(cssFile.indexOf(" {")).trimmed());
    cssFile.remove(0, cssFile.indexOf("\n")+1);
  }
  return classList;
}

QString CssParser::getUnit(QString line) {
  line = line.trimmed();
  QString anUnit = "-1";
  QSqlQuery query;
  query.exec("SELECT UNIT FROM TCSS_TAG WHERE TITLE = '"+ getPropriety(line) +"'");
  while (query.next()) {
    QStringList possibleValues = query.value(0).toString().split(";");
    for (int i =0;i < possibleValues.count(); i++)
      if ((possibleValues[i]+";") == line.right(possibleValues[i].count()+1)) 
        anUnit = possibleValues[i];
  }
  return anUnit;
}

QString CssParser::getValue(QString line) {
  line = line.trimmed();
  QString anUnit(getUnit(line)),line2((line.right(line.count() - line.indexOf(":")-1)).trimmed());
  if((anUnit.count() != 0) && (anUnit != "-1"))
    line2 = line2.left(line2.indexOf(anUnit));
  else
    line2 = line2.left(line2.indexOf(";"));
  return line2.trimmed().toLower();
}

QStringList CssParser::getClass(QString className) {
  QString parsedCSS = parseCSS();
  QStringList aClass;
  parsedCSS.remove(0,parsedCSS.indexOf(className + " {")+ className.count() +2 );
  parsedCSS.truncate(parsedCSS.indexOf("\n}"));
  while ((parsedCSS.count() != 0) && (parsedCSS.indexOf(";") != -1)) {
    if ((parsedCSS.indexOf("/*") != -1) && ((parsedCSS.indexOf("/*") < (parsedCSS.indexOf(";"))))) {
      aClass << parsedCSS.left(parsedCSS.indexOf("*/")+2).trimmed();
      parsedCSS.remove(0,parsedCSS.indexOf("*/")+2);
    }
    else {
      aClass << parsedCSS.left(parsedCSS.indexOf(";")+1).trimmed();
      parsedCSS.remove(0,parsedCSS.indexOf(";")+1);
    }
    parsedCSS = parsedCSS.trimmed();
  }
  return aClass;
}

QString CssParser::getPropriety(QString line) {
  return line.trimmed().left(line.indexOf(":")).trimmed().toLower();
}

QString CssParser::parseCSS() {
  QString parsedCSS, tmpCSS(cssFile.trimmed());
  if (tmpCSS.trimmed().isEmpty() == false) 
    while (tmpCSS.count() != 0) {
      if ((tmpCSS.indexOf("/*") != -1) && ((tmpCSS.indexOf("/*") < (tmpCSS.indexOf("{"))))) {
        parsedCSS += tmpCSS.left(tmpCSS.indexOf("*/")+1) + "\n";
        tmpCSS.remove(0,tmpCSS.indexOf("*/")+1);
      }
      else {
        parsedCSS += tmpCSS.left(tmpCSS.indexOf("{")).trimmed() + " {\n";
        tmpCSS.remove(0,tmpCSS.indexOf("{")+1);
        while ((tmpCSS.indexOf(";") < tmpCSS.indexOf("}")) && (tmpCSS.indexOf(";") != -1) && (tmpCSS.indexOf("}") != -1)) {
            parsedCSS += "    " +tmpCSS.left(tmpCSS.indexOf(";")+1).trimmed() + "\n";
            tmpCSS.remove(0,tmpCSS.indexOf(";")+1);
        }
      }
        parsedCSS += "}\n\n";
        tmpCSS.remove(0,tmpCSS.indexOf("}")+1).trimmed();
    }
  return parsedCSS;
}