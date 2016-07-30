// -------------------------------------------------------------------
/// \file   TrackingSD.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef TRACKING_SD_H
#define TRACKING_SD_H

#include <G4VSensitiveDetector.hh>
#include "TrackingHit.h"


class TrackingSD: public G4VSensitiveDetector
{
public:
  TrackingSD(const G4String& name,
             const G4String& collection_name);
  virtual ~TrackingSD();
  
  virtual void Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  virtual void EndOfEvent(G4HCofThisEvent*);

private:
  TrackingHitsCollection* hits_collection_;
};

#endif  