//____________________________________________________________________________
/*!

\class    FluxFileLoadAlgorithm

\brief    Class used to read in data from neutrino beam file

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool


\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef FLUXGEN_FLUXFILELOADALGORITHM_H
#define FLUXGEN_FLUXFILELOADALGORITHM_H 1

#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TRandom3.h>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "LbnoDataLib.hh"
#include "DataCards.hh"

class FluxFileLoadAlgorithm {
 public:
  FluxFileLoadAlgorithm();
  virtual ~FluxFileLoadAlgorithm();

  bool getNextEvent(PionDecayEvent* piDecay);

  void initDataCards();
  void loadDataCards();
  void initialize();
  void close();

  int getEntries() { return nEntries_;};
  int getNoIterations() { return nIterations_;};
  void incrementIterationCount()  { nIterations_++;}

  void setVerbose(int verb) { verbose_ = verb;};

 private:

  DataCards* cards_;
  bool cardsLoaded_;

  TRandom3 * randGen;

  std::string inputBeamFileName_;
  std::string inputBeamTreeName_;
  int entryCount_;
  int nEntries_;
  int nIterations_;
  int nLoops_;
  TFile* inputBeamFile_;
  TTree* inputTree_;

  double nuPx_, nuPy_, nuPz_, nuEn_;
  double maxNuE_;
  double parentVx_, parentVy_, parentVz_;
  int nuPID_, nuPDG_;

  int verbose_;

};

#endif
