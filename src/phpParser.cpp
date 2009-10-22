#include "phpParser.h"

QHash<int, QString> PhpParser::extractPhp(HtmlData &page) {
  int id =1000;
  QHash<int, QString> toReturn;
  for (int i=0;i < page.levelList.size();i++)
    if (page.tagList[i].left(5) == "<?php") {
      toReturn[id] = page.tagList[i];
      page.tagList.insert(i, "<kimberlitePhpMarker style=\"background-color:orange;\" id=\""+QString::number(id)+"\">" );
      page.tagList.insert(i+2, "</kimberlitePhpMarker>" );
      page.levelList.insert(i, 0 );
      page.levelList.insert(i+2, 0 );
      id++;
      i++;
    }
    
  for (int i=0;i < page.levelList.size();i++) {
    if (page.tagList[i].left(4) == "<div") {
      page.tagList.insert(i, "<kimberliteBlocMarker style=\"border-color:#FFDD00;border-style:dotted;border-width:1px;display:block;margin:1px;\">" );
      page.levelList.insert(i, 0 );
      i++;
    }
    else if (page.tagList[i].left(6) == "</div>") {
      page.tagList.insert(i+1, "</kimberliteBlocMarker>" );
      page.levelList.insert(i+1, 0 );
      i++;
    }
  }
  return toReturn;
}

void PhpParser::restorePhp(HtmlData &page, QHash<int, QString> snipet) {
  for (int i=0;i < page.tagList.size();i++) 
    if ((HtmlParser::getTag(page.tagList[i]) == "KIMBERLITEPHPMARKER") && (page.tagList[i].left(2) != "</")) {
      if (snipet.find(HtmlParser::getAttribute(page.tagList[i],"id").toInt()) != snipet.end()) {
	QString phpCode = snipet[HtmlParser::getAttribute(page.tagList[i],"id").toInt()];
	int level = page.levelList[i];
	page.tagList.remove(i);
	page.levelList.remove(i);
	page.tagList.insert(i,phpCode);
	page.levelList.insert(i,level);
	int found = i;
	
	while (!((HtmlParser::getTag(page.tagList[found]) == "KIMBERLITEPHPMARKER") && (page.tagList[found].left(2) == "</")) && (found < page.tagList.size())) 
	  found++;
	page.tagList.erase(page.tagList.begin()+i+1,page.tagList.begin()+i+(found -i)+1);
	page.levelList.erase(page.levelList.begin()+i+1,page.levelList.begin()+i+(found -i)+1);
	i++;
      }
    }
    
  for (int i=0;i < page.tagList.size();i++) 
    if (HtmlParser::getTag(page.tagList[i]) == "KIMBERLITEBLOCMARKER") {
      page.tagList.remove(i);
      page.levelList.remove(i);
      i--;
    }
    else if (HtmlParser::getTag(page.tagList[i]) == "KIMBERLITELABELMARKER") {
      for (int j=0;j < 7;j++) {
	page.tagList.remove(i);
	page.levelList.remove(i);
      }
      /*while (page.tagList[i] != "</kimberliteLabelMarker>") {
	page.tagList.remove(i);
	page.levelList.remove(i);
      }*/
    }
}

void PhpParser::testReplacePhp(HtmlData &page) {
  for (int i=0;i < page.tagList.size();i++) 
    if (page.tagList[i].left(5) == "<?php") 
      page.tagList[i] = "<b><center>Ceci est le <font color=pink>text </font> ki <i>etais</i> en php</center></b>";
}
