// MAUS WARNING: THIS IS LEGACY CODE.
/*
** Modified example 1 from the GEANT4 distribution to simulate the
** MICE scintillating fiber tracker for the MICE proposal
** Ed McKigney - August 21, 2002
*/
#include "LqArTankSensitiveDetector.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "DataCards.hh"

#include <G4StepStatus.hh>
#include <cstring>
#include <iostream>

LqArTankSensitiveDetector::LqArTankSensitiveDetector(const G4String& name)
: G4VSensitiveDetector(name) {}

LqArTankSensitiveDetector::~LqArTankSensitiveDetector() {}

void LqArTankSensitiveDetector::Initialize(G4HCofThisEvent* HCE) {
  //std::cout << "This is LqArTankSensitiveDetector::Initialize " << nSteps_ << std::endl ;
  nSteps_ = 0;
  hit_->setNSteps(nSteps_);
  hit_->setPosition(TLorentzVector(0., 0., 0., 0.));
  hit_->setMuon(ParticleDescrShortRecord());
}

G4bool LqArTankSensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist) {
  //std::cout << "This is LqArTankSensitiveDetector::ProcessHits " << nSteps_ << std::endl;

  if (nSteps_ == 0) {
    G4Track* track = aStep->GetTrack();
    std::cout << "TrackID : " << track->GetTrackID() << std::endl;

    TLorentzVector pos( aStep->GetPreStepPoint()->GetPosition().x(),
                        aStep->GetPreStepPoint()->GetPosition().y(),
                        aStep->GetPreStepPoint()->GetPosition().z());

    ParticleDescrShortRecord muon;
    TLorentzVector p4( track->GetMomentum().x(),
                       track->GetMomentum().y(),
                       track->GetMomentum().z(),
                       track->GetTotalEnergy() );
    muon.setP4(p4);
    muon.setPDG( track->GetDefinition()->GetPDGEncoding() );

    hit_->setPosition(pos);
    hit_->setMuon(muon);
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

  nSteps_++;
  return true;
}

void LqArTankSensitiveDetector::EndOfEvent( G4HCofThisEvent* HCE ) {
/*
  std::cout << "This is LqArTankSensitiveDetector::EndOfEvent " << nSteps_ << std::endl ;
  if (nSteps_) {
    hit_->printToStream(std::cout); 
  }
*/
  hit_->setNSteps(nSteps_);
}
