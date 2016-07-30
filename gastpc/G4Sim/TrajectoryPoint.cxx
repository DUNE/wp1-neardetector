// -------------------------------------------------------------------
/// \file   TrajectoryPoint.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "TrajectoryPoint.h"


G4ThreadLocal G4Allocator<TrajectoryPoint>* TrajectoryPointAllocator = 0;


TrajectoryPoint::TrajectoryPoint(): 
  _position(0.,0.,0.), _time(0.)
{
}



TrajectoryPoint::TrajectoryPoint(G4ThreeVector pos, G4double t):
  _position(pos), _time(t)
{
}



TrajectoryPoint::TrajectoryPoint(const TrajectoryPoint& other)
{
  *this = other;
}



const TrajectoryPoint& TrajectoryPoint::operator=(const TrajectoryPoint& other)
{
  _position = other._position;
  _time     = other._time;

  return *this;
}


TrajectoryPoint::~TrajectoryPoint()
{
}


const G4ThreeVector TrajectoryPoint::GetPosition() const
{ 
  return _position; 
}


G4double TrajectoryPoint::GetTime() const 
{ 
  return _time; 
}
