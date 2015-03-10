//____________________________________________________________________________
/*!

\class    TrackInformation

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update Sep 2013

*/
//____________________________________________________________________________
#include "TrackInformation.hh"
#include "G4ios.hh"

G4Allocator<TrackInformation> aTrackInformationAllocator;

TrackInformation::TrackInformation()
{
    originalParentID = 0;
    originalTrackID = 0;
    originalPDG = 0;
    particleDefinition = 0;
    originalPosition = G4ThreeVector(0.,0.,0.);
    originalMomentum = G4ThreeVector(0.,0.,0.);
    originalKineticEnergy = 0.;
    originalEnergy = 0.;
    originalTime = 0.;
    originalMass = 0.;
}

TrackInformation::TrackInformation(const G4Track* aTrack)
{
    originalParentID = aTrack->GetParentID();
    originalTrackID = aTrack->GetTrackID();
    originalPDG = aTrack->GetDefinition()->GetPDGEncoding();
    particleDefinition = aTrack->GetDefinition();
    originalPosition = aTrack->GetPosition();
    originalMomentum = aTrack->GetMomentum();
    originalEnergy = aTrack->GetTotalEnergy();
    originalMass = aTrack->GetParticleDefinition()->GetPDGMass();
    originalKineticEnergy = aTrack->GetVertexKineticEnergy();
    originalTime = aTrack->GetGlobalTime();
}

TrackInformation::TrackInformation(const TrackInformation* aTrackInfo)
{
    originalParentID = aTrackInfo->originalParentID;
    originalTrackID = aTrackInfo->originalTrackID;
    originalPDG = aTrackInfo->originalPDG;
    particleDefinition = aTrackInfo->particleDefinition;
    originalPosition = aTrackInfo->originalPosition;
    originalMomentum = aTrackInfo->originalMomentum;
    originalEnergy = aTrackInfo->originalEnergy;
    originalMass = aTrackInfo->originalMass;
    originalKineticEnergy = aTrackInfo->originalKineticEnergy;
    originalTime = aTrackInfo->originalTime;
}

TrackInformation::~TrackInformation(){;}

void TrackInformation::Print() const
{
    G4cout 
     << "Original track ID " << originalTrackID
     << ", Original track PDG " << originalPDG 
     << ", Original track KineticEnergy " << originalKineticEnergy
     << ", at " << originalPosition << G4endl;
}

