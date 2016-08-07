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
  /// Define user configuration commands via a generic messenger class
  void DefineCommands();

  ///
  void DefineHall();
  void DefineEnvelopeAndMagnet();
  void DefineVessel();
  void DefineCalorimeters();

private:
  G4GenericMessenger* msg_; ///< Commands messenger

  G4double hall_size_; /// Size of near detector hall

  G4double vessel_diameter_; ///< Internal diameter of pressure vessel
  G4double vessel_length_;   ///< Internal length of pressure vessel
  G4double vessel_barrel_thickness_; ///< Thickness of pressure vessel barrel
  G4double vessel_endcap_thickness_; ///< Thickness of pressure vessel endcaps

  G4double vessel_ecal_gap_; ///< Air gap between vessel and ecal modules

  G4double barrel_ecal_abs_thickness_;
  G4double barrel_ecal_scint_thickness_;
  G4int barrel_ecal_num_layers_;

  G4double upstream_ecal_abs_thickness_;
  G4double upstream_ecal_scint_thickness_;
  G4int upstream_ecal_num_layers_;

  G4double downstream_ecal_abs_thickness_;
  G4double downstream_ecal_scint_thickness_;
  G4int downstream_ecal_num_layers_;

  G4double magnet_yoke_width_;     ///< Internal width of magnet yoke
  G4double magnet_yoke_height_;    ///< Internal height of magnet yoke
  G4double magnet_yoke_length_;    ///< Internal length of magnet yoke
  G4double magnet_yoke_thickness_; ///< Thickness of magnet yoke

  G4double magnet_coil_width_;     ///< Internal width of magnet coils
  G4double magnet_coil_height_;    ///< Internal height of magnet coils
  G4double magnet_coil_length_;    ///< Internal length of magnet coils
  G4double magnet_coil_thickness_; ///< Thickness of magnet coils

  G4double magfield_strength_; /// Magnetic field strength

  G4VPhysicalVolume* hall_phys_vol_;   ///< Physical volume of near det. hall
  G4VPhysicalVolume* detenv_phys_vol_; ///< Physical volume wrapping the ND
};

#endif