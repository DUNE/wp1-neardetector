#include "EMCalcPrimaryGeneratorAction.hh"

#include "EMCalcDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

EMCalcPrimaryGeneratorAction::EMCalcPrimaryGeneratorAction( EMCalcDetectorConstruction* DC)
:detector_(DC) {
  particleGun_  = new G4ParticleGun(1);
}

EMCalcPrimaryGeneratorAction::~EMCalcPrimaryGeneratorAction() {
  delete particleGun_;
}

void EMCalcPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  //this function is called at the begining of event
  //
  particleGun_->GeneratePrimaryVertex(anEvent); 
}


