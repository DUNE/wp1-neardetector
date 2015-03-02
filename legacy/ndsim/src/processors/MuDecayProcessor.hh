//____________________________________________________________________________
/*!

\class    MuDecayProcessor

\brief    Processor class to implement muon decay

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef PROCESSORS_MUDECAY_H
#define PROCESSORS_MUDECAY_H 1

#include <string>
#include <iostream>

#include "LbnoProcessor.hh"
#include "MuDecayAlgorithm.hh"

class MuDecayProcessor : public LbnoProcessor {

 public:
  MuDecayProcessor();
  virtual ~MuDecayProcessor() {}

  bool process();
  void initOutDataTree() {}
  void loadInDataTree() {}
  void loadDataCards() {}

 private:
  MuDecayAlgorithm generator_;
};

#endif

