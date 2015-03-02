//
// Title: Cavity Environment for the enclosure around the ND
//
// Author: Tom Stainer
//
// Description: Cavity geometry for root in the ND geometry

#ifndef CAVITYCONSTRUCTION_HH
#define CAVITYCONSTRUCTION_HH

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <CLHEP/Units/SystemOfUnits.h>

#include "TGeoMaterial.h"
#include "TGeoTube.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"
//#include "TGeoMixture.h"

#include "TpcVesselConstruction.hh"
#include "RockConstruction.hh"
#include "MagnetConstruction.hh"

#include "DataCards.hh"

#include "Materials.hh"

class CavityConstruction{

public:
  CavityConstruction();
  ~CavityConstruction();

  void initialize();
  void construct(Materials * mats,TpcVesselConstruction *vessel,MagnetConstruction * magnet,RockConstruction * rock,double detectorOffset);
  void print();

  void initDataCards();
  void loadDataCards();

  void setGap(double g){ gap_ = g;};
  void setDensity(double rho);

  double const getGap() {return gap_;};
  double const getDensity() {return density_;};
  double const getRadius() {return radius_;};
  double const getLength() {return length_;};

  TGeoVolume * getVolume() {return volume_;};

protected:
  double gap_,radius_,length_;
  double density_;

  double volumeSize;
  std::string materialName;

  TGeoVolume * volume_;
  TGeoMixture * material_;
  TGeoMedium * medium_;
  TGeoTube * tube_;

  DataCards * cards_;
  bool cardsLoaded_;

};

#endif
