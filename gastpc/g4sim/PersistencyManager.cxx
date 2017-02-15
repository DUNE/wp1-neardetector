// -------------------------------------------------------------------
///  \file   PersistencyManager.cxx
///  \brief  Wrapper class that contains all used GHEP records.
///
///  \author  <justo.martin-albo \at physics.ox.ac.uk>
///  \date    Created 14 Mar 2016
// -------------------------------------------------------------------

#include "PersistencyManager.h"

#include "Trajectory.h"
#include "TrajectoryRegister.h"
#include "TrackingHit.h"
#include "TrackingSD.h"
#include "EventGenerationInfo.h"
#include "PrimaryParticleInfo.h"

#include "RootFileWriter.h"
#include "EventRecord.h"
#include "MCGenInfo.h"
#include "MCParticle.h"
#include "MCTrack.h"
#include "MCHit.h"

#include <Ntuple/NtpMCEventRecord.h>

#include <Randomize.hh>
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
  G4VPersistencyManager(), msg_(0), writer_(0), evtrec_(0),
  store_current_event_(true), depth_output_family_tree_(3)
{
  msg_ = new G4GenericMessenger(this, "/gastpc/persistency/");
  msg_->DeclareMethod("output_file", &PersistencyManager::OpenFile, "");
  msg_->DeclareProperty("depth_output_family_tree_", depth_output_family_tree_, "");

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

  // Create a new event record and set basic properties
  evtrec_ = new gastpc::EventRecord();
  evtrec_->SetEventID(event->GetEventID());
  evtrec_->SetRunID(CLHEP::HepRandom::getTheSeed());

  ProcessTrajectories(event->GetTrajectoryContainer());
  ProcessEventGenerationInfo(event);
  ProcessDetectorHits(event->GetHCofThisEvent());

  // We will set now the family relations between particles.
  // Loop through all the particles we have stored above in the record.
  for (gastpc::MCParticle* mcp: evtrec_->GetMCParticles()) {
    Trajectory* trj = (Trajectory*) TrajectoryRegister::Get(mcp->GetMCID());

    mcp->SetFamilyTreeLevel(trj->GetFamilyTreeLevel());

    // Find and set parent particle
    auto result = mcparticles_map_.find(trj->GetParentID());
    if (result != mcparticles_map_.end()) {
      gastpc::MCParticle* parent = result->second;
      mcp->SetParent(parent);
      parent->AddDaughter(mcp);
    }

    // Find and set ancestor particle
    result = mcparticles_map_.find(trj->GetAncestorID());
    if (result != mcparticles_map_.end()) {
      mcp->SetAncestor(result->second);
    }
  }

  writer_->Write(*evtrec_);

  // Get ready for the following event
  TrajectoryRegister::Clear();
  mcparticles_map_.clear();

  delete evtrec_;

  return true;
}


void PersistencyManager::ProcessTrajectories(G4TrajectoryContainer* tc)
{
  if (!tc) return; // Make sure the pointer is defined

  // Loop through all the trajectories stored in this event
  for(G4int i=0; i<tc->entries(); ++i) {

    Trajectory* trj = dynamic_cast<Trajectory*>((*tc)[i]);
    if (!trj) continue; // Make sure the pointer is defined

    if (trj->GetFamilyTreeLevel() <= depth_output_family_tree_)
      StoreTrajectory(trj);
  }
}


void PersistencyManager::StoreTrajectory(Trajectory* trj)
{
  // Create the MCParticle and set its properties
  gastpc::MCParticle* mcp = new gastpc::MCParticle();
  mcp->SetPDGCode(trj->GetPDGEncoding());
  mcp->SetMCID(trj->GetTrackID());

  mcp->SetInitialXYZT(trj->GetInitialPosition().x(),
                      trj->GetInitialPosition().y(),
                      trj->GetInitialPosition().z(),
                      trj->GetInitialTime());

  mcp->SetFinalXYZT(trj->GetFinalPosition().x(),
                    trj->GetFinalPosition().y(),
                    trj->GetFinalPosition().z(),
                    trj->GetFinalTime());

  mcp->SetInitialMomentum(trj->GetInitialMomentum().x(),
                          trj->GetInitialMomentum().y(),
                          trj->GetInitialMomentum().z());

  // Add MCParticle to the event record
  evtrec_->Add(mcp);

  // Register the particle in the map to keep track of it
  mcparticles_map_[mcp->GetMCID()] = mcp;
}


void PersistencyManager::ProcessEventGenerationInfo(const G4Event* event)
{
  EventGenerationInfo* egi =
    dynamic_cast<EventGenerationInfo*>(event->GetUserInformation());

  if (!egi) return; // Make sure the pointer is defined

  G4int mcid = 0;

  for (genie::NtpMCEventRecord* gmcrec: egi->GetEntries()) {
    gastpc::MCGenInfo* mcgi = new gastpc::MCGenInfo();
    mcgi->SetGeneratorRecord(gmcrec);
    mcgi->SetMCID(mcid);
    evtrec_->Add(mcgi);
    ++mcid;
  }

  // Remove entries from event generation info container to avoid
  // memory-deallocation problems at destruction
  egi->DropEntries();

  // Associate primary particles to the mc generation info entries
  for (G4int i=0; i<event->GetNumberOfPrimaryVertex(); ++i) {
    G4PrimaryVertex* vertex = event->GetPrimaryVertex(i);

    for (G4int j=0; j<vertex->GetNumberOfParticle(); ++j) {
      G4PrimaryParticle* particle = vertex->GetPrimary(j);

      auto result = mcparticles_map_.find(particle->GetTrackID());

      if (result != mcparticles_map_.end()) {
        gastpc::MCParticle* mcp = result->second;
        const std::vector<gastpc::MCGenInfo*>& vmcgi = evtrec_->GetMCGenInfo();
        PrimaryParticleInfo* pp_info =
          dynamic_cast<PrimaryParticleInfo*>(particle->GetUserInformation());
        mcp->SetMCGenInfo(vmcgi[pp_info->GetInteractionID()]);
        vmcgi[pp_info->GetInteractionID()]->AddMCParticle(mcp);
      }
      else {
        // We seem to have found a primary particle that didn't make it
        // to the tracking stage
        G4Exception("PersistencyManager::ProcessPrimaryGenerationInfo()",
          "WARNING", JustWarning, "Found a track without ID.");
        continue;
      }
    }
  }
}


void PersistencyManager::ProcessDetectorHits(G4HCofThisEvent* hce)
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
      StoreTrackingHits(hits);
    else
      G4Exception("PersistencyManager::StoreDetectorHits()", "WARNING",
        JustWarning, "Unknown collection of hits will not be stored.");
  }
}


void PersistencyManager::StoreTrackingHits(G4VHitsCollection* hc)
{
  TrackingHitsCollection* hits = dynamic_cast<TrackingHitsCollection*>(hc);
  if (!hits) return; // Make sure the pointer is defined

  // We'll use a map to keep record of the mc tracks we create
  // indexing them by their track id
  std::map<G4int, gastpc::MCTrack*> mctracks_map;

  for (G4int i=0; i<hits->entries(); ++i) {

    TrackingHit* hit = dynamic_cast<TrackingHit*>(hits->GetHit(i));
    if (!hit) continue; // Make sure the pointer is defined

    gastpc::MCHit* mchit = new gastpc::MCHit();
    G4LorentzVector xyzt = hit->GetPositionAndTime();
    mchit->SetXYZT(xyzt.x(), xyzt.y(), xyzt.z(), xyzt.t());
    mchit->SetAmplitude(hit->GetEdep());

    // Let's find out which track is the owner of this hit
    gastpc::MCTrack* mctrack = 0;
    auto result = mctracks_map.find(hit->GetTrackID());

    if (result == mctracks_map.end()) {
      // We have not seen this track before.
      // Let's then create a new MCTrack and set its properties.
      mctrack = new gastpc::MCTrack();
      mctrack->SetLabel(hits->GetSDname());

      // Find the particle associated to this track and set
      // the bi-directional relationship pointers
      gastpc::MCParticle* mcp = 0;
      auto result2 = mcparticles_map_.find(hit->GetTrackID());
      if (result2 == mcparticles_map_.end()) {
        Trajectory* trj = dynamic_cast<Trajectory*>
          (TrajectoryRegister::Get(hit->GetTrackID()));
        if (!trj) continue; // Make sure the pointer is defined
        StoreTrajectory(trj);
        mcp = mcparticles_map_[hit->GetTrackID()];
      }
      else {
        mcp = result2->second;
      }

      mctrack->SetMCParticle(mcp);
      mcp->AddMCTrack(mctrack);

      // Add track to map and to persistent event record
      mctracks_map[hit->GetTrackID()] = mctrack;
      evtrec_->Add(mctrack);
    }
    else {
      // The track was created for a previous hit
      mctrack = result->second;
    }

    // Add hit to the track
    mctrack->Add(mchit);
  }
}
