//____________________________________________________________________________
/*!

\class    MyUserPostDetConstruction

\brief    Class to read in ".root" geometry file - NOT COMPLETED AND NOT USED

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2013

*/
//____________________________________________________________________________
#ifndef SIMUL_MYUSERPOSTDETCONSTRUCTION_H
#define SIMUL_MYUSERPOSTDETCONSTRUCTION_H

#include <TG4RootDetectorConstruction.h>
#include <G4SDManager.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>

#include "TGeoManager.h"

#include "DetectorSD.hh"
#include "MagneticField.hh"

class TVirtualUserPostDetConstruction;
class MyUserPostDetConstruction : public TVirtualUserPostDetConstruction {

public:

  MyUserPostDetConstruction(std::string ,G4double,TGeoManager *);
  ~MyUserPostDetConstruction();

  void Initialize(TG4RootDetectorConstruction *);
  //static MyUserPostDetConstruction *GetInstance(std::string ,G4double,TGeoManager *);

 // void setSDHitPtr(SDHit* hitPtr) {DetectorSD_->setHitPtr(hitPtr);}
  void listVolumes();

private:
  
  //static MyUserPostDetConstruction *fgInstance; // Self pointer

  G4LogicalVolume* FindLogicalVolume(const G4String& vn);

  G4SDManager* 	               SDMgr_;

  G4VPhysicalVolume*           World_;

  TGeoManager  *geoMgr_;

  DetectorSD * DetectorSD_;
  G4LogicalVolume* SD_lv;
  std::string SDName;

  //Magnetic Field Strength
  G4double MagFieldStength;
  MagneticField * fpMagField_;
};
#endif
