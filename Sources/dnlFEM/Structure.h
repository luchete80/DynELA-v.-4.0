/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2006                                                *
 *                                                                         *
 *      Equipe C.M.A.O                                                     *
 *      Laboratoire Genie de production                                    *
 *      Ecole Nationale d'Ingenieurs de Tarbes                             *
 *      BP 1629 - 65016 TARBES cedex                                       *
 *                                                                         *
 *                                                                         *
 *  Main Author: Olivier PANTALE                                           *
 *                                                                         *
 **************************************************************************/

// begin date :
// revision date :

/*
	Class Structure definition
*/

#ifndef __Structure_h__
#define __Structure_h__

#include <upFemSolver.h>
#include <Timer.h>
#include <iostream>

using namespace std;
class Tensor2;

class Structure;
extern Structure *Global_Structure;
class ElementSet;

#define versionCheckDefine 1.02
#define separatorCheckDefine 69

/*!
  \file Structure.h
  \brief fichier .h de d�finition des grilles �l�ments finis
  \ingroup femSolver

  Ce fichier sert � la d�finition de la classe Structure.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/

class Element;
class Domain;
class Node;
class Material;
class Solver;
class io_Structure;
class Interface;
class HistoryFile;
class BoundaryCondition;
class Boundary;
class NodeSet;

//#include <io_Structure.h>

/*!
  \class Structure Structure.h
  \brief Classe de d�finition et de manipulation des structures �l�ments finis.
  \ingroup femLibrary


  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
/** @dia:pos 119.065,0.2794 */
class Structure
{
public:
  double startSaveTime;
  double endSaveTime;
  double saveTime;
  double nextSaveTime;
  double previousSaveTime;
  Timers cpuTimes; //!< Store the CPU Times

private:
  /** @dia:route 20,26;h,123.065,81.1794,114.75,9.5794,119.065 */
  Element *defaultElement; //!< Element par d�faut
public:
  double currentTime; //!< Temps actuel de la structure

protected:
  /** @dia:route 9,4;h,247.15,72.7,273.75,0.9794,141.965 */
  List<Node *> nodes; //!< liste des noeuds de la structure
  /** @dia:route 24,22;h,123.065,82.7794,112.75,7.7794,119.065 */
  List<Element *> elements; //!< liste des elements de la structure

public:
  /** @dia:route 12,19;h,199.8,10.3,173.065,6.1794,141.965 */
  List<Domain *> domains; //!< liste des domaines de la structure
  /** @dia:route 13,12;h,103.865,3.7794,107.75,3.7794,119.065 */
  List<Material *> materials; //!< liste des mat�riaux de la structure
  /** @dia:route 0,2;h,-139.1,-164.8,96.5,-68.4794,-10.3647 */
  // LogFile *logFile; //!< Fichier de logs
  /** @dia:route 47,3;h,141.965,17.5794,162.75,22.9794,169.065 */
  io_Structure *resultFile;
  /** @dia:route 0,2;h,-120.6,-172.85,78,-68.4794,-10.3647 */
  String name;

public:
  // constructor
  Structure();
  Structure(const Structure &X);
  ~Structure();

  // member functions
  void setSaveTimes(double startST, double stopST, double ST);
  void createNode(long num, double x, double y, double z);
  void createElement(long num, ...);
  Node *getNodeByNum(long num);
  Element *getElementByNum(long num);
  Node *getNodeById(long num);
  Element *getElementById(long num);
  Node *getNode(long num);
  Element *getElement(long num);
  long getNodesNumber();
  long getElementsNumber();
  void setDefaultElement(Element *pel);
  Domain *getCurrentDomain() { return domains.current(); }
  bool initSolve();
  void addMaterial(Material *pmat);
  Material *getMaterial(String name);
  void attachMaterialToElements(Material *mat, ElementSet *els);
  void attachBCToNodes(BoundaryCondition *BC, NodeSet *els);
  void attachInitialBC(Boundary *B, NodeSet *els);
  void attachConstantBC(Boundary *bnd, NodeSet *nds);
  void addSolver(Solver *solver);
  void addHistoryFile(HistoryFile *hf);
  void setDomain(Domain *domain);
  void getGlobalBox(Vec3D &min, Vec3D &max);
  void displayOnline();
  void solve();
  void readData(ifstream &pfile);
  double getReadTimeData(ifstream &pfile);
  void writeData(ofstream &pfile);
  void checkBinaryVersionWrite(ofstream &pfile, int);
  bool checkBinaryVersion(ifstream &pfile, int);
  void sortElementsAndNodes();
  void saveResults();
  void addInterface(Interface *pinter);
  void translate(Vec3D vec, NodeSet *ns = NULL);
  void scale(double scale, NodeSet *ns = NULL);
  void scale(Vec3D scale, NodeSet *ns = NULL);
  void rotate(String set, double val, NodeSet *pns = NULL);
  void rotate(Vec3D axis, double val, NodeSet *pns = NULL);
  void compact();
  void readResultFile(long);
  double getCurrentTime();
  void getDataFileBounds(long &min, long &max);
  long getLastDataFileNumber();
  long getFirstDataFileNumber();
  double getNodalValue(long nod, String field, long component = 0);
  Vec3D getNodalVector(long nod, String field);
  Tensor2 getNodalTensor(long nod, String field);
  void setResultFile(String);
  void mergeDomains();
};

#endif
