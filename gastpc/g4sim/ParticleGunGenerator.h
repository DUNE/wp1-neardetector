// -------------------------------------------------------------------
/// \file   ParticleGunGenerator.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 29 July 2016
// -------------------------------------------------------------------

#ifndef PARTICLE_GUN_GENERATOR
#define PARTICLE_GUN_GENERATOR

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ThreeVector.hh>
#include <G4String.hh>

class G4GenericMessenger;
class G4ParticleDefinition;



/// TODO: Class descriptions

class ParticleGunGenerator: public G4VUserPrimaryGeneratorAction
{
public:
  /// Constructor
  ParticleGunGenerator();
  /// Destructor
  virtual ~ParticleGunGenerator();

  virtual void GeneratePrimaries(G4Event*);
  
private:

  void DefineCommands();

  void SetParticleID(G4String);

private:
  G4GenericMessenger* msg_;

  G4ParticleDefinition* particle_def_;

  G4double momentum_;
  //G4double kinetic_energy_;

  G4ThreeVector initial_position_;
  G4ThreeVector momentum_direction_;
};

#endif