// -------------------------------------------------------------------
/// \file   TrackingSD.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "TrackingSD.h"

#include <G4SDManager.hh>



TrackingSD::TrackingSD(const G4String& name,
                       const G4String& collection_name):
  G4VSensitiveDetector(name), hits_collection_(0)
{
  collectionName.insert(collection_name);
}


TrackingSD::~TrackingSD()
{
}


void TrackingSD::Initialize(G4HCofThisEvent* hce)
{
  hits_collection_ = 
    new TrackingHitsCollection(SensitiveDetectorName, collectionName[0]);

  G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcid, hits_collection_);
}


G4bool TrackingSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep <= 0.) return false;

  TrackingHit* hit = new TrackingHit();

  hit->SetTrackID(step->GetTrack()->GetTrackID());
  hit->SetEdep(edep);
  hit->SetPositionAndTime(step->GetPostStepPoint()->GetPosition().x(),
                          step->GetPostStepPoint()->GetPosition().y(),
                          step->GetPostStepPoint()->GetPosition().z(),
                          step->GetTrack()->GetGlobalTime());

  hits_collection_->insert(hit);

  return true;
}


void TrackingSD::EndOfEvent(G4HCofThisEvent*)
{
  return;
}

