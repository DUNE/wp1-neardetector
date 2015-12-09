//____________________________________________________________________________
/*!

\class    GeometryLoader

\brief    Class to be used to read in geometry file

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>'
          University of Liverpool

\created  Dec 2012
\last update Jun 2015
*/
//____________________________________________________________________________
#ifndef GEOM_GEOMETRYLOADER_H
#define GEOM_GEOMETRYLOADER_H 1

#include <iostream>
#include <string>

#include <TGeoManager.h>
#include <TGeoVolume.h>

#include "GasTPCDataLib.hh"

class GeometryLoader {

public:
  
  GeometryLoader();
  GeometryLoader(std::string);
  virtual ~GeometryLoader();
  
  void initialize(std::string);
  void close();
  
  TGeoManager * getManager() {return geoMgr_;};
  TGeoVolume * getWorldVolume();
  TGeoVolume * getTPCVolume(std::string volume);
  
  double getVolumeXBoundary(TGeoVolume * boxVolume);
  double getVolumeYBoundary(TGeoVolume * boxVolume);
  double getVolumeZBoundary(TGeoVolume * boxVolume);
  
  double getXRange() {return xRange_;}
  double getYRange() {return yRange_;}
  double getZRange() {return zRange_;}
  
private:
  
  TGeoManager * geoMgr_;
  
  double xRange_;
  double yRange_;
  double zRange_;
  
};

#endif
