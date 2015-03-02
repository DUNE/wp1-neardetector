//
// Title: Cavity Environment for the enclosure around the ND
//
// Author: Tom Stainer
//
// Description: Cavity geometry for root in the ND geometry

#include "CavityConstruction.hh"

CavityConstruction::CavityConstruction()// : TubeConstruction()
{
  initDataCards();
}

void CavityConstruction::initialize(){

  //load the data cards
  loadDataCards();
}

void CavityConstruction::construct(Materials * mats,TpcVesselConstruction *vessel,MagnetConstruction *magnet,RockConstruction * rock,double detectorYAxisOffset){

  materialName = "Air";  
  
  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if( material_ ){
  	material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
	material_ = (TGeoMixture*)mats->FindMaterial("Vacuum");
	std::cout << "\n------------------------------------------------------------"
	    	<< "\nCavityConstruction::Construct MATERIAL NOT SET - USING VACUUM"
		<< "\n------------------------------------------------------------" <<std::endl;
  }
  //define the gap from the ends of the vessel
  //radius_ = vessel->getLength()/(2.0*CLHEP::cm) + gap_/(CLHEP::cm);
  radius_ = vessel->getLength()/2.0 + gap_;

  //length of cavity is half the height of the rock
  // plus the addition of the detector offset
  // THe detector should lie at the bottom of a topless pit
  //length_ = rock->getY()/(2.0*CLHEP::cm) + detectorYAxisOffset/(CLHEP::cm) + vessel->getRadius()/(CLHEP::cm);
  length_ = rock->getY()/2.0 + detectorYAxisOffset + magnet->getRadius();

  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  tube_ = new TGeoTube("cavityTube",0,radius_/(CLHEP::cm),length_/(2.0*(CLHEP::cm)) );
  volume_ = new TGeoVolume("cavityVolume",tube_,medium_);
  volumeSize = 3.14*length_*radius_*radius_; 

}

CavityConstruction::~CavityConstruction(){
}

void CavityConstruction::setDensity(double density){
  density_ = density;
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
}

void CavityConstruction::initDataCards() {
  cards_ = DataCards::getInstance();
  //char* LBNO = getenv("LBNO");

  cards_->AddDataCardDouble("cavityGap", 2.0*CLHEP::m);
  cards_->AddDataCardDouble("cavityDensity", 1.29e-3*(CLHEP::g/CLHEP::cm3));
  cardsLoaded_ = false;
}

void CavityConstruction::loadDataCards() {

  gap_  = cards_->fetchValueDouble("cavityGap");
  density_  = cards_->fetchValueDouble("cavityDensity");
  cardsLoaded_ = true;

  std::cout <<"\n***********************************"
	    << "\nCavityConstruction Data Card loaded."
	    <<"\n***********************************"
	    << std::endl;
}

void CavityConstruction::print(){

  std::cout << "\n==================================================================="
	    << "\nCavityConstruction::print() Gap: 		\t" << gap_* (1 / CLHEP::m) <<" m"
	    << "\nCavityConstruction::print() Radius: 		\t" << radius_* (1 / CLHEP::m) <<" m"
	    << "\nCavityConstruction::print() Length: 		\t" << length_* (1 / CLHEP::m) <<" m"
	    << "\nCavityConstruction::print() Volume: 		\t" << volumeSize* (1 / CLHEP::m3) <<" m3"
	    << "\nCavityConstruction::print() Density is: 	\t" << density_ * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\n==================================================================="
	    << std::endl;

}
