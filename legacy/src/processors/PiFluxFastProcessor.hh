//____________________________________________________________________________
/*!

\class    PiFluxFastProcessor

\brief    Processor class to implement quick pion decay

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef PROCESSORS_PIFASTFLUX
#define PROCESSORS_PIFASTFLUX   1

#include <string>
#include <iostream>

#include <TLorentzVector.h>

#include "LbnoProcessor.hh"
#include "PiFluxProjectionAlgorithm.hh"
#include "PiDecayAlgorithm.hh"

class PiFluxFastProcessor : public LbnoProcessor {

 public:
  PiFluxFastProcessor();
  virtual ~PiFluxFastProcessor() {}

  bool process();
  void initOutDataTree();
  void loadInDataTree() {};
  void loadDataCards();

 private:
  void initDataCards();

  PiFluxProjectionAlgorithm projector_;
  PiDecayAlgorithm generator_;

  NeutrinoHit* neutrino_;
  PionDecayEvent *piDecay_;
  int nDecay_;
};

#endif

