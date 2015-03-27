//
// Title: Rock Environment for the enclosure around the ND
//
// Author: Tom STainer, Georgios Christodoulou
//
// Description: Rock geometry for root in the ND geometry

#include "RockConstruction.hh"

RockConstruction::RockConstruction()// : TubeConstruction()
{
}

void RockConstruction::initialize(){

  //load the data cards
  x_ = x_*(CLHEP::m);
  y_ = y_*(CLHEP::m);
  z_ = z_*(CLHEP::m);
  volumeSize = x_*y_*z_;
}

void RockConstruction::construct(Materials * mats){
  materialName = rmaterial_;

  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if(material_){
    material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    materialName = "Vacuum";
    material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
    std::cout << "\n------------------------------------------------------------"
	      << "\nRockConstruction::Construct MATERIAL NOT SET - using vacuum as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  box_ = new TGeoBBox("rockBox",x_/(2.0*CLHEP::cm),y_/(2.0*CLHEP::cm),z_/(2.0*CLHEP::cm));
  //tube_ = new TGeoTube("rockTube",0,radius_/(CLHEP::cm),length_/(2.0*(CLHEP::cm)) );
  volume_ = new TGeoVolume("rockVolume",box_,medium_);
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
  //volumeSize = x_*y_*z_;
}

RockConstruction::~RockConstruction()
{
  delete material_;
  delete medium_;
  delete box_;
  delete volume_;
}
/*
void RockConstruction::setDensity(double density){
  density_ = density;
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
}
*/
void RockConstruction::print(){

  //root stores positions in cm
  double tmp_box_x = box_->GetDX() * CLHEP::cm;
  double tmp_box_y = box_->GetDY() * CLHEP::cm;
  double tmp_box_z = box_->GetDZ() * CLHEP::cm;
  double tmp_volume_size = tmp_box_x*tmp_box_y*tmp_box_z;
  //root stores densities in gcm-3
  double tmp_density = material_->GetDensity()*(CLHEP::g/CLHEP::cm3);
  double tmp_mass = tmp_volume_size*tmp_density;

  std::cout << "\n==================================================================="
	    << "\nRockConstruction::print() material: 		\t" << materialName
	    << "\nRockConstruction::print() X: 			\t" << tmp_box_x* (1 / CLHEP::m) <<" m"
	    << "\nRockConstruction::print() Y: 			\t" << tmp_box_y* (1 / CLHEP::m) <<" m"
	    << "\nRockConstruction::print() Z: 			\t" << tmp_box_z* (1 / CLHEP::m) <<" m"
	    << "\nRockConstruction::print() Volume: 		\t" << tmp_volume_size* (1 / CLHEP::m3) <<" m3"
	    << "\nRockConstruction::print() Density is:   	\t" << tmp_density * (CLHEP::m3/CLHEP::kg) <<" kgm-3"
	    << "\n==================================================================="
	    << std::endl;

}
