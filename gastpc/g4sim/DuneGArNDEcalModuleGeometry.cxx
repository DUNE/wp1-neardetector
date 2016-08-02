// -------------------------------------------------------------------
/// \file   DuneGArNDEcalModuleGeometry.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#include "DuneGArNDEcalModuleGeometry.h"

#include "TrackingSD.h"

#include <G4Material.hh>
#include <G4SubtractionSolid.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4SDManager.hh>


DuneGArNDEcalModuleGeometry::DuneGArNDEcalModuleGeometry(
  G4double width, G4double height, 
  G4int num_layers, G4double abs_thickness, G4double scint_thickness):
  BaseGeometry(), unique_name_("ECAL_MODULE"),
  width_(width), height_(height), num_layers_(num_layers),
  abs_thickness_(abs_thickness), scint_thickness_(scint_thickness)
{
  depth_ = DuneGArNDEcalModuleGeometry::CalculateThickness(num_layers_,
    abs_thickness_, scint_thickness_);
}


DuneGArNDEcalModuleGeometry::~DuneGArNDEcalModuleGeometry()
{
}


void DuneGArNDEcalModuleGeometry::DefineVolumes()
{
  // ECAL MODULE -- ENVELOPE ///////////////////////////////

  G4Box* ecal_module_solid_vol =
    new G4Box(unique_name_, width_/2., height_/2., depth_/2.);

  G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  G4LogicalVolume* ecal_module_logic_vol = 
    new G4LogicalVolume(ecal_module_solid_vol, air, unique_name_);
  this->SetLogicalVolume(ecal_module_logic_vol);

  // SCINTILLATOR LAYER ////////////////////////////////////

  G4Box* scint_layer_solid_vol =
    new G4Box("ECAL_SCINT", width_/2., height_/2., scint_thickness_/2.);

  G4Material* plastic_scint = 
    G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYSTYRENE");

  G4LogicalVolume* scint_layer_logic_vol = 
    new G4LogicalVolume(scint_layer_solid_vol, plastic_scint, "ECAL_SCINT");

  G4String sdname = "/DUNE/" + unique_name_ + "/ECAL_SCINT";
  TrackingSD* tsd = new TrackingSD(sdname, "");
  scint_layer_logic_vol->SetSensitiveDetector(tsd);
  G4SDManager::GetSDMpointer()->AddNewDetector(tsd);

  // ABSORBER LAYER ////////////////////////////////////////

  G4Box* abs_layer_solid_vol =
    new G4Box("ECAL_ABSORBER", width_/2., height_/2., abs_thickness_/2.);

  G4Material* lead = 
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");

  G4LogicalVolume* abs_layer_logic_vol = 
    new G4LogicalVolume(abs_layer_solid_vol, lead, "ECAL_ABSORBER");

  // PLACEMENT OF LAYERS ///////////////////////////////////

  G4double layer_position = -depth_/2. + scint_thickness_/2.;

  new G4PVPlacement(0, G4ThreeVector(0., 0., layer_position),
    scint_layer_logic_vol, "ECAL_SCINT", ecal_module_logic_vol, 
    false, 0, true);

  for (G4int i=1; i<num_layers_; i++) {

    layer_position = layer_position + scint_thickness_/2. + abs_thickness_/2.;

    new G4PVPlacement(0, G4ThreeVector(0., 0., layer_position),
      abs_layer_logic_vol, "ECAL_ABSORBER", ecal_module_logic_vol, 
      false, i, true);

    layer_position = layer_position + abs_thickness_/2. + scint_thickness_/2.;

    new G4PVPlacement(0, G4ThreeVector(0., 0., layer_position), 
      scint_layer_logic_vol, "ECAL_SCINT", ecal_module_logic_vol, 
      false, i, true);
  }
}

