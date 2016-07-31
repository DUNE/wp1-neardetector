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
#include <string>

class G4VUserDetectorConstruction;
class G4VUserPrimaryGeneratorAction;



class RunManager: public G4RunManager
{
public:
  /// Constructor
  RunManager(const std::string& detector_tag, 
             const std::string& generator_tag);

  /// Destructor
  virtual ~RunManager();

  ///
  virtual void Initialize();

  ///
  void ExecuteMacroFile(const G4String&);

private:
  G4VUserDetectorConstruction* CreateDetectorConstruction(const std::string&);

  G4VUserPrimaryGeneratorAction* CreatePrimaryGenerator(const std::string&);

  /// Set a seed for the random number generator
  /// If a negative integer is given, the system time is set as seed.
  void SetRandomSeed(G4int) const;
};

#endif