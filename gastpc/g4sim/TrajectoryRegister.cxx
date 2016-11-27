// -------------------------------------------------------------------
/// \file   TrajectoryRegister.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "TrajectoryRegister.h"

#include <G4VTrajectory.hh>


std::map<int, G4VTrajectory*> TrajectoryRegister::map_;


TrajectoryRegister::TrajectoryRegister()
{
}


TrajectoryRegister::~TrajectoryRegister()
{
  map_.clear();
}


void TrajectoryRegister::Clear()
{
  map_.clear();
}


G4VTrajectory* TrajectoryRegister::Get(int track_id)
{
  std::map<int, G4VTrajectory*>::iterator it = map_.find(track_id);
  if (it == map_.end()) return 0;
  else return it->second;
}


void TrajectoryRegister::Add(G4VTrajectory* trj)
{
  map_[trj->GetTrackID()] = trj;
}
