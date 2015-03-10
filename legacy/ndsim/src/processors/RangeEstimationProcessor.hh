//____________________________________________________________________________
/*!

\class    RangeEstimationProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef PROCESSORS_RANGECALC
#define PROCESSORS_RANGECALC 1

#include "GasTPCProcessor.hh"
#include "GasTPCDataLib.hh"
#include "RangeEstimationAlgorithm.hh"

class RangeEstimationProcessor  : public GasTPCProcessor {
 public:
  RangeEstimationProcessor();
  virtual ~RangeEstimationProcessor() {}

  bool process();
  void initOutDataTree();
  void loadInDataTree();
  void initDataCards() {};
  void loadDataCards();
  int getNoIterations();

 private:
  tpcFidHit* MChits_;
  MuonRange* range_;

  RangeEstimationAlgorithm g4ranger_;

};

#endif

