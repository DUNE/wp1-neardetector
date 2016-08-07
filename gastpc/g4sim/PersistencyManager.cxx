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
#include "TrackingHit.h"
#include "TrackingSD.h"
#include "EventGenerationInfo.h"
#include "PrimaryParticleInfo.h"

#include "RootFileWriter.h"
#include "EventRecord.h"
#include "NuInteraction.h"
#include "MCParticle.h"
#include "MCTrack.h"
#include "MCHit.h"

#include <Ntuple/NtpMCEventRecord.h>

#include <G4GenericMessenger.hh>
#include <G4Event.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4TrajectoryContainer.hh>
#include <G4ParticleDefinition.hh>
#include <G4SDManager.hh>
#include <G4HCofThisEvent.hh>
#include <G4VHitsCollection.hh>



PersistencyManager::PersistencyManager(): 
  G4VPersistencyManager(), msg_(0), writer_(0)
{
  msg_ = new G4GenericMessenger(this, "/gastpc/persistency/");
  msg_->DeclareMethod("output_file", &PersistencyManager::OpenFile, "");

}


PersistencyManager::~PersistencyManager()
{
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

  G4cout << "Here" << G4endl;

  ProcessTrajectories(event->GetTrajectoryContainer(), evtrec);
  ProcessPrimaryGenerationInfo(event, evtrec);
  ProcessDetectorHits(event->GetHCofThisEvent(), evtrec);

  G4cout << "Here" << G4endl;

  writer_->Write(evtrec);

  G4cout << "Here" << G4endl;

  // Get ready for the following event
  TrajectoryMap::Clear();
  mcparticles_map_.clear();
  mctracks_map_.clear();

  return true;
}


void PersistencyManager::ProcessPrimaryGenerationInfo(const G4Event* event,
                                                      gastpc::EventRecord& er)
{
  EventGenerationInfo* eg_info =
    dynamic_cast<EventGenerationInfo*>(event->GetUserInformation());

  std::vector<gastpc::NuInteraction*> nuints;
  G4int mcid = -1;

  for (genie::NtpMCEventRecord* gmcrec: eg_info->GetEntries()) {

    ++mcid;

    gastpc::NuInteraction* nuint = new gastpc::NuInteraction();
    nuint->SetGenieRecord(gmcrec);
    nuint->SetMCID(mcid);
    er.Add(nuint);
    nuints.push_back(nuint);
  }

  for (G4int i=0; i<event->GetNumberOfPrimaryVertex(); ++i) {

    G4PrimaryVertex* vertex = event->GetPrimaryVertex(i);

    for (G4int j=0; j<vertex->GetNumberOfParticle(); ++j) {

      G4PrimaryParticle* particle = vertex->GetPrimary(j);
      auto result = mcparticles_map_.find(particle->GetTrackID());
      if (result != mcparticles_map_.end()) {
        PrimaryParticleInfo* pp_info =
          dynamic_cast<PrimaryParticleInfo*>(particle->GetUserInformation());
        gastpc::MCParticle* mcp = result->second;
        mcp->SetInteraction(nuints[pp_info->GetInteractionID()]);
        nuints[pp_info->GetInteractionID()]->AddParticle(mcp);
      }
      else {
        // We seem to have found a primary particle that didn't make it
        // to the tracking stage
        G4cerr << "Something went wrong" << G4endl;
        continue;
      }
    }
  }
}


void PersistencyManager::ProcessTrajectories(G4TrajectoryContainer* tc,
                                             gastpc::EventRecord& evtrec)
{
  if (!tc) return; // Make sure the pointer is defined

  // Loop through all the trajectories converting them into MCParticles
  // and storing them in the gastpc event record

  for (G4int i=0; i<tc->entries(); i++) {

    Trajectory* trj = dynamic_cast<Trajectory*>((*tc)[i]);
    if (!trj) continue; // Make sure the pointer is defined

    gastpc::MCParticle* mcpart = new gastpc::MCParticle();

    mcpart->SetPDGCode(trj->GetPDGEncoding());
    mcpart->SetMCID(trj->GetTrackID());

    G4ThreeVector xyz = trj->GetInitialPosition();
    G4double time     = trj->GetInitialTime();
    mcpart->SetInitialXYZT(xyz.x(), xyz.y(), xyz.z(), time);

    xyz  = trj->GetFinalPosition();
    time = trj->GetFinalTime();
    mcpart->SetFinalXYZT(xyz.x(), xyz.y(), xyz.z(), time);

    mcpart->SetInitialVolume(trj->GetInitialVolume());
    mcpart->SetFinalVolume(trj->GetFinalVolume());

    G4double mass = trj->GetParticleDefinition()->GetPDGMass();
    G4ThreeVector mom = trj->GetInitialMomentum();
    G4double energy = sqrt(mom.mag2() + mass*mass);
    mcpart->SetInitial4Momentum(mom.x(), mom.y(), mom.z(), energy);

    mcpart->SetCreatorProcess(trj->GetCreatorProcess());

    // Register the particle in the map to keep track of it
    mcparticles_map_[mcpart->GetMCID()] = mcpart;

    // Add particle to the event record
    evtrec.Add(mcpart);
  }

  // We will set now the family relations between particles.
  // Loop through all the particles we have stored above in the record.

  for (gastpc::MCParticle* mcpart: evtrec.GetMCParticles()) {
    Trajectory* trj = (Trajectory*) TrajectoryMap::Get(mcpart->GetMCID());
    int mother_id = trj->GetParentID();
    if (mother_id == 0) {
      mcpart->SetPrimary(true);
    }
    else {
      gastpc::MCParticle* mother = mcparticles_map_[mother_id];
      mcpart->SetPrimary(false);
      mcpart->SetMother(mother);
      mother->AddDaughter(mcpart);
    }
  }
}


void PersistencyManager::ProcessDetectorHits(G4HCofThisEvent* hce,
                                             gastpc::EventRecord& evtrec)
{
  if (!hce) return; // Make sure the pointer is defined

  G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
  G4HCtable* hct = sdmgr->GetHCtable();

  for (G4int i=0; i<hct->entries(); ++i) {
    // Hit collections are identified univocally (in principle) by
    // their ID number, and this can be obtained using the collection
    // and sensitive detector names.
    G4String hcname = hct->GetHCname(i);
    G4String sdname = hct->GetSDname(i);
    int hcid = sdmgr->GetCollectionID(sdname + "/" + hcname);

    // Fetch collection using the above id number
    G4VHitsCollection* hits = hce->GetHC(hcid);

    if (hcname == TrackingSD::HitCollectionUniqueName()) 
      ProcessTrackingHits(hits, evtrec);
    else
      G4Exception("PersistencyManager::StoreDetectorHits()", "WARNING",
        JustWarning, "Unknown collection of hits will not be stored."); 
  }
}


void PersistencyManager::ProcessTrackingHits(G4VHitsCollection* hc,
                                             gastpc::EventRecord& evtrec)
{
  TrackingHitsCollection* hits = dynamic_cast<TrackingHitsCollection*>(hc);
  if (!hits) return;

  for (G4int i=0; i<hits->entries(); ++i) {

    TrackingHit* hit = dynamic_cast<TrackingHit*>(hits->GetHit(i));
    if (!hit) continue;

    gastpc::MCHit* mchit = new gastpc::MCHit();
    G4LorentzVector xyzt = hit->GetPositionAndTime();
    mchit->SetXYZT(xyzt.x(), xyzt.y(), xyzt.z(), xyzt.t());
    mchit->SetAmplitude(hit->GetEdep());

    // Let's find out which track is the owner of this hit
    gastpc::MCTrack* mctrack = 0;
    G4int trackid = hit->GetTrackID();
    auto result = mctracks_map_.find(trackid);

    if (result == mctracks_map_.end()) {
      // We have not seen this track before; 
      // let's create a new MCTrack then
      mctrack = new gastpc::MCTrack();
      mctrack->SetLabel(hits->GetSDname());
      mctrack->SetParticle(mcparticles_map_[trackid]);
      mctracks_map_[trackid] = mctrack;
      evtrec.Add(mctrack);
    }
    else {
      // We created the track already for a previous hit
      mctrack = result->second;
    }

    // Add hit to the track
    mctrack->Add(mchit);
  }
}


G4bool PersistencyManager::Store(const G4VPhysicalVolume*)
{
  return true;
}

