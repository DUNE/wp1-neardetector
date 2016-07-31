// -------------------------------------------------------------------
/// \file   DuneGArNDVesselGeometry.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 21 June 2016
// -------------------------------------------------------------------

#include "DuneGArNDVesselGeometry.h"

#include "MaterialsList.h"
#include "TrackingSD.h"

#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4SDManager.hh>



DuneGArNDVesselGeometry::DuneGArNDVesselGeometry(
  G4double diameter, G4double length, 
  G4double barrel_thickness, G4double endcap_thickness):
  BaseGeometry(), diameter_(diameter), length_(length), 
  barrel_thickness_(barrel_thickness), endcap_thickness_(endcap_thickness)
{
}


DuneGArNDVesselGeometry::~DuneGArNDVesselGeometry()
{
}


void DuneGArNDVesselGeometry::DefineVolumes()
{
  G4Material* Ti_alloy = MaterialsList::TitaniumR56323();
  G4Material* GAr = MaterialsList::GAr();

  G4Tubs* vessel_solid_vol = new G4Tubs("VESSEL", 0., 
    diameter_/2. + barrel_thickness_, length_/2. + endcap_thickness_, 
    0., CLHEP::twopi);

  G4LogicalVolume* vessel_logic_vol =
    new G4LogicalVolume(vessel_solid_vol, Ti_alloy, "VESSEL");
  this->SetLogicalVolume(vessel_logic_vol);

  G4Tubs* gas_solid_vol = 
    new G4Tubs("GAS", 0., diameter_/2., length_/2., 0., CLHEP::twopi);

  G4LogicalVolume* gas_logic_vol =
    new G4LogicalVolume(gas_solid_vol, GAr, "GAS");

  new G4PVPlacement(0, G4ThreeVector(), gas_logic_vol, "GAS", 
    vessel_logic_vol, false, 0, true);

  //////////////////////////////////////////////////////////

  G4double tpc_side   = sqrt(2.) * diameter_/2.;
  G4double tpc_length = length_;

  G4Box* tpc_solid_vol = new G4Box("TPC", tpc_side/2., tpc_side/2., tpc_length/2.);
  
  G4LogicalVolume* tpc_logic_vol = 
    new G4LogicalVolume(tpc_solid_vol, GAr, "TPC");
  
  new G4PVPlacement(0, G4ThreeVector(), tpc_logic_vol, "TPC", gas_logic_vol, 
    false, 0, true);

  TrackingSD* tsd = new TrackingSD("/DUNEGARND/TPC", "");
  tpc_logic_vol->SetSensitiveDetector(tsd);
  G4SDManager::GetSDMpointer()->AddNewDetector(tsd);

  //G4VisAttributes* vis = new G4VisAttributes();
  //vis->SetColor(0.5,0.5,0.5);
  //vis->SetForceSolid(true);
  //vessel_logic_vol->SetVisAttributes(vis);
}
