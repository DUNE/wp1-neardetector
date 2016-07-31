// -------------------------------------------------------------------
/// \file   RunManager.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 1 June 2016
// -------------------------------------------------------------------

#ifndef GASTPC_RUN_MANAGER_H
#define GASTPC_RUN_MANAGER_H

#include <G4RunManager.hh>



class RunManager: public G4RunManager
{
public:
  /// Constructor
  RunManager();
  /// Destructor
  virtual ~RunManager();

  virtual void Initialize();

  void SetPhysicsList(const G4String&);

  void SetGeometry(const G4String&);

  void SetPrimaryGenerator(const G4String&);

  ///
  void ExecuteMacroFile(const G4String&);

private:
  /// Set a seed for the random number generator
  /// If a negative integer is given, the system time is set as seed.
  void SetRandomSeed(G4int) const;

};

#endif