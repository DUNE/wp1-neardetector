// -------------------------------------------------------------------
/// \file   TrajectoryMap.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef TRAJECTORY_POINT_H
#define TRAJECTORY_POINT_H

#include <G4VTrajectoryPoint.hh>
#include <G4Allocator.hh>
#include <trkgdefs.hh>


/// A point (position and time) in the trajectory of a simulated track

class TrajectoryPoint: public G4VTrajectoryPoint
{
public:
  /// Default constructor
  TrajectoryPoint();
  /// Constructor giving a position and time
  TrajectoryPoint(G4ThreeVector, G4double);
  /// Copy constructor
  TrajectoryPoint(const TrajectoryPoint&);
  /// Destructor
  virtual ~TrajectoryPoint();
  /// Assignement operator
  const TrajectoryPoint& operator =(const TrajectoryPoint&);
  /// Equality operator
  int operator ==(const TrajectoryPoint&) const;
  /// Memory allocation
  void* operator new(size_t);
  /// Memory deallocation
  void operator delete(void*);

  /// Return the (global) position of the point
  const G4ThreeVector GetPosition() const;
  /// Return the (global) time of the point
  G4double GetTime() const;

private:
  G4ThreeVector _position;
  G4double _time;
};

extern G4TRACKING_DLL G4ThreadLocal 
  G4Allocator<TrajectoryPoint>* TrajectoryPointAllocator;

inline void* TrajectoryPoint::operator new(size_t)
{ if (!TrajectoryPointAllocator)
    TrajectoryPointAllocator = new G4Allocator<TrajectoryPoint>;
  return (void*) TrajectoryPointAllocator->MallocSingle(); }

inline void TrajectoryPoint::operator delete(void* tp)
{ TrajectoryPointAllocator->FreeSingle((TrajectoryPoint*) tp); }

#endif
