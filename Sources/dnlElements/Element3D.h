/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_Element3D_h__
#define __dnlElements_Element3D_h__

#include <Element.h>

//-----------------------------------------------------------------------------
// Class : Element3D
//
// Used to manage Elements in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Element3D : public Element
{

public:
  // constructeurs
  Element3D(long elementNumber = 1);
  Element3D(const Element3D &X);
  ~Element3D();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const = 0;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const = 0;
  double getCharacteristicLength() = 0;
  double getRadiusAtIntegrationPoint();
  double getRadiusAtUnderIntegrationPoint();
  bool computeJacobian(bool reference = false);
  bool computeUnderJacobian(bool reference = false);
  void computeDeformationGradient(Tensor2 &F, short time);
  bool checkLevel2();
  void getV_atIntPoint(Vec3D &v, short time);
  void getdV_atIntPoint(Tensor2 &dv, short time);
  void computeElasticStiffnessMatrix(bool underIntegration = false);

  /*   long numberOfUnderIntegrationPoints () = 0;
  double getVolume () = 0;
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) = 0;
   //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;
   */
  virtual void computeGlob2Loc () = 0;
  virtual void glob2Loc (const Vec3D & point, Vec3D & local) = 0;
/*
  void getU_atIntPoint (Vec3D & u, short time);
    {

    }
  void getdU_atIntPoint (Tensor2 & du, short time);
  void computeDeformationGradient (Tensor2 & du, short time);
   */
};

#endif
