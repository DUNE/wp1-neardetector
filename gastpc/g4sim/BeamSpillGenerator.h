// -------------------------------------------------------------------
/// \file   BeamSpillGenerator.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 June 2016
// -------------------------------------------------------------------

#ifndef GASTPC_BEAM_SPILL_GENERATOR_H
#define GASTPC_BEAM_SPILL_GENERATOR_H

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4String.hh>
#include <vector>

class G4GenericMessenger;
class BeamSpillSource;


/// TODO: Class description

class BeamSpillGenerator: public G4VUserPrimaryGeneratorAction
{
public:
  /// Constructor
  BeamSpillGenerator();
  /// Destructor
  virtual ~BeamSpillGenerator();

  /// Add a GHEP source to the primary generator.
  /// (Similar methods could exist for other source types.)
  void AddGHepSource(G4String, G4double);

  void AddCosmicsSource(G4String);
  
  ///
  virtual void GeneratePrimaries(G4Event*);

private:
  G4GenericMessenger* msg_;
  std::vector<BeamSpillSource*> sources_;
};

#endif