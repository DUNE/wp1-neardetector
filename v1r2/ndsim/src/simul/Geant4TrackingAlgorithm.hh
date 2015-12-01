//____________________________________________________________________________
/*!

\class    Geant4TrackingAlgorithm

\brief    Main class to implement geant4 simulation

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jun 2015
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

#include "GasTPCDataLib.hh"
#include "GeometryLoader.hh"
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
  Geant4TrackingAlgorithm(std::string geomFile, std::string geomVol);
  Geant4TrackingAlgorithm(std::string geomFile, std::string geomVol, GeometryLoader *geomLoader); 
  virtual ~Geant4TrackingAlgorithm();

  bool Process(NeutrinoEvent* event);

  //void runGunMac();
  void runVisMac();
  void initialize();

  // Setters
  void setPhysicsList(std::string physicsList) {physicsList_ = physicsList;};
  void setPhysicsList(std::string emlist, std::string hadroniclist) {emlist_ = emlist; hadroniclist_ = hadroniclist;};
  void setHadPhysicsList(std::string hadroniclist) {hadroniclist_ = hadroniclist;};
  void setEMPhysicsList(std::string emlist) {emlist_ = emlist;};
  void setMagneticField(double magFieldStrength) {magFieldStrength_ = magFieldStrength;};
  void setVerbose(int verbose) {verbose_ = verbose;};
  void setEnergyCut(double cutEnergy) {cutEnergy_ = cutEnergy;};
  void setDefaultCut(double cutDefault) {defaultCut_ = cutDefault;};
  void setElectronCut(double electronCut) {electronCut_ = electronCut;};
  void setPositronCut(double positronCut) {positronCut_ = positronCut;};
  void setGammaCut(double gammaCut) {gammaCut_ = gammaCut;};
  void setNeutronCut(double neutronCut) {neutronCut_ = neutronCut;};
  void setNEcals(int n) {necal_ = n;};
  void doVis(bool runVisMacro) {runVisMacro_ = runVisMacro;}

  // Getters
  std::string getPhysicsList() {return physicsList_;};
  std::string getHadPhysicsList() {return hadroniclist_;};
  std::string getEMPhysicsList() {return emlist_;};
  std::string getGeometryFile() {return geomFile_;};
  int getVerbose() {return verbose_;};
  double getEnergyCut() {return cutEnergy_;};
  double getDefaultCut() {return defaultCut_;};
  double getElectronCut() {return electronCut_;};
  double getPositronCut() {return positronCut_;};
  double getGammaCut() {return gammaCut_;};
  double getNeutronCut() {return neutronCut_;};
  double getMagneticField() {return magFieldStrength_;};
  int getNEcals() {return necal_;};

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
  std::string physicsList_,hadroniclist_,emlist_;
  std::string targetVolName_;

  double magFieldStrength_;
  G4double cutEnergy_;

  G4double defaultCut_;
  G4double gammaCut_;
  G4double electronCut_;
  G4double positronCut_;
  G4double neutronCut_;

  bool      runVisMacro_;

  int verbose_, necal_;
};

#endif



