// -------------------------------------------------------------------
///  \file   PersistencyManager.cxx
///  \brief
///
///  \author  <justo.martin-albo \at physics.ox.ac.uk>
///  \date    Created 14 Mar 2016
// -------------------------------------------------------------------

#include "PersistencyManager.h"

#include "RootFileWriter.h"

#include <G4GenericMessenger.hh>



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
    writer_->OpenFile(filename.data());
  }
  else {
    G4Exception("OpenFile()", "PersistencyManager", 
      JustWarning, "WARNING: Output file already open.");
  }
}


void PersistencyManager::CloseFile()
{
  if (!writer_) return;
  writer_->CloseFile();
}


G4bool PersistencyManager::Store(const G4Event*)
{
  return true;
}


G4bool PersistencyManager::Store(const G4Run*)
{
  return true;
}


G4bool PersistencyManager::Store(const G4VPhysicalVolume*)
{
  return true;
}

