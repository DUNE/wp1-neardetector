// -------------------------------------------------------------------
/// \file   TrajectoryMap.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "TrajectoryMap.h"

#include <G4VTrajectory.hh>


std::map<int, G4VTrajectory*> TrajectoryMap::map_;


TrajectoryMap::TrajectoryMap()
{
}


TrajectoryMap::~TrajectoryMap()
{
  map_.clear();
}


void TrajectoryMap::Clear()
{
  map_.clear();
}


G4VTrajectory* TrajectoryMap::Get(int trackId)
{
  std::map<int, G4VTrajectory*>::iterator it = map_.find(trackId);
  if (it == map_.end()) return 0;
  else return it->second;
}


void TrajectoryMap::Add(G4VTrajectory* trj)
{
  map_[trj->GetTrackID()] = trj;
}
