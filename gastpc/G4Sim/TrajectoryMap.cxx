// -------------------------------------------------------------------
/// \file   TrajectoryMap.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "TrajectoryMap.h"

#include <G4VTrajectory.hh>


std::map<int, G4VTrajectory*> TrajectoryMap::_map;


TrajectoryMap::TrajectoryMap()
{
}


TrajectoryMap::~TrajectoryMap()
{
  _map.clear();
}


void TrajectoryMap::Clear()
{
  _map.clear();
}


G4VTrajectory* TrajectoryMap::Get(int trackId)
{
  std::map<int, G4VTrajectory*>::iterator it = _map.find(trackId);
  if (it == _map.end()) return 0;
  else return it->second;
}


void TrajectoryMap::Add(G4VTrajectory* trj)
{
  _map[trj->GetTrackID()] = trj;
}
