// -------------------------------------------------------------------
/// \file   TrackingHit.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef TRACKING_HIT_H
#define TRACKING_HIT_H

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4LorentzVector.hh>
#include <G4Allocator.hh>


/// TODO: Class description

class TrackingHit: public G4VHit
{
public:
  TrackingHit();
  TrackingHit(const TrackingHit&);
  virtual ~TrackingHit();

  const TrackingHit& operator=(const TrackingHit&);
  G4int operator==(const TrackingHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  void SetTrackID(G4int);
  G4int GetTrackID() const;

  void SetEdep(G4double);
  G4double GetEdep() const;

  void SetPositionAndTime(G4double, G4double, G4double, G4double);
  const G4LorentzVector& GetPositionAndTime() const;

private:
  G4int track_id_; 
  G4double edep_;
  G4LorentzVector xyzt_;
};

typedef G4THitsCollection<TrackingHit> TrackingHitsCollection;

extern G4ThreadLocal G4Allocator<TrackingHit>* TrackingHitAllocator;

inline void* TrackingHit::operator new(size_t)
{ 
  if (!TrackingHitAllocator)
    TrackingHitAllocator = new G4Allocator<TrackingHit>;
  return (void*) TrackingHitAllocator->MallocSingle();
}

inline void TrackingHit::operator delete(void* hit)
{
  TrackingHitAllocator->FreeSingle((TrackingHit*) hit);
}

inline void TrackingHit::SetTrackID(G4int id) { track_id_ = id; }
inline G4int TrackingHit::GetTrackID() const { return track_id_; }

inline void TrackingHit::SetEdep(G4double e) { edep_ = e; }
inline G4double TrackingHit::GetEdep() const { return edep_; }

#endif