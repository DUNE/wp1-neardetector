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
#include "DuneGArNDEcalModuleGeometry.h"
#include "UniformMagneticField.h"

#include <G4GenericMessenger.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4SubtractionSolid.hh>
#include <G4GDMLParser.hh>
#include <G4FieldManager.hh>
#include <G4AutoDelete.hh>
#include <G4ProductionCuts.hh>
#include <G4Region.hh>



DuneGArNDDetConstr::DuneGArNDDetConstr(): 
  G4VUserDetectorConstruction(),
  msg_(0),
  magfield_strength_(0.4*tesla),
  // HALL
  hall_size_(20.*m),
  // MAGNET YOKE
  magnet_yoke_width_(0.), magnet_yoke_height_(0.), magnet_yoke_length_(0.),
  magnet_yoke_thickness_(60.*cm),
  // MAGNET COIL
  magnet_coil_width_(0.), magnet_coil_height_(0.), magnet_coil_length_(0.),
  magnet_coil_thickness_(20.*cm), 
  // VESSEL
  vessel_diameter_(3.5*m), 
  vessel_length_(6.5*m),
  vessel_barrel_thickness_(9.9*mm), 
  vessel_endcap_thickness_(17.5*mm),
  vessel_ecal_gap_(10.*cm),
  // BARREL ECAL
  barrel_ecal_abs_thickness_(3.5*mm), 
  barrel_ecal_scint_thickness_(10.*mm),
  barrel_ecal_num_layers_(18),
  // UPSTREAM ECAL
  upstream_ecal_abs_thickness_(3.5*mm), 
  upstream_ecal_scint_thickness_(10.*mm),
  upstream_ecal_num_layers_(18),
  // DOWNSTREAM ECAL
  downstream_ecal_abs_thickness_(1.75*mm), 
  downstream_ecal_scint_thickness_(10.*mm), 
  downstream_ecal_num_layers_(60)
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
  DefineEnvelopeAndMagnet();
  DefineCalorimeters();
  DefineVessel();

  if (gdml_output_filename_ != "") {
    G4GDMLParser parser;
    parser.Write(gdml_output_filename_, hall_phys_vol_, true);
  }

  return hall_phys_vol_;
}


void DuneGArNDDetConstr::DefineHall()
{
  // The experimental hall is defined here as an air-filled box
  // big enough to contain the near detector and allow the generation
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


void DuneGArNDDetConstr::DefineEnvelopeAndMagnet()
{
  // Compute the internal dimensions of the magnet yoke and coils.
  // In order to do that, we need to calculate first the thicknesses
  // of all ECAL modules.

  G4double barrel_ecal_thickness =
    DuneGArNDEcalModuleGeometry::CalculateThickness(barrel_ecal_num_layers_,
      barrel_ecal_abs_thickness_, barrel_ecal_scint_thickness_);

  G4double upstream_ecal_thickness =
    DuneGArNDEcalModuleGeometry::CalculateThickness(upstream_ecal_num_layers_,
      upstream_ecal_abs_thickness_, upstream_ecal_scint_thickness_);

  G4double downstream_ecal_thickness =
    DuneGArNDEcalModuleGeometry::CalculateThickness(downstream_ecal_num_layers_,
      downstream_ecal_abs_thickness_, downstream_ecal_scint_thickness_);

  magnet_yoke_width_ = vessel_diameter_ + 2.*vessel_barrel_thickness_ +
    2.*vessel_ecal_gap_ + 2.*barrel_ecal_thickness;

  magnet_yoke_height_ = vessel_diameter_ + 2.*vessel_barrel_thickness_ +
    2.*vessel_ecal_gap_ + 2.*barrel_ecal_thickness + 2.*magnet_coil_thickness_;

  magnet_yoke_length_ = vessel_length_ + 2.*vessel_endcap_thickness_ + 
    2.*vessel_ecal_gap_ + upstream_ecal_thickness + downstream_ecal_thickness + 
    2.*magnet_coil_thickness_;

  magnet_coil_width_ = magnet_yoke_width_/4.;
  magnet_coil_height_ = magnet_yoke_height_ - 2.*magnet_coil_thickness_;
  magnet_coil_length_ = magnet_yoke_length_ - 2.*magnet_coil_thickness_;


  // DETECTOR ENVELOPE /////////////////////////////////////

  // We define first an envelope volume holding the entire detector with
  // dimensions equal to the external dimensions of the magnet yoke
  // (i.e. we assume that all other volumes fit within the yoke, as in the
  // T2K-ND280 detector). The first face of the detector is positioned at
  // the origin of coordinated of the simulation.

  G4double detenv_width  = magnet_yoke_width_ + 2.*magnet_yoke_thickness_;
  G4double detenv_height = magnet_yoke_height_ + 2.*magnet_yoke_thickness_;
  G4double detenv_length = magnet_yoke_length_;  

  G4Box* detenv_solid_vol = new G4Box("NEAR_DETECTOR_ENV",
    detenv_width/2., detenv_height/2., detenv_length/2.);

  G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* detenv_logic_vol =
    new G4LogicalVolume(detenv_solid_vol, air, "NEAR_DETECTOR_ENV");
  detenv_logic_vol->SetVisAttributes(G4VisAttributes::Invisible);

  detenv_phys_vol_ = 
    new G4PVPlacement(0, G4ThreeVector(0.,0.,detenv_length/2.),
      "NEAR_DETECTOR_ENV", detenv_logic_vol, hall_phys_vol_, 0, false, 0);

  // MAGNETIC FIELD ////////////////////////////////////////

  UniformMagneticField* magfield = new UniformMagneticField(magfield_strength_);
  G4FieldManager* fieldmgr = new G4FieldManager();
  fieldmgr->SetDetectorField(magfield);
  fieldmgr->CreateChordFinder(magfield);
  detenv_logic_vol->SetFieldManager(fieldmgr, true);

  G4AutoDelete::Register(magfield);
  G4AutoDelete::Register(fieldmgr);

  // MAGNET YOKE ///////////////////////////////////////////

  DuneGArNDMagnetYokeGeometry yoke_geom(magnet_yoke_width_,
                                        magnet_yoke_height_,
                                        magnet_yoke_length_,
                                        magnet_yoke_thickness_);
  new G4PVPlacement(0, G4ThreeVector(), "MAGNET_YOKE", 
    yoke_geom.GetLogicalVolume(), detenv_phys_vol_, false, 0, true);

  // MAGNET COILS //////////////////////////////////////////

  magnet_coil_width_  = magnet_yoke_width_/4.;
  magnet_coil_height_ = magnet_yoke_height_ - 2.*magnet_coil_thickness_;
  magnet_coil_length_ = magnet_yoke_length_ - 2.*magnet_coil_thickness_;

  DuneGArNDMagnetCoilGeometry coil_geom(magnet_coil_width_,
                                        magnet_coil_height_,
                                        magnet_coil_length_,
                                        magnet_coil_thickness_);
  for (G4int i=0; i<4; i++) {
    G4double origin_x = -1.5*magnet_coil_width_ + i * magnet_coil_width_;
    new G4PVPlacement(0, G4ThreeVector(origin_x, 0., 0.), "MAGNET_COIL",
      coil_geom.GetLogicalVolume(), detenv_phys_vol_, false, 0, true);
  }

  // Production cuts ///////////////////////////////////////

  G4ProductionCuts* prodcuts = new G4ProductionCuts();
  prodcuts->SetProductionCut(5.*cm); // For all particles

  G4Region* yoke_region = new G4Region("MAGNET_YOKE");
  yoke_region->AddRootLogicalVolume(yoke_geom.GetLogicalVolume());
  yoke_region->SetProductionCuts(prodcuts);

  G4Region* coils_region = new G4Region("MAGNET_COILS");
  coils_region->AddRootLogicalVolume(coil_geom.GetLogicalVolume());
  coils_region->SetProductionCuts(prodcuts);  
}


void DuneGArNDDetConstr::DefineCalorimeters()
{
  // UPSTREAM ECAL /////////////////////////////////////////

  G4double width = vessel_diameter_ + 2.*vessel_barrel_thickness_ ;
  G4double height = width;

  DuneGArNDEcalModuleGeometry 
    upstream_ecal_geom(width, height, upstream_ecal_num_layers_,
      upstream_ecal_abs_thickness_, upstream_ecal_scint_thickness_);
  upstream_ecal_geom.SetUniqueName("UPSTREAM_ECAL");

  G4double origin_z = -magnet_yoke_length_/2. + magnet_coil_thickness_ + 
    upstream_ecal_geom.GetModuleDepth()/2.;

  new G4PVPlacement(0, G4ThreeVector(0., 0., origin_z), "UPSTREAM_ECAL",
    upstream_ecal_geom.GetLogicalVolume(), detenv_phys_vol_, false, 0, true);


  // DOWNSTREAM ECAL ///////////////////////////////////////

  DuneGArNDEcalModuleGeometry 
    downstream_ecal_geom(width, height, downstream_ecal_num_layers_,
      downstream_ecal_abs_thickness_, downstream_ecal_scint_thickness_);
  downstream_ecal_geom.SetUniqueName("DOWNSTREAM_ECAL");

  origin_z = magnet_yoke_length_/2. - magnet_coil_thickness_ -
    downstream_ecal_geom.GetModuleDepth()/2.;

  new G4PVPlacement(0, G4ThreeVector(0., 0., origin_z), "DOWNSTREAM_ECAL",
    downstream_ecal_geom.GetLogicalVolume(), detenv_phys_vol_, false, 0, true);

  // BARREL ECAL ///////////////////////////////////////////

  width = vessel_length_ + 2.*vessel_endcap_thickness_ + 3.*vessel_ecal_gap_;

  height = magnet_coil_height_/2. + 
    vessel_diameter_/2. + vessel_barrel_thickness_;

  DuneGArNDEcalModuleGeometry
    barrel_ecal_geom(width, height, barrel_ecal_num_layers_,
      barrel_ecal_abs_thickness_, barrel_ecal_scint_thickness_);
  barrel_ecal_geom.SetUniqueName("BARREL_ECAL");

  G4RotationMatrix* rot_horz = new G4RotationMatrix();
  rot_horz->rotateY(CLHEP::pi/2.);
  rot_horz->rotateX(CLHEP::pi/2.);

  G4double origin_x = magnet_coil_height_/2. - height/2.;

  G4double origin_y = - (vessel_diameter_/2. + vessel_barrel_thickness_ + 
    vessel_ecal_gap_ + barrel_ecal_geom.GetModuleDepth()/2.);

  origin_z = -magnet_yoke_length_/2. + magnet_coil_thickness_ + 
    upstream_ecal_geom.GetModuleDepth() + vessel_ecal_gap_ + 
    vessel_endcap_thickness_ + vessel_length_/2.;

  new G4PVPlacement(rot_horz, G4ThreeVector(origin_x, origin_y, origin_z),
    "BARREL_ECAL_0", barrel_ecal_geom.GetLogicalVolume(), detenv_phys_vol_,
    false, 0, true);

  new G4PVPlacement(rot_horz, G4ThreeVector(-origin_x, -origin_y, origin_z),
    "BARREL_ECAL_1", barrel_ecal_geom.GetLogicalVolume(), detenv_phys_vol_, 
    false, 1, true);

  G4RotationMatrix* rot_vert = new G4RotationMatrix();
  rot_vert->rotateY(CLHEP::pi/2.);

  G4double tmp = origin_x;
  origin_x = origin_y;
  origin_y = tmp;

  new G4PVPlacement(rot_vert, G4ThreeVector(origin_x, -origin_y, origin_z),
    "BARREL_ECAL_2", barrel_ecal_geom.GetLogicalVolume(), detenv_phys_vol_, 
    false, 2, true);

  new G4PVPlacement(rot_vert, G4ThreeVector(-origin_x, origin_y, origin_z),
    "BARREL_ECAL_3", barrel_ecal_geom.GetLogicalVolume(), detenv_phys_vol_, 
    false, 3, true);

}


void DuneGArNDDetConstr::DefineVessel()
{
  DuneGArNDVesselGeometry vessel_geom(vessel_diameter_, vessel_length_,
    vessel_barrel_thickness_, vessel_endcap_thickness_);

  G4double upstream_ecal_thickness =
    DuneGArNDEcalModuleGeometry::CalculateThickness(upstream_ecal_num_layers_,
      upstream_ecal_abs_thickness_, upstream_ecal_scint_thickness_);

  G4double origin_z = -magnet_yoke_length_/2. + magnet_coil_thickness_ + upstream_ecal_thickness + vessel_ecal_gap_ + vessel_endcap_thickness_ +
    vessel_length_/2.;

  new G4PVPlacement(0, G4ThreeVector(0., 0., origin_z), "VESSEL",
    vessel_geom.GetLogicalVolume(), detenv_phys_vol_, false, 0, true);
}



void DuneGArNDDetConstr::DefineCommands()
{
  // Define configuration commands using a generic messenger.
  // (The default values for the properties are set in the constructor's
  // initialization list above, since the corresponding methods of the
  // messenger do not seem to work properly.)

  msg_ = new G4GenericMessenger(this, "/gastpc/geometry/");

  // Magnetic field //////////////////////////////

  G4GenericMessenger::Command& magfield_strength_cmd =
    msg_->DeclarePropertyWithUnit("magfield_strength", "tesla", 
      magfield_strength_, "Magnetic field strength.");
  magfield_strength_cmd.SetRange("value>=0.");

  // Hall ////////////////////////////////////////

  G4GenericMessenger::Command& hall_size_cmd = 
    msg_->DeclarePropertyWithUnit("hall_size", "m", hall_size_);
  hall_size_cmd.SetRange("value>0.");

  // Magnet yoke /////////////////////////////////

  G4GenericMessenger::Command& magnet_yoke_thickn_cmd = 
    msg_->DeclarePropertyWithUnit("magnet_yoke_thickness", "m",
      magnet_yoke_thickness_, "Thickness of magnet yoke.");
  magnet_yoke_thickn_cmd.SetRange("value>0.");

  // Magnet coils ////////////////////////////////

  G4GenericMessenger::Command& magnet_coil_thickn_cmd = 
    msg_->DeclarePropertyWithUnit("magnet_coil_thickness", "m",
      magnet_coil_thickness_, "Thickness of magnet coil.");
  magnet_coil_thickn_cmd.SetRange("value>0.");

  // Pressure vessel /////////////////////////////

  G4GenericMessenger::Command& vessel_diam_cmd =
    msg_->DeclarePropertyWithUnit("vessel_diam", "cm",
      vessel_diameter_, "Internal diameter of pressure vessel.");
  vessel_diam_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& vessel_length_cmd =
    msg_->DeclarePropertyWithUnit("vessel_length", "cm",
      vessel_length_, "Internal length of pressure vessel.");
  vessel_length_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& vessel_barrel_thickness_cmd =
    msg_->DeclarePropertyWithUnit("vessel_barrel_thickness", "cm",
      vessel_barrel_thickness_, "Thickness of vessel barrel.");
  vessel_barrel_thickness_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& vessel_endcap_thickness_cmd =
    msg_->DeclarePropertyWithUnit("vessel_endcap_thickness", "cm",
      vessel_endcap_thickness_, "Thickness of vessel endcaps.");
  vessel_endcap_thickness_cmd.SetRange("value>0.");

  // Electromagnetic calorimeters (ECALs) ////////

  G4GenericMessenger::Command& vessel_ecal_gap_cmd =
    msg_->DeclarePropertyWithUnit("vessel_ecal_gap", "cm",
      vessel_ecal_gap_, "Size of the gap between vessel and ECALs.");
  vessel_ecal_gap_cmd.SetRange("value>=0.");


  G4GenericMessenger::Command& barrel_ecal_abs_thickness_cmd =
    msg_->DeclarePropertyWithUnit("barrel_ecal_abs_thickness", "cm",
      barrel_ecal_abs_thickness_, 
      "Thickness of absorber layers in barrel ECAL.");
  barrel_ecal_abs_thickness_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& barrel_ecal_scint_thickness_cmd =
    msg_->DeclarePropertyWithUnit("barrel_ecal_scint_thickness", "cm",
      barrel_ecal_scint_thickness_, 
      "Thickness of scintillator layers in barrel ECAL.");
  barrel_ecal_scint_thickness_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& barrel_ecal_num_layers_cmd =
    msg_->DeclarePropertyWithUnit("barrel_ecal_num_layers", "cm",
      barrel_ecal_num_layers_, 
      "Number of layers in barrel ECAL.");
  barrel_ecal_num_layers_cmd.SetRange("value>0.");


  G4GenericMessenger::Command& upstream_ecal_abs_thickness_cmd =
    msg_->DeclarePropertyWithUnit("upstream_ecal_abs_thickness", "cm",
      upstream_ecal_abs_thickness_, 
      "Thickness of absorber layers in upstream ECAL.");
  upstream_ecal_abs_thickness_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& upstream_ecal_scint_thickness_cmd =
    msg_->DeclarePropertyWithUnit("upstream_ecal_scint_thickness", "cm",
      upstream_ecal_scint_thickness_, 
      "Thickness of scintillator layers in upstream ECAL.");
  upstream_ecal_scint_thickness_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& upstream_ecal_num_layers_cmd =
    msg_->DeclarePropertyWithUnit("upstream_ecal_num_layers", "cm",
      upstream_ecal_num_layers_, 
      "Number of layers in upstream ECAL.");
  upstream_ecal_num_layers_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& downstream_ecal_abs_thickness_cmd =
    msg_->DeclarePropertyWithUnit("downstream_ecal_abs_thickness", "cm",
      downstream_ecal_abs_thickness_, 
      "Thickness of absorber layers in downstream ECAL.");
  downstream_ecal_abs_thickness_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& downstream_ecal_scint_thickness_cmd =
    msg_->DeclarePropertyWithUnit("downstream_ecal_scint_thickness", "cm",
      downstream_ecal_scint_thickness_, 
      "Thickness of scintillator layers in downstream ECAL.");
  downstream_ecal_scint_thickness_cmd.SetRange("value>0.");

  G4GenericMessenger::Command& downstream_ecal_num_layers_cmd =
    msg_->DeclarePropertyWithUnit("downstream_ecal_num_layers", "cm",
      downstream_ecal_num_layers_, 
      "Number of layers in downstream ECAL.");
  downstream_ecal_num_layers_cmd.SetRange("value>0.");

  ////////////////////////////////////////////////

  msg_->DeclareProperty("gdml_output_filename", gdml_output_filename_,
    "Name of GDML output file");
}

