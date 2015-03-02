//____________________________________________________________________________
/*!

\class    MyUserPostDetConstruction

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2013

*/
//____________________________________________________________________________
#include "MyUserPostDetConstruction.hh"
//#include "DetectorConstruction.hh"

MyUserPostDetConstruction::MyUserPostDetConstruction(std::string sdName,G4double magFieldStength,TGeoManager * geoMgr) {

  //set the sensitive detector name
  SDName = sdName;

  //set the mag field strength
  MagFieldStength = magFieldStength;

  //set the manager
  geoMgr_ = geoMgr;
}

MyUserPostDetConstruction::~MyUserPostDetConstruction(){

  if(fpMagField_) delete fpMagField_;
  if(DetectorSD_) delete DetectorSD_;
}

/*
MyUserPostDetConstruction * MyUserPostDetConstruction::GetInstance(std::string sdName,G4double magFieldStength,TGeoManager * geoMgr){

  // Returns self pointer.
   if (fgInstance) return fgInstance;
   fgInstance = new MyUserPostDetConstruction(sdName,magFieldStength,geoMgr);
   return fgInstance;

}
*/
void MyUserPostDetConstruction::Initialize(TG4RootDetectorConstruction *det_){
  G4cout << "\n----------------------------------------"
	 << "\nUserPostDetConstruction initializing ..." 
	 << "\n----------------------------------------\n";
  //TGeoManager *geom = det_->GetGeometryManager();

  World_ = det_->GetTopPV();

  G4cout << "\nMyUserPostDetConstruction::PV Name " << det_->GetTopPV()->GetName() << G4endl;
  
  //------------------------------------------------------------------//
  //                     Sensitive Detector Parts                     //
  //------------------------------------------------------------------//

  SDMgr_ = G4SDManager::GetSDMpointer();

  DetectorSD_ = new DetectorSD(SDName.c_str(),geoMgr_);
  
  SDMgr_->AddNewDetector(DetectorSD_);

  //Find the TPC_fiducial volume
  SD_lv = FindLogicalVolume(SDName.c_str());

  if ( SD_lv ) {
    SD_lv->SetVisAttributes(G4Colour::Blue());
    SD_lv->SetSensitiveDetector(DetectorSD_);

    std::cout << "\n*****************************************************"
		<< "\nFound detector with name: " << SDName.c_str() 
		<< " -- SD set!" 
		<< "\n*****************************************************" <<std::endl;
  
    //Add the magnetic field 
    //magnetise the sensitive detector
    fpMagField_ = new MagneticField(G4ThreeVector(MagFieldStength*tesla,0,0),SD_lv);

  }
 
   else {
      
      	std::cout << "\n*****************************************************"
			<< "\nCannot find detector with name: " << SDName.c_str() 
			<< " -- No SD set!" 		
			<< "\n*****************************************************" <<std::endl;
      	SD_lv = 0;
	
	this->listVolumes();
  	fpMagField_ = NULL;
    }

}


G4LogicalVolume* MyUserPostDetConstruction::FindLogicalVolume( const G4String& vn ) {

  //std::cout << "\n---------------------"
  //          << "\nVolumes: ";

  G4LogicalVolume * vol = World_->GetLogicalVolume();
  G4LogicalVolume * foundVol = World_->GetLogicalVolume();

  for(int i=0; i<vol->GetNoDaughters();i++) {

    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();
    if(daughter){
      std::string lv_name = daughter->GetName();
      //std::cout << "\n\t -- " << lv_name;
      
      if(daughter->GetNoDaughters() > 0){
	vol = daughter;
	i=-1;
      }
      if( lv_name == vn ) {
	foundVol = daughter;
      }
      else{
	foundVol = NULL;
      }
    }
    //std::cout << "\nVol name: " << vol->GetName() << ", daughters: " << vol->GetNoDaughters()
    //	      << "\nDaughter name: " << daughter->GetName() << ", daughters: " << daughter->GetNoDaughters()
    //	      << "\ni = :" << i << std::endl;
  }

  return foundVol;
}

void MyUserPostDetConstruction::listVolumes(){

  std::cout << "\n---------------------"
            << "\nVolumes: ";

  G4LogicalVolume * vol = World_->GetLogicalVolume();

  for(int i=0; i<vol->GetNoDaughters();i++) {
    
    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();
    if(daughter){
      std::string lv_name = daughter->GetName();
      std::cout << "\n\t -- " << lv_name;
      //std::cout << "\nNumber of daughters for volume: " << lv_name << " is: " << daughter->GetNoDaughters();
      if(daughter->GetNoDaughters() > 0){
	vol = daughter;
	i=-1;
      }
    }
  }
 
  std::cout << "\n---------------------\n";
}
