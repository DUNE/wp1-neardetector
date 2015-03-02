//____________________________________________________________________________
/*!

\class    Geant4TrackingAlgorithm

\brief    Main class to implement geant4 simulation

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#ifndef SIMUL_GEANT4TRACKINGALGORITHM_H
#define SIMUL_GEANT4TRACKINGALGORITHM_H 1

#include <string>

#include <G4RunManager.hh>
#include <G4PhysListFactory.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "TG4RootNavMgr.h"

#include "DataCards.hh"
#include "LbnoDataLib.hh"

#include "MyUserPostDetConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "TrackingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "PhysicsList.hh"

class Geant4TrackingAlgorithm {
 public:

  Geant4TrackingAlgorithm();
  virtual ~Geant4TrackingAlgorithm();

  bool Process();
  bool Process(NeutrinoEvent* event);

  bool isGunOn() {return runGunMacro_;};
  int getVerbose() {return verbose_;};

  void runGunMac();
  void runVisMac();

  void initDataCards();
  void loadDataCards();
  void initialize();
  void setManager(TGeoManager * manager)    {geoMgr_ = manager;};
  //void setTargetVolume(std::string name)   {targetVolName_ = name;};

  PrimaryGeneratorAction*     getPrimaryGenerator()         const {return pgAction_;}
  SteppingAction*             getSteppingAction()           const {return stpAction_;}
  DetectorConstruction*       getDetectorConstruction()     const {return detector_;}
  MyUserPostDetConstruction*  getPostDetectorConstruction() const {return postDetConstruction_;}

 private:
  G4RunManager*             runManager_;

  MyUserPostDetConstruction * postDetConstruction_;

  PhysicsList*              physList_;
  RunAction*                runAction_;
  EventAction*              eventAction_;
  SteppingAction*           stpAction_;
  StackingAction*	    stackAction_;
  TrackingAction*           trAction_;
  PrimaryGeneratorAction*   pgAction_;
  DetectorConstruction*     detector_;

  TGeoManager *  geoMgr_;
  TG4RootNavMgr *detMgr;

  G4UImanager* UImanager;
  G4VisManager* visManager;

  std::string geomFile_;
  std::string SDName_;
  std::string eventOutputFile_;
  std::string physicsList_;
  std::string targetVolName_;

  std::string hadroniclist_,emlist_;

  double magFieldStrength_;
  G4double cutEnergy_;
  G4double deltaEnergyCut_;

  DataCards *cards_;
  bool      cardsLoaded_;
  bool      runVisMacro_;
  bool      runGunMacro_;
  bool 	    piZeroTracker_;

  int verbose_;
};

#endif



