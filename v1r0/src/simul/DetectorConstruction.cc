//____________________________________________________________________________
/*!

\class    DetectorConstruction

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Apr 2015
*/
//____________________________________________________________________________
#include "DetectorConstruction.hh"
#include <G4UserLimits.hh>

DetectorConstruction::DetectorConstruction(std::string geomFile, G4double magFieldStength, int necal, TGeoManager * geoMgr) {

  //geoMgr_ = 0;
  //rootMgr_ = 0;

  rootDet_ = NULL;

  necalvol = necal;

  //is it root geometry or gdml?
  //root file 
  //if( geomFile.compare( geomFile.size()-4, 4, "root" ) ==0)
  if(geomFile.find("root") != std::string::npos)
    {
      std::cout << "\n-----------------------------------"
		<< "\n    Using root geometry file."
		<< "\n-----------------------------------"
		<< std::endl;

      usingRootFile = 1;
      geoMgr_ = geoMgr;
      rootMgr_ = TG4RootNavMgr::GetInstance(geoMgr_);
      rootDet_ = new TG4RootDetectorConstruction(geoMgr_);
      //rootDet_ = rootMgr_->GetDetConstruction();
    }

  //gdml file
  //else if ( geomFile.compare( geomFile.size()-4, 4, "gdml" ) ==0 )
  else if(geomFile.find("gdml") != std::string::npos)
    {
      std::cout << "\n-----------------------------------"
		<< "\n    Using gdml geometry file."
		<< "\n-----------------------------------"
		<< std::endl;
      
      usingRootFile = 0;
      gdmlParser_.Read(geomFile);
    }

  //use default geant geometry
  //else if ( geomFile.compare( geomFile.size()-5, 5, "geant" ) ==0 )
  else if(geomFile.find("geant") != std::string::npos)
    {
      usingRootFile = 2;
      // For the default geometry - if no gdml file is input

      //----------------------------------------------// 
      //                 MATERIALS                    //
      //----------------------------------------------//

      //-----------------ELEMENTS---------------------//
      Element_Si = new G4Element("Silicon","Si",14,28.00*CLHEP::g/CLHEP::mole);
      Element_O = new G4Element("Oxygen","O", 8, 16.00*CLHEP::g/CLHEP::mole);
      Element_N = new G4Element("Nitrogen", "N", 7, 14.0*CLHEP::g/CLHEP::mole);
      Element_Ar = new G4Element("Argon","Ar",18, 39.95*CLHEP::g/CLHEP::mole);
      Element_Al = new G4Element("Aluminium","Al",13, 26.98*CLHEP::g/CLHEP::mole);
      Element_C = new G4Element("Carbon","C",6,12.00*CLHEP::g/CLHEP::mole);
      Element_H = new G4Element("Hydrogen","H",1,1.01*CLHEP::g/CLHEP::mole);
      Element_Pb = new G4Element("Lead","Pb",82,207.2*CLHEP::g/CLHEP::mole);
      Element_Fe = new G4Element("Iron","Fe",26,55.85*CLHEP::g/CLHEP::mole);
 
      //-------------------ROCK-----------------------//
      Rock_MaterialName = "Sandstone";
      Rock_Density = 2.323*CLHEP::g/CLHEP::cm3;

      //ratios by mass - approximate Sandstone to SiO2
      //53.33% Oxygen & 46.67% Silicon
      Rock_Material = new G4Material(Rock_MaterialName.c_str(), Rock_Density,Rock_Nel = 2);
      Rock_Material->AddElement(Element_Si, 0.4667);
      Rock_Material->AddElement(Element_O, 0.5333);

      //--------------------CAVITY---------------------//
      Cavity_MaterialName = "Air";
      Cavity_Density = 0.0012*CLHEP::g/CLHEP::cm3;

      //Air
      Cavity_Material = new G4Material(Cavity_MaterialName.c_str(), Cavity_Density,Cavity_Nel = 2);
      Cavity_Material->AddElement(Element_N, 0.7);
      Cavity_Material->AddElement(Element_O, 0.3);

      //--------------------TPC Fiducial----------------//
      TPCFiducial_MaterialName = "Argos Gas";
      TPCFiducial_Density = 0.035*CLHEP::g/CLHEP::cm3;

      TPCFiducial_Material = new G4Material(TPCFiducial_MaterialName.c_str(),
					TPCFiducial_Density,TPCFiducial_Nel = 1);
      TPCFiducial_Material->AddElement(Element_Ar,1.0);

      //----------------------------------------------//
      //                  POSITIONS                   //
      //----------------------------------------------//

      G4ThreeVector CavityND_Position = G4ThreeVector(0,0,0);
      G4ThreeVector TPCEnclosure_Position = G4ThreeVector(0,0,0);
      G4ThreeVector TPCFiducial_Position = G4ThreeVector(0,0,0);
   

      //----------------------------------------------//
      //                  VOLUMES                     //
      //----------------------------------------------//

      //-----------------------------Layer 1
      G4double startAngle = 0.*CLHEP::deg;
      G4double spanningAngle = 360.*CLHEP::deg;

      Rock_Solid = new G4Tubs("World",0,30*CLHEP::m,100*CLHEP::m,startAngle,spanningAngle);
      Rock_Logic = new G4LogicalVolume( Rock_Solid, Rock_Material, "World", 0, 0, 0);
      Rock_Physic = new G4PVPlacement(0,              // no rotation
				      G4ThreeVector(), // at (0,0,0)
				      "WorldPV",       // its name
				      Rock_Logic,      // its logical volume
				      0,               // its mother  volume
				      false,           // no boolean operations
				      0);              // nofield specific to volume
/*
      //----------------------------Layer 2
      CavityND_Box = new G4Box("CavityND",CavityND_X/2.0,CavityND_Y/2.0,CavityND_Z/2.0);
      CavityND_Logic = new G4LogicalVolume( CavityND_Box, Cavity_Material, "CavityND", 0, 0, 0);
      CavityND_Physic = new G4PVPlacement(0,                // no rotation
					  CavityND_Position,  // position
					  "CavityNDPV",       // its name
					  CavityND_Logic,     // its logical volume
					  Rock_Physic,      // its mother  volume
					  false,            // no boolean operations
					  0);               // nofield specific to volume

*/

      //----------------------------Layer 3
      TPCFiducial_Box = new G4Box("TPCFiducial",1.2*CLHEP::m,1.2*CLHEP::m,1.5*CLHEP::m);
      TPCFiducial_Logic = new G4LogicalVolume(TPCFiducial_Box,TPCFiducial_Material,"TPCFiducial");

      TPCFiducial_Physic = new G4PVPlacement(0,                         // rotation
					     TPCFiducial_Position,      // translation
					     "TPCFiducialPV",           // its name
					     TPCFiducial_Logic,         // its logical volume
					     Rock_Physic,               // its physical mother volume
					     false,                     // no boolean operations
					     0);                        // no particular field 

  }
  else{
    std::cout << "\n-----------------------------------"
	      << "\n         No geometry set!          "
	      << "\n-----------------------------------"
	      << std::endl;
  }

  //set the mag field strength
  MagFieldStength = magFieldStength*(CLHEP::tesla);

  //set the manager
  geoMgr_ = geoMgr;
}

DetectorConstruction::DetectorConstruction(std::string geomFile, G4double magFieldStength, int necal) {

  necalvol = necal;

  //if ( geomFile.compare( geomFile.size()-4, 4, "gdml" ) ==0 )
  if(geomFile.find("gdml") != std::string::npos)
    {
      std::cout << "\n-----------------------------------"
		<< "\n    Using gdml geometry file."
		<< "\n-----------------------------------"
		<< std::endl;

      usingRootFile = 0;
      gdmlParser_.Read(geomFile);
    }

  //use default geant geometry
  //else if ( geomFile.compare( geomFile.size()-5, 5, "geant" ) ==0 )
  else if(geomFile.find("geant") != std::string::npos)
    {
      usingRootFile = 2;
      // For the default geometry - if no gdml file is input

      //----------------------------------------------// 
      //                 MATERIALS                    //
      //----------------------------------------------//

      //-----------------ELEMENTS---------------------//
      Element_Si = new G4Element("Silicon","Si",14,28.00*CLHEP::g/CLHEP::mole);
      Element_O = new G4Element("Oxygen","O", 8, 16.00*CLHEP::g/CLHEP::mole);
      Element_N = new G4Element("Nitrogen", "N", 7, 14.0*CLHEP::g/CLHEP::mole);
      Element_Ar = new G4Element("Argon","Ar",18, 39.95*CLHEP::g/CLHEP::mole);
      Element_Al = new G4Element("Aluminium","Al",13, 26.98*CLHEP::g/CLHEP::mole);
      Element_C = new G4Element("Carbon","C",6,12.00*CLHEP::g/CLHEP::mole);
      Element_H = new G4Element("Hydrogen","H",1,1.01*CLHEP::g/CLHEP::mole);
      Element_Pb = new G4Element("Lead","Pb",82,207.2*CLHEP::g/CLHEP::mole);
      Element_Fe = new G4Element("Iron","Fe",26,55.85*CLHEP::g/CLHEP::mole);
 
      //-------------------ROCK-----------------------//
      Rock_MaterialName = "Sandstone";
      Rock_Density = 2.323*CLHEP::g/CLHEP::cm3;

      //ratios by mass - approximate Sandstone to SiO2
      //53.33% Oxygen & 46.67% Silicon
      Rock_Material = new G4Material(Rock_MaterialName.c_str(), Rock_Density,Rock_Nel = 2);
      Rock_Material->AddElement(Element_Si, 0.4667);
      Rock_Material->AddElement(Element_O, 0.5333);

      //--------------------CAVITY---------------------//
      Cavity_MaterialName = "Air";
      Cavity_Density = 0.0012*CLHEP::g/CLHEP::cm3;

      //Air
      Cavity_Material = new G4Material(Cavity_MaterialName.c_str(), Cavity_Density,Cavity_Nel = 2);
      Cavity_Material->AddElement(Element_N, 0.7);
      Cavity_Material->AddElement(Element_O, 0.3);

      //--------------------TPC Fiducial----------------//
      TPCFiducial_MaterialName = "Argos Gas";
      TPCFiducial_Density = 0.035*CLHEP::g/CLHEP::cm3;

      TPCFiducial_Material = new G4Material(TPCFiducial_MaterialName.c_str(),
					TPCFiducial_Density,TPCFiducial_Nel = 1);
      TPCFiducial_Material->AddElement(Element_Ar,1.0);

      //----------------------------------------------//
      //                  POSITIONS                   //
      //----------------------------------------------//

      G4ThreeVector CavityND_Position = G4ThreeVector(0,0,0);
      G4ThreeVector TPCEnclosure_Position = G4ThreeVector(0,0,0);
      G4ThreeVector TPCFiducial_Position = G4ThreeVector(0,0,0);
   

      //----------------------------------------------//
      //                  VOLUMES                     //
      //----------------------------------------------//

      //-----------------------------Layer 1
      G4double startAngle = 0.*CLHEP::deg;
      G4double spanningAngle = 360.*CLHEP::deg;

      Rock_Solid = new G4Tubs("World",0,30*CLHEP::m,100*CLHEP::m,startAngle,spanningAngle);
      Rock_Logic = new G4LogicalVolume( Rock_Solid, Rock_Material, "World", 0, 0, 0);
      Rock_Physic = new G4PVPlacement(0,              // no rotation
				      G4ThreeVector(), // at (0,0,0)
				      "WorldPV",       // its name
				      Rock_Logic,      // its logical volume
				      0,               // its mother  volume
				      false,           // no boolean operations
				      0);              // nofield specific to volume
/*
      //----------------------------Layer 2
      CavityND_Box = new G4Box("CavityND",CavityND_X/2.0,CavityND_Y/2.0,CavityND_Z/2.0);
      CavityND_Logic = new G4LogicalVolume( CavityND_Box, Cavity_Material, "CavityND", 0, 0, 0);
      CavityND_Physic = new G4PVPlacement(0,                // no rotation
					  CavityND_Position,  // position
					  "CavityNDPV",       // its name
					  CavityND_Logic,     // its logical volume
					  Rock_Physic,      // its mother  volume
					  false,            // no boolean operations
					  0);               // nofield specific to volume

*/

      //----------------------------Layer 3
      TPCFiducial_Box = new G4Box("TPCFiducial",1.2*CLHEP::m,1.2*CLHEP::m,1.5*CLHEP::m);
      TPCFiducial_Logic = new G4LogicalVolume(TPCFiducial_Box,TPCFiducial_Material,"TPCFiducial");

      TPCFiducial_Physic = new G4PVPlacement(0,                         // rotation
					     TPCFiducial_Position,      // translation
					     "TPCFiducialPV",           // its name
					     TPCFiducial_Logic,         // its logical volume
					     Rock_Physic,               // its physical mother volume
					     false,                     // no boolean operations
					     0);                        // no particular field 

  }
  else{
    std::cout << "\n-----------------------------------"
	      << "\n         No geometry set!          "
	      << "\n-----------------------------------"
	      << std::endl;
  }
  //set the mag field strength
  MagFieldStength = magFieldStength*(CLHEP::tesla);
}

DetectorConstruction::~DetectorConstruction(){
  if ( rootDet_ ) delete rootDet_;
  if(fpMagField_) delete fpMagField_;
  
  //delete SDs
  for(unsigned int i=0;i<detectorSDVector.size();i++){
	delete detectorSDVector.at(i);
  }
  //delete scintSD vector
  for(unsigned int i =0;i<scintSDVector_.size();i++){
	delete scintSDVector_.at(i);
  }

  //if ( geoMgr_ )  delete geoMgr_;
  //if ( rootMgr_ ) delete rootMgr_;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

  if(usingRootFile == 1){
    World_ = rootDet_->GetTopPV();
    this->setDetectorAttributes();
  }

  else if(usingRootFile == 0) {

    World_ = gdmlParser_.GetWorldVolume();
    this->setDetectorAttributes();
  }

  else if(usingRootFile == 2) {
    
    World_ =  Rock_Physic;
    this->setDetectorAttributes();
  }
  
  else { World_ = NULL;};
  
  return World_;
}

G4LogicalVolume* DetectorConstruction::FindLogicalVolume( const G4String& vn ) {

  G4LogicalVolume * vol = World_->GetLogicalVolume();
  G4LogicalVolume * foundVol = NULL;
  
  std::vector<G4LogicalVolume *> daughterVolVtr;
  std::vector<G4LogicalVolume *> nextDaughterVolVtr;
  
  bool allChecked = false;
  
  //add first daughters to vector
  for(int i=0; i<vol->GetNoDaughters();i++) {
    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();
    daughterVolVtr.push_back(daughter);
    std::string lv_name = daughter->GetName();
    if(lv_name.compare( 0,vn.size(), vn ) ==0) {
      foundVol = daughter;
      return foundVol;
    }
    else{
      foundVol = NULL;
    }
  }

  //if no daughters then end
  if( daughterVolVtr.size()==0 )allChecked = true;
  
  while(!allChecked){
    
    if(daughterVolVtr.size()==0)break;
    
    //loop over this vector to check for volumes and add second 
    for(unsigned int i=0; i<daughterVolVtr.size();i++){
      if(daughterVolVtr.at(i)){
	std::string lv_name = daughterVolVtr.at(i)->GetName();
	if(lv_name.compare( 0,vn.size(), vn ) ==0) {
	  foundVol = daughterVolVtr.at(i);
	  return foundVol;
	}
	else{
	  foundVol = NULL;
	}
	//std::cout << "\nNumber of daughters for volume: " << lv_name << " is: " << daughter->GetNoDaughters();
	if(daughterVolVtr.at(i)->GetNoDaughters() > 0){
	  for(int j=0;j<daughterVolVtr.at(i)->GetNoDaughters();j++){
	    nextDaughterVolVtr.push_back(daughterVolVtr.at(i)->GetDaughter(j)->GetLogicalVolume());
	  }
	}
      }
    }
    //clear the parent volumes
    daughterVolVtr.resize(0);
    
    //end if no more daughters
    if( nextDaughterVolVtr.size()==0)allChecked=true;
    else daughterVolVtr.swap(nextDaughterVolVtr);
    
  }
/*
  G4LogicalVolume * vol = World_->GetLogicalVolume();
  G4LogicalVolume * foundVol;

  for(int i=0; i<vol->GetNoDaughters();i++) {

    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();

    if(daughter){
      std::string lv_name = daughter->GetName();
      //std::cout << "\n\t -- " << lv_name;
      
      if(daughter->GetNoDaughters() > 0){
	vol = daughter;
	i=-1;
      }
      if(lv_name.compare( 0,vn.size(), vn ) ==0) {
	foundVol = daughter;
	return foundVol;
      }
      else{
	foundVol = NULL;
      }
    }
    //std::cout << "\nVol name: " << vol->GetName() << ", daughters: " << vol->GetNoDaughters()
    //	      << "\nDaughter name: " << daughter->GetName() << ", daughters: " << daughter->GetNoDaughters()
    //	      << "\ni = :" << i << std::endl;
  }
*/
  return foundVol;
}

G4LogicalVolume* DetectorConstruction::FindLogicalVolumeCopy( const G4String& vn,int copyNumber ) {

  //std::cout << "\n---------------------"
  //          << "\nVolumes: ";
  
  G4LogicalVolume * vol = World_->GetLogicalVolume();
  G4LogicalVolume * foundVol;

  int count = 0;

  for(int i=0; i<vol->GetNoDaughters();i++) {

    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();

    if(daughter){
      std::string lv_name = daughter->GetName();
      
      if(daughter->GetNoDaughters() > 0){
	vol = daughter;
	i=-1;
      }
      if( lv_name.compare( 0,vn.size(), vn ) ==0) {
	if(count == copyNumber){
	  foundVol = daughter;
	  return foundVol;
	}
	count++;
      }
      else{
	foundVol = NULL;
      }
    }
  }

  return foundVol;
}
int DetectorConstruction::GetNumberOfCopies( const G4String& vn) {

  G4LogicalVolume * vol = World_->GetLogicalVolume();
  int count = 0;

  for(int i=0; i<vol->GetNoDaughters();i++) {

    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();
    
    if(daughter){
      std::string lv_name = daughter->GetName();
      if(daughter->GetNoDaughters() > 0){
	vol = daughter;
	i=-1;
      }
      if( lv_name.compare( 0,vn.size(), vn ) ==0) {
	count++;
      }
    }
  }

  return count;
}

void DetectorConstruction::listVolumes(){

  std::cout << "\n---------------------"
            << "\nVolumes: ";

  G4LogicalVolume * vol = World_->GetLogicalVolume();

  std::vector<G4LogicalVolume *> daughterVolVtr;
  std::vector<G4LogicalVolume *> nextDaughterVolVtr;

  bool allChecked = false;

  //add first daughters to vector
  for(int i=0; i<vol->GetNoDaughters();i++) {
    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();
    daughterVolVtr.push_back(daughter);
    std::string lv_name = daughter->GetName();
    std::cout << "\n\t -- " << lv_name;
  }

  //if no daughters then end
  if( daughterVolVtr.size()==0 )allChecked = true;
  
  while(!allChecked){

    if(daughterVolVtr.size()==0)break;
    
    //loop over this vector to check for volumes and add second 
    for(unsigned int i=0; i<daughterVolVtr.size();i++){
      if(daughterVolVtr.at(i)){
	std::string lv_name = daughterVolVtr.at(i)->GetName();
	std::cout << "\n\t -- " << lv_name;
	//std::cout << "\nNumber of daughters for volume: " << lv_name << " is: " << daughter->GetNoDaughters();
	if(daughterVolVtr.at(i)->GetNoDaughters() > 0){
	  for(int j=0;j<daughterVolVtr.at(i)->GetNoDaughters();j++){
	    nextDaughterVolVtr.push_back(daughterVolVtr.at(i)->GetDaughter(j)->GetLogicalVolume());
	  }
	}
      }
    }
    //clear the parent volumes
    daughterVolVtr.resize(0);

    //end if no more daughters
    if( nextDaughterVolVtr.size()==0)allChecked=true;
    else daughterVolVtr.swap(nextDaughterVolVtr);

  }
/*
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
 */
  std::cout << "\n---------------------\n";
}

void DetectorConstruction::setDetectorAttributes(){

  fpMagField_ = NULL;
  
  //------------------------------------------------------------------//
  //                     Sensitive Detector Parts                     //
  //------------------------------------------------------------------//

  G4LogicalVolume * vol = World_->GetLogicalVolume();

  std::vector<G4LogicalVolume *> daughterVolVtr;
  std::vector<G4LogicalVolume *> nextDaughterVolVtr;

  //add first daughters to vector
  for(int i=0; i<vol->GetNoDaughters();i++) {
    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();
    daughterVolVtr.push_back(daughter);
  }

  for(unsigned int i=0; i<daughterVolVtr.size();i++){
    std::string lv_name = daughterVolVtr.at(i)->GetName();
    std::cout << "\n\t -- " << lv_name;
    //if(lv_name.find("Scint") != std::string::npos){
    //ScintillatorSensitiveDetector *scint = new ScintillatorSensitiveDetector(lv_name);
    //scintSDVector_.push_back(scint);
    //}
    
    for(int j=0;j<daughterVolVtr.at(i)->GetNoDaughters();j++){
      std::string lv_name1 = daughterVolVtr.at(i)->GetDaughter(j)->GetLogicalVolume()->GetName();
      std::cout << "\n\t -- " << lv_name1;
      //G4LogicalVolume * daughter = daughterVolVtr.at(i)->GetDaughter(j)->GetLogicalVolume();
      if(lv_name1.find("Scint") != std::string::npos){
	ScintillatorSensitiveDetector *scint = new ScintillatorSensitiveDetector(lv_name1);
	scintSDVector_.push_back(scint);
      }
      
      //nextDaughterVolVtr.push_back(daughter);
      
    }
  }
  
  /*
  std::cout << "---------------- " << std::endl;
  for(unsigned int i=0; i<nextDaughterVolVtr.size();i++){
    std::string lv_name = nextDaughterVolVtr.at(i)->GetName();
    ScintillatorSensitiveDetector *scint = new ScintillatorSensitiveDetector(lv_name);
    scintSDVector_.push_back(scint);
    std::cout << "\n\t -- " << lv_name;
  }
  */
  
  G4SDManager* SDMgr_ = G4SDManager::GetSDMpointer();
  
  //add the sensitive detectors
  //DetectorSD_ = new DetectorSD(SDName.c_str(),geoMgr_);
  //tpcFidSD_   = new TpcFidSensitiveDetector("tpcFid");
  //tpcFidSD_   = new TpcFidSensitiveDetector("innerVessel");
  tpcFidSD_     = new DetectorSD("innerVessel");
  detectorSDVector.push_back(tpcFidSD_);
  //add other detectors here...

  //loop over SD vector to set all detector attributes- other than scintillator
  for(unsigned int i=0;i<detectorSDVector.size();i++){
    
    DetectorSD * tmpSD = detectorSDVector.at(i);
    G4String tmpSDName = tmpSD->GetName() + "Volume";
    //cout << "tmpSDName = " << tmpSDName << endl;
    if(tmpSD){
      SDMgr_->AddNewDetector(tmpSD);
      
      //Find the sensitive detector logical volume  	  	
      G4LogicalVolume * tmpSD_lv = FindLogicalVolume(tmpSDName);
      
      if ( tmpSD_lv ) {
	//tmpSD_lv->SetVisAttributes(G4Colour::Blue());
	tmpSD_lv->SetSensitiveDetector(tmpSD);
	//tmpSD_lv->SetUserLimits(new G4UserLimits(GetSteppingLimit()));
	tmpSD_lv->SetUserLimits(new G4UserLimits(5.0*CLHEP::mm));
	std::cout << "\n*****************************************************"
		  << "\nFound detector with name: " << tmpSD_lv->GetName() 
		  << " -- SD set!" 
		  << "\n*****************************************************" <<std::endl;
      
	//Add the magnetic field for the TPC only
	//magnetise the sensitive detector
	if(tmpSD->GetName() == "tpcFid" || tmpSD->GetName() == "innerVessel"){
	  fpMagField_ = new MagneticField(G4ThreeVector(MagFieldStength,0,0),tmpSD_lv);
	  std::cout << "\n*****************************************************"
		    << "\nFound TPC! Setting Magnetic field strength to: " << MagFieldStength*1/(CLHEP::tesla) << " T"
		    << "\n*****************************************************" <<std::endl;
	}	
      }
      else{
	std::cout << "\n*****************************************************"
		  << "\nCannot find sensitive detector with name: " << tmpSDName 
		  << " -- No SD set!" 		
		  << "\n*****************************************************" <<std::endl;
      }     
    }
    else {
      std::cout << "\n*****************************************************"
		<< "\nCannot find detector with name: " << tmpSDName 
		<< " -- No SD set!" 		
		<< "\n*****************************************************" <<std::endl;      
      listVolumes();
      
    }    
  }


   //loop over SD vector to set all detector attributes- other than scintillator
  for(unsigned int i=0;i<scintSDVector_.size();i++){
    
    ScintillatorSensitiveDetector * tmpSD = scintSDVector_.at(i);
    G4String tmpSDName = tmpSD->GetName();// + "Volume";
    std::cout << "tmpSDName = " << tmpSDName << std::endl;
    if(tmpSD){
      SDMgr_->AddNewDetector(tmpSD);
      
      //Find the sensitive detector logical volume  	  	
      G4LogicalVolume * tmpSD_lv = FindLogicalVolume(tmpSDName);
      //for(unsigned int j=0; j<nextDaughterVolVtr.size();j++){
      //G4LogicalVolume * tmpSD_lv = nextDaughterVolVtr.at(j);
	if ( tmpSD_lv ) {
	  //tmpSD_lv->SetVisAttributes(G4Colour::Blue());
	  tmpSD_lv->SetSensitiveDetector(tmpSD);
	  //tmpSD_lv->SetUserLimits(new G4UserLimits(GetSteppingLimit()));
	  //tmpSD_lv->SetUserLimits(new G4UserLimits(1.0*CLHEP::mm));
	  std::cout << "\n*****************************************************"
		    << "\nFound detector with name: " << tmpSD_lv->GetName() 
		    << " -- SD set!" 
		    << "\n*****************************************************" <<std::endl;
	  
	  //Add the magnetic field for the ECAL only
	  //magnetise the sensitive detector
	  //if(tmpSD->GetName() == "Ecal"){
	    //fecalMagField_ = new MagneticField(G4ThreeVector(MagFieldStength,0,0),tmpSD_lv);
	    //std::cout << "\n*****************************************************"
	  //	      << "\nFound Ecal! Setting Magnetic field strength to: " << MagFieldStength*1/(CLHEP::tesla) << " T"
	  //	      << "\n*****************************************************" <<std::endl;
	  //}	
	}
	else{
	  std::cout << "\n*****************************************************"
		    << "\nCannot find sensitive detector with name: " << tmpSDName 
		    << " -- No SD set!" 		
		    << "\n*****************************************************" <<std::endl;
	}     
	//}
    }
    else {
      std::cout << "\n*****************************************************"
		<< "\nCannot find detector with name: " << tmpSDName 
		<< " -- No SD set!" 		
		<< "\n*****************************************************" <<std::endl;      
      listVolumes();
    }    
  }


  //the scintillator must have its own vector as it must have a SD for each layer - do this separately
  /*
  char ecalstring[300];
  for(int i=1; i < 7; i++){
    sprintf(ecalstring, "Ecal%i", i);
    scintSD_   = new ScintillatorSensitiveDetector(ecalstring,i);
    G4String tmpSDName = scintSD_->GetName() + "Volume";
    G4LogicalVolume * tmpSD_lv = FindLogicalVolume(tmpSDName);
    if(tmpSD_lv){
      SDMgr_->AddNewDetector(scintSD_);
      scintSDVector_.push_back(scintSD_);
      tmpSD_lv->SetSensitiveDetector(scintSD_);
      std::cout << "\n*****************************************************"
		<< "\nFound detector with name: " << tmpSDName 
		<< " -- SD set!" 
		<< "\n*****************************************************" <<std::endl;
    }
    else{
      std::cout << "\n*****************************************************"
		<< "\nCannot find detector with name: " <<  tmpSDName
		<< " -- No SD set!" 		
		<< "\n*****************************************************" <<std::endl;
    }
  }
  */




  /*
  char ecalstring[300];
  bool found = true;

  sprintf(ecalstring, "Ecal");
  scintSD_   = new ScintillatorSensitiveDetector(ecalstring,0);
  G4String ftmpSDName = scintSD_->GetName() + "Volume";
  G4LogicalVolume * ftmpSD_lv = FindLogicalVolume(ftmpSDName);
  if(ftmpSD_lv){
    SDMgr_->AddNewDetector(scintSD_);
    scintSDVector_.push_back(scintSD_);
    ftmpSD_lv->SetSensitiveDetector(scintSD_);
    std::cout << "\n*****************************************************"
	      << "\nFound detector with name: " << ftmpSDName 
	      << " -- SD set!" 
	      << "\n*****************************************************" <<std::endl;
  }
  else{
    for(int i=1; i < necalvol+1; i++){
      int j=0;
      while(true){
	sprintf(ecalstring, "Ecal%i_%i",i,j);
	scintSD_   = new ScintillatorSensitiveDetector(ecalstring,i,j);
	G4String tmpSDName = scintSD_->GetName() + "Volume";
	G4LogicalVolume * tmpSD_lv = FindLogicalVolume(tmpSDName);
	if(tmpSD_lv){
	  SDMgr_->AddNewDetector(scintSD_);
	  scintSDVector_.push_back(scintSD_);
	  tmpSD_lv->SetSensitiveDetector(scintSD_);
	  std::cout << "\n*****************************************************"
		    << "\nFound detector with name: " << tmpSDName 
		    << " -- SD set!" 
		    << "\n*****************************************************" <<std::endl;
	  j++;
	}
	else{
	  if(j == 0)
	    std::cout << "\n*****************************************************"
		      << "\nCannot find detector with name: " <<  tmpSDName
		      << " -- Will try another Ecal configuration!" 		
		      << "\n*****************************************************" <<std::endl;
	  found = false;
	  break;
	}
      }
      if(!found && j == 0){
	sprintf(ecalstring, "Ecal%i", i);
	scintSD_   = new ScintillatorSensitiveDetector(ecalstring,i);
	G4String tmpSDName = scintSD_->GetName() + "Volume";
	G4LogicalVolume * tmpSD_lv = FindLogicalVolume(tmpSDName);
	if(tmpSD_lv){
	  SDMgr_->AddNewDetector(scintSD_);
	  scintSDVector_.push_back(scintSD_);
	  tmpSD_lv->SetSensitiveDetector(scintSD_);
	  std::cout << "\n*****************************************************"
		    << "\nFound detector with name: " << tmpSDName 
		    << " -- SD set!" 
		    << "\n*****************************************************" <<std::endl;
	}
	else{
	  std::cout << "\n*****************************************************"
		    << "\nCannot find detector with name: " <<  tmpSDName
		    << " -- No SD set!" 		
		    << "\n*****************************************************" <<std::endl;
	}
      }
    }
    
  }

  */

  /*
  int j=0;
  while(true){
    
    scintSD_   = new ScintillatorSensitiveDetector("scint",j);
    G4String tmpSDName = scintSD_->GetName() + "Volume";
    G4LogicalVolume * tmpSD_lv = FindLogicalVolumeCopy(tmpSDName,j);
    if(tmpSD_lv==NULL){
      delete scintSD_;
      break;
    }
    else{
      //scintSD_->SetLayerNumber(j);
      SDMgr_->AddNewDetector(scintSD_);
      scintSDVector_.push_back(scintSD_);
      tmpSD_lv->SetSensitiveDetector(scintSD_);
      std::cout << "\n*****************************************************"
		<< "\nFound detector with name: " << tmpSD_lv->GetName() 
		<< " -- SD set!" 
		<< "\n*****************************************************" <<std::endl;
      j++;
    }
  }
  */
}

HitCollection * DetectorConstruction::getHitCollections(int i){
  if(detectorSDVector.at(i)){
    return detectorSDVector.at(i)->getHitCollectionPtr();
  }
  else return NULL;
}

HitCollection * DetectorConstruction::getTpcHitCollection(){
  if(tpcFidSD_){
    return tpcFidSD_->getHitCollectionPtr();
  }
  else return NULL;
}

ScintHitCollection * DetectorConstruction::getScintHitCollection(){  
  if(scintSDVector_.size()>0){
    return scintSDVector_.at(0)->getHitCollectionPtr();
  }
  else return NULL;
}

ScintHitCollection * DetectorConstruction::getScintHitCollection(int i){
  if(scintSDVector_.at(i)){
    return scintSDVector_.at(i)->getHitCollectionPtr();
  }
  else return NULL;
}

void DetectorConstruction::clearHitPtrs(){

  for(unsigned int i=0;i<detectorSDVector.size();i++){
    detectorSDVector.at(i)->clear();
  }
  //only need to clear it once as its a static member
  if(scintSDVector_.size()>0)scintSDVector_.at(0)->clear();
  //for(unsigned int i=0;i<scintSDVector_.size();i++){
  //scintSDVector_.at(i)->clear();
  //}
}

