//____________________________________________________________________________
/*!

\class    ScintillatorSensitiveDetector

\brief    Class for the sensitive detector for the scintillator

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update April 2014

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

#include "LbnoDataLib.hh"
#include "DetectorSD.hh"

#include "DataCards.hh"

class G4Step;
class G4HCofThisEvent;

class ScintillatorSensitiveDetector : public G4VSensitiveDetector {
 public:
  //! Constructor
  ScintillatorSensitiveDetector(const G4String&,int layerID);
  ScintillatorSensitiveDetector(const G4String&,const G4String &hitsCollectionName,int layerID);

  //! Default destructor
  ~ScintillatorSensitiveDetector();

   void Initialize(G4HCofThisEvent*HCE);

   G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
   void EndOfEvent(G4HCofThisEvent* HCE);
   void clear();

   void setHitCollectionPtr(ScintHitCollection* h) {ScintSDHitCollection = h;};
   ScintHitCollection * getHitCollectionPtr() {return ScintSDHitCollection;};

   int GetLayerNumber()	const	{return layerNumber;};
   void SetLayerNumber(int no)  {layerNumber = no;};

 private:
   G4double positionResolution;

   G4String SDname,SDBaseName;
   G4String HCname;

  double energyDepLimit;
  int layerNumber;

  int NSteps_;
  int PDG_;
  int TrackID_,ParentID_;
  G4double kinEnergy_;
  G4double Edep_;
  TLorentzVector Pos_,P4_;
  bool trackLeftVolume;
  
  DetectorHit* Hit_;
  DetectorHitsCollection* HitsCollection_;

  scintHit SDHit_;
  //make it static so you get only one hit collection per event
  // includes all scintillator SD
  static ScintHitCollection * ScintSDHitCollection;

};

#endif



