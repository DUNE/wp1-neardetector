//
// Title: Detector construction
//
// Author: Tom STainer, Georgios Christodoulou
//
// Description: Detector geometry for root in the ND geometry

#ifndef NDCONSTRUCTION_HH 
#define NDCONSTRUCTION_HH

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
#include "Materials.hh"
#include "TGeoManager.h"

#include "TVector3.h" 

class NDConstruction{

public:
  NDConstruction();
  virtual ~NDConstruction();

  // Initialization methods
  void initializeTPC(double length, double radius, double density, std::string material);
  void initializeVessel(double thickness, double density, std::string material);
  void initializeEcal(double density, double thickness, int layers, double absdensity, double absthickness, std::string material, std::string absmaterial, bool buildlayers=false);
  void initializeMagnet(double density, double thickness, std::string material);

  // Geometry construction methods
  void construction(Materials * mats, bool ecal=true, bool magnet=false);

  // Getters
  double getTPCLength()            {return tpc_length_;};
  double getTPCDensity()           {return tpc_density_;};
  double getTPCRadius()            {return tpc_radius_;};
  std::string getTPCMaterial()     {return tpc_matName_;};
  double getTPCMass()              {return tpc_mass_;};

  double getVesselThickness()      {return vessel_thickness_;};
  double getVesselDensity()        {return vessel_density_;};
  std::string getVesselMaterial()  {return vessel_matName_;};

  double getEcalDensity()          {return ecal_density_;};
  double getEcalAbsDensity()       {return ecal_absdensity_;};
  double getEcalThickness()        {return ecal_thickness_;};
  double getEcalAbsThickness()     {return ecal_absthickness_;};
  double getEcalGap()              {return ecal_gap;};
  int getEcalLayers()              {return ecal_layers;;}
  std::string getEcalMaterial()    {return ecal_matName_;};
  std::string getEcalAbsMaterial() {return ecal_absmatName_;};

  double getMagnetDensity()        {return magnet_density_;};
  double getMagnetThickness()      {return magnet_thickness_;};
  std::string getMagnetMaterial()  {return magnet_matName_;}
  
  TGeoVolume * getVolume()         {return volume_;};
  TVector3 getStartVol()           {return start_vol_;};

private:

  // TPC
  double tpc_length_, tpc_radius_, tpc_thickness_, tpc_density_, tpc_mass_;
  std::string tpc_matName_;
  // Vessel
  double vessel_thickness_, vessel_density_;
  std::string vessel_matName_;
  // Ecal
  double ecal_density_, ecal_thickness_, ecal_absdensity_, ecal_absthickness_, ecal_gap;
  int ecal_layers;
  std::string ecal_matName_, ecal_absmatName_;
  bool ecal_buildlayers_;
  // Magnet
  double magnet_density_, magnet_thickness_;
  std::string magnet_matName_;

  // Volume
  TGeoVolume * volume_;

  // The start of the detector
  TVector3 start_vol_; 

};

#endif
