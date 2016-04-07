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
\last update Nov 2015
*/
//____________________________________________________________________________
#include "DetectorConstruction.hh"
#include <G4UserLimits.hh>

DetectorConstruction::DetectorConstruction(std::string geomFile, G4double magFieldStength, TGeoManager * geoMgr) {
  rootDet_ = NULL;

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
      /****** OBSOLETE FOR NOW ******/
      usingRootFile = 2;
      // For the default geometry - if no gdml file is input
      /*
      //----------------------------------------------// 
      //                 MATERIALS                    //
      //----------------------------------------------//

      //-----------------ELEMENTS---------------------//
      G4Element *Element_Si = new G4Element("Silicon","Si",14,28.00*CLHEP::g/CLHEP::mole);
      G4Element *Element_O = new G4Element("Oxygen","O", 8, 16.00*CLHEP::g/CLHEP::mole);
      G4Element *Element_N = new G4Element("Nitrogen", "N", 7, 14.0*CLHEP::g/CLHEP::mole);
      G4Element *Element_Ar = new G4Element("Argon","Ar",18, 39.95*CLHEP::g/CLHEP::mole);
      //G4Element *Element_Al = new G4Element("Aluminium","Al",13, 26.98*CLHEP::g/CLHEP::mole);
      //G4Element *Element_C = new G4Element("Carbon","C",6,12.00*CLHEP::g/CLHEP::mole);
      //G4Element *Element_H = new G4Element("Hydrogen","H",1,1.01*CLHEP::g/CLHEP::mole);
      //G4Element *Element_Pb = new G4Element("Lead","Pb",82,207.2*CLHEP::g/CLHEP::mole);
      //G4Element *Element_Fe = new G4Element("Iron","Fe",26,55.85*CLHEP::g/CLHEP::mole);
 
      //-------------------ROCK-----------------------//
      G4String Rock_MaterialName = "Sandstone";
      G4double Rock_Density = 2.323*CLHEP::g/CLHEP::cm3;
      G4int Rock_Nel = 2;
      //ratios by mass - approximate Sandstone to SiO2
      //53.33% Oxygen & 46.67% Silicon
      G4Material *Rock_Material = new G4Material(Rock_MaterialName.c_str(), Rock_Density,Rock_Nel = 2);
      Rock_Material->AddElement(Element_Si, 0.4667);
      Rock_Material->AddElement(Element_O, 0.5333);

      //--------------------CAVITY---------------------//
      G4String Cavity_MaterialName = "Air";
      G4double Cavity_Density = 0.0012*CLHEP::g/CLHEP::cm3;
      G4double Cavity_Nel = 2;
      //Air
      G4Material *Cavity_Material = new G4Material(Cavity_MaterialName.c_str(), Cavity_Density,Cavity_Nel = 2);
      Cavity_Material->AddElement(Element_N, 0.7);
      Cavity_Material->AddElement(Element_O, 0.3);

      //--------------------TPC Fiducial----------------//
      G4String TPCFiducial_MaterialName = "Argos Gas";
      G4double TPCFiducial_Density = 0.035*CLHEP::g/CLHEP::cm3;
      G4double TPCFiducial_Nel = 1;
      G4Material *TPCFiducial_Material = new G4Material(TPCFiducial_MaterialName.c_str(),
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

      G4Tubs *Rock_Solid = new G4Tubs("World",0,30*CLHEP::m,100*CLHEP::m,startAngle,spanningAngle);
      G4LogicalVolume *Rock_Logic = new G4LogicalVolume( Rock_Solid, Rock_Material, "World", 0, 0, 0);
      Rock_Physic = new G4PVPlacement(0,              // no rotation
				      G4ThreeVector(), // at (0,0,0)
				      "WorldPV",       // its name
				      Rock_Logic,      // its logical volume
				      0,               // its mother  volume
				      false,           // no boolean operations
				      0);              // nofield specific to volume
      */

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

      /*
      //----------------------------Layer 3
      G4Box *TPCFiducial_Box = new G4Box("TPCFiducial",1.2*CLHEP::m,1.2*CLHEP::m,1.5*CLHEP::m);
      G4LogicalVolume *TPCFiducial_Logic = new G4LogicalVolume(TPCFiducial_Box,TPCFiducial_Material,"TPCFiducial");

      G4VPhysicalVolume *TPCFiducial_Physic = new G4PVPlacement(0,                         // rotation
								TPCFiducial_Position,      // translation
								"TPCFiducialPV",           // its name
								TPCFiducial_Logic,         // its logical volume
								Rock_Physic,               // its physical mother volume
								false,                     // no boolean operations
								0);                        // no particular field 

      */

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
  //geoMgr_ = geoMgr;
}

DetectorConstruction::DetectorConstruction(std::string geomFile, G4double magFieldStength) {
  rootDet_ = NULL;
  
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
  else if(geomFile.find("geant") != std::string::npos)
    {
      /****** OBSOLETE FOR NOW ******/
      usingRootFile = 2;
      // For the default geometry - if no gdml file is input
      /*
      //----------------------------------------------// 
      //                 MATERIALS                    //
      //----------------------------------------------//

      //-----------------ELEMENTS---------------------//
      G4Element *Element_Si = new G4Element("Silicon","Si",14,28.00*CLHEP::g/CLHEP::mole);
      G4Element *Element_O = new G4Element("Oxygen","O", 8, 16.00*CLHEP::g/CLHEP::mole);
      G4Element *Element_N = new G4Element("Nitrogen", "N", 7, 14.0*CLHEP::g/CLHEP::mole);
      G4Element *Element_Ar = new G4Element("Argon","Ar",18, 39.95*CLHEP::g/CLHEP::mole);
      //G4Element *Element_Al = new G4Element("Aluminium","Al",13, 26.98*CLHEP::g/CLHEP::mole);
      //G4Element *Element_C = new G4Element("Carbon","C",6,12.00*CLHEP::g/CLHEP::mole);
      //G4Element *Element_H = new G4Element("Hydrogen","H",1,1.01*CLHEP::g/CLHEP::mole);
      //G4Element *Element_Pb = new G4Element("Lead","Pb",82,207.2*CLHEP::g/CLHEP::mole);
      //G4Element *Element_Fe = new G4Element("Iron","Fe",26,55.85*CLHEP::g/CLHEP::mole);
 
      //-------------------ROCK-----------------------//
      G4String Rock_MaterialName = "Sandstone";
      G4double Rock_Density = 2.323*CLHEP::g/CLHEP::cm3;
      G4int Rock_Nel = 2;
      //ratios by mass - approximate Sandstone to SiO2
      //53.33% Oxygen & 46.67% Silicon
      G4Material *Rock_Material = new G4Material(Rock_MaterialName.c_str(), Rock_Density,Rock_Nel = 2);
      Rock_Material->AddElement(Element_Si, 0.4667);
      Rock_Material->AddElement(Element_O, 0.5333);

      //--------------------CAVITY---------------------//
      G4String Cavity_MaterialName = "Air";
      G4double Cavity_Density = 0.0012*CLHEP::g/CLHEP::cm3;
      G4double Cavity_Nel = 2;
      //Air
      G4Material *Cavity_Material = new G4Material(Cavity_MaterialName.c_str(), Cavity_Density,Cavity_Nel = 2);
      Cavity_Material->AddElement(Element_N, 0.7);
      Cavity_Material->AddElement(Element_O, 0.3);

      //--------------------TPC Fiducial----------------//
      G4String TPCFiducial_MaterialName = "Argos Gas";
      G4double TPCFiducial_Density = 0.035*CLHEP::g/CLHEP::cm3;
      G4double TPCFiducial_Nel = 1;
      G4Material *TPCFiducial_Material = new G4Material(TPCFiducial_MaterialName.c_str(),
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

      G4Tubs *Rock_Solid = new G4Tubs("World",0,30*CLHEP::m,100*CLHEP::m,startAngle,spanningAngle);
      G4LogicalVolume *Rock_Logic = new G4LogicalVolume( Rock_Solid, Rock_Material, "World", 0, 0, 0);
      Rock_Physic = new G4PVPlacement(0,              // no rotation
				      G4ThreeVector(), // at (0,0,0)
				      "WorldPV",       // its name
				      Rock_Logic,      // its logical volume
				      0,               // its mother  volume
				      false,           // no boolean operations
				      0);              // nofield specific to volume

      */
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
      /*
      //----------------------------Layer 3
      G4Box *TPCFiducial_Box = new G4Box("TPCFiducial",1.2*CLHEP::m,1.2*CLHEP::m,1.5*CLHEP::m);
      G4LogicalVolume *TPCFiducial_Logic = new G4LogicalVolume(TPCFiducial_Box,TPCFiducial_Material,"TPCFiducial");

      G4VPhysicalVolume *TPCFiducial_Physic = new G4PVPlacement(0,                         // rotation
								TPCFiducial_Position,      // translation
								"TPCFiducialPV",           // its name
								TPCFiducial_Logic,         // its logical volume
								Rock_Physic,               // its physical mother volume
								false,                     // no boolean operations
								0);                        // no particular field 
      */

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
  if(rootDet_ ) delete rootDet_;
  if(fpMagField_) delete fpMagField_;
  if(rootNav_) delete rootNav_;
  if(rootMgr_) delete rootMgr_;
  if(geoMgr_) delete geoMgr_;
  if(World_) delete World_;
  //if(Rock_Physic)  delete Rock_Physic;

  //delete SDs
  for(unsigned int i=0;i<detectorSDVector.size();i++){
    delete detectorSDVector.at(i);
  }
  //delete scintSD vector
  for(unsigned int i =0;i<scintSDVector_.size();i++){
    delete scintSDVector_.at(i);
  }

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
    
    //World_ =  Rock_Physic;
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
  std::vector<G4LogicalVolume *> next2nextDaughterVolVtr;

  //add first daughters to vector
  for(int i=0; i<vol->GetNoDaughters();i++) {
    G4LogicalVolume * daughter = vol->GetDaughter(i)->GetLogicalVolume();
    std::string lv_name = daughter->GetName();
    std::cout << "\n\t -- " << lv_name;
    daughterVolVtr.push_back(daughter);
    if(lv_name.find("Scint") != std::string::npos){
      ScintillatorSensitiveDetector *scint = new ScintillatorSensitiveDetector(lv_name);
      scintSDVector_.push_back(scint);
    }
    if(lv_name.find("innerVesselVolume") != std::string::npos){
      DetectorSD *tpcFidSD = new DetectorSD(lv_name);
      detectorSDVector.push_back(tpcFidSD);
    }
  }

  for(unsigned int i=0; i<daughterVolVtr.size();i++){
    G4LogicalVolume *mother = daughterVolVtr.at(i);
    int ndaughters = mother->GetNoDaughters();
    for(int j=0; j<ndaughters;j++){
      G4LogicalVolume * daughter = mother->GetDaughter(j)->GetLogicalVolume();
      std::string lv_name = daughter->GetName();
      std::cout << "\n\t ------- " << lv_name;
      nextDaughterVolVtr.push_back(daughter);
      if(lv_name.find("Scint") != std::string::npos){
	ScintillatorSensitiveDetector *scint = new ScintillatorSensitiveDetector(lv_name);
	scintSDVector_.push_back(scint);
      }
      if(lv_name.find("innerVesselVolume") != std::string::npos){
	DetectorSD *tpcFidSD = new DetectorSD(lv_name);
	detectorSDVector.push_back(tpcFidSD);
      }
    }
  }
  

  for(unsigned int i=0; i<nextDaughterVolVtr.size();i++){
    G4LogicalVolume *mother = nextDaughterVolVtr.at(i);
    int ndaughters = mother->GetNoDaughters();
    for(int j=0;j<ndaughters;j++){
      G4LogicalVolume * daughter = mother->GetDaughter(j)->GetLogicalVolume();
      std::string lv_name = daughter->GetName();
      std::cout << "\n\t ---------------------- " << lv_name;
      next2nextDaughterVolVtr.push_back(daughter);
      if(lv_name.find("Scint") != std::string::npos){
	ScintillatorSensitiveDetector *scint = new ScintillatorSensitiveDetector(lv_name);
	scintSDVector_.push_back(scint);
      }
      if(lv_name.find("innerVesselVolume") != std::string::npos){
	DetectorSD *tpcFidSD = new DetectorSD(lv_name);
	detectorSDVector.push_back(tpcFidSD);
      }
    }
  }

  for(unsigned int i=0; i<next2nextDaughterVolVtr.size();i++){
    G4LogicalVolume *mother = next2nextDaughterVolVtr.at(i);
    int ndaughters = mother->GetNoDaughters();
    for(int j=0;j<ndaughters;j++){
      G4LogicalVolume * daughter = mother->GetDaughter(j)->GetLogicalVolume();
      std::string lv_name = daughter->GetName();
      std::cout << "\n\t --------------------------------------- " << lv_name;
      if(lv_name.find("Scint") != std::string::npos){
	ScintillatorSensitiveDetector *scint = new ScintillatorSensitiveDetector(lv_name);
	scintSDVector_.push_back(scint);
      }
      if(lv_name.find("innerVesselVolume") != std::string::npos){
	DetectorSD *tpcFidSD = new DetectorSD(lv_name);
	detectorSDVector.push_back(tpcFidSD);
      }     
    }
  }
 
  // Clear the vectors
  std::vector<G4LogicalVolume *>().swap(daughterVolVtr); daughterVolVtr.clear(); daughterVolVtr.resize(0);
  std::vector<G4LogicalVolume *>().swap(nextDaughterVolVtr); nextDaughterVolVtr.clear(); nextDaughterVolVtr.resize(0);
  std::vector<G4LogicalVolume *>().swap(next2nextDaughterVolVtr); next2nextDaughterVolVtr.clear(); next2nextDaughterVolVtr.resize(0);
  
  G4SDManager* SDMgr_ = G4SDManager::GetSDMpointer();
  
  //add the sensitive detectors
  //tpcFidSD_     = new DetectorSD("innerVesselVolume");
  //detectorSDVector.push_back(tpcFidSD_);

  //loop over SD vector to set all detector attributes- other than scintillator
  for(unsigned int i=0;i<detectorSDVector.size();i++){    
    DetectorSD * tmpSD = detectorSDVector.at(i);
    G4String tmpSDName = tmpSD->GetName();// + "Volume";
    if(tmpSD){
      SDMgr_->AddNewDetector(tmpSD);
      
      //Find the sensitive detector logical volume  	  	
      G4LogicalVolume * tmpSD_lv = FindLogicalVolume(tmpSDName);
      
      if(tmpSD_lv) {
	//tmpSD_lv->SetVisAttributes(G4Colour::Blue());
	tmpSD_lv->SetSensitiveDetector(tmpSD);
	//tmpSD_lv->SetUserLimits(new G4UserLimits(GetSteppingLimit()));
	tmpSD_lv->SetUserLimits(new G4UserLimits(1.0*CLHEP::mm)); // same as T2K
	std::cout << "\n*****************************************************"
		  << "\nFound detector with name: " << tmpSD_lv->GetName() 
		  << " -- SD set!" 
		  << "\n*****************************************************" <<std::endl;
      
	//Add the magnetic field for the TPC only
	//magnetise the sensitive detector
	if(tmpSD->GetName() == "tpcFid" || tmpSD->GetName() == "innerVesselVolume"){
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
    if(tmpSD){
      SDMgr_->AddNewDetector(tmpSD);
      
      //Find the sensitive detector logical volume  	  	
      G4LogicalVolume * tmpSD_lv = FindLogicalVolume(tmpSDName);
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
    }
    else {
      std::cout << "\n*****************************************************"
		<< "\nCannot find detector with name: " << tmpSDName 
		<< " -- No SD set!" 		
		<< "\n*****************************************************" <<std::endl;      
      listVolumes();
    }    
  }
}

HitCollection * DetectorConstruction::getHitCollections(int i){
  if(detectorSDVector.at(i)){
    return detectorSDVector.at(i)->getHitCollectionPtr();
  }
  else return NULL;
}

HitCollection * DetectorConstruction::getTpcHitCollection(){
  //if(tpcFidSD_){
  //return tpcFidSD_->getHitCollectionPtr();
  //}
  
  if(detectorSDVector.size()>0){
    return detectorSDVector.at(0)->getHitCollectionPtr();
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
  //if(scintSDVector_.size()>0)scintSDVector_.at(0)->clear();
  for(unsigned int i=0;i<scintSDVector_.size();i++){
    scintSDVector_.at(i)->clear();
  }
  
}

