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

namespace gastpc { class RootFileWriter; }
class G4GenericMessenger;
class G4Event;
class G4Run;
class G4VPhysicalVolume;


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
  virtual G4bool Store(const G4Event*);
  virtual G4bool Store(const G4Run*);
  virtual G4bool Store(const G4VPhysicalVolume*);

  virtual G4bool Retrieve(G4Event*&);
  virtual G4bool Retrieve(G4Run*&);
  virtual G4bool Retrieve(G4VPhysicalVolume*&);

  PersistencyManager();
  ~PersistencyManager();  

private:
  G4GenericMessenger* msg_; ///< Pointer to the user-command messenger

  gastpc::RootFileWriter* writer_; ///< Pointer to the ROOT file writer

  G4bool store_current_event_;
};

inline void PersistencyManager::StoreCurrentEvent(G4bool b) 
{ store_current_event_ = b; }
inline G4bool PersistencyManager::Retrieve(G4Event*&) { return false; }
inline G4bool PersistencyManager::Retrieve(G4Run*&) { return false; }
inline G4bool PersistencyManager::Retrieve(G4VPhysicalVolume*&) { return false; }

#endif