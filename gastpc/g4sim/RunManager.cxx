// -------------------------------------------------------------------
/// \file   RunManager.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 1 June 2016
// -------------------------------------------------------------------

#include "RunManager.h"

#include "DuneGArNDDetConstr.h"
#include "BeamSpillGenerator.h"
#include "ParticleGunGenerator.h"
#include "PersistencyManager.h"
#include "DefaultRunAction.h"
#include "DefaultTrackingAction.h"
#include "DefaultSteppingAction.h"

#include <G4UImanager.hh>
#include <G4VModularPhysicsList.hh>
#include <QGSP_BERT.hh>
#include <G4StepLimiterPhysics.hh>



RunManager::RunManager(const std::string& detector_tag,
                       const std::string& generator_tag): 
  G4RunManager()
{
  G4VModularPhysicsList* physlist = new QGSP_BERT();
  physlist->RegisterPhysics(new G4StepLimiterPhysics());
  this->SetUserInitialization(physlist);

  this->SetUserInitialization(CreateDetectorConstruction(detector_tag));

  this->SetUserAction(CreatePrimaryGenerator(generator_tag));

  this->SetUserAction(new DefaultRunAction());
  this->SetUserAction(new DefaultTrackingAction());
  this->SetUserAction(new DefaultSteppingAction());

  PersistencyManager::Initialize();
}


RunManager::~RunManager()
{}


void RunManager::Initialize()
{
  G4RunManager::Initialize();
}


void RunManager::ExecuteMacroFile(const G4String& filename)
{
  G4UImanager* UI = G4UImanager::GetUIpointer();
  UI->ExecuteMacroFile(filename);
}


void RunManager::SetRandomSeed(G4int seed) const
{
  if (seed < 0) CLHEP::HepRandom::setTheSeed(time(0));
  else CLHEP::HepRandom::setTheSeed(seed);
}


G4VUserDetectorConstruction* 
  RunManager::CreateDetectorConstruction(const std::string& /*tag*/)
{
  // if      (tag == "") return (new );
  // else if (tag == "") return (new );
  return (new DuneGArNDDetConstr());
}


G4VUserPrimaryGeneratorAction*
  RunManager::CreatePrimaryGenerator(const std::string& tag)
{
  if      (tag == "BEAM_SPILL") return (new BeamSpillGenerator());
  else if (tag == "PARTICLE_GUN") return (new ParticleGunGenerator());
  else {
    G4String msg = "User provided an unknown geometry tag: " + tag;
    G4Exception("CreatePrimaryGenerator()", "RunManager", FatalException, msg);
    return 0;
  }
}

