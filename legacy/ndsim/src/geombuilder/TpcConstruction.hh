//
// Title: TPC Fiducial design
//
// Author: Tom STainer, Georgios Christodoulou
//
// Description: TPC Fiducial geometry for root in the ND geometry

#ifndef TPCCONSTRUCTION_HH
#define TPCCONSTRUCTION_HH 1

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

#include "Materials.hh"

class TpcConstruction{

public:
  TpcConstruction();
  ~TpcConstruction();

  void initialize();
  void loadMaterial();
  void construct(Materials * mats);
  void print();

  void setXYZ(double X, double Y, double Z){ x_ = X; y_ = Y; z_ = Z;};
  void setX(double X){ x_ = X;};
  void setY(double Y){ y_ = Y;};
  void setZ(double Z){ z_ = Z;};
  void setDensity(double rho) {density_ = rho;};
  void setMatName(std::string fmatname) {matName_ = fmatname;}

  double getX() {return x_;};
  double getY() {return y_;};
  double getZ() {return z_;};
  double getLength() {return 2*z_;};
  double getDensity() {return density_;};

  std::string getMatName() {return materialName;};
  
  TGeoMixture * getMaterial() {return material_;};
  TGeoMedium * getMedium() {return medium_;};

  TGeoVolume * getVolume() {return volume_;};

private:
  double x_,y_,z_;
  double density_;

  double volumeSize;
  double mass;
  std::string materialName;
  std::string matName_;
  
  TGeoVolume * volume_;
  TGeoMixture * material_;
  TGeoMedium * medium_;
  TGeoBBox * box_;

  TGeoMixture * anodePlateMaterial;
  TGeoMixture * cathodePlateMaterial;
  TGeoMedium * cathodeMedium;
  TGeoMedium * anodeMedium;
  TGeoBBox * cathodeBox;
  TGeoBBox * anodeBox;
  TGeoVolume * cathodeVolume;
  TGeoVolume * anodeVolume;

};

#endif
