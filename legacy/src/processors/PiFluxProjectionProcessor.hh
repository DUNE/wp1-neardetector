//____________________________________________________________________________
/*!

\class    PiFluxProjectionProcessor

\brief    Processor class to implement pion forward projection

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef PROCESSORS_PIDECAYFLUXPROJECTION
#define PROCESSORS_PIDECAYFLUXPROJECTION   1

#include <string>
#include <iostream>

#include <TLorentzVector.h>

#include "LbnoProcessor.hh"
#include "PiFluxProjectionAlgorithm.hh"

class PiFluxProjectionProcessor : public LbnoProcessor {

 public:
  PiFluxProjectionProcessor();
  virtual ~PiFluxProjectionProcessor() {}

  bool process();
  void initOutDataTree();
  void loadInDataTree();
  void loadDataCards();

 private:
  void initDataCards();

  PiFluxProjectionAlgorithm projector;
  NeutrinoHit* neutrino_;
  PionDecayEvent *piDecay_;
  int nDecay_;
};

#endif

