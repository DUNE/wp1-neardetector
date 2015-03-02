#ifndef SteppingAction_h
#define SteppingAction_h 1

#include <G4UserSteppingAction.hh>
#include <G4Step.hh>
#include <G4ThreeVector.hh>

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"

//#include <TLorentzVector.h>
#include <TGeoManager.h>

#include <iostream>

#include "LbnoDataLib.hh"

class SteppingAction: public G4UserSteppingAction
{
 public:
  SteppingAction(TGeoManager * geoMgr,std::string fluxOutputFile_); 
  ~SteppingAction();

  //DetectorHits* getHit() {return hit_;}
  //void setHit(DetectorHits* h)  {hit_ = h;}
  bool hasHit() const {return hasHit_;}
  void UserSteppingAction(const G4Step* aStep);

  void unsetHit() {hasHit_ = false;}

  void Write();
  void Print();

  void setVerbose(int verb) { verbose_ = verb;};
  void setVolume(std::string name) { targetVolName_ = name;};

 protected:
  SDHit* hit_;
  bool hasHit_;

 private:
  TGeoManager * rootGeoMgr_;
  TGeoVolume * World_;
  TGeoVolume * preVol_;
  TGeoVolume * postVol_;

  TFile * rootFile;
  TTree *posZPlaneTree;
  TTree *negZPlaneTree;
  TTree *posYPlaneTree;
  TTree *negYPlaneTree;
  TTree *posXPlaneTree;
  TTree *negXPlaneTree;

  int TrajTrackID;
  int TrajParentID;
  int TrajPDG;
  double TrajMomentumX;
  double TrajMomentumY;
  double TrajMomentumZ;
  double TrajMomentum;
  double TrajKineticEnergy;
  double TrajEnergy;
  double TrajInitialXPosition;
  double TrajInitialYPosition;
  double TrajInitialZPosition;
  double TrajFinalXPosition;
  double TrajFinalYPosition;
  double TrajFinalZPosition;

  G4ThreeVector preStepPos;
  G4ThreeVector postStepPos;

  int posZcount,negZcount,posYcount,negYcount,posXcount,negXcount;

  std::string targetVolName_;

  int verbose_;
};

#endif
