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
  void SetParticleID(G4int);
  G4double RandomMomentum() const;

private:
  G4GenericMessenger* msg_; ///< Messenger for configuration commands
  G4ParticleDefinition* particle_def_; ///< Particle definition
  G4double momentum_range_min_; ///< Lower value of momentum
  G4double momentum_range_max_; ///< Upper value of momentum
};

#endif
