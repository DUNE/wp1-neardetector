// MAUS WARNING: THIS IS LEGACY CODE.
/*
** Modified example 1 from the GEANT4 distribution to simulate the
** MICE scintillating fiber tracker for the MICE proposal
** Ed McKigney - August 21, 2002
*/
#include "MINDSensitiveDetector.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VSolid.hh"
#include "DataCards.hh"

#include <G4StepStatus.hh>
#include <cstring>
#include <iostream>

MINDSensitiveDetector::MINDSensitiveDetector(const G4String& name,const G4String &hitsCollectionName)
: DetectorSD(name,hitsCollectionName) {

	SDname = name;
	HCname = "MINDHitsCollection";
  	collectionName.insert(HCname);
  	positionResolution = 0.3*mm;
}

MINDSensitiveDetector::~MINDSensitiveDetector() {}

void MINDSensitiveDetector::Initialize(G4HCofThisEvent* HCE) {
  std::cout << "This is MINDSensitiveDetector::Initialize "  << std::endl;

  //declare hit types
  Hit_ = new DetectorHit();

  // Create hits collection with name set by constructer
  HitsCollection_ = new DetectorHitsCollection(SDname,HCname); 

  // Get the ID for this Hit Collection
  // and add this collection in hce
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
  HCE->AddHitsCollection(HCID,HitsCollection_);

  NSteps_ = 0;
  Edep_ = 0.0;
  PDG_ = 0;
  TrackID_ = 0;
  kinEnergy_ = 0.;
  Pos_.SetXYZT(0.,0.,0.,0.);

  Hit_->setNSteps(NSteps_);
  Hit_->setEdep(Edep_);
  Hit_->setPDG(PDG_);
  Hit_->setTrackID(TrackID_);
  Hit_->setKinEnergy(kinEnergy_);
  Hit_->setPos(Pos_);
  //Hit_->setMuon(ParticleDescrRecord());
}

G4bool MINDSensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist) {
if (NSteps_ == 0) {
    G4Track* track = aStep->GetTrack();
    std::cout << "TrackID : " << track->GetTrackID() << std::endl;

    TLorentzVector pos( aStep->GetPreStepPoint()->GetPosition().x(),
                        aStep->GetPreStepPoint()->GetPosition().y(),
                        aStep->GetPreStepPoint()->GetPosition().z());

    ParticleDescrRecord muon;
    TLorentzVector p4( track->GetMomentum().x(),
                       track->GetMomentum().y(),
                       track->GetMomentum().z(),
                       track->GetTotalEnergy() );
    muon.setP4(p4);
    muon.setPDG( track->GetDefinition()->GetPDGEncoding() );

    Hit_->setPos(pos);
    //Hit_->setMuon(muon);
/*
    ThreeVector pos( aStep->GetPreStepPoint()->GetPosition().x(),
                     aStep->GetPreStepPoint()->GetPosition().y(),
                     aStep->GetPreStepPoint()->GetPosition().z()); 
    ThreeVector mom( track->GetMomentum().x(),
                     track->GetMomentum().y(),
                     track->GetMomentum().z());

    _hit_cppdata.SetTrackId( track->GetTrackID() );
    _hit_cppdata.SetParticleId( track->GetDefinition()->GetPDGEncoding() );
    _hit_cppdata.SetEnergy( track->GetTotalEnergy() );
    _hit_cppdata.SetCharge( track->GetDefinition()->GetPDGCharge() );
    _hit_cppdata.SetTime( aStep->GetPreStepPoint()->GetGlobalTime() );
    _hit_cppdata.SetPosition( pos );
    _hit_cppdata.SetMomentum( mom );
*/
  }

  NSteps_++;
  return true;
}

void MINDSensitiveDetector::EndOfEvent( G4HCofThisEvent* HCE ) {
  if (nSteps_) {
    //std::cout << "This is MINDSensitiveDetector::EndOfEvent " << std::endl ;
    //hit_->setPath(path_);
    hit_->setEdep(Edep_);
  } 
  hit_->setNSteps(NSteps_);
}
