
// Title: Tube Shape Base class
//
// Author: Tom STainer
//
// Description: Tube geometry for root in the ND geometry

#include "TubeConstruction.hh"

TubeConstruction::TubeConstruction()
{
  initDataCards();
}

void TubeConstruction::initialize(){

  //load the data cards
  loadDataCards();
}

void TubeConstruction::construct(std::string matName, TGeoMixture * mixture, double radius,double length){
  volumeSize = 3.14*length*radius*radius;
  
  medium_ = new TGeoMedium(matName.c_str(),1,mixture);
  tube_ = new TGeoTube("Tube",0,radius/(CLHEP::cm),length/(2.0*(CLHEP::cm)));
  volume_ = new TGeoVolume("tubeVolume",tube_,medium_);
}

void TubeConstruction::construct(std::string matName, TGeoMaterial * material, double radius,double length){
  volumeSize = 3.14*length*radius*radius;
  
  medium_ = new TGeoMedium(matName.c_str(),1,material);
  tube_ = new TGeoTube("Tube",0,radius/(CLHEP::cm),length/(2.0*(CLHEP::cm)));
  volume_ = new TGeoVolume("tubeVolume",tube_,medium_);
}

TubeConstruction::~TubeConstruction()
{
  delete medium_;
  delete tube_;
  delete volume_;
}

void TubeConstruction::setDensity(double density){
  density_ = density;
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
}

void TubeConstruction::initDataCards() {
  cards_ = DataCards::getInstance();
  //char* LBNO = getenv("LBNO");

  cards_->AddDataCardDouble("radius", 15.0*CLHEP::m);
  cards_->AddDataCardDouble("length", 30.0*CLHEP::m);
  cards_->AddDataCardDouble("density", 2.323e-12*(CLHEP::g/CLHEP::cm-3));
  cardsLoaded_ = false;
}

void TubeConstruction::loadDataCards() {

  radius_  = cards_->fetchValueDouble("radius");
  length_  = cards_->fetchValueDouble("length");
  density_  = cards_->fetchValueDouble("density");
  cardsLoaded_ = true;

  std::cout <<"\n***********************************"
	    << "\nTubeConstruction Data Card loaded."
	    <<"\n***********************************"
	    << std::endl;
}

void TubeConstruction::print(){

  std::cout << "\n------------------------------------------------------------"
	    << "\nTubeConstruction::print() Radius: " << radius_* (1 / CLHEP::m) <<" m"
	    << "\nTubeConstruction::print() Length: " << length_* (1 / CLHEP::m) <<" m"
	    << "\nTubeConstruction::print() Volume: " << volumeSize* (1 / CLHEP::m3) <<" m3"
	    << "\nTubeConstruction::print() Density is: " << density_ * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\n------------------------------------------------------------"
	    << std::endl;

}