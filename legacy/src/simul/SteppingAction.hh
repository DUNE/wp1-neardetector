//____________________________________________________________________________
/*!

\class    SteppingAction

\brief    Class for the user stepping action in geant4

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UImanager.hh"
#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "G4UserSteppingAction.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4VProcess.hh"
#include "G4OpBoundaryProcess.hh"
#include "globals.hh"

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"

//#include <TLorentzVector.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>

#include <iostream>

#include "TrackInformation.hh"
#include "LbnoDataLib.hh"
#include "ApplyCuts.hh"
#include "Tracker.hh"

class SteppingAction: public G4UserSteppingAction
{
 public:
  SteppingAction(G4double cutEnergy,bool trackPiZero=false); 
  SteppingAction(TGeoManager * geoMgr,G4double cutEnergy,bool trackPiZero=false); 
  ~SteppingAction();

  //DetectorHits* getHit() {return hit_;}
  //void setHit(DetectorHits* h)  {hit_ = h;}
  bool hasHit() const {return hasHit_;}
  void UserSteppingAction(const G4Step* aStep);

  void unsetHit() {hasHit_ = false;}

  void Initialise();

  void Print();
  void ResetValues();
  void ClearTrackVtrs();

  void IncrementID();

  void setVerbose(int verb) { verbose_ = verb;};
  void setVolume(std::string name) { targetVolName_ = name;};
  void setParticleKinematics(GeantParticle & p);
  void setOriginalKinematics(GeantParticle & p,TrackInformation * info);

  bool particleSet(std::vector<GeantPrimaryParticle> * particleVtr,int trackid);
  bool findParent(GeantBasicParticle *p,TrackInformation * info);

  //getters
  std::vector<GeantPrimaryParticle> * getPrimariesVtrPtr()	{return primariesVtr;};
  std::vector<GeantDaughterParticle> * getInParticlesVtrPtr()	{return inParticleVtr;};
  std::vector<GeantDaughterParticle> * getOutParticlesVtrPtr()	{return outParticleVtr;};

 protected:
  SDHit* hit_;
  bool hasHit_;

 private:

  template<class dataTypeOne,class dataTypeTwo>
  bool IsDescendant(int parentTrackId,dataTypeOne &daugher,dataTypeTwo &vector);

  //root geomanager volumes
  TGeoManager * rootGeoMgr_;
  TGeoVolume * World_;
  TGeoVolume * preVol_;
  TGeoVolume * postVol_;

  //use lbnoDataLib definitions to make GeantParticles
  std::vector<GeantPrimaryParticle> * primariesVtr;
  std::vector<GeantDaughterParticle> * inParticleVtr;
  std::vector<GeantDaughterParticle> * outParticleVtr;

  int eventID,geantID,trackID,parentID;
  int steps,pdg; 
  double preX,preY,preZ;
  double postX,postY,postZ;
  double kinenergy,mass,energy,magMom;

  G4StepPoint* prePoint;
  G4StepPoint* postPoint;

  G4ThreeVector preStepPos;
  G4ThreeVector postStepPos;
  G4ThreeVector momDir;

  //volume names 
  std::string trackVolumeName,trackNextVolumeName;
  //mother volume names
  std::string trackMotherVolumeName,trackNextMotherVolumeName;

  //target volume name from input card
  std::string targetVolName_;

  ApplyCuts * cuts;
  double cutEnergy_;
  bool trackPiZero_;

  int particleEnterCount,particleLeaveCount; 
  int condition;

  int verbose_;
};

#endif
