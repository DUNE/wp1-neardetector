//____________________________________________________________________________
/*!

\class    GasTPCGenieGeom

\brief    Class for geometry driver used in GENIE

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Oct 2013
\last update Nov 2013

*/
//____________________________________________________________________________
#ifndef EVGEN_GasTPCGENIEGEOM_H
#define EVGEN_GasTPCGENIEGEOM_H 1

#include <Geo/ROOTGeomAnalyzer.h>
#include <EVGDrivers/GeomAnalyzerI.h>
#include <Utils/UnitUtils.h>
#include <Utils/StringUtils.h>
#include <Conventions/Controls.h>
#include <Conventions/Units.h>

#include <TGeoManager.h>
#include <TGeoVolume.h>

#include "GasTPCDataLib.hh"
#include "DataCards.hh"

using namespace genie;

class GasTPCGenieGeom {

  public:
	GasTPCGenieGeom(TGeoManager * geoMgr);
	~GasTPCGenieGeom();

	void initialize();

	GeomAnalyzerI * getGeomAnalyzer() {return geom_anal_;};

  private:

	TGeoManager * geoMgr_;
	TGeoVolume* topVol_;

	std::string targetVolume_;

	DataCards* cards_;
	bool cardsLoaded_;

  	GeomAnalyzerI *              geom_anal_;
	geometry::ROOTGeomAnalyzer * root_geom_;


};

#endif