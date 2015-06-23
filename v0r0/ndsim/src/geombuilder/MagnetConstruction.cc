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
  thickness_ = thickness_*(CLHEP::cm);
  gap_ = gap_*(CLHEP::cm);
  mass_= 0.;
}

void MagnetConstruction::construct(Materials * mats,TpcVesselConstruction *vessel, bool cylindrical){
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
  
  innerRadius_ = vessel->getRadius()/(CLHEP::cm) + gap_/(CLHEP::cm);
  radius_ = innerRadius_ + thickness_+ gap_/(CLHEP::cm);

  length_ = vessel->getTubeLength() + gap_/(CLHEP::cm);

  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  if(cylindrical){
    //TGeoTube *tube_ = new TGeoTube("magnetTube",innerRadius_/(CLHEP::cm),radius_/(CLHEP::cm),length_/(2.0*(CLHEP::cm)) );
    TGeoTube *tube_ = new TGeoTube("magnetTube",innerRadius_,radius_,length_/(2.0*(CLHEP::cm)) );
    volume_ = new TGeoVolume("magnetVolume",tube_,medium_);
    volumeSize = 3.14*length_*(radius_*radius_ - innerRadius_*innerRadius_);
    volume_->GetMedium()->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
    mass_ = (volumeSize)*(density_);
  }
  else{
    double origin[3] = {0.0,0.0,0.0};
    double x1 = 0.0;
    double y1 = 0.0;
    double z1 = 0.0;
    origin[2] = (length_-thickness_/2);
    x1 = radius_;
    y1 = radius_;
    z1 = thickness_/2;
    volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
    TGeoBBox *motherBoxZ1_ = new TGeoBBox("MagnetMotherBoxZ1",x1,y1,z1,origin);
    //
    origin[2] = -(length_-thickness_/2);
    x1 = radius_;
    y1 = radius_ ;
    z1 = thickness_/2;
    TGeoBBox *motherBoxZ2_ = new TGeoBBox("MagnetMotherBoxZ2",x1,y1,z1,origin);
    //
    origin[1] = radius_+thickness_/2;
    y1 = thickness_/2;
    x1 = radius_ + thickness_;
    z1 = length_;
    TGeoBBox *motherBoxY1_ = new TGeoBBox("MagnetMotherBoxY1",x1,y1,z1,origin);
    //
    origin[1] = -(radius_+thickness_/2);
    y1 = thickness_/2;
    x1 = radius_ + thickness_;
    z1 = length_;
    TGeoBBox *motherBoxY2_ = new TGeoBBox("MagnetMotherBoxY2",x1,y1,z1,origin);
    //
    origin[0] = radius_+thickness_/2;
    x1 = thickness_/2;
    y1 = radius_;
    z1 = length_;
    TGeoBBox *motherBoxX1_ = new TGeoBBox("MagnetMotherBoxX1",x1,y1,z1,origin);
    //
    origin[0] = -(radius_+thickness_/2);
    x1 = thickness_/2;
    y1 = radius_;
    z1 = length_;
    TGeoBBox *motherBoxX2_ = new TGeoBBox("MagnetMotherBoxX2",x1,y1,z1,origin);
    
    // Total volume
    TGeoCompositeShape *compositeShape_ = new TGeoCompositeShape("(MagnetMotherBoxZ1) + (MagnetMotherBoxZ2) + (MagnetMotherBoxY1) + (MagnetMotherBoxY2) + (MagnetMotherBoxX1) + (MagnetMotherBoxX2)");
    volume_ = new TGeoVolume("vesselVolume",compositeShape_,medium_);
    volume_->GetMedium()->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));

    mass_   = (volumeSize)*(density_);
    volumeSize = 6*volumeSize;
    mass_   = 6*mass_;
  }
}

MagnetConstruction::~MagnetConstruction()
{
  delete material_;
  delete medium_;
  //delete tube_;
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
