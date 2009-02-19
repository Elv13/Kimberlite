#include "htmlParser.h"

HtmlParser::HtmlParser()  {
  QSqlQuery query;
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDCLOSE = 0");
  while (query.next()) 
    orphelinTags << query.value(0).toString();
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 0");
  while (query.next()) 
    noNewLineTags << query.value(0).toString();
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 2");
  while (query.next()) {
    needNewLineOnClose << query.value(0).toString();
    noNewLineTags << query.value(0).toString();
  }
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 3");
  while (query.next()) {
    needNewLineOnOpen << query.value(0).toString();
    noNewLineTags << query.value(0).toString();
  }
}

QString HtmlParser::compressFile(QString path) {
      QFile file(path);
      file.open(QIODevice::ReadOnly);
      QString output;
      while (!file.atEnd())
            output += file.readLine();
      return output;
}

QString HtmlParser::compressString(QString file) {
  while (file.indexOf(0x0A) != -1) 
    file.remove(file.indexOf(0x0A),1);
  return file;
}

QString HtmlParser::getTag(QString aTag) {
  QString tag;
  if (aTag != "") {
    if (aTag[1] == '!') 
      tag = "!--";
    else {
      tag = aTag.remove(0,1);
      while (tag[0] == ' ') 
            tag = tag.mid(1, tag.size() -1);
      if (tag.indexOf(" ") != -1) 
            tag = tag.left(tag.indexOf(" "));
      else 
            tag  = tag.left(tag.size() -1);
      if (tag[0] == '/') 
            tag = aTag.remove(0,1);
      if (tag.right(1) == ">") 
            tag = aTag.remove(tag.size()-1,1);
    }
  }
  return tag.toUpper();;
}

QVector<QString> HtmlParser::listTag(QString inputFile) {
      QVector<QString> tagList;
      while (inputFile != "") {
            while ((inputFile[0] == ' ') || (inputFile[0] == '  ') || (inputFile[0] == 0x0A))
              inputFile = inputFile.remove(0,1);
            if (inputFile.indexOf("<") < inputFile.lastIndexOf("<",inputFile.indexOf(">"))) {
                  tagList.push_back(inputFile.mid(inputFile.lastIndexOf("<",inputFile.indexOf(">")-1)));
                  inputFile = inputFile.remove(0,inputFile.lastIndexOf("<",inputFile.indexOf(">")-1));
            }
            else if (inputFile.left(9) == "<!DOCTYPE") {
                  tagList.push_back(inputFile.left(inputFile.indexOf(">")+1));
                  inputFile = inputFile.remove(0,inputFile.indexOf(">")+1);
            }
            else if (inputFile.left(4) == "<!--") {
                  tagList.push_back(inputFile.left(inputFile.indexOf("->")+2));
                  inputFile = inputFile.remove(0,inputFile.indexOf("->")+2);
            }
            else if ((inputFile.left(7) == "<SCRIPT")) { //TODO add SCRIPT too
                  tagList.push_back(inputFile.left(inputFile.indexOf(">")+1));
                  inputFile = inputFile.remove(0,inputFile.indexOf(">")+1);
                  if (inputFile.indexOf("</script>") != 0) {
                    tagList.push_back(inputFile.left(inputFile.indexOf("</script>")-1).trimmed());
                    inputFile = inputFile.remove(0,inputFile.indexOf("</script>")-1);
                  }
            }
            else if (inputFile.indexOf("<") == 0) {
                  tagList.push_back(inputFile.left(inputFile.indexOf(">")+1));
                  inputFile = inputFile.remove(0,inputFile.indexOf(">")+1);
            }
            else if (inputFile != "") {
                  tagList.push_back(inputFile.left(inputFile.indexOf("<")).trimmed());
                  inputFile = inputFile.remove(0,inputFile.indexOf("<"));
            }
      }
  return tagList;
}

QString HtmlParser::parse(QString inputFile, bool debug, bool toTemplate, bool mode = true /*true = indent, false = tree*/, QTreeWidget* aTree = NULL) {
  /*QVector<QString> tagList(listTag(inputFile));
  QVector<uint> levelList = htmlParser(tagList);
  QString tag;
  for (int i=0; i < tagList.size();i++) {
    tag = getTag(tagList[i]);
    if (debug == true) {
      if ((orphelinTags.indexOf(tag) != -1) && (tagList[i][1] == '/')) 
        debugVector << *(new debugItem(i, 0,  "Warning:  tag \"" + tag + "\" don't need to be closed (HTML 4.01)"));
      else if ((orphelinTags.indexOf(tag) == -1) && (tagList[i][0] == '<') && (tagList[i][1] == '/')) { //TODO remove all duplicated if crap
        int k =i-1;
        bool found = false;
        while ((k >=0) && (found == false)) {
          if (levelList[i] == levelList[k])
            found = true;
          else
            k--;
        }
        if ((found == true) && (getTag(tagList[i]) != getTag(tagList[k]))) 
          debugVector << *(new debugItem(k, 1, "Error:  tag \"" + getTag(tagList[k]) + "\" does not close at the right place"));
      }
      else if ((orphelinTags.indexOf(tag) != -1) && (tagList[i][tagList[i].size() -2] != '/') && (tagList[i].indexOf(" ") != -1) && (tagList[i][1] != '!'))  //TODO recheck if it need "else"
        debugVector << *(new debugItem(i, 0, "Warning:  Missing \"/\" at the end of tag \"" + getTag(tagList[i]) + "\" add it to comply with xHTML standard"));
      
      if ((tag == "SCRIPT") && (getTag(tagList[i+1]) != "SCRIPT"))  //BUG array overflow possible
        debugVector << *(new debugItem(i, 3, "Information: Using javascript or css code directly in HTML file is not a good practice, use separate file"));
      else if ((tag == "STYLE") && (getTag(tagList[i+1]) != "STYLE"))  //BUG array overflow possible
        debugVector << *(new debugItem(i, 3, "Information: Using javascript or css code directly in HTML file is not a good practice, use separate file"));
    }
  }
  
  if ((debug == true) && (levelList.size() != 0) && (levelList[levelList.size() -1] != 0)) 
    debugVector << *(new debugItem(levelList.size(), 1, "Error:  Parsing failed, one or more tag may be missing"));
  if (mode == false)  {
    updateTree(toTemplate, tagList, levelList, aTree);
    return QString("salut");
  }
  else
    return indentHtml(toTemplate, tagList, levelList);*/
}

QVector<uint> HtmlParser::htmlParser(QVector<QString> tagList){
      //QVector<QString> tagList(listTag(inputFile));
      QVector<uint> levelList;
      QString tag;
      bool orphelin = false;
      for (int i=0; i < tagList.size();i++) {
        tag = getTag(tagList[i]);
        if (i==0)
              levelList.push_back(0);
        else if ((tagList[i][0] == '<') && (orphelinTags.indexOf(getTag(tagList[i])) == -1)) {
              if (tagList[i][1] == '/') {
                    if ((getTag(tagList[i]) == getTag(tagList[i-1])) && (tagList[i-1][1] != '/'))
                      levelList.push_back(levelList[i-1]);
                    else 
                      levelList.push_back((levelList[i-1] > 0)?levelList[i-1]-1:0);
              }
              else if ((tagList[i-1][0]  == '<') && ((tagList[i-1][1]  != '/')) && (orphelinTags.indexOf(getTag(tagList[i-1])) == -1)) 
                    levelList.push_back(levelList[i-1]+1);
              else 
                    levelList.push_back(levelList[i-1]);
        }
        else {
              if ((tagList[i][0] == '<') && (orphelinTags.indexOf(tag) != -1))
                orphelin = true;
              if ((tagList[i-1][0]  == '<') && ((tagList[i-1][1]  != '/')) && (orphelinTags.indexOf(getTag(tagList[i-1])) == -1)) 
                    levelList.push_back(levelList[i-1]+1);
              else 
                    levelList.push_back(levelList[i-1]);
        }
    }
    return levelList;
}

void HtmlParser::updateTree(QString file, QTreeWidget* aTree) {
  /*HtmlData pageData;
  pageData.tagList = listTag(file);
  pageData.levelList = htmlParser(pageData.tagList);
  updateTree(pageData.tagList,pageData.levelList,aTree);*/
}

void HtmlParser::updateTree(QVector<QString> tagList, QVector<uint> levelList, QTreeWidget* aTree) {
  /*QTreeWidgetItem* previousNode; //TODO ?
  QTreeWidgetItem* aNode;
  aTree->clear();
  for (int j=0; j < tagList.size();j++) {
    if (levelList[j] == 0) {
      aNode = new QTreeWidgetItem(QStringList(tagList[j]));
      aTree->addTopLevelItem(aNode);
    }
    else if (levelList[j] > levelList[(j > 0)?j-1:0]) 
      aNode = new QTreeWidgetItem(previousNode,QStringList(tagList[j]));
    else if (levelList[j] == levelList[(j > 0)?j-1:0]) 
      aNode = new QTreeWidgetItem(previousNode->parent(),QStringList(tagList[j]));
    else 
      aNode = new QTreeWidgetItem(previousNode->parent()->parent(),QStringList(tagList[j]));
    previousNode = aNode;
  }
  aTree->expandAll();*/
}

/*QVector<QString> HtmlParser::ConvertToTemplate(QVector<QString> tagList, QString &markerDefinition) {
  markerList.clear();
  for (int k=0; k < tagList.size();k++) 
    if ((tagList[k][0] != '<') && (tagList[k].left(3) != "###")) {
          StringToMarker aPopup(&tagList,k,&markerDefinition, 0,&markerList);
          aPopup.exec();
    }
  return tagList;
}*/

QVector<QString> HtmlParser::translate(QVector<QString> tagList, QString markerDefinition) {
  //ConvertToTemplate(tagList, markerDefinition);
}

HtmlData HtmlParser::getHtmlData(QString inputFile) {
  HtmlData pageData;
  pageData.tagList = listTag(inputFile);
  pageData.levelList = htmlParser(pageData.tagList);
  return pageData;
}

QString HtmlParser::getParsedHtml(QString inputFile) {
  HtmlData pageData = getHtmlData(inputFile);
  return getParsedHtml(pageData);
}

QString HtmlParser::getParsedHtml(HtmlData &pageData) {
  QString tab, parsedHTML, tag2, previousTag;
  for (int j=0; j < pageData.tagList.size();j++) {
    tag2 = getTag(pageData.tagList[j]);
    tab.clear();
    if ((noNewLineTags.indexOf(tag2) != -1) || (pageData.tagList[j][0] != '<')) {
        if ((pageData.tagList[j][0] == '<') && (needNewLineOnOpen.indexOf(tag2) != -1) && (parsedHTML[parsedHTML.size()-1] != '\n') && (pageData.tagList[j].left(2) != "</")) {
              parsedHTML += "\n";
              for (int k =0; k < pageData.levelList[j]; k++) 
                    tab += "   ";
              parsedHTML += tab;
        }
        if (parsedHTML[parsedHTML.size()-1] == '\n') {
              for (int k =0; k < pageData.levelList[j]; k++) 
                    tab += "   ";
              parsedHTML += tab + pageData.tagList[j];
        }
        else 
              parsedHTML += pageData.tagList[j];
        if ((pageData.tagList[j].left(2) == "</") && (needNewLineOnClose.indexOf(tag2) != -1)) 
              parsedHTML += "\n";
    }
    else {
        if ((parsedHTML.right(1) != "\n") && (parsedHTML != "")) 
              parsedHTML += "\n";
        for (int k =0; k < pageData.levelList[j]; k++) 
              tab += "   ";
        parsedHTML += tab + pageData.tagList[j].trimmed() + "\n";
    }
  }
  return parsedHTML;
}