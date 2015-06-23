//
// Title: Scintillator bars design
//
// Author: Tom STainer, Georgios Christodoulou
//
// Description: Scintillator bars geometry for root in the ND geometry

#ifndef SCINTILLATORCONSTRUCTION_HH
#define SCINTILLATORCONSTRUCTION_HH 1

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <CLHEP/Units/SystemOfUnits.h>

#include "TGeoMaterial.h"
#include "TGeoBBox.h"
#include "TGeoTube.h"
#include "TGeoSphere.h"
#include "TGeoVolume.h"
#include "TGeoCompositeShape.h"
#include "TGeoMedium.h"
//#include "TGeoMixture.h"

#include "TpcConstruction.hh"
#include "TpcVesselConstruction.hh"

#include "Materials.hh"

class ScintillatorConstruction{

public:
  ScintillatorConstruction();
  ~ScintillatorConstruction();

  void print();
  void construct(Materials * mats,TpcConstruction * tpc,TpcVesselConstruction * vessel);
  void initialize();

  void addExtraScintInY(double lastYPosition);
  void addExtraScintInX(double lastXPosition);
  void addExtraScintInZ(double lastZPosition,double lastXWidth,double lastYWidth);
  double calculateMass(TGeoBBox * box);

  void setNoOfLayers(int num){ layers_ = num;};
  void setThickness(double thickness){ thickness_ = thickness;};
  void setXYZ(double X, double Y, double Z){ x_ = X; y_ = Y; z_ = Z;};
  void setX(double X){ x_ = X;};
  void setY(double Y){ y_ = Y;};
  void setZ(double Z){ z_ = Z;};
  void setDensity(double rho) {density_ = rho;};
  void setMatName(std::string fmatname) {matName_ = fmatname;};
  void setGap(double gap) {gap_ = gap;};

  double getNoOfLayers() {return layers_;};
  double getThickness() {return thickness_;};
  double getX() {return x_;};
  double getY() {return y_;};
  double getZ() {return z_;};
  double getDensity() {return density_;};
  std::string getMatName() {return materialName;};
  double getGap() {return gap_;};

  TGeoVolume * getVolume() {return volume_;};
  TGeoVolume * getMotherVolumeTop() {return motherVolumeTop_;};
  TGeoVolume * getMotherVolumeBottom() {return motherVolumeBottom_;};

private:
  int layers_,layersRequested_,additionalLayers_;
  double thickness_,gap_;
  double x_,y_,z_;
  double density_;
  double theta;

  double volumeSize;
  double totalMass;
  std::string materialName;
  std::string matName_;
  
  TGeoVolume * volume_;
  TGeoVolume * tempVolume_;
  TGeoVolume * tempVolume2_;
  TGeoVolume * motherVolume_,*motherVolumeTop_,*motherVolumeBottom_;
  TGeoVolume * motherVolumeLeft_,*motherVolumeRight_;
  TGeoMixture * material_;
  TGeoMixture * motherMaterial_;
  TGeoMedium * medium_;
  TGeoMedium * motherMedium_;
  TGeoBBox * tempBox_;
  TGeoTube * tempTube_;
  TGeoBBox * motherBox_;
  TGeoTubeSeg *motherTubeTop_,*motherTubeBottom_;
  TGeoTubeSeg *motherTubeLeft_,*motherTubeRight_;
  TGeoSphere * motherFarEnd_;

  TGeoTranslation * t3;

  //TpcConstruction * tpcPtr_;
  TpcVesselConstruction * vesselPtr_;

};

#endif
