// -------------------------------------------------------------------
/// \file   TrackingHit.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "TrackingHit.h"

#include <G4LorentzVector.hh>


G4ThreadLocal G4Allocator<TrackingHit>* TrackingHitAllocator = 0;


TrackingHit::TrackingHit(): 
  G4VHit(), track_id_(-1), edep_(0.), xyzt_(G4LorentzVector())
{
}


TrackingHit::~TrackingHit()
{
}


TrackingHit::TrackingHit(const TrackingHit& other): G4VHit()
{
  track_id_ = other.track_id_;
  edep_ = other.edep_;
  xyzt_ = other.xyzt_;
}


const TrackingHit& TrackingHit::operator=(const TrackingHit& other)
{
  track_id_ = other.track_id_;
  edep_ = other.edep_;
  xyzt_ = other.xyzt_;

  return *this;
}


G4int TrackingHit::operator==(const TrackingHit& other) const
{
  return (this == &other) ? 1 : 0;
}


void TrackingHit::SetPositionAndTime(G4double x, G4double y, G4double z, 
                                     G4double t)
{
  xyzt_.setX(x);
  xyzt_.setY(y);
  xyzt_.setZ(z);
  xyzt_.setT(t);
}


const G4LorentzVector& TrackingHit::GetPositionAndTime() const
{
  return xyzt_;
}





