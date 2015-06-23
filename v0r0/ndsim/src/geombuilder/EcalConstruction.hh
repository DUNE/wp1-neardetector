//
// Title: Ecal Environment for the enclosure around the ND
//
// Author: Tom Stainer, Georgios Christodoulou
//
// Description: Ecal geometry for root in the ND geometry

#ifndef ECALCONSTRUCTION_HH 
#define ECALCONSTRUCTION_HH

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
#include "TObjArray.h"
//#include "TGeoMixture.h"

#include "TpcVesselConstruction.hh"

#include "Materials.hh"

class EcalConstruction{

public:
  EcalConstruction();
  ~EcalConstruction();

  void initialize();
  void construct(Materials * mats,TpcVesselConstruction * vessel);
  void construct(Materials * mats,TpcVesselConstruction * vessel, std::string ecaldirection, bool buildlayers=false, int layernumber=-1);
  void print();

  void setThickness(double t){thickness_ = t;};
  void setDensity(double rho){density_ = rho;};
  void setMatName(std::string fmatname) {matName_ = fmatname;};
  void setLayers(int nlayers) {layers_ = nlayers;};
  void setAbsThickness(double abst){absthickness_ = abst;};
  void setAbsDensity(double absrho){absdensity_ = absrho;};
  void setAbsMatName(std::string absmatname) {absmatName_ = absmatname;};

  double getThickness() {return thickness_;};
  double getAbsThickness() {return absthickness_;};
  double getRadius() {return radius_;};
  double getInnerRadius() {return innerRadius_;};
  double getLength() {return length_;};
  double getDensity() {return density_;};
  double getAbsDensity() {return absdensity_;};
  std::string getMatName() {return materialName;};
  std::string getAbsMatName() {return absmaterialName;};
  int getLayers() {return layers_;};

  TGeoVolume * getVolume() {return volume_;};
  TGeoMaterial *getMaterial() {return material_;};
  TGeoMedium *getMedium() {return medium_;};
  TGeoMaterial *getAbsMaterial() {return absmaterial_;};
  TGeoMedium *getAbsMedium() {return absmedium_;};

private:

  double radius_;
  double innerRadius_;
  double length_;
  double thickness_, absthickness_;
  double density_, absdensity_;
  double mass_;
  int layers_;

  double volumeSize;
  std::string materialName, absmaterialName, absmatName_, matName_;
  
  TGeoTube * tube_;

  TGeoVolume * volume_;
  TGeoMaterial * material_;
  TGeoMaterial * absmaterial_;
  TGeoMedium * medium_;
  TGeoMedium * absmedium_;
};

#endif
