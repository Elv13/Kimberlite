#include "htmlParser.h"
/*W3Cinfo HtmlParser::htmlInfo = load();

W3Cinfo load()  {
  QSqlQuery query;
  W3Cinfo someInfo;
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDCLOSE = 0");
  while (query.next()) 
    someInfo.orphelinTags << query.value(0).toString();
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 0");
  while (query.next()) 
    someInfo.noNewLineTags << query.value(0).toString();
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 2");
  while (query.next()) {
    someInfo.needNewLineOnClose << query.value(0).toString();
    someInfo.noNewLineTags << query.value(0).toString();
  }
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 3");
  while (query.next()) {
    someInfo.needNewLineOnOpen << query.value(0).toString();
    someInfo.noNewLineTags << query.value(0).toString();
  }
  return someInfo;
}*/

HtmlParser::HtmlParser()  {
  QSqlQuery query;
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDCLOSE = 0");
  while (query.next()) 
    htmlInfo.orphelinTags << query.value(0).toString();
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 0");
  while (query.next()) 
    htmlInfo.noNewLineTags << query.value(0).toString();
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 2");
  while (query.next()) {
    htmlInfo.needNewLineOnClose << query.value(0).toString();
    htmlInfo.noNewLineTags << query.value(0).toString();
  }
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 3");
  while (query.next()) {
    htmlInfo.needNewLineOnOpen << query.value(0).toString();
    htmlInfo.noNewLineTags << query.value(0).toString();
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
            tag.chop(1);
    }
  }
  return tag.toUpper();
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

QVector<uint> HtmlParser::htmlParser(QVector<QString> tagList){
  QVector<uint> levelList;
  QString tag;
  bool orphelin = false;
  for (int i=0; i < tagList.size();i++) {
    tag = getTag(tagList[i]);
    if (i==0)
      levelList.push_back(0);
    else if ((tagList[i][0] == '<') && (htmlInfo.orphelinTags.indexOf(getTag(tagList[i])) == -1)) {
      if (tagList[i][1] == '/') {
        if ((getTag(tagList[i]) == getTag(tagList[i-1])) && (tagList[i-1][1] != '/'))
          levelList.push_back(levelList[i-1]);
        else 
          levelList.push_back((levelList[i-1] > 0)?levelList[i-1]-1:0);
      }
      else if ((tagList[i-1][0]  == '<') && ((tagList[i-1][1]  != '/')) && (htmlInfo.orphelinTags.indexOf(getTag(tagList[i-1])) == -1)) 
        levelList.push_back(levelList[i-1]+1);
      else 
        levelList.push_back(levelList[i-1]);
    }
    else {
      if ((tagList[i][0] == '<') && (htmlInfo.orphelinTags.indexOf(tag) != -1))
        orphelin = true;
      if ((tagList[i-1][0]  == '<') && ((tagList[i-1][1]  != '/')) && (htmlInfo.orphelinTags.indexOf(getTag(tagList[i-1])) == -1)) 
        levelList.push_back(levelList[i-1]+1);
      else 
        levelList.push_back(levelList[i-1]);
    }
  }
  return levelList;
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
    if ((htmlInfo.noNewLineTags.indexOf(tag2) != -1) || (pageData.tagList[j][0] != '<')) {
      if ((pageData.tagList[j][0] == '<') && (htmlInfo.needNewLineOnOpen.indexOf(tag2) != -1) && (parsedHTML[parsedHTML.size()-1] != '\n') && (pageData.tagList[j].left(2) != "</")) {
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
      if ((pageData.tagList[j].left(2) == "</") && (htmlInfo.needNewLineOnClose.indexOf(tag2) != -1)) 
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
