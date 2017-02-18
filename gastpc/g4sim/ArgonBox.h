// -------------------------------------------------------------------
/// \file   ArgonBox.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 18 Feb 2017
// -------------------------------------------------------------------

#ifndef ARGON_BOX_H
#define ARGON_BOX_H

#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;
class G4GenericMessenger;


/// TO DO. Class description

class ArgonBox: public G4VUserDetectorConstruction
{
public:
  /// Constructor
  ArgonBox();
  /// Destructor
  virtual ~ArgonBox();

  virtual G4VPhysicalVolume* Construct();

private:
  /// Define user configuration commands via a generic messenger class
  void DefineCommands();

private:
  G4GenericMessenger* msg_; ///< Commands messenger
  G4double size_; ///< World size_
  G4double magfield_strength_; ///< Magnetic field strength
};

#endif
