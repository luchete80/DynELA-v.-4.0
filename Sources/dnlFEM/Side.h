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

// begin date : 22/05/1997

/*
	Class Side definition
*/

#ifndef __Side_h__
#define __Side_h__

#include <upFemSolver.h>
#include "dnlMaths.h"
/* #include <SideFace.h> */
class SideFace;
class NodeSet;
class lagMotion;
class Node;
class Interface;

/* #incluclass Node;
de <Domain.h> */

/**
*/
/** @dia:pos 289.8,97.8 */
class Side
{
  //protected:
public:
  /// type de la surface (master ou slave)
  bool type;
  /// liste des SideFaces qui composent la surface de contact
  /** @dia:route 29,13;h,306.05,107.9,315.75,186.5,250.25 */
  List<SideFace *> sides;
  /// Liste des noeuds qui composent la surface de contact
  /** @dia:route 45,10;h,247.15,87.3,260.75,100.7,289.8 */
  ListIndex<Node *> nodes;

public:
  /** @dia:route 25,11;h,302.85,84.3,308.75,100.7,306.05 */
  Interface *interface;

public:
  // constructeurs
  Side();
  Side(const Side &X);
  ~Side();

  // fonctions membres
  bool &isMaster();
  bool addSideFace(SideFace *pSideFace);
  void computeScanVolume();
  void computeSideFacesNormals();
  void Init();
  long scanIncludeNodes(Side *scanned_side);
  void toFile(FILE *pfile);
  bool check();
  void addNodeSet(NodeSet *nset);
};

#endif
