// -------------------------------------------------------------------
/// \file   DuneGArNDMagnetCoilGeometry.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#include "DuneGArNDMagnetCoilGeometry.h"

#include <G4Material.hh>
#include <G4SubtractionSolid.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>



DuneGArNDMagnetCoilGeometry::DuneGArNDMagnetCoilGeometry(
  G4double width, G4double height, G4double length, G4double thickness):
  BaseGeometry(), 
  width_(width), height_(height), length_(length), thickness_(thickness)
{
}


DuneGArNDMagnetCoilGeometry::~DuneGArNDMagnetCoilGeometry()
{
}


void DuneGArNDMagnetCoilGeometry::DefineVolumes()
{
  G4Material* copper = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");

  G4SubtractionSolid* coil_solid_vol =
    new G4SubtractionSolid("MAGNET_COIL",
                           new G4Box("LARGE_BOX", width_/2.,
                                                  height_/2.+thickness_,
                                                  length_/2.+thickness_),
                           new G4Box("SMALL_BOX", width_/2.+1*cm,
                                                  height_/2.,
                                                  length_/2.));

  G4LogicalVolume* coil_logic_vol = 
    new G4LogicalVolume(coil_solid_vol, copper, "MAGNET_COIL");

  this->SetLogicalVolume(coil_logic_vol);
}