#ifndef _LS_DYNA_READER_
#define _LS_DYNA_READER_

#include <string>
#include <vector>

class Keyword {
public:
};

#define FLOAT_FIELD   0
#define INT_FIELD     1


double readDoubleField(std::string &str, const int &pos, const int &length);
int    readIntField   (std::string &str, const int &pos, const int &length);
  
struct ls_node {
  ls_nodeconst (const int &id_, const double &x, const double &y, const double &z){
    m_id = id_;
    m_x[0]=x;m_x[1]=y;m_x[2]=z;
  }
  int m_id;
  double m_x[3];
};

struct ls_element {
  int id;
  int pid;  //Part
  std::vector <int> node;
};

struct ls_property {
  
  
};

struct ls_section{
  
  
};

struct ls_material{
  
  
};

struct ls_spc{
  int  m_node_id;
  bool m_fix_UR[6];
};

class lsdynaReader{
public:  
  lsdynaReader(){}
  lsdynaReader(const char *);

protected:
  int m_line_count;
  int m_node_count;
  int m_elem_count;
  std::vector <std::string> m_line;
  void readNodes();
  void removeComments();
  void readElementSolid();
  void readSPCNodes();
  bool findSection(std::string &str, int * ini_pos, int *end_pos);
  
  std::vector < ls_node    > m_node;
  std::vector < ls_element > m_elem;
};


#endif