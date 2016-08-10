// -------------------------------------------------------------------
/// \file   RunManager.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 1 June 2016
// -------------------------------------------------------------------

#include "RunManager.h"

#include "PersistencyManager.h"
#include "DuneGArNDDetConstr.h"
#include "BeamSpillGenerator.h"
#include "ParticleGunGenerator.h"
#include "DefaultRunAction.h"
#include "DefaultEventAction.h"
#include "DefaultTrackingAction.h"
#include "DefaultSteppingAction.h"
#include "UIBatch.h"

#include <globals.hh>
#include <G4SystemOfUnits.hh>
#include <G4GenericMessenger.hh>
#include <G4UImanager.hh>
#include <G4VModularPhysicsList.hh>
#include <QGSP_BERT.hh>
#include <G4StepLimiterPhysics.hh>
#include <FTFP_BERT.hh>



RunManager::RunManager(const std::string& detector_tag,
                       const std::string& generator_tag): 
  G4RunManager(), msg_(0)
{
  DefineCommands();

  G4VModularPhysicsList* physlist = new QGSP_BERT();
  physlist->RegisterPhysics(new G4StepLimiterPhysics());
  physlist->SetDefaultCutValue(50.*mm);

  physlist->DumpCutValuesTable();
  this->SetUserInitialization(physlist);

  this->SetUserInitialization(CreateDetectorConstruction(detector_tag));

  this->SetUserAction(CreatePrimaryGenerator(generator_tag));

  this->SetUserAction(new DefaultRunAction());
  this->SetUserAction(new DefaultEventAction());
  this->SetUserAction(new DefaultTrackingAction());
  this->SetUserAction(new DefaultSteppingAction());

  PersistencyManager::Initialize();
}


RunManager::~RunManager()
{
  PersistencyManager* pm = dynamic_cast<PersistencyManager*>
    (G4VPersistencyManager::GetPersistencyManager());
  if (pm) pm->CloseFile();

  delete msg_;
}


void RunManager::Initialize()
{
  G4RunManager::Initialize();
}


void RunManager::ExecuteMacroFile(const G4String& filename)
{
  G4UImanager* UI = G4UImanager::GetUIpointer();
  G4UIsession* batch_session = new UIBatch(filename.data(), UI->GetSession());
  UI->SetSession(batch_session);
  G4UIsession* prev_session = UI->GetSession()->SessionStart();
  delete (UI->GetSession());
  UI->SetSession(prev_session);
}


// void RunManager::ExecuteMacroFile(const G4String& filename)
// {
//   G4UImanager* UI = G4UImanager::GetUIpointer();
//   UI->ExecuteMacroFile(filename);
// }


void RunManager::SetRandomSeed(G4int seed)
{
  if (seed < 0) CLHEP::HepRandom::setTheSeed(time(0));
  else CLHEP::HepRandom::setTheSeed(seed);
  CLHEP::HepRandom::showEngineStatus()
  ;
}


G4VUserDetectorConstruction* 
  RunManager::CreateDetectorConstruction(const std::string& tag)
{
  if (tag == "DUNE") return (new DuneGArNDDetConstr());
  else {
    G4String error_msg =  "Unknown detector construction class: " + tag;
    G4Exception("RunManager::CreateDetectorConstruction()", "ERROR",
      FatalException, error_msg);
    return 0;
  }
}


G4VUserPrimaryGeneratorAction*
  RunManager::CreatePrimaryGenerator(const std::string& tag)
{
  if      (tag == "BEAM_SPILL") return (new BeamSpillGenerator());
  else if (tag == "PARTICLE_GUN") return (new ParticleGunGenerator());
  else {
    G4String error_msg = "Unknown primary generator class: " + tag;
    G4Exception("RunManager::CreatePrimaryGenerator()", "ERROR", 
      FatalException, error_msg);
    return 0;
  }
}


void RunManager::DefineCommands()
{
  msg_ = new G4GenericMessenger(this, "/gastpc/manager/");
  msg_->DeclareMethod("random_seed", &RunManager::SetRandomSeed, "");
}


