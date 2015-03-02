#ifndef EMCalcDetectorConstruction_h
#define EMCalcDetectorConstruction_h 1

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

class EMCalcDetectorConstruction : public G4VUserDetectorConstruction {
 public:
  EMCalcDetectorConstruction();
  ~EMCalcDetectorConstruction();

  G4VPhysicalVolume* Construct();

  void SetMaterial (G4String);

  const G4VPhysicalVolume* GetWorld()      {return pBox;};

  G4double           GetSize()       {return BoxSize;};
  G4Material*        GetMaterial()   {return aMaterial;};

  void               PrintParameters();

 private:
  G4VPhysicalVolume*    pBox;
  G4double              BoxSize;
  G4Material*           aMaterial;
  DetectorMessenger* detectorMessenger;

  G4Material *concrete_;
  G4Material *iron_;
  G4Material *lead_;
  G4Material *air_;
  G4Material *scint_;

  G4Material *MIND_;

  void               DefineMaterials();
  G4VPhysicalVolume* ConstructVolumes();
};



#endif

