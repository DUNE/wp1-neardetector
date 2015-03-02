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

class SteppingAction: public G4UserSteppingAction
{
 public:
  SteppingAction(std::string fluxOutputFile_,G4double); 
  SteppingAction(TGeoManager * geoMgr,std::string fluxOutputFile_,G4double); 
  ~SteppingAction();

  //DetectorHits* getHit() {return hit_;}
  //void setHit(DetectorHits* h)  {hit_ = h;}
  bool hasHit() const {return hasHit_;}
  void UserSteppingAction(const G4Step* aStep);

  void unsetHit() {hasHit_ = false;}

  void InitRootFile(std::string);
  void Write();
  void Print();
  void ResetValues();

  void IncrementID();

  void setVerbose(int verb) { verbose_ = verb;};
  void setVolume(std::string name) { targetVolName_ = name;};

  void setWriteToFile(bool write){writeToFile = write;};
  bool getWriteToFile(){return writeToFile;};

  // Get the inFlux tree
  TTree * getInFluxTree() {return fROOTInFluxTree;};
  // Get the outFlux tree
  TTree * getOutFluxTree() {return fROOTOutFluxTree;};
  // Get the PMT events tree
  TTree * getPrimaryTree() {return fROOTPrimaryTree;};

 protected:
  SDHit* hit_;
  bool hasHit_;

 private:

  //root geomanager volumes
  TGeoManager * rootGeoMgr_;
  TGeoVolume * World_;
  TGeoVolume * preVol_;
  TGeoVolume * postVol_;

  //use lbnoDataLib definitions to make GeantParticles
  std::vector<GeantPrimaryParticle *> primariesVtr;
  std::vector<GeantParticle *> inParticleVtr;
  std::vector<GeantParticle *> outParticleVtr;

  //for output stepping file
  TFile * fROOTfile;
  TTree * fROOTPrimaryTree;
  TTree * fROOTInFluxTree;
  TTree * fROOTOutFluxTree;

  //-----------VARIABLES---------------
  //-----------primary tree-----------
  G4int fPrimEventID;
  G4int fPrimGeantID;
  G4int fPrimTrackID,prevPrimTrackID;
  G4int fPrimPDG;
  G4double fPrimEnergy;
  G4double fPrimKinEnergy;
  G4double fPrimPx;
  G4double fPrimPy;
  G4double fPrimPz;
  G4double fPrimPosX;
  G4double fPrimPosY;
  G4double fPrimPosZ;

  //----------InFlux tree----------------
  G4int fInEventID, fInGeantID,fInTrackID,fInDetectorID,fInPDG;
  G4int fInPrimScat,fInNoParticle;
  G4double fInMomX,fInMomY,fInMomZ,fInEnergy;
  G4double fInKEAtVertex,fInTotEDep;
  G4double fInPosX,fInPosY,fInPosZ,fInSolAngA,fInSolAngB;

  //----------OutFlux tree----------------
  G4int fOutEventID, fOutGeantID,fOutTrackID,fOutDetectorID,fOutPDG;
  G4int fOutPrimScat,fOutNoParticle;
  G4double fOutMomX,fOutMomY,fOutMomZ,fOutEnergy;
  G4double fOutKEAtVertex,fOutTotEDep;
  G4double fOutPosX,fOutPosY,fOutPosZ;

  int eventID,geantID;

  bool writeToFile;

  int TrajTrackID;
  int TrajParentID;
  int TrajPDG;
  double interactionPointX;
  double interactionPointY;
  double interactionPointZ;
  double TrajMomentumX;
  double TrajMomentumY;
  double TrajMomentumZ;
  double TrajMomentum;
  double TrajKineticEnergy,TrajDeltaEnergy;
  double TrajEnergy;
  double TrajInitialXPosition;
  double TrajInitialYPosition;
  double TrajInitialZPosition;
  double TrajFinalXPosition;
  double TrajFinalYPosition;
  double TrajFinalZPosition;
  double TrajFinalKineticEnergy;
  double prevTrajFinalKineticEnergy;
  std::string theParticleName;

  G4StepPoint* prePoint;
  G4StepPoint* postPoint;

  G4ThreeVector preStepPos;
  G4ThreeVector postStepPos;

  //volume names 
  std::string trackVolumeName,trackNextVolumeName;
  //mother volume names
  std::string trackMotherVolumeName,trackNextMotherVolumeName;

  //target volume name from input card
  std::string targetVolName_;

  ApplyCuts * cuts;

  int particleEnterCount,particleLeaveCount; 
  int steps;
  int condition;

  int verbose_;
};

#endif
