// -------------------------------------------------------------------
/// \file   DefaultEventAction.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 Aug 2016
// -------------------------------------------------------------------

#include "DefaultEventAction.h"

#include "Trajectory.h"

#include <G4Event.hh>
#include <G4TrajectoryContainer.hh>
#include <G4VVisManager.hh>


DefaultEventAction::DefaultEventAction()
{}


DefaultEventAction::~DefaultEventAction()
{}


void DefaultEventAction::BeginOfEventAction(const G4Event*)
{}


void DefaultEventAction::EndOfEventAction(const G4Event* event)
{
  G4TrajectoryContainer* trjc = event->GetTrajectoryContainer();
  if (!trjc) return;

  for (size_t i=0; i<trjc->size(); ++i) {
    Trajectory* trj = dynamic_cast<Trajectory*>((*trjc)[i]);
    if (G4VVisManager::GetConcreteInstance()) trj->DrawTrajectory();
  }
}