//____________________________________________________________________________
/*!

\class    

\brief    Header file containing all class types needed for software

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________

#ifndef EMCALC_RANGEESTIMATIONALGORITHM_H
#define EMCALC_RANGEESTIMATIONALGORITHM_H 1

#include <string>
#include <iostream>

#include <G4RunManager.hh>
#include <G4GDMLParser.hh>

#include "G4EmCalculator.hh"

//#include "G4ParticleTable.hh"
#include "G4UnitsTable.hh"

#include "GasTPCDataLib.hh"

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

  void initialize();

 private:
  G4RunManager*                   runManager_;

  EMCalcPhysicsList*              physList_;
  RunAction*                      runAction_;
  EMCalcPrimaryGeneratorAction*   pgAction_;
  EMCalcDetectorConstruction*     detector_;

  std::string material_;
};

#endif


