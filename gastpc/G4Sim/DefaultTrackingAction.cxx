// -------------------------------------------------------------------
/// \file   DefaultTrackingAction.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "DefaultTrackingAction.h"

#include "Trajectory.h"
#include "TrajectoryMap.h"

#include <G4Track.hh>
#include <G4TrackingManager.hh>


void DefaultTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  G4VTrajectory* trj = new Trajectory(track);
  this->fpTrackingManager->SetStoreTrajectory(true);
  this->fpTrackingManager->SetTrajectory(trj);
}


void DefaultTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  Trajectory* trj = (Trajectory*) TrajectoryMap::Get(track->GetTrackID());

  if (!trj) return;

  // Record final time and position of the track
  trj->SetFinalPosition(track->GetPosition());
  trj->SetFinalTime(track->GetGlobalTime());
  trj->SetTrackLength(track->GetTrackLength());
  trj->SetDecayVolume(track->GetVolume()->GetName());
}
