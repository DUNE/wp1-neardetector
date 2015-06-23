//
// Title: TPC Fiducial design
//
// Author: Tom STainer, Georgios Christodoulou
//
// Description: TPC Fiducial geometry for root in the ND geometry

#include "TpcConstruction.hh"

TpcConstruction::TpcConstruction()
{
}

void TpcConstruction::initialize(){

  x_ = (x_/2)*CLHEP::m;
  y_ = (y_/2)*CLHEP::m;
  z_ = (z_/2)*CLHEP::m;
  density_ = density_*(CLHEP::g / CLHEP::cm3);
  volumeSize = 8*x_*y_*z_;
  mass = volumeSize*density_; 

}

void TpcConstruction::construct(Materials * mats){

  //--------------- Construct gas region ------------------//
  materialName = matName_;

  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if(material_){
    material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    material_ = (TGeoMixture*)mats->FindMaterial("ArgonGas");
    materialName = "ArgonGas";
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::TpcConstruction::Construct MATERIAL NOT SET - using gas Argon as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  
  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  box_    = new TGeoBBox("tpcFidBox",x_/(CLHEP::cm),y_/(CLHEP::cm),z_/(CLHEP::cm));
  volume_ = new TGeoVolume("tpcFidVolume",box_,medium_);

  volume_->GetMedium()->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  //volume_->SetLineColor(1);
  //--------------- Add Anode and Catahode plates ------------------//
  std::string anodePlateMaterialName = "G10Roha";
  std::string cathodePlateMaterialName = anodePlateMaterialName;

  anodePlateMaterial = (TGeoMixture*)mats->FindMaterial(anodePlateMaterialName.c_str());
  if( anodePlateMaterial ){
    cathodePlateMaterial = anodePlateMaterial;
    double plateDensity = 0.2877*(CLHEP::g / CLHEP::cm3);
    anodePlateMaterial->SetDensity(plateDensity/(CLHEP::g/CLHEP::cm3));		//densities for g10roha
  } 
  else{
    anodePlateMaterialName = "Aluminium";
    cathodePlateMaterialName = anodePlateMaterialName;
    anodePlateMaterial = (TGeoMixture*)mats->FindMaterial(anodePlateMaterialName.c_str());
    cathodePlateMaterial = anodePlateMaterial;
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::TpcConstruction::Construct Cathode MATERIAL NOT SET - using Aluminium as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  
  cathodeMedium = new TGeoMedium(cathodePlateMaterialName.c_str(),2,cathodePlateMaterial);
  anodeMedium   = new TGeoMedium(anodePlateMaterialName.c_str(),3,anodePlateMaterial);

  //x is the thickness
  double cathodeThickness = 13.2*CLHEP::mm;
  double anodeThickness = cathodeThickness;

  double gap = 5.0*CLHEP::cm;
  double cathodeY = y_ - 5.0*CLHEP::cm;
  double cathodeZ = z_ - 5.0*CLHEP::cm;

  cathodeBox    = new TGeoBBox("cathodeBox",cathodeThickness/(CLHEP::cm),cathodeY/(CLHEP::cm),cathodeZ/(CLHEP::cm));
  anodeBox      = new TGeoBBox("anodeBox",anodeThickness/(CLHEP::cm),cathodeY/(CLHEP::cm),cathodeZ/(CLHEP::cm));

  cathodeVolume = new TGeoVolume("cathodeVolume",cathodeBox,cathodeMedium);
  anodeVolume   = new TGeoVolume("anodeVolume",anodeBox,anodeMedium);

  cathodeVolume->SetLineColor(2);
  anodeVolume->SetLineColor(3);

  //place the cathode and anode in the tpc
  volume_->AddNode(cathodeVolume,1, new TGeoTranslation(x_/(CLHEP::cm) - cathodeThickness/(1.0*(CLHEP::cm)) - gap/(CLHEP::cm),0.0,0.0));
  volume_->AddNode(anodeVolume,  2, new TGeoTranslation(- x_/(CLHEP::cm) + anodeThickness/(1.0*(CLHEP::cm)) + gap/(CLHEP::cm),0.0,0.0));
  //middle cathode
  volume_->AddNode(cathodeVolume,3, new TGeoTranslation(0.0,0.0,0.0));
  //--------------- Add Micromegas ------------------//
}

TpcConstruction::~TpcConstruction()
{
  delete medium_;
  delete box_;
  delete volume_;

  delete cathodeMedium;
  delete anodeMedium;
  delete cathodeBox;
  delete anodeBox;
}
/*
void TpcConstruction::setDensity(double density){
  density_ = density*(CLHEP::g / CLHEP::cm3);
  volume_->GetMedium()->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
}
*/
void TpcConstruction::print(){

  //root stores positions in cm
  double tmp_box_x = box_->GetDX() * 2.*CLHEP::cm;
  double tmp_box_y = box_->GetDY() * 2.*CLHEP::cm;
  double tmp_box_z = box_->GetDZ() * 2.*CLHEP::cm;
  double tmp_volume_size = tmp_box_x*tmp_box_y*tmp_box_z;
  //root stores densities in gcm-3
  double tmp_density = material_->GetDensity()*(CLHEP::g/CLHEP::cm3);
  double tmp_density_plate = anodePlateMaterial->GetDensity()*(CLHEP::g/CLHEP::cm3);
  double tmp_mass = tmp_volume_size*tmp_density;

  std::cout << "\n==================================================================="
	    << "\nTpcConstruction::print() material: 	\t" << materialName
	    << "\nTpcConstruction::print() X: 		\t" << tmp_box_x* (1 / CLHEP::m) <<" m"
	    << "\nTpcConstruction::print() Y: 		\t" << tmp_box_y* (1 / CLHEP::m) <<" m"
	    << "\nTpcConstruction::print() Z: 		\t" << tmp_box_z* (1 / CLHEP::m) <<" m"
	    << "\nTpcConstruction::print() Volume:	\t" << tmp_volume_size* (1 / CLHEP::m3) <<" m3"
            << "\nTpcConstruction::print() Mass: 	\t\t" << tmp_mass* (1 / CLHEP::kg) <<" kg"
	    << "\nTpcConstruction::print() Density is: 	\t" << tmp_density * (CLHEP::m3/CLHEP::kg) <<" kgm-3"
	    << "\nTpcConstruction::print() Plate Density is:\t" << tmp_density_plate * (CLHEP::m3/CLHEP::kg) <<" kgm-3"
	    << "\n==================================================================="
	    << std::endl;

}


