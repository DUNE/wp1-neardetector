// -------------------------------------------------------------------
/// \file   TrackingSD.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "TrackingSD.h"

#include <G4SDManager.hh>



TrackingSD::TrackingSD(const G4String& detector_name):
  G4VSensitiveDetector(detector_name), hc_(0)
{
  collectionName.insert(HitCollectionUniqueName());
}


TrackingSD::~TrackingSD()
{
}


void TrackingSD::Initialize(G4HCofThisEvent* hce)
{
  hc_ = new TrackingHitsCollection(this->GetName(),this->GetCollectionName(0));
  G4int hcid = G4SDManager::GetSDMpointer()->
    GetCollectionID(this->GetName() + "/" + this->GetCollectionName(0));
  hce->AddHitsCollection(hcid, hc_);
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

  hc_->insert(hit);

  return true;
}


void TrackingSD::EndOfEvent(G4HCofThisEvent*)
{
  return;
}

