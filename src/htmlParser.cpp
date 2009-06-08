#include "htmlParser.h"
W3Cinfo HtmlParser::htmlInfo = W3Cinfo();

HtmlParser::HtmlParser()  {
  QSqlQuery query;
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDCLOSE = 0");
  while (query.next()) 
    htmlInfo.orphelinTags << query.value(0).toString();
  query.exec("SELECT NAME,IFNEEDNEWLINE FROM THTML_TAG WHERE IFNEEDNEWLINE <> 1");
  while (query.next()) {
    htmlInfo.noNewLineTags << query.value(0).toString();
    if ((query.value(0).toInt() >= 2)&&(query.value(0).toInt() <= 3))
      (query.value(0).toInt() == 2)?htmlInfo.noNewLineTags:htmlInfo.noNewLineTags << query.value(0).toString();
  }
}

QString HtmlParser::getTag(QString aTag) {
  QString tag;
  if (aTag.left(3) == "!--") 
    return "!--";
  tag = aTag.remove(0,(aTag[1] == '/')?2:1).trimmed(); //Remove < or </
  tag.chop((tag.indexOf(" ") == -1)?1:tag.size()-tag.indexOf(" ")); //Remove > and attribute //BUG fait with tag like <br/>
  return tag.toUpper();
}

void split(QVector<QString> &tagList, QString &inputFile, uint index) {
  tagList.push_back(inputFile.left(index).trimmed());
  inputFile = inputFile.remove(0,index);
}

QVector<QString> HtmlParser::listTag(QString inputFile) {
  QVector<QString> tagList;
  while (inputFile != "") {
    while ((inputFile[0] == 0x20/*space*/) || (inputFile[0] == 0x09/*tab*/) || (inputFile[0] == 0x0A/*line break*/))
      inputFile = inputFile.remove(0,1);
    if (inputFile.indexOf("<") == -1) 
      split(tagList, inputFile, inputFile.size());
    else if (inputFile.indexOf("<") < inputFile.lastIndexOf("<",inputFile.indexOf(">"))) 
      split(tagList, inputFile, inputFile.lastIndexOf("<",inputFile.indexOf(">")-1));
    else if (inputFile.left(4) == "<!--") 
      split(tagList, inputFile, inputFile.indexOf("->")+2);
    else if ((inputFile.left(7).toUpper() == "<SCRIPT")) {
      split(tagList, inputFile, inputFile.indexOf(">")+1);
      if (inputFile.indexOf("</script>",Qt::CaseInsensitive) != 0) 
	split(tagList, inputFile, inputFile.indexOf("</script>",Qt::CaseInsensitive)-1);
    }
    else if (inputFile.indexOf("<") == 0) 
      split(tagList, inputFile, inputFile.indexOf(">")+1);
    else if (inputFile != "") 
      split(tagList, inputFile, inputFile.indexOf("<"));
  }
  return tagList;
}

QVector<uint> HtmlParser::levelParser(QVector<QString> tagList){
  QVector<uint> levelList;
  QString tag;
  for (int i=0; i < tagList.size();i++) {
    tag = getTag(tagList[i]);
    if (i==0)
      levelList.push_back(0);
    else if ((tagList[i][0] == '<') && (htmlInfo.orphelinTags.indexOf(tag) == -1))
      if (tagList[i][1] == '/')
        if ((tag == getTag(tagList[i-1])) && (tagList[i-1][1] != '/'))
          levelList.push_back(levelList[i-1]);
        else 
          levelList.push_back((levelList[i-1] > 0)?levelList[i-1]-1:0);
      else if ((tagList[i-1][0]  == '<') && ((tagList[i-1][1]  != '/')) && (htmlInfo.orphelinTags.indexOf(getTag(tagList[i-1])) == -1)) 
        levelList.push_back(levelList[i-1]+1);
      else 
        levelList.push_back(levelList[i-1]);
    else 
      if ((tagList[i-1][0]  == '<') && ((tagList[i-1][1]  != '/')) && (htmlInfo.orphelinTags.indexOf(getTag(tagList[i-1])) == -1)) 
        levelList.push_back(levelList[i-1]+1);
      else 
        levelList.push_back(levelList[i-1]);
  }
  return levelList;
}

HtmlData HtmlParser::getHtmlData(QString inputFile) {
  HtmlData pageData= {listTag(inputFile),levelParser(pageData.tagList)};
  return pageData;
}

QString HtmlParser::getParsedHtml(QString inputFile) {
  HtmlData pageData = getHtmlData(inputFile);
  return getParsedHtml(pageData);
}

QString HtmlParser::getParsedHtml(HtmlData &pageData) {
  QString tab, parsedHTML, tag2, previousTag;
  if (pageData.tagList.size() == 1)
    return pageData.tagList[0].trimmed();
  for (int j=0; j < pageData.tagList.size();j++) {
    tag2 = getTag(pageData.tagList[j]);
    tab.clear();
    for (int k =0; k < pageData.levelList[j]; k++) 
        tab += "   ";
    if ((htmlInfo.noNewLineTags.indexOf(tag2) != -1) || (pageData.tagList[j][0] != '<')) {
      if ((pageData.tagList[j][0] == '<') && (htmlInfo.needNewLineOnOpen.indexOf(tag2) != -1) && (parsedHTML[parsedHTML.size()-1] != '\n') && (pageData.tagList[j].left(2) != "</"))
        parsedHTML += "\n" + tab;
      if (parsedHTML[(parsedHTML.size())?parsedHTML.size()-1:0] == '\n')
        parsedHTML += tab + pageData.tagList[j];
      else 
        parsedHTML += pageData.tagList[j];
      if ((pageData.tagList[j].left(2) == "</") && (htmlInfo.needNewLineOnClose.indexOf(tag2) != -1)) 
        parsedHTML += "\n";
    }
    else {
      if ((parsedHTML.right(1) != "\n") && (parsedHTML != "")) 
        parsedHTML += "\n";
      parsedHTML += tab + pageData.tagList[j].trimmed() + "\n";
    }
  }
  return parsedHTML;
}

void HtmlParser::setAttribute(HtmlData &pageData, QString tag, uint index, QString attribute, QString value) {
  int i=0;
  for (int j =0; j < pageData.tagList.size(); j++)
    if ((HtmlParser::getTag(pageData.tagList[j]).toUpper() == tag.toUpper()) && (pageData.tagList[j][1] != '/'))
      if (i == index) {
	if (getAttribute(pageData.tagList[j],attribute) == NULL)
	  pageData.tagList[j].insert(pageData.tagList[j].count() - 1, " " + attribute + "=\"" + value + "\"");
	else
	  pageData.tagList[j].replace(getAttribute(pageData.tagList[j],attribute),value);
	break;
      }
      else
	i++;
}

QString HtmlParser::getAttribute(QString tag, QString attribute) {
  int position = tag.toLower().indexOf(attribute.toLower());
  if (tag[position+attribute.count()] == '=')
    return (position == -1)?NULL:tag.mid(tag.indexOf("=",position)+2,tag.indexOf((tag.indexOf(" ",position) != -1)?" ":">",position)-2 - (position+1+attribute.count()));
  else
    return NULL; //It is just safer than checking position == -1 in if
}