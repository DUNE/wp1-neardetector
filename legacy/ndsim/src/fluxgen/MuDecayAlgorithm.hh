//____________________________________________________________________________
/*!

\class    MuDecayAlgorithm

\brief    Class used to decay muons

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef FLUXGEN_MUONDECAYFLUXALGORITHM_H
#define FLUXGEN_MUONDECAYFLUXALGORITHM_H 1

#include <string>
#include <iostream>

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

#define MUON_MASS 105.658367*CLHEP::MeV // pm 0.000004 (PDG 2010)

class MuDecayAlgorithm {
 public:
  MuDecayAlgorithm();
  ~MuDecayAlgorithm();

  TRandom* getRand() { return &rand_; }
  void getNextEvent();
  void Process();

  TTree* getDataTree() { return dataTree_; }

  void initDataCards();
  void loadDataCards();

 private:
  void initialize();
  void MakeNeutrino(TF2 *func, double funcMax, double decayZ, TLorentzVector& nuP4, TLorentzVector& nuPos);

  int muSign_;
  double muPolarization_;  // muon beam polarization
  double muEnergy_;        // muon beam energy
  double muEnergySigma_;   // muon beam energy sigma
  double muThetaSigma_;
  double straightSection_; // length of straight section
  double distance_;        // distance to the detector
  TRandom3 rand_;          // Using "Mersenne Twister generator" based random numbers

  MuonDecayEvent *event_;
  NeutrinoHit* neutrino_;
  TTree *dataTree_;
  int nDecay_;
  double radiusCut_;

  TF2* func1_;
  TF2* func2_;
  double max1_;
  double max2_;

  DataCards *cards_;
  bool cardsLoaded_;
};

#endif
