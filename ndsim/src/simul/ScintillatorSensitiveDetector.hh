//____________________________________________________________________________
/*!

\class    ScintillatorSensitiveDetector

\brief    Class for the sensitive detector for the scintillator

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
         University of Liverpool

\created  Sep 2012
\last update Nov 2015
*/
//____________________________________________________________________________
#ifndef SCINTILLATORSENSITIVEDETECTOR_hh
#define SCINTILLATORSENSITIVEDETECTOR_hh 1

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4VSensitiveDetector.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4THitsCollection.hh"
#include "G4TransportationManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4TouchableHandle.hh"

#include <G4StepStatus.hh>
#include <cstring>
#include <iostream>

#include "GasTPCDataLib.hh"
#include "DetectorSD.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ScintillatorSensitiveDetector : public G4VSensitiveDetector {
 public:
  //! Constructor
  ScintillatorSensitiveDetector(const G4String name);
  ScintillatorSensitiveDetector(const G4String name,const G4String &hitsCollectionName);

  //! Default destructor
  virtual ~ScintillatorSensitiveDetector();

  void Initialize(G4HCofThisEvent*HCE);

  G4bool ProcessHits(G4Step *aStep , G4TouchableHistory *touchHistory);//ROhist);
  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  
  void setHitCollectionPtr(ScintHitCollection* h) {ScintSDHitCollection = h;};
  static ScintHitCollection * getHitCollectionPtr() {return ScintSDHitCollection;};

  virtual G4String GetName() {return SDname;};
  virtual G4String GetHCName() {return HCname;};

 private:
  G4String SDname;
  G4String HCname;

  int NSteps_,charge_;
  int PDG_;
  int TrackID_,ParentID_;
  G4double kinEnergy_;
  G4double Edep_;
  TLorentzVector Pos_,P4_;
  bool trackLeftVolume;
  
  DetectorHit* Hit_;
  DetectorHitsCollection* HitsCollection_;

  scintHit SDHit_;
  //make it static so you get only one hit collection per event includes all scintillator SD - need to fix
  static ScintHitCollection * ScintSDHitCollection;
};

#endif


