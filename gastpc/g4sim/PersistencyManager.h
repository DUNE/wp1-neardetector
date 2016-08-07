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
class G4GenericMessenger;
class G4Event;
class G4Run;
class G4VPhysicalVolume;
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
  /// (This method can be invoked from user macro command.)
  void OpenFile(G4String);

  /// Close the output file
  void CloseFile();

private:
  /// Constructor (private, singleton class)
  PersistencyManager();
  /// Destructor
  ~PersistencyManager();  

  ///
  virtual G4bool Store(const G4Run*);
  ///
  virtual G4bool Store(const G4Event*);
  ///
  virtual G4bool Store(const G4VPhysicalVolume*);

  void ProcessTrajectories(G4TrajectoryContainer*, gastpc::EventRecord&);
  void ProcessDetectorHits(G4HCofThisEvent*, gastpc::EventRecord&);
  void ProcessTrackingHits(G4VHitsCollection*, gastpc::EventRecord&);
  void ProcessPrimaryGenerationInfo(const G4Event*, gastpc::EventRecord&);

  virtual G4bool Retrieve(G4Event*&);
  virtual G4bool Retrieve(G4Run*&);
  virtual G4bool Retrieve(G4VPhysicalVolume*&);

private:
  G4GenericMessenger* msg_; ///< Pointer to the user-command messenger

  gastpc::RootFileWriter* writer_; ///< Pointer to the ROOT file writer

  G4bool store_current_event_;

  std::map<G4int, gastpc::MCParticle*> mcparticles_map_;
  std::map<G4int, gastpc::MCTrack*> mctracks_map_;
};

// Inline definitions //////////////////////////////////////

inline void PersistencyManager::StoreCurrentEvent(G4bool b) 
{ store_current_event_ = b; }

inline G4bool PersistencyManager::Store(const G4Run*) 
{ return false; }

inline G4bool PersistencyManager::Retrieve(G4Run*&) 
{ return false; }
inline G4bool PersistencyManager::Retrieve(G4Event*&) 
{ return false; }
inline G4bool PersistencyManager::Retrieve(G4VPhysicalVolume*&) 
{ return false; }

#endif
