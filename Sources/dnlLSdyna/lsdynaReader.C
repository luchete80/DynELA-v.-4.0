#include "lsdynaReader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

bool isComment(const string &line){
  
  bool ret = false;
  if (line.find('$') == 0)
    ret = true;
  return ret;
}
// str.erase(
  // std::remove_if(str.begin(), str.end(), [](char chr){ return chr == '&' || chr == ' ';}),
  // str.end());

int findNextCommandLine(const int &curr_line, std::vector <std::string> line) {
  string first;
  int i = curr_line;
  bool end = false;
  while (!end) {
    i++;
    if (i==line.size()) end = true; 
      first = line[i].substr(0,1);
      //cout << "first "<<first <<", "<<line[i]<<endl;
      //Can a line start with spaces??
      //if (first.find("$")!=string::npos)
      if (first=="*"){
        //cout << "FOUND * at line "<<i<<endl;
        end = true;
      }
  }// while end
  
        //cout << "* NOT FOUND "<<i<<endl;
  return i;
}

void lsdynaReader::removeComments(){
  std::vector<std::string>::iterator it;
  int i=0;
  for (it = m_line.begin();it!=m_line.end();){
    //cout << *it<<endl;
    string first = it->substr(0,1);
    //cout << first <<endl;
    if (first=="$"){
      m_line.erase(it);
      m_line_count--;
    } else {
      it ++;
      //cout << "NOT $ FOUND "<<first<<", LIN: "<<*it<<endl;
    }
    i++;
  }
}

string removeSpaces(string str) 
{ 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    return str; 
}

double readDoubleField(string &str, const int &pos, const int &length) {
  double ret = 0.0;
    std::string f_str = str.substr(pos,length);
    f_str = removeSpaces(f_str);

  if (str.size()<pos+length) {
    cout << "ERROR TRYING TO READ: "<<str<<", subs: " <<f_str<<endl;
    return ret;
  }
  // return ret;
  ret = strtod(f_str.c_str(),NULL);
  // cout << "readed: "<<ret;
  return ret;
}

int readIntField(string &str, const int &pos, const int &length) {
  int ret = 0.0;
    std::string f_str = str.substr(pos,length);
    f_str = removeSpaces(f_str);

  if (str.size()<pos+length) {
    cout << "ERROR TRYING TO READ: "<<str<<", subs: " <<f_str<<endl;
    return ret;
  }
  // return ret;
  ret = stoi(f_str.c_str());
  // cout << "readed: "<<ret;
  return ret;
}

bool lsdynaReader::findSection(string &str, int * ini_pos, int *end_pos){
  
    bool end = false;
  int i = 0;
  cout << "Reading Elements"<<endl;
  while (!end){

      if (m_line[i].find(str) != std::string::npos){
        cout << "Element Solid found at line "<<m_line_count<<endl;
        *ini_pos = i+1;
        m_elem_count = findNextCommandLine(i,m_line) - i;
        end_pos = ini_pos + m_elem_count -1 ;
        cout << "Elem count: "<<m_elem_count<<endl;
        end = true;
      }
    if (i==m_line_count) {
      end = true;
      cout << "ELEMENT not defined "<<endl;
    }
    i++;
  } 
}

void lsdynaReader::readNodes() {
  bool end = false;
  int ini_pos, end_pos;
  int i = 0;
  while (!end){

      if (m_line[i].find("*NODE") != std::string::npos){
        cout << "Node command found at line "<<m_line_count<<endl;
        ini_pos = i+1;
        m_node_count = findNextCommandLine(i,m_line) - i;
        end_pos = ini_pos + m_node_count -1;
        cout << "Node count: "<<m_node_count<<endl;
        end = true;
      }
    if (i==m_line_count) {
      end = true;
      cout << "NODES not defined "<<endl;
    }
    i++;
  } 
  
  for (i=ini_pos;i<end_pos;i++){
    int id;
    id = readDoubleField(m_line[i], 0, 8);
    ls_node nod;
    nod.m_id = id;
    for (int d=0;d<3;d++)
      nod.m_x[d] = readDoubleField(m_line[i], 8+16*d, 16);
      // cout << "Node "<<id <<"XYZ: "<<nod.m_x[0]<<", "<<nod.m_x[1]<<", "<<nod.m_x[2]<<endl; 
    
    
  }

}  //line

void lsdynaReader::readElementSolid() {
  bool end = false;
  int ini_pos, end_pos;
  int i = 0;
  cout << "Reading Elements"<<endl;
  while (!end){

      if (m_line[i].find("*ELEMENT_SOLID") != std::string::npos){
        cout << "Element Solid found at line "<<m_line_count<<endl;
        ini_pos = i+1;
        m_elem_count = findNextCommandLine(i,m_line) - i;
        end_pos = ini_pos + m_elem_count -1 ;
        cout << "Elem count: "<<m_elem_count<<endl;
        end = true;
      }
    if (i==m_line_count) {
      end = true;
      cout << "ELEMENT not defined "<<endl;
    }
    i++;
  } 
  
  for (i=ini_pos;i<end_pos;i++){
    int id, pid;
    ls_element ls_el;
    int nodecount;
    if (m_line[i].size()>16) nodecount = (int)((m_line[i].size()-16)/8);
    // cout << "Elem node count "<<nodecount <<endl;
    ls_el.id  = readIntField(m_line[i], 0, 8);
    ls_el.pid = readIntField(m_line[i], 1, 8);
    ls_node nod;
    nod.m_id = id;
    for (int d=0;d<nodecount;d++)
      ls_el.node.push_back(readIntField(m_line[i], 16+8*d, 8));
      // cout << "Node "<<id <<"XYZ: "<<nod.m_x[0]<<", "<<nod.m_x[1]<<", "<<nod.m_x[2]<<endl; 
      m_elem.push_back(ls_el);
    
  }

}  //line

void lsdynaReader::readSPCNodes(){
  
    bool end = false;
  int ini_pos, end_pos;
  int i = 0;
  cout << "Reading Elements"<<endl;
  while (!end){

      if (m_line[i].find("*ELEMENT_SOLID") != std::string::npos){
        cout << "Element Solid found at line "<<m_line_count<<endl;
        ini_pos = i+1;
        m_elem_count = findNextCommandLine(i,m_line) - i;
        end_pos = ini_pos + m_elem_count -1 ;
        cout << "Elem count: "<<m_elem_count<<endl;
        end = true;
      }
    if (i==m_line_count) {
      end = true;
      cout << "ELEMENT not defined "<<endl;
    }
    i++;
  } 
  
  for (i=ini_pos;i<end_pos;i++){
    int id, pid;
    ls_element ls_el;
    int nodecount;
    if (m_line[i].size()>16) nodecount = (int)((m_line[i].size()-16)/8);
    // cout << "Elem node count "<<nodecount <<endl;
    ls_el.id  = readIntField(m_line[i], 0, 8);
    ls_el.pid = readIntField(m_line[i], 1, 8);
    ls_node nod;
    nod.m_id = id;
    for (int d=0;d<nodecount;d++)
      ls_el.node.push_back(readIntField(m_line[i], 16+8*d, 8));
      // cout << "Node "<<id <<"XYZ: "<<nod.m_x[0]<<", "<<nod.m_x[1]<<", "<<nod.m_x[2]<<endl; 
      m_elem.push_back(ls_el);
    
  }
  
}

lsdynaReader::lsdynaReader(const char *fname){
  string line;
  m_line_count = 0;
  int start, end;
  char dl = ' ';
  ifstream file(fname);
  if (file.is_open()) {
    while (getline(file, line)) {
      m_line.push_back(line);
      m_line_count++;
    }
  }
        file.close();
  cout << "Line count: "<< m_line_count << endl;
  
  removeComments();
  cout << "Line count w/o comments: "<< m_line_count << endl;
  
  readNodes();
  readElementSolid();
  


}