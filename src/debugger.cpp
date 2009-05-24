#include "debugger.h"

QVector<debugItem> HtmlDebugger::debug(HtmlData& pageData) {
  QString tag;
  QVector<debugItem> debugVector;
  qDebug() << "Size =" <<  QString::number(pageData.tagList.size()) << ", First =";
  for (int i=0; i < pageData.tagList.size();i++) {
    qDebug() << "Tag = " << pageData.tagList[i];
    tag = HtmlParser::getTag(pageData.tagList[i]);
      if ((HtmlParser::htmlInfo.orphelinTags.indexOf(tag) != -1) && (pageData.tagList[i][1] == '/')) 
        debugVector << *(new debugItem(i, 0,  "Warning:  tag \"" + tag + "\" don't need to be closed (HTML 4.01)"));
      else if ((HtmlParser::htmlInfo.orphelinTags.indexOf(tag) == -1) && (pageData.tagList[i][0] == '<') && (pageData.tagList[i][1] == '/')) { //TODO remove all duplicated if crap
        int k =i-1;
        bool found = false;
        while ((k >=0) && (found == false)) {
          if (pageData.levelList[i] == pageData.levelList[k])
            found = true;
          else
            k--;
        }
        if ((found == true) && (HtmlParser::getTag(pageData.tagList[i]) != HtmlParser::getTag(pageData.tagList[k]))) 
          debugVector << *(new debugItem(k, 1, "Error:  tag \"" + HtmlParser::getTag(pageData.tagList[k]) + "\" does not close at the right place"));
      }
      else if ((HtmlParser::htmlInfo.orphelinTags.indexOf(tag) != -1) && (pageData.tagList[i][pageData.tagList[i].size() -2] != '/') && (pageData.tagList[i].indexOf(" ") != -1) && (pageData.tagList[i][1] != '!'))  //TODO recheck if it need "else"
        debugVector << *(new debugItem(i, 0, "Warning:  Missing \"/\" at the end of tag \"" + HtmlParser::getTag(pageData.tagList[i]) + "\" add it to comply with xHTML standard"));
      
      if ((tag == "SCRIPT") && (HtmlParser::getTag(pageData.tagList[i+1]) != "SCRIPT"))  //BUG array overflow possible
        debugVector << *(new debugItem(i, 3, "Information: Using javascript or css code directly in HTML file is not a good practice, use separate file"));
      else if ((tag == "STYLE") && (HtmlParser::getTag(pageData.tagList[i+1]) != "STYLE"))  //BUG array overflow possible
        debugVector << *(new debugItem(i, 3, "Information: Using javascript or css code directly in HTML file is not a good practice, use separate file"));
    
  }
  
  if ((pageData.levelList.size() != 0) && (pageData.levelList[pageData.levelList.size() -1] != 0)) 
    debugVector << *(new debugItem(pageData.levelList.size(), 1, "Error:  Parsing failed, one or more tag may be missing"));

  return debugVector;
}