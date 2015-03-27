//
// Title: Rock Environment for the enclosure around the ND
//
// Author: Tom STainer, Georgios Christodoulou
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

#include "Materials.hh"

class RockConstruction{

public:
  RockConstruction();
  ~RockConstruction();

  void initialize();
  void construct(Materials * mats);
  void print();

  void setX(double X){ x_ = X;};
  void setY(double Y){ y_ = Y;};
  void setZ(double Z){ z_ = Z;};
  void setXYZ(double X, double Y, double Z){ x_ = X; y_ = Y; z_ = Z;};
  void setDensity(double rho) {density_ = rho;};
  void setMatName(std::string mat) {rmaterial_ = mat;};

  double getX() {return x_;};
  double getY() {return y_;};
  double getZ() {return z_;};
  double getDensity() {return density_;};
  std::string getMaterial() {return rmaterial_;};

  TGeoVolume * getVolume() {return volume_;};

protected:
  double x_,y_,z_;
  double density_;

  double volumeSize;
  std::string materialName;
  std::string rmaterial_;
  
  TGeoVolume * volume_;
  TGeoMixture * material_;
  TGeoMedium * medium_;
  TGeoBBox * box_;
  //TGeoTube * tube_;

};

#endif
