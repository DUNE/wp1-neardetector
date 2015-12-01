//____________________________________________________________________________
/*!

\class    Geant4TrackingProcessor

\brief    Processor class to implement the geant4 tracking

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jun 2015
*/
//____________________________________________________________________________
#ifndef PROCESSORS_GEANT4TRACKING
#define PROCESSORS_GEANT4TRACKING 1

#include "GasTPCProcessor.hh"
#include "GasTPCDataLib.hh"
#include "DetectorHit.hh"
#include "Geant4TrackingAlgorithm.hh"

class Geant4TrackingProcessor  : public GasTPCProcessor {
 public:
  Geant4TrackingProcessor(Geant4TrackingAlgorithm * g4tracking); 
  virtual ~Geant4TrackingProcessor();

  bool process();
  void initOutDataTree();
  void loadInDataTree();

  void setHitCollections();
  void setTracking();

 private:

  NeutrinoEvent* 	event_;
  SimulData*      	MChits_;
  GeantTrackingTruth*	trackingRecord_;

  Geant4TrackingAlgorithm *g4tracking_;

};

#endif


