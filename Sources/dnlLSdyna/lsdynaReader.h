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

struct ls_set {
  int m_id;
  std::vector <int> m_node_id;
};

struct ls_property {
  
  
};

struct ls_section{
  
  
};

struct ls_material{
  
  
};

struct ls_spc_node{
  int  m_node_id;
  bool m_fix_dof[6];
};

struct ls_load_curve{
  
};

//Boundary prescribed motion set
struct ls_bpm_set{
  int     m_set_id; //set id
  int     m_dof;    
  int     m_vad;    //1: velocity, 2: acceleration, 3: displacement
  int     m_lcid;   //load curve id
  double  m_sf;     //scale factor
};

/////////////////////////
//NON CLASS FUNCTIONS ///
/////////////////////////
// FOR FORTRAN OR OTHER LANGUAGE VERSION
//std::vector <string> getLines(char* fname){}
/* extern "C" */ void readNodes(char *fName, double **nodes, int *node_count);
void readSPCNodes(int *sections, int **node_ids, bool **dofs);

class lsdynaReader{
public:  
  lsdynaReader(){}
  lsdynaReader(const char *);

  int m_line_count;
  int m_node_count;
  int m_elem_count;
  std::vector <std::string> m_line;
  void readNodes();
  void removeComments();
  void readElementSolid();
  void readSPCSet();
  void readSPCNodes();
  void readSetNodeList();
  bool findSection(std::string str, int * ini_pos, int *end_pos, int i_from=0);
  void readBPMotionSet();
  bool readBPMNodes(); //*BOUNDARY_PRESCRIBED_MOTION_NODE
  
  std::vector < ls_node    >  m_node;
  std::vector < ls_element >  m_elem;
  std::vector < ls_spc_node > m_spc_nod;
  std::vector < ls_spc_node > m_spc_set;
  std::vector < ls_set >      m_set_nod;
  std::vector <ls_bpm_set>    m_bpm_set;
};


#endif