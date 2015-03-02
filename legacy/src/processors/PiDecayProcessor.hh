//____________________________________________________________________________
/*!

\class    PiDecayProcessor

\brief    Processor class to implement pion decay

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef PROCESSORS_PIDECAY_H
#define PROCESSORS_PIDECAY_H 1

#include <string>
#include <iostream>

#include "LbnoProcessor.hh"
#include "PiDecayAlgorithm.hh"

class PiDecayProcessor  : public LbnoProcessor {

 public:
  PiDecayProcessor();
  virtual ~PiDecayProcessor() {}

  bool process();
  void initOutDataTree();
  void loadInDataTree() {}
  void loadDataCards();

 private:
  void initDataCards();

  PiDecayAlgorithm generator_;
  PionDecayEvent *event_;
};

#endif

