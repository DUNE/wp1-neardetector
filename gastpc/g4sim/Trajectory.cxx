// -------------------------------------------------------------------
/// \file   Trajectory.cxx
/// \brief  Transient record of a Geant4 particle/track.
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "Trajectory.h"

#include "TrajectoryPoint.h"
#include "TrajectoryMap.h"

#include <G4Track.hh>
#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>


G4ThreadLocal G4Allocator<Trajectory>* TrajectoryAllocator = 0;


Trajectory::Trajectory(const G4Track* track): 
  G4VTrajectory(), 
  pdef_(track->GetDefinition()), 
  track_id_(track->GetTrackID()), 
  mother_id_(track->GetParentID()),
  initial_momentum_(track->GetMomentum()),
  initial_position_(track->GetVertexPosition()),
  final_position_(G4ThreeVector()),
  initial_time_(track->GetGlobalTime()),
  final_time_(0.),
  creator_process_(""),
  initial_volume_(""),
  final_volume_(""),
  rec_trjpoints_(true),
  trjpoints_(0)
{
  // Set creator process
  if (mother_id_ == 0) creator_process_ = "none";
  else creator_process_ = track->GetCreatorProcess()->GetProcessName();

  // Set name of volume where particle was created
  initial_volume_ = track->GetVolume()->GetName();

  if (rec_trjpoints_) trjpoints_ = new TrajectoryPointContainer();

  // Add this trajectory in the map, but only if no other
  // trajectory for this track id has been registered yet
  if (!TrajectoryMap::Get(track->GetTrackID())) TrajectoryMap::Add(this);
}


Trajectory::Trajectory(const Trajectory& other): G4VTrajectory()
{
  pdef_ = other.pdef_;
}


Trajectory::~Trajectory()
{
  if (trjpoints_) {
    for (unsigned int i=0; i<trjpoints_->size(); ++i) delete (*trjpoints_)[i];
    trjpoints_->clear();
    delete trjpoints_;
  }
}


G4String Trajectory::GetParticleName() const
{
  return pdef_->GetParticleName();
}


G4int Trajectory::GetPDGEncoding() const
{
  return pdef_->GetPDGEncoding();
}


G4double Trajectory::GetCharge() const
{
  return pdef_->GetPDGCharge();
}


void Trajectory::AppendStep(const G4Step* step)
{
  if (!rec_trjpoints_) return;

  TrajectoryPoint* point = 
    new TrajectoryPoint(step->GetPostStepPoint()->GetPosition(),
                        step->GetPostStepPoint()->GetGlobalTime());
  trjpoints_->push_back(point);
}


void Trajectory::MergeTrajectory(G4VTrajectory* second)
{
  if (!second) return;

  if (!rec_trjpoints_) return;

  Trajectory* tmp = (Trajectory*) second;
  G4int entries = tmp->GetPointEntries();

  // initial point of the second trajectory should not be merged
  for (G4int i=1; i<entries ; ++i) { 
    trjpoints_->push_back((*(tmp->trjpoints_))[i]);
  }

  delete (*tmp->trjpoints_)[0];
  tmp->trjpoints_->clear();
}


void Trajectory::ShowTrajectory(std::ostream& os) const
{
  // Invoke the default implementation
  G4VTrajectory::ShowTrajectory(os);
}


void Trajectory::DrawTrajectory() const
{
  // Invoke the default implementation
  G4VTrajectory::DrawTrajectory();
}
