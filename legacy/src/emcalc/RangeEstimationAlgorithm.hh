#ifndef EMCALC_RANGEESTIMATIONALGORITHM_H
#define EMCALC_RANGEESTIMATIONALGORITHM_H 1

#include <string>
#include <iostream>

#include <G4RunManager.hh>
#include <G4GDMLParser.hh>

#include "G4EmCalculator.hh"

//#include "G4ParticleTable.hh"
#include "G4UnitsTable.hh"

#include "DataCards.hh"
#include "LbnoDataLib.hh"

#include "G4ParticleGun.hh"
#include "EMCalcPhysicsList.hh"
#include "EMCalcPrimaryGeneratorAction.hh"
#include "EMCalcDetectorConstruction.hh"
#include "RunAction.hh"

class RangeEstimationAlgorithm {
 public:
  RangeEstimationAlgorithm();
  virtual ~RangeEstimationAlgorithm();

  bool Process(tpcFidHit* event, MuonRange *range);

  void initDataCards();
  void loadDataCards();
  void initialize();

 private:
  G4RunManager*                   runManager_;

  EMCalcPhysicsList*              physList_;
  RunAction*                      runAction_;
  EMCalcPrimaryGeneratorAction*   pgAction_;
  EMCalcDetectorConstruction*     detector_;

  std::string material_;

  DataCards *cards_;
  bool      cardsLoaded_;
};

#endif


