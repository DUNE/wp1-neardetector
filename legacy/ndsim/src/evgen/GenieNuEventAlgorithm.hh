//____________________________________________________________________________
/*!

\class    GenieNuEventAlgorithm

\brief    Class to create neutrino vertices using GENIE

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef EVGEN_NUEVENTALGORITHM_H
#define EVGEN_NUEVENTALGORITHM_H 1

#include <sys/stat.h>
#include <sys/types.h>

#include <TTree.h>
#include <TGeoManager.h>
#include <TGeoNode.h>

#include <Geo/PointGeomAnalyzer.h>
#include <Geo/ROOTGeomAnalyzer.h>
#include <EVGDrivers/GFluxI.h>
#include <EVGDrivers/GEVGDriver.h>
#include <EVGDrivers/GMCJDriver.h>
#include "EVGDrivers/GeomAnalyzerI.h"
#include "EVGDrivers/GMCJMonitor.h"
#include "EVGCore/EventRecord.h"
#include <Numerical/Spline.h>
#include <Utils/XSecSplineList.h>
#include <Utils/UnitUtils.h>
#include <Utils/StringUtils.h>
#include <Conventions/Controls.h>
#include <Conventions/Units.h>
#include "Interaction/Interaction.h"
#include "Numerical/RandomGen.h"
#include "Messenger/Messenger.h"
#include "Ntuple/NtpWriter.h"
#include "GHEP/GHepStatus.h"
#include "GHEP/GHepParticle.h"
#include "FluxDrivers/GCylindTH1Flux.h"

#include "DataCards.hh"
#include "LbnoGenieFlux.hh"
#include "LbnoGenieGeom.hh"
#include "LbnoDataLib.hh"

using namespace genie;
using namespace genie::controls;

class GenieNuEventAlgorithm {

 public:
  GenieNuEventAlgorithm();
  virtual ~GenieNuEventAlgorithm();

  EventRecord* makeGenieNuEvent();
  void convert(EventRecord* gEvent, NeutrinoEvent* event);
  bool Process(NeutrinoHit* hit, NeutrinoEvent* event);

  RandomGen * getRandGen() {return randGen_;};
  int getVerbose(){return verbose_;};

  void initDataCards();
  void loadDataCards();
  void initialize();
  void setManager(TGeoManager * manager)    {geoMgr_ = manager;};
  void resetVolumeCounts();

  GMCJDriver*             getMCjobDriver() {return mcj_driver_;}
  LbnoGenieGeom*          getGeomDriver()  {return lbno_geom_driver_;}
  LbnoGenieFlux*          getFluxDriver()  {return lbno_flux_driver_;}
  EventRecord*            getEventRecord() {return genie_event_;}

  void initGenieHstFluxDriver(std::string fluxFile);

private:
  void initSplines();
  void initRandGen(); 
  void initGenieGeomDriver();
  void initGenieFluxDriver();
  void initGenieMCjobDriver();
  void initGenieNtpWriter();
  void initGenieMCJMonitor();
  void initGenieMessenger();

  RandomGen*                   randGen_;

  XSecSplineList*              xspl_;
  GeomAnalyzerI*               geom_anal_;
  geometry::ROOTGeomAnalyzer*  root_geom_;

  TGeoManager *                geoMgr_;

  LbnoGenieGeom*               lbno_geom_driver_;
  LbnoGenieFlux*               lbno_flux_driver_;

  flux::GCylindTH1Flux*        hst_flux_driver_;
  TH1D *nu_e_h, *nu_mu_h, *anu_e_h, *anu_mu_h;

  GFluxI*                      flux_driver_;

  GMCJDriver*                  mcj_driver_;
  NtpWriter*                   ntpw_;
  GMCJMonitor*                 mcjmonitor_;
  EventRecord*                 genie_event_;

  int       ievent_;
  int       interactionCount;
  int       runNumber_;
  int       verbose_;
  int       randInt_;

  //counter
  GeomVolumeRecord geomVolumeCounter;

  bool      condition_;
  bool 	    wantGntp_;

  string    flux_;
  string    xSecSplines_;

  DataCards *cards_;
  bool      cardsLoaded_;
};

#endif


