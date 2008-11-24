/**     @file htmlParser.cpp

	This file is part of the Kimberlite project
	Copyright (C) 2008 Emmanuel Lepage Vallee <elv1313@gmail.com>
	
	This software is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 or 3 of the License, or (at your option) any later version.

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public License
	along with this library; see the file COPYING.LIB.  If not, write to
	the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
	Boston, MA 02111-1307, USA.


        @author Emmanuel Lepage Vallée
        @date 13 August 2008
        @version 0.1-pre-alpah0
*/
#include <iostream>
#include <vector>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QtSql>
#include <string>
#include <algorithm>
#include "htmlParser.h"
#include "stringToTemplate.h"

using namespace std;

HtmlParser::HtmlParser()  {
  QSqlQuery query;
  query.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDCLOSE = 0");
  while (query.next()) 
    orphelinTags << query.value(0).toString();

  QSqlQuery query2;
  query2.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 0");
  while (query2.next()) 
    noNewLineTags << query2.value(0).toString();

  QSqlQuery query3;
  query3.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 2");
  while (query3.next()) {
    needNewLineOnClose << query3.value(0).toString();
    noNewLineTags << query3.value(0).toString();
  }

  QSqlQuery query4;
  query4.exec("SELECT NAME FROM THTML_TAG WHERE IFNEEDNEWLINE = 3");
  while (query4.next()) {
    needNewLineOnOpen << query4.value(0).toString();
    noNewLineTags << query4.value(0).toString();
  }
}

string HtmlParser::compressFile(string path) {
      QFile file(QString::fromStdString(path));
      file.open(QIODevice::ReadOnly);
      QString output;
      while (!file.atEnd())
	    output += file.readLine();
      return output.toStdString();
}

string HtmlParser::compressString(string file) {
  while (file.find(0x0A) != -1) 
    file.erase(file.find(0x0A),1);
  return file;
}

string HtmlParser::getTag(string aTag) {
  string tag;
  if (aTag != "") {
    if (aTag[1] == '!') 
      tag = "!--";
    else {
      tag = aTag.erase(0,1);
      while (tag[0] == ' ') 
	    tag = tag.substr(1, tag.size() -1);
      if (tag.find(" ") != -1) 
	    tag = tag.substr(0, tag.find(" "));
      else 
	    tag  = tag.substr(0, tag.size() -1);
      if (tag[0] == '/') 
	    tag = aTag.erase(0,1);
      if (tag[tag.size()-1] == '>') 
	    tag = aTag.erase(tag.size()-1,1);
      std::transform(tag.begin(), tag.end(), tag.begin(), (int(*)(int)) std::toupper);
    }
  }
    return tag;
}

vector<string> HtmlParser::listTag(string inputFile) {
      vector<string> tagList;
      while (inputFile != "") {
	    while ((inputFile[0] == ' ') || (inputFile[0] == '	') || (inputFile[0] == 0x0A))
	      inputFile = inputFile.erase(0,1);
	    if (inputFile.find("<") < inputFile.find_last_of("<",inputFile.find(">"))) {
		  tagList.push_back(inputFile.substr(0,inputFile.find_last_of("<",inputFile.find(">")-1)));
		  inputFile = inputFile.erase(0,inputFile.find_last_of("<",inputFile.find(">")-1));
	    }
	    else if (inputFile.substr(0,9) == "<!DOCTYPE") {
		  tagList.push_back(inputFile.substr(0,inputFile.find(">")+1));
		  inputFile = inputFile.erase(0,inputFile.find(">")+1);
	    }
	    else if (inputFile.substr(0,4) == "<!--") {
		  tagList.push_back(inputFile.substr(0,inputFile.find("->")+2));
		  inputFile = inputFile.erase(0,inputFile.find("->")+2);
	    }
	    else if ((inputFile.substr(0,7) == "<script")) { //TODO add SCRIPT too
		  tagList.push_back(inputFile.substr(0,inputFile.find(">")+1));
		  inputFile = inputFile.erase(0,inputFile.find(">")+1);

		  if (inputFile.find("</script>") != 0) {
		    tagList.push_back(inputFile.substr(0,inputFile.find("</script>")-1));
		    inputFile = inputFile.erase(0,inputFile.find("</script>")-1);
		  }
	    }
	    else if (inputFile.find("<") == 0) {
		  tagList.push_back(inputFile.substr(0,inputFile.find(">")+1));
		  inputFile = inputFile.erase(0,inputFile.find(">")+1);
	    }
	    else if (inputFile != "") {
		  tagList.push_back(inputFile.substr(0,inputFile.find("<")));
		  inputFile = inputFile.erase(0,inputFile.find("<"));
	    }
      }
  return tagList;
}

string HtmlParser::htmlParser(string inputFile, bool debug, bool toTemplate) {
      vector<string> tagList = listTag(inputFile);
      vector<int> levelList;
      string tag;
      for (int i=0; i < tagList.size();i++) {
	    tag = getTag(tagList[i]);
	    std::transform(tag.begin(), tag.end(), tag.begin(), (int(*)(int)) std::toupper);
	    cout << i << ": ";
	    if (i==0)
		  levelList.push_back(0);
	    else if ((tagList[i][0] == '<') && (orphelinTags.indexOf(QString::fromStdString(getTag(tagList[i]))) == -1)) {
		  if (tagList[i][1] == '/') {
			if ((getTag(tagList[i]) == getTag(tagList[i-1])) && (tagList[i-1][1] != '/'))
			  levelList.push_back(levelList[i-1]);
			else 
			  levelList.push_back(levelList[i-1]-1);
		  }
		  else if ((tagList[i-1][0]  == '<') && ((tagList[i-1][1]  != '/')) && (orphelinTags.indexOf(QString::fromStdString(getTag(tagList[i-1]))) == -1)) 
			levelList.push_back(levelList[i-1]+1);
		  else 
			levelList.push_back(levelList[i-1]);
	    }
	    else {
		  if ((tagList[i-1][0]  == '<') && ((tagList[i-1][1]  != '/')) && (orphelinTags.indexOf(QString::fromStdString(getTag(tagList[i-1]))) == -1)) 
			levelList.push_back(levelList[i-1]+1);
		  else 
			levelList.push_back(levelList[i-1]);
	    }
	    cout << levelList[i] << " " << tagList[i] << endl;

	    if (debug == true) {
	      if ((orphelinTags.indexOf(QString::fromStdString(tag)) != -1) && (tagList[i][1] == '/')) {
		cout << "warning \"" << tag << "\" don't need to be closed" << endl;
		//exit(33);
	      }
	    }
      }

      //bool toTemplate = true;
      string markerDefinition;
      if (toTemplate == true) {
	    tagList = ConvertToTemplate(tagList, markerDefinition);
      }

      string tab, parsedHTML, tag2, previousTag;
      for (int j=0; j < tagList.size();j++) {
	tag2 = getTag(tagList[j]);
        std::transform(tag2.begin(), tag2.end(), tag2.begin(), (int(*)(int)) std::toupper);
        tab = "";
        if ((noNewLineTags.indexOf(QString::fromStdString(tag2)) != -1) || (tagList[j][0] != '<')) {
	    if ((tagList[j][0] == '<') && (needNewLineOnOpen.indexOf(QString::fromStdString(tag2)) != -1) && (parsedHTML[parsedHTML.size()-1] != '\n') && (tagList[j].substr(0,2) != "</")) {
		  parsedHTML += "\n";
		  for (int k =0; k < levelList[j]; k++) 
			tab += "   ";
		  parsedHTML += tab;
	    }
	    if (parsedHTML[parsedHTML.size()-1] == '\n') {
		  for (int k =0; k < levelList[j]; k++) 
			tab += "   ";
		  parsedHTML += tab + tagList[j];
	    }
	    else {
		  parsedHTML +=tagList[j];
	    }
	    if ((tagList[j].substr(0,2) == "</") && (needNewLineOnClose.indexOf(QString::fromStdString(tag2)) != -1)) {
		  parsedHTML += "\n";
	    }
        }
        else {
	    if ((parsedHTML[parsedHTML.size()-1] != '\n') && (parsedHTML != "")) {
		  parsedHTML += "\n";
	    }
	    for (int k =0; k < levelList[j]; k++) 
		  tab += "   ";
	    parsedHTML += tab + tagList[j] + "\n";
        }
      }
      if (toTemplate == true) {
	    parsedHTML += "<!-- \n" + markerDefinition + " \n-->";
      }

      return parsedHTML;
}


vector<string> HtmlParser::ConvertToTemplate(vector<string> tagList, string &markerDefinition) {
      markerList.clear();
      for (int k=0; k < tagList.size();k++) {
	    if (tagList[k][0] != '<') {
		  StringToMarker aPopup(&tagList,k,&markerDefinition, 0,&markerList);
		  aPopup.exec();
		  /*if (tagList[k].substr(0,3) == "###") {
			
		  }*/
	    }
      }
      return tagList;
}

vector<string> HtmlParser::translate(vector<string> tagList, string markerDefinition) {
      markerList.clear();
      for (int k=0; k < tagList.size();k++) {
	    if (tagList[k][0] != '<') {
		  StringToMarker aPopup(&tagList,k,&markerDefinition, 0,&markerList);
		  aPopup.exec();
		  /*if (tagList[k].substr(0,3) == "###") {
			
		  }*/
	    }
      }
      return tagList;
}