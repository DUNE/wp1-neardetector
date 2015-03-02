//____________________________________________________________________________
/*!

\class    DetectorConstruction

\brief    Class for detector construction in geant4

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update April 2014

*/
//____________________________________________________________________________
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4GDMLParser.hh>
#include <G4SDManager.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4GeometryManager.hh"
#include "G4PVPlacement.hh"

#include <TG4RootDetectorConstruction.h>

#include "TGeoManager.h"
#include "TG4RootNavigator.h"
#include "TG4RootNavMgr.h"

#include "LbnoDataLib.hh"
#include "MINDSensitiveDetector.hh"
#include "TpcFidSensitiveDetector.hh"
#include "ScintillatorSensitiveDetector.hh"
#include "DetectorSD.hh"
#include "MagneticField.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
 public:
  DetectorConstruction(std::string geomFile,G4double magFieldStength,TGeoManager * geoMgr);
  DetectorConstruction(std::string geomFile,G4double magFieldStength);
  ~DetectorConstruction();

  G4VPhysicalVolume *Construct();

  void setDetectorAttributes();
  void listVolumes();

  G4LogicalVolume* FindLogicalVolume(const G4String& vn);
  G4LogicalVolume* FindLogicalVolumeCopy(const G4String& vn,int copyNumber);
  int		   GetNumberOfCopies(const G4String& vn);

  TGeoManager * getGeoMgr()       const {return geoMgr_;}
  TG4RootNavMgr * getRootMgr()    const {return rootMgr_;};

  //void setTpcFidHitPtr(tpcFidHit* hitPtr) {tpcFidSD_->setHitPtr(hitPtr);}
  //void setMINDHitPtr(MINDHit* hitPtr) {MIND_SD_->setHitPtr(hitPtr);}
  //void setSDHitPtr(SDHit* hitPtr) {if(DetectorSD_)DetectorSD_->setHitPtr(hitPtr);}

  //get hit collections
  HitCollection * getHitCollections(int i);

  HitCollection * getTpcHitCollection();
  //for the scintillator, just use the first SD as the hits are a static member
  ScintHitCollection * getScintHitCollection();

  void clearHitPtrs();

  Bool_t GetValueUsingRootGeom(){return usingRootFile;};
 
 private:
  void MINDVisSetup(std::string detectorName);
  void setMINDSD(std::string detectorName);

  G4VPhysicalVolume *           World_;
  TGeoManager *			geoMgr_;
  TG4RootNavMgr *		rootMgr_ ;
  TG4RootNavigator *		rootNav_;
  TG4RootDetectorConstruction *	rootDet_;

  G4GDMLParser                 gdmlParser_;

  G4SDManager* 	               SDMgr_;

  //vector of sensitve detectors
  std::vector<DetectorSD*> detectorSDVector;

  std::string rootFile;

  //MINDSensitiveDetector*       	MIND_SD_;
  TpcFidSensitiveDetector*   			tpcFidSD_;
  ScintillatorSensitiveDetector * 		scintSD_;
  std::vector<ScintillatorSensitiveDetector*>	scintSDVector_;

  //Magnetic Field Strength
  G4double MagFieldStength;
  MagneticField * fpMagField_;
  MagneticField * fpMindMagField_;

  //using root or gdml tag
  int usingRootFile;

  //-----------Logical-Physical volumes-------------------

  //Rock
  G4Tubs * Rock_Solid;
  G4LogicalVolume * Rock_Logic;
  G4VPhysicalVolume * Rock_Physic;
  
  //Cavity
  G4Box * CavityND_Box;
  G4LogicalVolume * CavityND_Logic;
  G4VPhysicalVolume * CavityND_Physic;

  //TPCFiducial
  G4String TPC_Name;
  G4Box * TPCFiducial_Box;
  G4LogicalVolume * TPCFiducial_Logic;
  G4VPhysicalVolume * TPCFiducial_Physic; 

  //---------Materials--------------

  //Rock
  G4Material * Rock_Material;
  G4int Rock_Nel;
  G4String Rock_MaterialName;
  G4double Rock_Density;

  //Cavity
  G4Material * Cavity_Material;
  G4double Cavity_Nel;
  G4String Cavity_MaterialName;
  G4double Cavity_Density;

  //TPCFiducial
  G4Material * TPCFiducial_Material;
  G4double TPCFiducial_Nel;
  G4String TPCFiducial_MaterialName;
  G4double TPCFiducial_Density;

  //------------Elements--------------

  G4Element * Element_Si;
  G4Element * Element_O;
  G4Element * Element_N;
  G4Element * Element_Ar;
  G4Element * Element_Al;
  G4Element * Element_C;
  G4Element * Element_H;
  G4Element * Element_Pb;
  G4Element * Element_Fe;


};

#endif


