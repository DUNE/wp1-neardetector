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
#include <G4String.hh>
#include "TrackingHit.h"


class TrackingSD: public G4VSensitiveDetector
{
public:
  /// Constructor
  TrackingSD(const G4String& detector_name);
  /// Destructor
  virtual ~TrackingSD();
  
  virtual void Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  virtual void EndOfEvent(G4HCofThisEvent*);

  static G4String HitCollectionUniqueName();

private:
  TrackingHitsCollection* hc_;
};

inline G4String TrackingSD::HitCollectionUniqueName() 
  { return G4String("TrackingHitsCollection"); }

#endif  