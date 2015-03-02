//____________________________________________________________________________
/*!

\class    GenieNuEventProcessor

\brief    Processor class to implement the genie neutrino generator

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#ifndef PROCESSORS_GENIE_NUEVENT_H
#define PROCESSORS_GENIE_NUEVENT_H 1

#include <string>
#include <iostream>

#include <TGeoManager.h>

#include "GeometryLoader.hh"
#include "LbnoProcessor.hh"
#include "GenieNuEventAlgorithm.hh"

class GenieNuEventProcessor  : public LbnoProcessor {

 public:
  GenieNuEventProcessor(GeometryLoader * geomLoader);
  virtual ~GenieNuEventProcessor() {}

  bool process();
  void initOutDataTree();
  void loadInDataTree();
  void initDataCards();
  void loadDataCards();

  GenieNuEventAlgorithm * getGenieNuEventAlgo() {return & eventGenerator_;};

  double getGlobalProbScale() {return genieProbScale;};

 private:
  GenieNuEventAlgorithm eventGenerator_;
  NeutrinoHit* hit_;
  NeutrinoEvent* event_; 

  GeometryLoader * geomLoader_;
  //TGeoManager * geomMgr_;

  double genieProbScale;

  int nInteractions_;
  int nInteractionsCC_;
  int nInteractionsNC_;
  int nInteractionsMix_;
};

#endif

