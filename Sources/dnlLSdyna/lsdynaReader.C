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
  cout << "reading "<<str<<endl;
  double ret = 0.0;
    std::string f_str = str.substr(pos,length);
    f_str = removeSpaces(f_str);
    // int _rep = f_str.find("E");
    // if (_rep!=std::string::npos){
      // f_str.replace(_rep,1,"e");
      // // if (f_str[_rep+1]=='+')f_str.erase(_rep+1,1);
      
      // cout << "found E"<<", "<<"corrected: "<<f_str<<endl;
      
      // // sscanf(f_str.c_str(),"%f", ret); 
      // cout << "num: "<<ret<<endl;
    // }
  if (str.size()<pos+length) {
    cout << "ERROR TRYING TO READ: "<<str<<", subs: " <<f_str<<endl;
    return ret;
  }
  // return ret;
  ret = strtod(f_str.c_str(),NULL);
  // cout << "readed: "<<ret;
  return ret;
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
        end_pos = ini_pos + m_node_count ;
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
      cout << "Node "<<id <<"XYZ: "<<nod.m_x[0]<<", "<<nod.m_x[1]<<", "<<nod.m_x[2]<<endl; 
    cout << "ID: "<<id<<end;
    
    
  }

}  //line


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
  
  std::vector<std::string>::iterator it = m_line.begin();
  for (int i=0;i<m_line_count;i++) {
          start = end = 0;
          //cout << m_line[i]<<endl;
          // start = line.find_first_not_of(dl, end);
          if (!isComment(line)){
            if (m_line[i].find('*') != std::string::npos){
              //cout << "COMMAND FOUND at "<< line.find("*")<<", line "<<m_line_count<<endl;
                
            } 
            if (m_line[i].find("*NODE") != std::string::npos){
              cout << "Node command found at line "<<m_line_count<<endl;
              
              cout << "Node count: "<<findNextCommandLine(i,m_line) - i<<endl;
            }
          } else {
            // cout << "Commented line "<<m_line_count<<endl;
          }
          // if (start != string::npos){
            
          // }
          
          
    it++;

    }  //line

}