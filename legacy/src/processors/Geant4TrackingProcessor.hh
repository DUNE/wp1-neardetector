//____________________________________________________________________________
/*!

\class    Geant4TrackingProcessor

\brief    Processor class to implement the geant4 tracking

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#ifndef PROCESSORS_GEANT4TRACKING
#define PROCESSORS_GEANT4TRACKING 1

#include "LbnoProcessor.hh"
#include "GeometryLoader.hh"
#include "LbnoDataLib.hh"
#include "DetectorHit.hh"
#include "Geant4TrackingAlgorithm.hh"
#include "EventFilterAlgorithm.hh"

class Geant4TrackingProcessor  : public LbnoProcessor {
 public:
  Geant4TrackingProcessor();
  Geant4TrackingProcessor(GeometryLoader * geomLoader);
  virtual ~Geant4TrackingProcessor();

  bool process();
  void initOutDataTree();
  void loadInDataTree();
  void initDataCards();
  void loadDataCards();

  void setAndFill();
  void setHitCollections();
  void setTracking();

  void setGunOption(bool set) { gunOn_ = set;};
  bool getGunOption() {return gunOn_;};

 private:

  NeutrinoEvent* 	event_;
  SimulData*      	MChits_;
  GeantTrackingTruth*	trackingRecord_;

  Geant4TrackingAlgorithm g4tracking_;
  EventFilterAlgorithm filter_;

  GeometryLoader * geomLoader_;

  bool gunOn_;
  bool qeccOnly_;
};

#endif


