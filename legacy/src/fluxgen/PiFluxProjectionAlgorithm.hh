//____________________________________________________________________________
/*!

\class    PiFluxProjectionAlgorithm

\brief    Class to project neutrinos forward to specified distance with a radius cut

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update May 2014

*/
//____________________________________________________________________________
#ifndef FLUXGEN_PIONPROJECTIONFLUXALGORITHM_H
#define FLUXGEN_PIONPROJECTIONFLUXALGORITHM_H 1

#include "LbnoDataLib.hh"
#include "DataCards.hh"

class PiFluxProjectionAlgorithm {
 public:
  PiFluxProjectionAlgorithm();
  virtual ~PiFluxProjectionAlgorithm() {}

  void initDataCards();
  void loadDataCards();

  bool Process(PionDecayEvent *piDecay, NeutrinoHit* nuHit);

  void setOrigin(double zOrigin) { zOrigin_ = zOrigin;};
  void setVerbose(int verb) { verbose_ = verb;};

 private:
  DataCards* cards_;

  double distance_;
  double radiusCut_;
  double maxNuE_;
  int flavour_;

  double zOrigin_;

  int verbose_;
};

#endif


