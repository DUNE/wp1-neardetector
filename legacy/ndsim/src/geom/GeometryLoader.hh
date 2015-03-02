//____________________________________________________________________________
/*!

\class    GeometryLoader

\brief    Class to be used to read in geometry file

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Dec 2012
\last update Jun 2013

*/
//____________________________________________________________________________
#ifndef GEOM_GEOMETRYLOADER_H
#define GEOM_GEOMETRYLOADER_H 1

#include <iostream>
#include <string>

#include <TGeoManager.h>
#include <TGeoVolume.h>

#include "LbnoDataLib.hh"
#include "DataCards.hh"

class GeometryLoader {

  public:

	GeometryLoader();
	GeometryLoader(std::string);
	~GeometryLoader();

	void initDataCards();
	void loadDataCards();
  	void initialize();
  	void initialize(std::string);
  	void close();

	TGeoManager * getManager() {return geoMgr_;};
	TGeoVolume * getWorldVolume();
	TGeoVolume * getTPCVolume();
	
	double getVolumeXBoundary(TGeoVolume * boxVolume);
	double getVolumeYBoundary(TGeoVolume * boxVolume);
	double getVolumeZBoundary(TGeoVolume * boxVolume);
	
	double getXRange() {return xRange_;}
	double getYRange() {return yRange_;}
	double getZRange() {return zRange_;}

	std::string getFileName() {return rootFile_;};
	std::string getTargetVolumeName() {return targetVolume_;};

  private:

	TGeoManager * geoMgr_;
	TGeoVolume* World_;
	TGeoVolume* Tpc_;

  	double xRange_;
  	double yRange_;
  	double zRange_;

	DataCards* cards_;
	bool cardsLoaded_;

	std::string rootFile_;
	std::string targetVolume_;

};

#endif
