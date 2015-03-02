//____________________________________________________________________________
/*!

\class    EventAction

\brief    Class for geant4 user event action

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef EventAction_h
#define EventAction_h 1

#include <G4EventManager.hh>
#include <G4TrajectoryContainer.hh>
#include <G4VVisManager.hh>
#include <G4SDManager.hh>
#include <G4HCofThisEvent.hh>
#include <G4Trajectory.hh>
#include <G4ios.hh>
#include <G4Event.hh>
#include <G4UserEventAction.hh>
#include <G4RunManager.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleDefinition.hh>
#include <G4Navigator.hh>
#include <G4ThreeVector.hh>

// ROOT includes
#include "TGeoManager.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TGeoVolume.h"

#include "DetectorHit.hh"
#include "DetectorConstruction.hh"
#include "SteppingAction.hh"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

class EventAction : public G4UserEventAction
{
 public:
  EventAction(std::string sdName,SteppingAction *);
  EventAction(std::string sdName,TGeoManager * geoMgr,SteppingAction *);
  ~EventAction();
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

private:
  SteppingAction * stpaction;

  TGeoManager * rootGeoMgr_;
  TGeoVolume * World_;
  TGeoVolume * preVol_;
  TGeoVolume * postVol_;

  double HitPosX;
  double HitPosY;
  double HitPosZ;
  double HitEDep;
  double HitNeutronEDep;
  double HitChargedEDep;
  double HitGammaEDep;
  double HitKinEnergy;
  double HitRange;
  int HitPDG;
  int HitTrackID;
  int HitEventID;

  G4int	                 EventID,nTrajectories;
  std::vector<G4int>     detectorCollIDVect;

  G4String 		 SDName;

  std::vector<DetectorHitsCollection*> DHC_vct;
  G4HCofThisEvent * HCE;

};

#endif
