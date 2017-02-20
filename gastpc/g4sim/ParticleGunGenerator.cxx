// -------------------------------------------------------------------
/// \file   ParticleGunGenerator.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 29 July 2016
// -------------------------------------------------------------------

#include "ParticleGunGenerator.h"

#include <G4GenericMessenger.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4Event.hh>
#include <Randomize.hh>
#include <G4RandomDirection.hh>
#include <G4MuonMinus.hh>


ParticleGunGenerator::ParticleGunGenerator():
  G4VUserPrimaryGeneratorAction(),
  msg_(0),
  particle_def_(G4MuonMinus::Definition()),
  momentum_range_min_(1.*GeV),
  momentum_range_max_(0.)
{
  DefineCommands();
}


ParticleGunGenerator::~ParticleGunGenerator()
{
  delete msg_;
}


void ParticleGunGenerator::GeneratePrimaries(G4Event* event)
{
  G4double initial_time = 0.;
  G4ThreeVector initial_position(0.,0.,0.);
  G4ThreeVector momentum_direction = G4RandomDirection();
  G4double momentum = RandomMomentum();

  G4PrimaryVertex* vertex = new G4PrimaryVertex(initial_position, initial_time);

  G4PrimaryParticle* particle =
    new G4PrimaryParticle(particle_def_,
                          momentum_direction.x() * momentum,
                          momentum_direction.y() * momentum,
                          momentum_direction.z() * momentum);

  // Associate particle to the vertex and this to the event
  vertex->SetPrimary(particle);
  event->AddPrimaryVertex(vertex);
}


void ParticleGunGenerator::SetParticleID(G4int pdg_code)
{
  particle_def_ =
    G4ParticleTable::GetParticleTable()->FindParticle(pdg_code);
  if (!particle_def_)
    G4Exception("SetParticleID()", "ParticleGunGenerator",
      FatalException, "ERROR: Unknown PDG code.");
}


G4double ParticleGunGenerator::RandomMomentum() const
{
  G4double min = momentum_range_min_;
  G4double max = momentum_range_max_;

  if (min < max) return (G4UniformRand()*(max-min) + min);
  else return min;
}


void ParticleGunGenerator::DefineCommands()
{
  msg_ = new G4GenericMessenger(this, "/gastpc/generator/");

  msg_->DeclareMethod("pdg_code", &ParticleGunGenerator::SetParticleID, "");

  G4GenericMessenger::Command& momentum_range_min_cmd =
    msg_->DeclarePropertyWithUnit("momentum_range_min", "GeV",
      momentum_range_min_, "Lower value of momentum range.");
  momentum_range_min_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& momentum_range_max_cmd =
    msg_->DeclarePropertyWithUnit("momentum_range_max", "GeV",
      momentum_range_max_, "Upper value of momentum range.");
  momentum_range_max_cmd.SetRange("value>0.");

}
