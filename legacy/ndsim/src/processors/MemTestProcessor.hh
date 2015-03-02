//____________________________________________________________________________
/*!

\class    MemTestProcessor

\brief    Processor class to check for memory leaks

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2013
\last update Sep 2013

*/
//____________________________________________________________________________
#ifndef PROCESSORS_MEM_TEST_H
#define PROCESSORS_MEM_TEST_H 1

#include <string>
#include <iostream>
#include <TSystem.h>
#include <TRandom.h>

#include "LbnoProcessor.hh"
#include "PiDecayAlgorithm.hh"

class MemTestProcessor  : public LbnoProcessor {

 public:
  MemTestProcessor();
  virtual ~MemTestProcessor() {}

  bool process();
  void initOutDataTree();
  void loadInDataTree() {}
  void loadDataCards();

 private:
  void initDataCards();

  MemTestRecord *event_;
  TRandom rand_;
  ProcInfo_t info_;
};

#endif
