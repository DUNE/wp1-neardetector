// -------------------------------------------------------------------
/// \file   ArgonBox.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#include "ArgonBox.h"

#include "MaterialsList.h"
#include "UniformMagneticField.h"
#include "TrackingSD.h"

#include <G4GenericMessenger.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4FieldManager.hh>
#include <G4AutoDelete.hh>
#include <G4ProductionCuts.hh>
#include <G4UserLimits.hh>
#include <G4SDManager.hh>


ArgonBox::ArgonBox():
  G4VUserDetectorConstruction(),
  msg_(0), size_(5.*m), magfield_strength_(0.4*tesla)
{
  DefineCommands();
}


ArgonBox::~ArgonBox()
{
  delete msg_;
}


G4VPhysicalVolume* ArgonBox::Construct()
{
  // Definition of volumes

  G4Box* world_solid_vol = new G4Box("WORLD", size_/2., size_/2., size_/2.);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol, MaterialsList::GAr(), "WORLD");

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(0, G4ThreeVector(), world_logic_vol, "WORLD", 0, false, 0);

  // Magnetic field

  UniformMagneticField* magfield = new UniformMagneticField(magfield_strength_);
  G4FieldManager* fieldmgr = new G4FieldManager();
  fieldmgr->SetDetectorField(magfield);
  fieldmgr->CreateChordFinder(magfield);
  world_logic_vol->SetFieldManager(fieldmgr, true);
  G4AutoDelete::Register(magfield);
  G4AutoDelete::Register(fieldmgr);

  // Sensitive detector and stepping limit

  TrackingSD* tsd = new TrackingSD("TPC");
  G4SDManager::GetSDMpointer()->AddNewDetector(tsd);
  world_logic_vol->SetSensitiveDetector(tsd);
  world_logic_vol->SetUserLimits(new G4UserLimits(7.5*mm));

  //

  return world_phys_vol;
}


void ArgonBox::DefineCommands()
{
  // Define configuration commands using a generic messenger.
  // (The default values for the properties are set in the constructor's
  // initialization list above, since the corresponding methods of the
  // messenger do not seem to work properly.)

  msg_ = new G4GenericMessenger(this, "/gastpc/geometry/");

  G4GenericMessenger::Command& world_size_cmd =
    msg_->DeclarePropertyWithUnit("world_size", "m", size_, "World size.");
  world_size_cmd.SetRange("value>=0.");

  G4GenericMessenger::Command& magfield_strength_cmd =
    msg_->DeclarePropertyWithUnit("magfield_strength", "tesla",
      magfield_strength_, "Magnetic field strength.");
  magfield_strength_cmd.SetRange("value>=0.");

}
