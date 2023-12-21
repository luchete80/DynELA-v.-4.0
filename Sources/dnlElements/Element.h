/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_Element_h__
#define __dnlElements_Element_h__

#include <dnlMaterials.h>
#include <ElementData.h>

class Node;
class Model;

/*
@LABEL:Element::Element
@SHORT:Finite Element Element class.
This class is used to store information for DynELA Elements.
@ARG:long & number & Integration points of the element.
@ARG:ListIndex<Node*> & nodes & Nodes of the element.
@ARG:List<IntegrationPoint*> & integrationPoints & Integration points of the element.
@ARG:List<UnderIntegrationPoint*> & underIntegrationPoints & Under-integration points of the element.
@ARG:Material & material & Material associated to the element.
@ARG:Matrix & stiffnessMatrix & Stiffness matrix of the element.
The type of element can be one of the following:
\begin{itemize}
\item ElQua4N2D
\item ElTri3N2D
\item ElQua4NAx
\item ElHex8N3D
\item ElTet4N3D
\item ElTet10N3D
\end{itemize}
@END
*/
class Element
{
    friend class Node;
    friend class ListIndex<Element *>; // To be able to use ListIndex
    long _listIndex;                   // Local index used for the ListIndex management.
    Vec3D _nodeMin, _nodeMax;          // Bounding box of an element.

protected:
    Matrix _globalToLocal;
    const ElementData *_elementData;
    IntegrationPoint *_integrationPoint = NULL;
    UnderIntegrationPoint *_underIntegrationPoint = NULL;

public:
    List<IntegrationPoint *> integrationPoints;
    List<UnderIntegrationPoint *> underIntegrationPoints;
    ListIndex<Node *> nodes;
    long number;
    Material *material = NULL;
    Matrix stiffnessMatrix;

    enum // Enum defining the list of element names
    {
        ElGeneric = 0,
        ElQua4N2D,
        ElTri3N2D,
        ElQua4NAx,
        ElHex8N3D,
        ElTet4N3D,
        ElTet10N3D
    };

    enum // Enum defining the list of element types
    {
        Bidimensional = 0,
        Axisymetric,
        Threedimensional
    };

public:
    Element(long elementNumber = 1);
    Element(const Element &element);
    virtual ~Element();

    // Interface methods excluded from SWIG
#ifndef SWIG
    friend std::ofstream &operator<<(std::ofstream &, const Element &);
    friend std::ifstream &operator>>(std::ifstream &, Element &);
    void write(std::ofstream &) const;
    Element &read(std::ifstream &);
#endif

    // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

    bool operator==(const Element &) const;
    bool operator!=(const Element &) const;
    bool operator<(const Element &) const;
    bool operator>(const Element &) const;

    bool check();
    // double getDensityAtIntegrationPoint(short time);
    double getElongationWaveSpeed();
    double getIntPointValue(short field, short intPoint);
    IntegrationPoint *getIntegrationPoint(short point);
    UnderIntegrationPoint *getUnderIntegrationPoint(short point);
    long &internalNumber();
    short getFamily() const;
    //    short getLocalIdOfNodeOnEdge(short edge, short node) const;
    short getNumberOfDDL() const;
    short getNumberOfDimensions() const;
    //    short getNumberOfEdges() const;
    short getNumberOfFaces() const;
    short getNumberOfIntegrationPoints() const;
    //    short getNumberOfNeighbourNodes(short i) const;
    short getNumberOfNodes() const;
    //    short getNumberOfNodesOnEdge(short i) const;
    short getNumberOfNodesOnFace(short i) const;
    short getNumberOfNodesOnSideFace(short i) const;
    short getNumberOfSideFaces() const;
    short getType() const;
    short getVtkType() const;
    String getName() const;
    Vec3D getLocalNodeCoords(short node) const;
    virtual bool checkLevel2() = 0;
    virtual bool computeJacobian(bool reference = false) = 0;
    virtual bool computeUnderJacobian(bool reference = false) = 0;
    virtual double getCharacteristicLength() = 0;
    virtual double getRadiusAtIntegrationPoint() = 0;
    virtual void computeDeformationGradient(Tensor2 &F, short time) = 0;
    virtual void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const = 0;
    virtual void getdV_atIntPoint(Tensor2 &dv, short time) = 0;
    virtual void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const = 0;
    virtual void getV_atIntPoint(Vec3D &v, short time) = 0;
    void add(IntegrationPoint *_integrationPoint, short intPointId);
    void add(Material *newMaterial);
    void add(UnderIntegrationPoint *_integrationPoint, short intPointId);
    void addNode(Node *newNode);
    // void computeConstitutiveEquation();
    void computeFinalRotation();
    void computeInternalForces(Vector &F, double timeStep);
    // void computeMassEquation(MatrixDiag &M, Vector &F);
    void computeMassMatrix(MatrixDiag &massMatrix);
    // void computeMomentumEquation(MatrixDiag &M, Vector &F);
    void computePressure();
    //  void computeStateEquationOld();
    void computeStrains();
    void computeStrainRates(); //LUCHETE80, RADIOSS & LS-DYNA FORM TO CALC STRAINS (NO UV DECOMP)
    //  void computeStrainsOld(double);
    void computeStress(double timeStep);
    void computeStressDirect(double timeStep);
    //  void computeStressOld(double timeStep);
    void createIntegrationPoints();
    void initializeData();
    void setCurrentIntegrationPoint(short point);
    void setCurrentUnderIntegrationPoint(short point);
    double getIntPointValueExtract(short field, short intPoint);
    void computeDensity();
    Node *getNodeOnFace(short face, short node);
    const ElementData *getElementData() const;
    void dumpElementData() const;
    virtual void computeElasticStiffnessMatrix(bool underIntegration = false) = 0;
    virtual double getRadiusAtUnderIntegrationPoint() = 0;

    /*
    void clearIntegrationPoint();
    void detachMaterial();
    void getIntegrationPoint(long);
    void getUnderIntegrationPoint(long);
    long getLocalNumber(Node *);
    */
    bool isNodeinElement(Node *);
    bool isNodeinElement(Vec3D );
    bool isNodeinBoundBox(Node *);
    bool isNodeinBoundBox(Vec3D );
    /*
    void toFile(FILE *);
    */
    void computeBoundBox();

    
    Node *getNodeOnSideFace(short sideFace, short node);
    /*
    Node *getNodeOnEdge(short edge, short node);
    Node *getNeighbourNode(short node, short neighbour);

    void computeMassEquation(MatrixDiag &M);
    void computeEnergyEquation(MatrixDiag &M, Vector &F);
    void computeConstitutiveIntegration(MatrixDiag &M, Vector &F);
    void getFe_atIntPoint(Vec3D &force);
    void getEnergy(double &e);
    void getdTemp_atIntPoints(Vec3D &dT);
    */
    void getSigmaAtPoint(SymTensor2 &Stress, const Vec3D &point);
    /*
    void get_Sig_atNode(Tensor2 &Stress, long node);
    void get_Eps_atNode(Tensor2 &Strain, long node);
    void get_dEps_atNode(Tensor2 &StrainInc, long node);
    void get_EpsPlas_atNode(Tensor2 &Stress, long node);
    void get_evp_atNode(double &alpha, long i);
    virtual long numberOfUnderIntegrationPoints() = 0;
    virtual double getVolume() = 0;
    //bool getIntegrationPointCoords (short, Vec3D & coords, double& weight);
    bool linkIntegrationPointData(short);
    //virtualbool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) = 0;
    virtual void getU_atIntPoint(Vec3D &u, int t) = 0;
    virtual void getdU_atIntPoint(Tensor2 &du, int t) = 0;
    */
    virtual void computeGlob2Loc() = 0;
    virtual void glob2Loc(const Vec3D &point, Vec3D &local) = 0;
    //virtual //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;*/
};

bool compareElementsNumber(Element *, Element *);
long substractElementsNumber(Element *, const long);

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodes() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfNodes;
}

//-----------------------------------------------------------------------------
inline String Element::getName() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->name;
}

//-----------------------------------------------------------------------------
inline short Element::getType() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->type;
}

//-----------------------------------------------------------------------------
inline short Element::getFamily() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->family;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfFaces() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfFaces;
}

//-----------------------------------------------------------------------------
inline const ElementData *Element::getElementData() const
//-----------------------------------------------------------------------------
{
    return _elementData;
}
/* //-----------------------------------------------------------------------------
inline short Element::getNumberOfEdges() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfEdges;
}
 */
//-----------------------------------------------------------------------------
inline short Element::getNumberOfSideFaces() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfSideFaces;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfDDL() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfDDL;
}

//-----------------------------------------------------------------------------
inline short Element::getVtkType() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->vtkType;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfDimensions() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfDimensions;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnSideFace(short i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfSideFaces)
        fatalError("Element::getNumberOfNodesOnSideFace",
                   "sideface %d is out of bounds %d-%d\n", i, 0, _elementData->numberOfSideFaces);
#endif
    return _elementData->sideFaces[i].numberOfNodes;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnFace(short i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfFaces)
        fatalError("Element::getNumberOfNodesOnFace",
                   "face %d is out of bounds %d-%d\n", i, 0, _elementData->numberOfFaces);
#endif
    return _elementData->faces[i].numberOfNodes;
}

/* //-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnEdge(short i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfEdges)
        fatalError("Element::getNumberOfNodesOnEdge",
                   "edge %d is out of bounds %d-%d\n", i, 0, _elementData->numberOfEdges);
#endif
    return _elementData->edges[i].numberOfNodes;
}
 */
/* //-----------------------------------------------------------------------------
inline short Element::getNumberOfNeighbourNodes(short i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfNodes)
        fatalError("Element::getNumberOfNeighbourNodes",
                   "node %d is out of bounds %d-%d\n", i, 0, _elementData->numberOfNodes);
#endif
    return _elementData->nodes[i].numberOfNeighbourNodes;
}
 */
//-----------------------------------------------------------------------------
inline short Element::getNumberOfIntegrationPoints() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfIntegrationPoints;
}

/* //-----------------------------------------------------------------------------
inline short Element::getLocalIdOfNodeOnEdge(short edge, short node) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return (_elementData->edges[edge].number[node]);
}
 */
//-----------------------------------------------------------------------------
inline Vec3D Element::getLocalNodeCoords(short node) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return (_elementData->nodes[node].localCoords);
}

// Internal number. This method gives a direct access to the internal number of the current node.
/*
  Return : Internal number of the current node.
*/
//-----------------------------------------------------------------------------
inline long &Element::internalNumber()
//-----------------------------------------------------------------------------
{
    return (_listIndex);
}

//-----------------------------------------------------------------------------
inline IntegrationPoint *Element::getIntegrationPoint(short point)
//-----------------------------------------------------------------------------
{
    return integrationPoints(point);
}

//-----------------------------------------------------------------------------
inline UnderIntegrationPoint *Element::getUnderIntegrationPoint(short point)
//-----------------------------------------------------------------------------
{
    return underIntegrationPoints(point);
}

//-----------------------------------------------------------------------------
inline void Element::setCurrentIntegrationPoint(short point)
//-----------------------------------------------------------------------------
{
    _integrationPoint = integrationPoints(point);
}

//-----------------------------------------------------------------------------
inline void Element::setCurrentUnderIntegrationPoint(short point)
//-----------------------------------------------------------------------------
{
    _underIntegrationPoint = underIntegrationPoints(point);
}

/*


struct LateralSurface
{
    long tab[maxNumberOfNodesBySideFace];
    Element *pel;
    short face;
};
bool compareLateralSurfaces(LateralSurface *p1, LateralSurface *p2);

//------------------------------- Inline -----------------------------------
*/
#endif
