//
// Title: Support Structures
//
// Author: Tom STainer
//
// Description: Support Structures for inner vessel geometry for root in the ND geometry

#ifndef SUPPORTSTRUCTURE_HH 
#define SUPPORTSTRUCTURE_HH 1

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <CLHEP/Units/SystemOfUnits.h>

#include "TGeoMaterial.h"
#include "TGeoTube.h"
#include "TGeoSphere.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"
#include "TGeoMatrix.h"
#include "TGeoCompositeShape.h"
#include "TMath.h"
//#include "TGeoMixture.h"

#include "DataCards.hh"

#include "TpcFiducialConstruction.hh"
#include "Materials.hh"

class SupportStructure{// : public TubeConstruction{

public:
  SupportStructure();
  ~SupportStructure();

  void initialize();
  void construct(Materials * mats,TpcFiducialConstruction * tpc);
  void print();

  void initDataCards();
  void loadDataCards();

  void setLength(double Z){ length_ = Z;};
  void setRadius(double R){ radius_ = R;};
  void setDensity(double rho);

  double getRadius() {return radius_;};
  double getInnerRadius() {return innerRadius_;};
  double getLength() {return length_;};
  double getTubeLength() {return tubeLength_;};
  double getEndLength() {return endLength_;};
  double getDensity() {return density_;};
  double getTheta() {return theta_;};
  double getInnerTheta() {return innerTheta_;};

  TGeoVolume * getVolume() {return volume_;};
  TGeoVolume * getInnerVolume() {return innerVolume_;};

private:

  double radius_;
  double innerRadius_;
  double length_,tubeLength_;
  double endLength_;
  double thickness_;
  double density_;
  double ratio_;
  double theta_;
  double innerTheta_;

  double volumeSize;
  std::string materialName;
  std::string matName_;

  TGeoCompositeShape *compositeShape_;
  TGeoTube * tube_;
  TGeoTube * innerTube_;
  TGeoSphere * nearEnd_;
  TGeoSphere * farEnd_;
  TGeoSphere * innerFarEnd_;
  TGeoSphere * innerNearEnd_;

  TGeoVolume * centerVolume_;
  TGeoVolume * innerCenterVolume_;
  TGeoVolume * volume_;
  TGeoVolume * innerVolume_;
  TGeoMaterial * material_;
  TGeoMedium * medium_;
  TGeoMedium * innerMedium_;

  DataCards * cards_;
  bool cardsLoaded_;


};

#endif
