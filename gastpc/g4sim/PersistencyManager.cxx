// -------------------------------------------------------------------
///  \file   PersistencyManager.cxx
///  \brief
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
  G4VPersistencyManager(),
  msg_(0), writer_(0), evtrec_(0),
  store_current_event_(true),
  depth_output_family_tree_(3)
{
  msg_ = new G4GenericMessenger(this, "/gastpc/persistency/");
  msg_->DeclareMethod("output_file", &PersistencyManager::OpenFile, "");
  msg_->DeclareProperty("depth_output_family_tree", depth_output_family_tree_, "");
}


PersistencyManager::~PersistencyManager()
{
  delete msg_;
  delete writer_;
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

  // Create a new event record and set its basic properties
  evtrec_ = new gastpc::EventRecord();
  evtrec_->SetRunID(G4Random::getTheSeed());
  evtrec_->SetEventID(event->GetEventID());

  // Process now the different collections of objects that were recorded
  // for this event during the tracking
  ProcessTrajectories(event->GetTrajectoryContainer());
  ProcessEventGenerationInfo(event->GetUserInformation());
  ProcessDetectorHits(event->GetHCofThisEvent());

  // We will set now the bi-directional links between some of the objects
  // created above (e.g. family relations between particles).
  // We'll start assigning primary particles to the corresponding MC event
  // generator info.

  // Loop through all the primary vertices and then through all primary
  // particles that were generated in this event.
  // (Skip this step if there is no MCGenInfo in the event.)

  if (!mcgeninfos_map_.empty()) {

    for (G4int i=0; i<event->GetNumberOfPrimaryVertex(); ++i) {
      G4PrimaryVertex* vertex = event->GetPrimaryVertex(i);
      for (G4int j=0; j<vertex->GetNumberOfParticle(); ++j) {
        G4PrimaryParticle* particle = vertex->GetPrimary(j);

        // We'll try to locate now the persistent MCParticle object
        // corresponding to this primary particle
        G4int mcid = particle->GetTrackID();
        auto result = mcparticles_map_.find(mcid);

        if (result != mcparticles_map_.end()) {
          // We've got a match!
          // Link now the MCParticle and its associated MCGenInfo

          gastpc::MCParticle* mcp = result->second;

          PrimaryParticleInfo* pp_info =
            dynamic_cast<PrimaryParticleInfo*>(particle->GetUserInformation());
          if (!pp_info) continue; // make sure the pointer is valid

          gastpc::MCGenInfo* mcgi =
            mcgeninfos_map_[pp_info->GetEventGenerationID()];

          mcp->SetMCGenInfo(mcgi);
          mcgi->AddMCParticle(mcp);
        }
        else {
          // We seem to have found a primary particle without MC ID
          G4Exception("PersistencyManager::ProcessPrimaryGenerationInfo()",
            "WARNING", JustWarning, "Found a track without ID.");
          continue;
        }
      }
    }
  }

  // We'll set now the links between MCParticles

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

  // Write the current event and ready for the next one
  writer_->Write(*evtrec_);

  mcparticles_map_.clear();
  mcgeninfos_map_.clear();
  TrajectoryRegister::Clear();
  delete evtrec_;

  return true;
}


void PersistencyManager::ProcessTrajectories(G4TrajectoryContainer* tc)
{
  if (!tc) return; // Make sure the pointer is defined

  for(G4int i=0; i<tc->entries(); ++i) {
    // Fetch the pointer to the current trajectory and make sure it is valid
    Trajectory* trj = dynamic_cast<Trajectory*>((*tc)[i]);
    if (!trj) continue;
    // Store this trajectory only if its above the threshold set
    // by the user for the otput family tree
    if (trj->GetFamilyTreeLevel() <= depth_output_family_tree_)
      StoreTrajectory(trj);
  }
}


void PersistencyManager::StoreTrajectory(Trajectory* trj)
{
  // Create a new particle and set its properties
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

  // Register the particle in a map to keep track of it
  mcparticles_map_[mcp->GetMCID()] = mcp;
}


void PersistencyManager::ProcessEventGenerationInfo(G4VUserEventInformation* p)
{
  // Make sure the pointer is valid
  EventGenerationInfo* egi = dynamic_cast<EventGenerationInfo*>(p);
  if (!egi) return;

  G4int mcid = 0; // We'll use the entry index as MC ID for the MCGenInfo

  for (genie::NtpMCEventRecord* gmcrec: egi->GetEntries()) {

    // Create a new MCGenInfo object that will store the generator record
    gastpc::MCGenInfo* mcgi = new gastpc::MCGenInfo();
    mcgi->SetGeneratorRecord(gmcrec);
    mcgi->SetMCID(mcid);

    // Add the object to the persistent event record and
    // register it in the map to keep track of it
    evtrec_->Add(mcgi);
    mcgeninfos_map_[mcid] = mcgi;

    ++mcid; // Get ready for next entry
  }

  // Above we've transferred the ownership of the genie event records
  // to the event record. Therefore, we'll clear the entries from the container
  // to avoid memory-deallocation problems at its destruction.
  egi->DropEntries();
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
