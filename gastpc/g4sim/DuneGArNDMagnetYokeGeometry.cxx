// -------------------------------------------------------------------
/// \file   DuneGArNDMagnetYokeGeometry.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#include "DuneGArNDMagnetYokeGeometry.h"

#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>



DuneGArNDMagnetYokeGeometry::DuneGArNDMagnetYokeGeometry(
  G4double width, G4double height, G4double length, G4double thickness):
  BaseGeometry(), 
  width_(width), height_(height), length_(length), thickness_(thickness)
{
}


DuneGArNDMagnetYokeGeometry::~DuneGArNDMagnetYokeGeometry()
{
}


void DuneGArNDMagnetYokeGeometry::DefineVolumes()
{
  G4Material* iron = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");

  G4SubtractionSolid* yoke_solid_vol = 
    new G4SubtractionSolid("MAGNET_YOKE", 
                           new G4Box("LARGE_BOX", width_/2. + thickness_,
                                                  height_/2. + thickness_,
                                                  length_/2.),
                           new G4Box("SMALL_BOX", width_/2., 
                                                  height_/2.,
                                                  length_/2.+1.*cm));

  G4LogicalVolume* yoke_logic_vol =
    new G4LogicalVolume(yoke_solid_vol, iron, "MAGNET_YOKE");    

  this->SetLogicalVolume(yoke_logic_vol);
}
