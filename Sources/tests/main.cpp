#include "DynELA.h"
#include "Element.h"
#include "ElementSet.h"
#include "NodeSet.h"
#include "Model.h"
#include "Material.h"
#include "HardeningLaw.h"

#include "BoundarySpeed.h"
#include "BoundaryRestrain.h"
#include "Explicit.h"
#include "ElasticLaw.h"
#include "HistoryFile.h"

#include "Field.h"


#include "lsdynaReader.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  if (argc > 1) {
  //# Parameters of the model
  int nbreSaves = 20;
  double stopTime = 1e-3;
  int nbrePoints = 1000;
  int displacement = 7;
  double speed = 1.0;

  //# Material parameters
  double young = 206000;
  double poisson = 0.3;
  double density = 7.83e-09;
  double heatCapacity = 4.6e+08;
  double taylorQuinney = 0.9;
  double A = 806.0;
  double B = 614.0;
  double C = 0.0089;
  double n = 0.168;
  double m = 1.1;
  double depsp0 = 1.0;
  double Tm = 1540.0;
  double T0 = 20.0;
  
  char *name="BarNecking";
  DynELA model(name);

  cout << "Reading "<<argv[1]<<endl;
  string fName = argv[1];
  lsdynaReader reader(fName.c_str());
  
  cout << "Creating nodes..."<<endl;
  for (int n=0;n<reader.m_node_count;n++){
    model.createNode(reader.m_node[n].m_id, 
                     reader.m_node[n].m_x[0],
                     reader.m_node[n].m_x[1],
                     reader.m_node[n].m_x[2]);
  }
  
  cout << "Element count:" << reader.m_elem_count<<endl; 
  cout << "Creating elements... "<<endl;
  for (int e=0;e<reader.m_elem_count;e++){
    model.setDefaultElement(Element::ElHex8N3D);
    // cout << "nodes: ";
    // for (int i=0;i<8;i++) cout << reader.m_elem[e].node[i]<<", ";
    // cout <<endl;
    model.createElement(reader.m_elem[e].id, 
                          reader.m_elem[e].node[0], reader.m_elem[e].node[1], 
                          reader.m_elem[e].node[2], reader.m_elem[e].node[3], 
                          reader.m_elem[e].node[4], reader.m_elem[e].node[5], 
                          reader.m_elem[e].node[6], reader.m_elem[e].node[7]);
  }


  /// THIS SHOULD CHECK IF ALL BCS ARE THE SAME, THEN CREATE AN UNIQUE SET.
  /// 
  // NodeSet boundarySPC_NOD ("NS_boundarySPC");  //FROM BOUNDARY_SPC_NODE KEYWORD(s)
  // for (int bcn = 0; bcn < reader.m_spc_nod.size(); bcn++ ){
    // model.add(&boundarySPC_NOD, reader.m_spc_nod[bcn].m_node_id);
  // }
  cout << "Creating " <<reader.m_set_nod.size() <<" Node Sets..." << endl;
  for (int ns = 0; ns < reader.m_set_nod.size(); ns++ ){
    cout << "Node set "<< ns << ", node count: "<<reader.m_set_nod[ns].m_node_id.size()<<endl;
    NodeSet boundarySPC_SET ("NS_boundarySPC");  //FROM BOUNDARY_SPC_NODE KEYWORD(s)
    for (int bcn = 0; bcn < reader.m_set_nod[ns].m_node_id.size(); bcn++ ){
      model.add(&boundarySPC_SET, reader.m_set_nod[ns].m_node_id[bcn]);
    }
  }
  cout << "Node Set count: "<<reader.m_set_nod.size()<<endl;
  // model.solve();

  // svg.write('temperatureCP.svg', dnl.Field.T)
  // svg.write('vonMisesCP.svg', dnl.Field.vonMises)
  // svg.write('plasticStrainCP.svg', dnl.Field.plasticStrain)

  // # Plot the results as curves
  // import dnlCurves as cu
  // curves = cu.Curves()
  // curves.plotFile('Curves.ex')



  

  ElementSet allES("ES_All");
  model.add(&allES,1,1);

  NodeSet topNS ("NS_Top");
  model.add(&topNS,   5);
  model.add(&topNS,   6);
  model.add(&topNS,   7);
  model.add(&topNS,   8);
  
	omp_set_num_threads(1);
	
  NodeSet bottomNS("NS_Bottom");   model.add(&bottomNS, 1);
  NodeSet bottomNSy("NS_Bottomy"); model.add(&bottomNS, 2);
  NodeSet bottomNSx("NS_Bottomx"); model.add(&bottomNS, 3);
  NodeSet bottomNSz("NS_Bottomz"); model.add(&bottomNS, 4);
  // model.add(symzNS, 1474)
  // model.add(symzNS, 1475)
  // model.add(symzNS, 1476)
  // model.add(symzNS, 1477)
  // model.add(symzNS, 1478)

  // histNS = dnl.NodeSet('NS_Hist')
  // model.add(histNS, 1)

  // histES = dnl.ElementSet('ES_Hist')
  // model.add(histES, 910)

  // # Creates the hardening law
  // HardeningLaw hardLaw = dnl.JohnsonCookLaw();
  ElasticLaw *hardLaw = new ElasticLaw;
  // hardLaw.setParameters(A, B, C, n, m, depsp0, Tm, T0)

  // # Creates the material
  Material steel("Steel");
  steel.setHardeningLaw(hardLaw);
  steel.youngModulus = young;
  steel.poissonRatio = poisson;
  steel.density = density;
  //steel.heatCapacity = heatCapacity;
  //steel.taylorQuinney = taylorQuinney;
  //steel.T0 = T0;

  // # Finaly link the material to the structure
  model.add(&steel, &allES);

  // # Declaration of a boundary condition for bottom line
  BoundaryRestrain bottomBC ("BC_bottom");
  bottomBC.setValue(1, 1, 1);
  model.attachConstantBC(&bottomBC, &bottomNS);

  std::vector <BoundaryRestrain*> BC_set;
  for (int ns = 0; ns < reader.m_set_nod.size(); ns++ ){
    //NodeSet boundarySPC_SET ("NS_boundarySPC");  //FROM BOUNDARY_SPC_NODE KEYWORD(s)
    BC_set.push_back ( new BoundaryRestrain ("BC_set"));
    BC_set[ns]->setValue(1, 0, 1);
    //Search node set
    model.attachConstantBC(BC_set[ns], &bottomNS);
  }
  
  BoundaryRestrain bottomBCx ("BC_bottomxz");
  bottomBC.setValue(1, 0, 1);
  model.attachConstantBC(&bottomBCx, &bottomNS);
  
  BoundaryRestrain bottomBCy ("BC_bottomyz");
  bottomBC.setValue(0, 1, 1);
  model.attachConstantBC(&bottomBCy, &bottomNS);

  BoundaryRestrain bottomBCz ("BC_bottomz");
  bottomBC.setValue(0, 0, 1);
  model.attachConstantBC(&bottomBCx, &bottomNS);  

  // # Declaration of a boundary condition for SYMX plane
  // symxBC = dnl.BoundaryRestrain('SYMX_plane')
  // symxBC.setValue(1, 0, 0)
  // model.attachConstantBC(symxBC, symxNS)

  // # Declaration of a boundary condition for SYMZ plane
  // symzBC = dnl.BoundaryRestrain('SYMZ_plane')
  // symzBC.setValue(0, 0, 1)
  // model.attachConstantBC(symzBC, symzNS)

  // # Declaration of a boundary condition for top line
  BoundarySpeed speedBC ("BC_speed");
  speedBC.setValue(0, 0, -speed);
  model.attachConstantBC(&speedBC, &topNS);

  Explicit solver("Solver");
  solver.setTimes(0, stopTime);
  model.add(&solver);
  model.setSaveTimes(0, stopTime, stopTime / nbreSaves);

  // # Declaration of the history files
  HistoryFile vonMisesHist("vonMisesHistory");
  // vonMisesHist = dnl.HistoryFile('vonMisesHistory')
  vonMisesHist.setFileName("vonMises.plot");
  vonMisesHist.add(&allES, 0, Field::vonMises);
  // vonMisesHist.setSaveTime(stopTime / nbrePoints)
  model.add(&vonMisesHist);

  // plasticStrainHist = dnl.HistoryFile('plasticStrainHistory')
  // plasticStrainHist.setFileName('plasticStrain.plot')
  // plasticStrainHist.add(histES, 0, dnl.Field.plasticStrain)
  // plasticStrainHist.setSaveTime(stopTime / nbrePoints)
  // model.add(plasticStrainHist)

  // temperatureHist = dnl.HistoryFile('temperatureHistory')
  // temperatureHist.setFileName('temperature.plot')
  // temperatureHist.add(histES, 0, dnl.Field.T)
  // temperatureHist.setSaveTime(stopTime / nbrePoints)
  // model.add(temperatureHist)

  // internalEnergyHist = dnl.HistoryFile('internalEnergyHistory')
  // internalEnergyHist.setFileName('internalEnergy.plot')
  // internalEnergyHist.add(histES, 0, dnl.Field.internalEnergy)
  // internalEnergyHist.setSaveTime(stopTime / nbrePoints)
  // model.add(internalEnergyHist)

  // dtHist = dnl.HistoryFile('dtHistory')
  // dtHist.setFileName('dt.plot')
  // dtHist.add(dnl.Field.timeStep)
  // dtHist.setSaveTime(stopTime / nbrePoints)
  // model.add(dtHist)

  // keHist = dnl.HistoryFile('keHistory')
  // keHist.setFileName('ke.plot')
  // keHist.add(dnl.Field.kineticEnergy)
  // keHist.setSaveTime(stopTime / nbrePoints)
  // model.add(keHist)

  // gammaHist = dnl.HistoryFile('gammaHistory')
  // gammaHist.setFileName('gamma.plot')
  // gammaHist.add(histES, 1, dnl.Field.gamma)
  // gammaHist.add(histES, 2, dnl.Field.gamma)
  // gammaHist.add(histES, 3, dnl.Field.gamma)
  // gammaHist.add(histES, 4, dnl.Field.gamma)
  // gammaHist.setSaveTime(stopTime / nbrePoints)
  // model.add(gammaHist)

  // gammaCumulateHist = dnl.HistoryFile('gammaCumulateHistory')
  // gammaCumulateHist.setFileName('gammaCumulate.plot')
  // gammaCumulateHist.add(histES, 1, dnl.Field.gammaCumulate)
  // gammaCumulateHist.add(histES, 2, dnl.Field.gammaCumulate)
  // gammaCumulateHist.add(histES, 3, dnl.Field.gammaCumulate)
  // gammaCumulateHist.add(histES, 4, dnl.Field.gammaCumulate)
  // gammaCumulateHist.setSaveTime(stopTime / nbrePoints)
  // model.add(gammaCumulateHist)

  // # Declaration of the history files
  // vonMisesHist2 = dnl.HistoryFile('vonMisesHistory2')
  // vonMisesHist2.setFileName('vonMises2.plot')
  // vonMisesHist2.add(histES, 1, dnl.Field.vonMises)
  // vonMisesHist2.add(histES, 2, dnl.Field.vonMises)
  // vonMisesHist2.add(histES, 3, dnl.Field.vonMises)
  // vonMisesHist2.add(histES, 4, dnl.Field.vonMises)
  // vonMisesHist2.setSaveTime(stopTime / nbrePoints)
  // model.add(vonMisesHist2)

  // # Parallel computation
  // model.parallel.setCores(4)

  // svg = dnl.SvgInterface('SVG')
  // svg.setTitleDisplay(False)
  // svg.setLegendPosition(100, 280)
  // svg.rotate(dnl.Vec3D(0, 1, 0), 190)
  // svg.rotate(dnl.Vec3D(1, 0, 0), -70)
  // svg.rotate(dnl.Vec3D(0, 1, 0), -60)
  // svg.write('mesh.svg')
  
  } else {
    cout << "Please enter input file "<<endl;
  }
  return 0;
}
