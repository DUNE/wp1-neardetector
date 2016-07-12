// -------------------------------------------------------------------
/// \file   DuneGArNDDetConstr.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#ifndef DUNE_GARND_GEOMETRY_H
#define DUNE_GARND_GEOMETRY_H

#include <G4VUserDetectorConstruction.hh>
#include <G4ThreeVector.hh>

class G4VPhysicalVolume;
class G4GenericMessenger;


/// TO DO. Class description

class DuneGArNDDetConstr: public G4VUserDetectorConstruction
{
public:
  /// Constructor
  DuneGArNDDetConstr();
  /// Destructor
  virtual ~DuneGArNDDetConstr();
  
  virtual G4VPhysicalVolume* Construct();

private:
  ///
  void DefineCommands();

  ///
  void DefineHall();
  void DefineDetectorEnvelope();
  void DefineMagnet();
  void DefineVessel();
  void DefineCalorimeters();

private:
  G4GenericMessenger* msg_; ///< Commands messenger

  G4VPhysicalVolume* hall_phys_vol_;   ///< Physical volume of near det. hall
  G4VPhysicalVolume* detenv_phys_vol_; ///< Physical volume wrapping the ND

  G4double hall_size_;

  G4ThreeVector magnet_yoke_dims_; ///< Inner dimensions of magnet yoke 
  G4double magnet_yoke_thickness_; ///< Thickness of magnet yoke

  G4ThreeVector magnet_coil_dims_; ///< Inner dimensions of magnet coils 
  G4double magnet_coil_thickness_; ///< Thickness of magnet coils
};

#endif