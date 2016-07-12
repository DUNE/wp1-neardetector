// -------------------------------------------------------------------
/// \file   DuneGArNDDetConstr.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#include "DuneGArNDDetConstr.h"

#include "DuneGArNDMagnetYokeGeometry.h"
#include "DuneGArNDMagnetCoilGeometry.h"
#include "DuneGArNDVesselGeometry.h"

#include <G4GenericMessenger.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4SubtractionSolid.hh>



DuneGArNDDetConstr::DuneGArNDDetConstr(): 
  G4VUserDetectorConstruction(),
  msg_(0),
  hall_phys_vol_(0), detenv_phys_vol_(0),
  hall_size_(20.*m),
  magnet_yoke_dims_(G4ThreeVector(4.5*m, 4.5*m, 8.0*m)),
  magnet_yoke_thickness_(0.6*m),
  magnet_coil_dims_(G4ThreeVector(1.125*m, 4.1*m, 7.6*m)),
  magnet_coil_thickness_(0.2*m) 
{
  DefineCommands();
}


DuneGArNDDetConstr::~DuneGArNDDetConstr()
{
  delete msg_; 
}


G4VPhysicalVolume* DuneGArNDDetConstr::Construct()
{ 
  DefineHall();
  DefineDetectorEnvelope();
  DefineMagnet();
  DefineVessel();
  DefineCalorimeters();

  return hall_phys_vol_;
}


void DuneGArNDDetConstr::DefineHall()
{
  // The experimental hall is defined here as an air-filled
  // box big enough to contain the near detector and allow the generation
  // of external events (neutrino interactions in the rock and cosmics).

  G4Box* hall_solid_vol = 
    new G4Box("HALL", hall_size_/2., hall_size_/2., hall_size_/2.);

  G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* hall_logic_vol =  
    new G4LogicalVolume(hall_solid_vol, air, "HALL");
  hall_logic_vol->SetVisAttributes(G4VisAttributes::Invisible);

  hall_phys_vol_ = 
    new G4PVPlacement(0, G4ThreeVector(), hall_logic_vol, "HALL", 0, false, 0);
}


void DuneGArNDDetConstr::DefineDetectorEnvelope()
{
  // We define here an envelope volume wrapping the entire detector, with
  // dimensions equal to the external dimensions of the magnet yoke 
  // (i.e. we assume that all other volumes fit within the yoke, as in the
  // T2K-ND280 detector).
  // The first face of the detector is positioned at the origin of
  // coordinates of the simulation.

  G4double detenv_width  = magnet_yoke_dims_.x() + 2.*magnet_yoke_thickness_;
  G4double detenv_height = magnet_yoke_dims_.y() + 2.*magnet_yoke_thickness_;
  G4double detenv_length = magnet_yoke_dims_.z();

  G4Box* detenv_solid_vol = 
    new G4Box("NEAR_DETECTOR_ENV", detenv_width/2., detenv_height/2., detenv_length/2.);

  G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* detenv_logic_vol =
    new G4LogicalVolume(detenv_solid_vol, air, "NEAR_DETECTOR_ENV");
  detenv_logic_vol->SetVisAttributes(G4VisAttributes::Invisible);

  detenv_phys_vol_ =
    new G4PVPlacement(0, G4ThreeVector(0.,0.,detenv_length/2.), 
      "NEAR_DETECTOR_ENV", detenv_logic_vol, hall_phys_vol_, 0, false, 0);
}


void DuneGArNDDetConstr::DefineMagnet()
{
  DuneGArNDMagnetYokeGeometry yoke_geom(magnet_yoke_dims_.x(),
                                        magnet_yoke_dims_.y(),
                                        magnet_yoke_dims_.z(),
                                        magnet_yoke_thickness_);
  new G4PVPlacement(0, G4ThreeVector(), "MAGNET_YOKE", 
    yoke_geom.GetLogicalVolume(), detenv_phys_vol_, false, 0, true);

  DuneGArNDMagnetCoilGeometry coil_geom(magnet_coil_dims_.x(),
                                        magnet_coil_dims_.y(),
                                        magnet_coil_dims_.z(),
                                        magnet_coil_thickness_);
  for (G4int i=0; i<4; i++) {
    G4double origin_x = -1.5*magnet_coil_dims_.x() + i * magnet_coil_dims_.x();
    new G4PVPlacement(0, G4ThreeVector(origin_x, 0., 0.), "MAGNET_COIL",
      coil_geom.GetLogicalVolume(), detenv_phys_vol_, false, 0, true);
  }

  G4VisAttributes * visattrib_blue = new G4VisAttributes;
  visattrib_blue->SetColor(.7, 0.4, 0.1);
  visattrib_blue->SetForceSolid(true);
  coil_geom.GetLogicalVolume()->SetVisAttributes(visattrib_blue);
}


void DuneGArNDDetConstr::DefineVessel()
{
  DuneGArNDVesselGeometry vessel_geom(3.25*m,6.5*m, 9.*mm);
  new G4PVPlacement(0, G4ThreeVector(), "VESSEL",
    vessel_geom.GetLogicalVolume(), detenv_phys_vol_, false, 0, true);
}


void DuneGArNDDetConstr::DefineCalorimeters()
{
  // BARREL ////////////////////////////////////////////////

  G4double barrel_width = 4*magnet_coil_dims_.x();
  G4double barrel_height = magnet_coil_dims_.y();
  G4double barrel_length = 6.4*m;
  G4double barrel_thickness = 26.*cm;  

  G4SubtractionSolid* barrel_solid_vol =
    new G4SubtractionSolid("ECAL_BARREL",
      new G4Box("LARGE_BOX", barrel_width/2.,
                             barrel_height/2.,
                             barrel_length/2.),
      new G4Box("SMALL_BOX", barrel_width/2.-barrel_thickness,
                             barrel_height/2.-barrel_thickness,
                             barrel_length/2.+1.*cm));

  G4Material* bgo = G4NistManager::Instance()->FindOrBuildMaterial("G4_BGO");

  G4LogicalVolume* barrel_logic_vol =
    new G4LogicalVolume(barrel_solid_vol, bgo, "ECAL_BARREL");

  new G4PVPlacement(0, G4ThreeVector(), "ECAL_BARREL", barrel_logic_vol, detenv_phys_vol_, false, 0, true);

  // UPSTREAM //////////////////////////////////////////////

  G4Box* upstream_solid_vol =
    new G4Box("ECAL_UPSTREAM", 3.2/2.*m, 3.2/2.*m, 40./2.*cm);

  G4LogicalVolume* upstream_logic_vol =
    new G4LogicalVolume(upstream_solid_vol, bgo, "ECAL_UPSTREAM");

  G4double origin_z = 
    -magnet_yoke_dims_.z()/2. + magnet_coil_thickness_ + 40./2.*cm;

  new G4PVPlacement(0, G4ThreeVector(0.,0.,origin_z), "ECAL_UPSTREAM",
    upstream_logic_vol, detenv_phys_vol_, false, 0, true);

  // DOWNSTREAM ////////////////////////////////////////////

  G4Box* downstream_solid_vol =
    new G4Box("ECAL_DOWNSTREAM", 3.2/2.*m, 3.2/2.*m, 26./2.*cm);

  G4LogicalVolume* downstream_logic_vol =
    new G4LogicalVolume(downstream_solid_vol, bgo, "ECAL_DOWNSTREAM");

  origin_z = -magnet_yoke_dims_.z()/2. + magnet_coil_thickness_ + 26./2.*cm;

  new G4PVPlacement(0, G4ThreeVector(0.,0.,-origin_z), "ECAL_DOWNSTREAM",
    downstream_logic_vol, detenv_phys_vol_, false, 0, true);

}


void DuneGArNDDetConstr::DefineCommands()
{
  // Define configuration commands using a generic messenger.
  // (The default values for the properties are set in the constructor's
  // initialization list above.)

  msg_ = new G4GenericMessenger(this, "/gastpc/geometry/");

  G4GenericMessenger::Command& hall_size_cmd = 
    msg_->DeclarePropertyWithUnit("hall_size", "m", hall_size_);
  hall_size_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& magnet_yoke_dims_cmd =
    msg_->DeclarePropertyWithUnit("magnet_yoke_dims", "m", magnet_yoke_dims_, 
                                  "Inner dimensions of magnet yoke.");
  magnet_yoke_dims_cmd.SetRange("valueX>0. && valueY>0. && valueZ>0.");

  G4GenericMessenger::Command& magnet_yoke_thickn_cmd = 
    msg_->DeclarePropertyWithUnit("magnet_yoke_thickness", "m",
                                  magnet_yoke_thickness_,
                                  "Thickness of magnet yoke.");
  magnet_yoke_thickn_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& magnet_coil_dims_cmd =
    msg_->DeclarePropertyWithUnit("magnet_coil_dims", "m", magnet_coil_dims_,
                                  "Inner dimensions of a magnet coil.");
  magnet_coil_dims_cmd.SetRange("valueX>0. && ºvalueY>0. && valueZ>0.");

  G4GenericMessenger::Command& magnet_coil_thickn_cmd = 
    msg_->DeclarePropertyWithUnit("magnet_coil_thickness", "m",
                                  magnet_coil_thickness_,
                                  "Thickness of a magnet coil.");
  magnet_coil_thickn_cmd.SetRange("value>0.");

}



