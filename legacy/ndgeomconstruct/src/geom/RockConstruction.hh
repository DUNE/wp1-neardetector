//
// Title: Rock Environment for the enclosure around the ND
//
// Author: Tom STainer
//
// Description: Rock geometry for root in the ND geometry

#ifndef ROCKCONSTRUCTION_HH
#define ROCKCONSTRUCTION_HH

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <CLHEP/Units/SystemOfUnits.h>

#include "TGeoMaterial.h"
#include "TGeoBBox.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"
//#include "TGeoMixture.h"

#include "DataCards.hh"

#include "Materials.hh"

class RockConstruction{

public:
  RockConstruction();
  ~RockConstruction();

  void initialize();
  void construct(Materials * mats);
  void print();

  void initDataCards();
  void loadDataCards();

  void setX(double X){ x_ = X;};
  void setY(double Y){ y_ = Y;};
  void setZ(double Z){ z_ = Z;};
  void setDensity(double rho);

  double getX() {return x_;};
  double getY() {return y_;};
  double getZ() {return z_;};
  double getDensity() {return density_;};

  TGeoVolume * getVolume() {return volume_;};

protected:
  double x_,y_,z_;
  double density_;

  double volumeSize;
  std::string materialName;
  
  TGeoVolume * volume_;
  TGeoMixture * material_;
  TGeoMedium * medium_;
  TGeoBBox * box_;
  //TGeoTube * tube_;

  DataCards * cards_;
  bool cardsLoaded_;

};

#endif