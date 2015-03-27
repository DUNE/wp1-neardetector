
// Title: Tube Shape Base class
//
// Author: Tom STainer
//
// Description: Tube geometry for root in the ND geometry

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

#include "DataCards.hh"

//#include "Materials.hh"

class TubeConstruction{

public:
  TubeConstruction();
  ~TubeConstruction();

  virtual void initialize();
  void construct(std::string matName, TGeoMixture * mixture, double radius,double length);
  void construct(std::string matName, TGeoMaterial * material, double radius,double length);
  virtual void print();

  virtual void initDataCards();
  virtual void loadDataCards();

  void setZ(double Z){ length_ = Z;};
  void setR(double R){ radius_ = R;};
  void setDensity(double rho);

  double getRadius() {return radius_;};
  double getLength() {return length_;};
  double getDensity() {return density_;};

  TGeoVolume * getVolume() {return volume_;};

protected:
  double radius_;
  double length_;
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