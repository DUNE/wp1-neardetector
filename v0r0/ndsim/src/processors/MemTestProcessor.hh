//____________________________________________________________________________
/*!

\class    MemTestProcessor

\brief    Processor class to check for memory leaks

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2013
\last update Jun 2015
*/
//____________________________________________________________________________
#ifndef PROCESSORS_MEM_TEST_H
#define PROCESSORS_MEM_TEST_H 1

#include <string>
#include <iostream>
#include <TSystem.h>
#include <TRandom.h>

#include "GasTPCProcessor.hh"

class MemTestProcessor  : public GasTPCProcessor {

 public:
  MemTestProcessor();
  virtual ~MemTestProcessor() {}

  bool process();
  void initOutDataTree();
  void loadInDataTree() {}
  void SetNEntries(int n) {entries = n;};
  int GetNEntries() { return entries;};

 private:

  MemTestRecord *event_;
  TRandom rand_;
  ProcInfo_t info_;
  int entries;
};

#endif
