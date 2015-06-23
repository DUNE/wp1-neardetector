//____________________________________________________________________________
/*!

\class    

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________

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


