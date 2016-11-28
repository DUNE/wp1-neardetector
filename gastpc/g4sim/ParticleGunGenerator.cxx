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



ParticleGunGenerator::ParticleGunGenerator():
  G4VUserPrimaryGeneratorAction(), msg_(0), particle_def_(0), momentum_(0.)
  //kinetic_energy_(0.)
{
  SetParticleID("mu-");

  momentum_ = 1.*GeV;

  DefineCommands();
}


ParticleGunGenerator::~ParticleGunGenerator()
{
  delete msg_;
}


void ParticleGunGenerator::GeneratePrimaries(G4Event* event)
{
  G4cout << "ParticleGunGenerator::GeneratePrimaries()" << G4endl;

  G4double time = 0.;
  G4ThreeVector position(0.,0.,0.);

  G4PrimaryVertex* vertex = new G4PrimaryVertex(position, time);

  //G4ThreeVector momentum_direction = G4RandomDirection();
  momentum_direction_.setZ(1);


  G4PrimaryParticle* particle = 
    new G4PrimaryParticle(particle_def_, momentum_direction_.x()*momentum_,
                                         momentum_direction_.y()*momentum_,
                                         momentum_direction_.z()*momentum_);

  // Associate particle to the vertex and this to the event
  vertex->SetPrimary(particle);
  event->AddPrimaryVertex(vertex);
}


void ParticleGunGenerator::SetParticleID(G4String particle_name)
{
  particle_def_ = 
    G4ParticleTable::GetParticleTable()->FindParticle(particle_name);
  if (!particle_def_)
    G4Exception("SetParticleID()", "ParticleGunGenerator",
      FatalException, "ERROR: Unknown particle name.");
}


void ParticleGunGenerator::DefineCommands()
{
  msg_ = new G4GenericMessenger(this, "/gastpc/generator/");

  msg_->DeclareMethod("particle_id", &ParticleGunGenerator::SetParticleID, "");

}