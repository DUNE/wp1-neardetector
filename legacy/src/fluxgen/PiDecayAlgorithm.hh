//____________________________________________________________________________
/*!

\class    PiDecayAlgorithm

\brief    Class used to decay pions

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2014

*/
//____________________________________________________________________________
#ifndef FLUXGEN_PIONDECAYFLUXALGORITHM_H
#define FLUXGEN_PIONDECAYFLUXALGORITHM_H 1

#include <iostream>
#include <string>

#include <TH1F.h>
#include <TTree.h>
#include <TH2F.h>
#include <TF12.h>
#include <TLorentzVector.h>
#include <TRandom3.h>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "LbnoDataLib.hh"
#include "DataCards.hh"

#define PION_MASS       139.57018*CLHEP::MeV  // pm 0.00035   (PDG 2010)
#define PION_LIFETIME   2.6033e-8*CLHEP::second   // pm 0.0005e-8 (PDG 2010)

class PiDecayAlgorithm {
 public:
  PiDecayAlgorithm();
  virtual ~PiDecayAlgorithm() {};

  TRandom* getRand() { return &rand_; }
  bool getNextEvent(PionDecayEvent* piDecay);

  void initDataCards();
  void loadDataCards();

 private:
  void initialize();

  double Energy_NuMu_CM_;

  int    piSign_;
  double piMomentum_;      // pion beam mean momentum
  double piMomentumSigma_; // pion beam energy sigma
  double piThetaSigma_;
  double decaySection_;  // length of decay tunnel section
  //double distance_;      // distance to the detector

  TRandom3 rand_;        // Using "Mersenne Twister generator" based random numbers

  DataCards* cards_;
  bool cardsLoaded_;

};

#endif
