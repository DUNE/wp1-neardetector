//
// Title: TPC Vessel
//
// Author: Tom STainer
//
// Description: TPC Vessel geometry for root in the ND geometry

#ifndef TPCVESSELCONSTRUCTION_HH 
#define TPCVESSELCONSTRUCTION_HH 1

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
//#include "TubeConstruction.hh"
#include "Materials.hh"

class TpcVesselConstruction{// : public TubeConstruction{

public:
  TpcVesselConstruction();
  ~TpcVesselConstruction();

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
  double getInnerSphereRadius() {return innerSphereRadius_;};
  double getLength() {return length_;};
  double getTubeLength() {return tubeLength_;};
  double getEndLength() {return endLength_;};
  double getDensity() {return density_;};
  double getTheta() {return theta_;};
  double getInnerTheta() {return innerTheta_;};

  TGeoVolume * getVolume() {return volume_;};
  TGeoVolume * getInnerVolume() {return innerVolume_;};

  TGeoSphere * getInnerNearEndSphereShape() {return innerNearEnd_;};
  TGeoSphere * getInnerFarEndSphereShape() {return innerFarEnd_;};
  TGeoTranslation * getInnerNearEndSpherePosition() {return t3;};
  TGeoTranslation * getInnerFarEndSpherePosition() {return t4;};
  double getInnerNearEndSphereZOffset() {return InnerNearEndSphereZOffSet_;};
  double getInnerFarEndSphereZOffset() {return InnerFarEndSphereZOffSet_;};

private:

  double radius_,sphereRadius_;
  double innerRadius_,innerSphereRadius_;
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

  TGeoTranslation * t3;
  TGeoTranslation * t4;
  double InnerNearEndSphereZOffSet_;
  double InnerFarEndSphereZOffSet_;

  DataCards * cards_;
  bool cardsLoaded_;


};

#endif
