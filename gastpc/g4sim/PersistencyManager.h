// -------------------------------------------------------------------
///  \file   PersistencyManager.h
///  \brief  
///
///  \author  <justo.martin-albo \at physics.ox.ac.uk>
///  \date    Created 14 Mar 2016
// -------------------------------------------------------------------

#ifndef PERSISTENCY_MANAGER_H
#define PERSISTENCY_MANAGER_H

#include <G4VPersistencyManager.hh>
#include <G4String.hh>
#include <map>

namespace gastpc { class RootFileWriter; }
namespace gastpc { class EventRecord; }
namespace gastpc { class MCParticle; }
namespace gastpc { class MCTrack; }

class Trajectory;
class EventGenerationInfo;

class G4GenericMessenger;
class G4VPhysicalVolume;
class G4Event;
class G4Run;
class G4TrajectoryContainer;
class G4HCofThisEvent;
class G4VHitsCollection;



/// TODO: Class description

class PersistencyManager: public G4VPersistencyManager
{
public:
  /// Create the singleton instance of the persistency manager
  static void Initialize();

  /// Should the current event be stored?
  void StoreCurrentEvent(G4bool);

  /// Open output file.
  /// (This method can be invoked via user macro command.)
  void OpenFile(G4String);

  /// Close the output file
  void CloseFile();

private:
  /// Constructor (private, singleton class)
  PersistencyManager();
  /// Destructor
  ~PersistencyManager();  

  ///
  virtual G4bool Store(const G4Event*);

  /// Process and store the trajectories generated in this event
  void ProcessTrajectories(G4TrajectoryContainer*);
  /// Store a given trajectory in the persistent event record
  void StoreTrajectory(Trajectory*);

  /// Process and store the primary generator information of the event
  void ProcessEventGenerationInfo(const G4Event*);

  /// Lorem ipsum dolor sit amet, consectetur adipiscing elit
  void ProcessDetectorHits(G4HCofThisEvent*);
  /// Lorem ipsum dolor sit amet, consectetur adipiscing elit
  void StoreTrackingHits(G4VHitsCollection*);

private:
  virtual G4bool Store(const G4Run*);
  virtual G4bool Store(const G4VPhysicalVolume*);
  virtual G4bool Retrieve(G4Event*&);
  virtual G4bool Retrieve(G4Run*&);
  virtual G4bool Retrieve(G4VPhysicalVolume*&);

private:
  G4GenericMessenger* msg_; ///< Pointer to the user-command messenger

  gastpc::RootFileWriter* writer_; ///< Pointer to the ROOT file writer
  gastpc::EventRecord* evtrec_;    ///< Pointer to the persistent event record

  G4bool store_current_event_;

  G4int depth_output_family_tree_;

  std::map<G4int, gastpc::MCParticle*> mcparticles_map_;
};

// Inline definitions //////////////////////////////////////

inline void PersistencyManager::StoreCurrentEvent(G4bool b) 
{ store_current_event_ = b; }

inline G4bool PersistencyManager::Store(const G4Run*) 
{ return false; }
inline G4bool PersistencyManager::Store(const G4VPhysicalVolume*)
{ return true; }
inline G4bool PersistencyManager::Retrieve(G4Run*&) 
{ return false; }
inline G4bool PersistencyManager::Retrieve(G4Event*&) 
{ return false; }
inline G4bool PersistencyManager::Retrieve(G4VPhysicalVolume*&) 
{ return false; }

#endif
