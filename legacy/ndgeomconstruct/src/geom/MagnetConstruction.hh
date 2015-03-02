//
// Title: Magnet Environment for the enclosure around the ND
//
// Author: Tom Stainer
//
// Description: Magnet geometry for root in the ND geometry

#ifndef MAGNETCONSTRUCTION_HH 
#define MAGNETCONSTRUCTION_HH

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

#include "TpcVesselConstruction.hh"

#include "Materials.hh"

class MagnetConstruction{

public:
  MagnetConstruction();
  ~MagnetConstruction();

  void initialize();
  void construct(Materials * mats,TpcVesselConstruction * vessel);
  void print();

  void initDataCards();
  void loadDataCards();

  void setThickness(double t){ thickness_ = t;};
  void setDensity(double rho);

  double getThickness() {return thickness_;};
  double getRadius() {return radius_;};
  double getInnerRadius() {return innerRadius_;};
  double getLength() {return length_;};
  double getDensity() {return density_;};

  TGeoVolume * getVolume() {return volume_;};

private:

  double radius_;
  double innerRadius_;
  double length_;
  double thickness_;
  double density_;
  double mass_;

  double volumeSize;
  std::string materialName;
  
  TGeoTube * tube_;

  TGeoVolume * volume_;
  TGeoMaterial * material_;
  TGeoMedium * medium_;

  DataCards * cards_;
  bool cardsLoaded_;


};

#endif
