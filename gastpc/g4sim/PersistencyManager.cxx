// -------------------------------------------------------------------
///  \file   PersistencyManager.cxx
///  \brief
///
///  \author  <justo.martin-albo \at physics.ox.ac.uk>
///  \date    Created 14 Mar 2016
// -------------------------------------------------------------------

#include "PersistencyManager.h"

#include "Trajectory.h"
#include "TrajectoryMap.h"
#include "RootFileWriter.h"
#include "EventRecord.h"
#include "MCParticle.h"

#include <G4GenericMessenger.hh>
#include <G4Event.hh>
#include <G4TrajectoryContainer.hh>
#include <G4ParticleDefinition.hh>



PersistencyManager::PersistencyManager(): 
  G4VPersistencyManager(), msg_(0), writer_(0)
{
  msg_ = new G4GenericMessenger(this, "/gastpc/persistency/");
  msg_->DeclareMethod("output_file", &PersistencyManager::OpenFile, "");

}


PersistencyManager::~PersistencyManager()
{
  CloseFile();
  delete writer_;
  delete msg_;
}


void PersistencyManager::Initialize()
{
  // Get a pointer to the manager's singleton instance using the method
  // of the base class. If no instance exists yet, create then a new one.
  PersistencyManager* current = dynamic_cast<PersistencyManager*>
    (G4VPersistencyManager::GetPersistencyManager());
  if (!current) current = new PersistencyManager();
}


void PersistencyManager::OpenFile(G4String filename)
{
  if (!writer_) {
    writer_ = new gastpc::RootFileWriter();
    G4bool ok = writer_->OpenFile(filename.data());
    if (!ok)
      G4Exception("PersistencyManager::OpenFile()", "ERROR",
        FatalException, "Failed opening output file.");
  }
  else {
    G4Exception("PersistencyManager::OpenFile()", "WARNING", 
      JustWarning, "Output file already open.");
  }
}


void PersistencyManager::CloseFile()
{
  if (!writer_)
    G4Exception("PersistencyManager::CloseFile()", "WARNING", JustWarning, 
      "No writer available at this time. Cannot close output file.");
  else 
    writer_->CloseFile();
}


G4bool PersistencyManager::Store(const G4Event* event)
{
  if (!store_current_event_) return false;

  gastpc::EventRecord evtrec;

  StoreTrajectories(event->GetTrajectoryContainer(), evtrec);

  writer_->Write(evtrec);

  return true;
}


void PersistencyManager::StoreTrajectories(G4TrajectoryContainer* tc,
                                           gastpc::EventRecord& evtrec)
{
  if (!tc) return; // No trajectories were created in this event

  // Loop through all the trajectories to convert them into MCParticles
  for (G4int i=0; i<tc->entries(); i++) {

    Trajectory* trj = dynamic_cast<Trajectory*>((*tc)[i]);
    if (!trj) continue;

    gastpc::MCParticle* mcpart = new gastpc::MCParticle();
    mcpart->SetPDGCode(trj->GetPDGEncoding());
    //mcpart->SetLabel(trj->GetParticleName());

    mcpart->SetMCID(trj->GetTrackID());

    G4ThreeVector xyz = trj->GetInitialPosition();
    G4double time     = trj->GetInitialTime();
    mcpart->SetInitialXYZT(xyz.x(), xyz.y(), xyz.z(), time);

    xyz  = trj->GetFinalPosition();
    time = trj->GetFinalTime();
    mcpart->SetFinalXYZT(xyz.x(), xyz.y(), xyz.z(), time);

    mcpart->SetInitialVolume(trj->GetInitialVolume());
    mcpart->SetFinalVolume(trj->GetDecayVolume());

    G4double mass = trj->GetParticleDefinition()->GetPDGMass();
    G4ThreeVector mom = trj->GetInitialMomentum();
    G4double energy = sqrt(mom.mag2() + mass*mass);
    mcpart->SetInitial4Momentum(mom.x(), mom.y(), mom.z(), energy);

    mcpart->SetCreatorProcess(trj->GetCreatorProcess());

    evtrec.Add(mcpart);
  }

  // We will set now the family relations between particles.
  // Loop through all the particles we  have stored above in the record.
  for (gastpc::MCParticle* mcpart: evtrec.GetMCParticles()) {
    Trajectory* trj = (Trajectory*) TrajectoryMap::Get(mcpart->GetMCID());
    int mother_id = trj->GetParentID();
    if (mother_id == 0) {
      mcpart->SetPrimary(true);
    }
    else {
      //gastpc::MCParticle* mother = map[mother_id];
      mcpart->SetPrimary(false);
    }
  }

  //   gate::MCParticle* ipart = (gate::MCParticle*) iparts.at(i);
  //   Trajectory* trj = (Trajectory*) TrajectoryMap::Get(ipart->GetID());
  //   int parent_id = trj->GetParentID();
   

  //   if (parent_id == 0) {
  //     ipart->SetPrimary(true);
  //   }
  //   else {
  //     gate::MCParticle* mother = _iprtmap[parent_id];
  //     ipart->SetPrimary(false);
  //     ipart->SetMother(mother);
  //     mother->AddDaughter(ipart);
  //   }
  // }

}


// void PersistencyManager::StoreDetectorHits(G4HCofThisEvent* hce,
//                                            gastpc::EventRecord& evtrec)
// {
//   if (!hce) return;

//   G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
//   G4HCtable* 
// }


G4bool PersistencyManager::Store(const G4VPhysicalVolume*)
{
  return true;
}

