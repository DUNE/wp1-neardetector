//
// Title: Magnet Environment for the enclosure around the ND
//
// Author: Tom Stainer, Georgios Christodoulou
//
// Description: Magnet geometry for root in the ND geometry

#include "MagnetConstruction.hh"

MagnetConstruction::MagnetConstruction()// : TubeConstruction()
{
}

void MagnetConstruction::initialize(){
  density_   = density_*(CLHEP::g / CLHEP::cm3);
  thickness_ = thickness_*(CLHEP::m);
   mass_= 0.;
}

void MagnetConstruction::construct(Materials * mats,TpcVesselConstruction *vessel){
  materialName = matName_;
  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if( material_ ){
    material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    materialName = "Iron";
    material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
    std::cout << "\n------------------------------------------------------------"
	      << "\nMagnetConstruction::Construct MATERIAL NOT SET - using " << materialName << " as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  
  innerRadius_ = vessel->getRadius();
  radius_ = innerRadius_ + thickness_;

  length_ = vessel->getTubeLength();

  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  //tube_ = new TGeoTube("magnetTube",innerRadius_/(CLHEP::cm),radius_/(CLHEP::cm),length_/(2.0*(CLHEP::cm)) );
  tube_ = new TGeoTube("magnetTube",innerRadius_/(CLHEP::cm),radius_/(CLHEP::cm),length_/(2.0*(CLHEP::cm)) );
  volume_ = new TGeoVolume("magnetVolume",tube_,medium_);
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
  volumeSize = 3.14*length_*(radius_*radius_ - innerRadius_*innerRadius_); 
  mass_ = (volumeSize)*(density_);
}

MagnetConstruction::~MagnetConstruction()
{
  delete material_;
  delete medium_;
  delete tube_;
  delete volume_;
}
/*
void MagnetConstruction::setDensity(double density){
  density_ = density;
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
}
*/
void MagnetConstruction::print(){

  std::cout << "\n==================================================================="
	    << "\nMagnetConstruction::print() Thickness: 	\t\t" << thickness_* (1 / CLHEP::m) <<" m"
	    << "\nMagnetConstruction::print() Radius: 		\t" << radius_* (1 / CLHEP::m) <<" m"
	    << "\nMagnetConstruction::print() InnerRadius: 	\t" << innerRadius_* (1 / CLHEP::m) <<" m"
	    << "\nMagnetConstruction::print() Length: 		\t" << length_* (1 / CLHEP::m) <<" m"
	    << "\nMagnetConstruction::print() Volume: 		\t" << volumeSize* (1 / CLHEP::m3) <<" m3"
	    << "\nMagnetConstruction::print() Density is: 	\t" << density_ * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\nMagnetConstruction::print() Mass is: 	 	\t" << mass_ * (1 / CLHEP::kg) <<" kg"
	    << "\nMagnetConstruction::print() Material is:  	\t" << materialName.c_str()
	    << "\n==================================================================="
	    << std::endl;

}
