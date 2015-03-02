//____________________________________________________________________________
/*!

\class    StackingAction

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update April 2014

*/
//____________________________________________________________________________
#include <globals.hh>
#include <G4ParticleDefinition.hh>
// #include <G4VProcess.hh>

#include "StackingAction.hh"

StackingAction::StackingAction() { }

StackingAction::~StackingAction() { }
  /*
G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* aTrack) {
  
    if (aTrack->GetCurrentStepNumber() == 0) {
        // Get the particle type of the new track.
        const G4ParticleDefinition* particle = aTrack->GetDefinition();
        
        // This is where we can throw away particles that we don't want to
        // track.
        if (particle->GetParticleName() == "gamma") {
            if (aTrack->GetKineticEnergy() < 100.*keV) return fKill;
        }
        if (particle->GetParticleName() == "e-") {
            if (aTrack->GetKineticEnergy() < 100.*keV) return fKill;
        }
	
    }

    return fUrgent;
}
*/