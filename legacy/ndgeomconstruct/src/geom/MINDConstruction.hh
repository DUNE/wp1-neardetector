//
// Title: MIND  design
//
// Author: Tom STainer
//
// Description: MIND geometry for root in the ND geometry

#ifndef MINDCONSTRUCTION_HH
#define MINDCONSTRUCTION_HH 1

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

class MINDConstruction{

public:
  MINDConstruction();
  ~MINDConstruction();

  void print();
  void construct(Materials * mats);
  void initialize();

  void initDataCards();
  void loadDataCards();

  void setThickness(double thickness){ thickness_ = thickness;};
  void setX(double X){ x_ = X;};
  void setY(double Y){ y_ = Y;};
  void setZ(double Z){ z_ = Z;};
  void setDensity(double rho);

  double getNoOfLayers() {return layers;};
  double getThickness() {return thickness_;};
  double getX() {return x_;};
  double getY() {return y_;};
  double getZ() {return z_;};
  double getDensity() {return density_;};

  TGeoVolume * getVolume() {return volume_;};

private:
  int layers;
  double thickness_,scintThickness_;
  double x_,y_,z_;
  double density_,scintDensity_;
  std::string materialName_,scintMaterialName_;

  double volumeSize;
  double totalMass;

  TGeoVolume * volume_;
  TGeoVolume * tempVolume_;
  TGeoVolume * tempVolume2_;
  TGeoVolume * motherVolume_;
  TGeoMixture * material_;
  TGeoMixture * motherMaterial_;
  TGeoMedium * medium_;
  TGeoMedium * motherMedium_;
  TGeoBBox * tempBox_;
  TGeoBBox * tempBox2_;
  TGeoBBox * motherBox_;

  DataCards * cards_;
  bool cardsLoaded_;

};

#endif
