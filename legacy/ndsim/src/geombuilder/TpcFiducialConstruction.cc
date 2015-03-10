//
// Title: TPC Fiducial design
//
// Author: Tom STainer
//
// Description: TPC Fiducial geometry for root in the ND geometry

#include "TpcFiducialConstruction.hh"

TpcFiducialConstruction::TpcFiducialConstruction()
{
  initDataCards();
}

void TpcFiducialConstruction::initialize(){

  //load the data cards
  loadDataCards();
  volumeSize = 8*x_*y_*z_;
  mass = volumeSize*density_; 

}

void TpcFiducialConstruction::construct(Materials * mats){

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
	    	<< "\nTpcFiducialConstruction::Construct MATERIAL NOT SET - using gas Argon as default"
		<< "\n------------------------------------------------------------" <<std::endl;
  }

  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  box_ = new TGeoBBox("tpcFidBox",x_/(CLHEP::cm),y_/(CLHEP::cm),z_/(CLHEP::cm));
  volume_ = new TGeoVolume("tpcFidVolume",box_,medium_);

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
	    	<< "\nTpcFiducialConstruction::Construct Cathode MATERIAL NOT SET - using Aluminium as default"
		<< "\n------------------------------------------------------------" <<std::endl;
  }

  cathodeMedium = new TGeoMedium(cathodePlateMaterialName.c_str(),2,cathodePlateMaterial);
  anodeMedium = new TGeoMedium(anodePlateMaterialName.c_str(),3,anodePlateMaterial);

  //x is the thickness
  double cathodeThickness = 13.2*CLHEP::mm;
  double anodeThickness = cathodeThickness;

  double gap = 5.0*CLHEP::cm;
  double cathodeY = y_ - 5.0*CLHEP::cm;
  double cathodeZ = z_ - 5.0*CLHEP::cm;

  cathodeBox = new
	 TGeoBBox("cathodeBox",cathodeThickness/(CLHEP::cm),cathodeY/(CLHEP::cm),cathodeZ/(CLHEP::cm));
  anodeBox = new TGeoBBox("anodeBox",cathodeThickness/(CLHEP::cm),cathodeY/(CLHEP::cm),cathodeZ/(CLHEP::cm));

  cathodeVolume = new TGeoVolume("cathodeVolume",cathodeBox,cathodeMedium);
  anodeVolume = new TGeoVolume("anodeVolume",anodeBox,anodeMedium);

  cathodeVolume->SetLineColor(2);
  anodeVolume->SetLineColor(2);

  //place the cathode and anode in the tpc
  volume_->AddNode(cathodeVolume,1, 
		new TGeoTranslation(x_/(CLHEP::cm) - cathodeThickness/(1.0*(CLHEP::cm)) - gap/(CLHEP::cm),
					0.0,
					0.0));
  volume_->AddNode(anodeVolume,2, 
		new TGeoTranslation(- x_/(CLHEP::cm) + anodeThickness/(1.0*(CLHEP::cm)) + gap/(CLHEP::cm),
					0.0,
					0.0));
  //middle cathode
  volume_->AddNode(cathodeVolume,3, 
		new TGeoTranslation(0.0,0.0,0.0));
/**/
  //--------------- Add Micromegas ------------------//
}

TpcFiducialConstruction::~TpcFiducialConstruction()
{
  delete medium_;
  delete box_;
  delete volume_;

  delete cathodeMedium;
  delete anodeMedium;
  delete cathodeBox;
  delete anodeBox;
}

void TpcFiducialConstruction::setDensity(double density){
  density_ = density;
  volume_->GetMedium()->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
}

void TpcFiducialConstruction::initDataCards() {
  cards_ = DataCards::getInstance();
  //char* LBNO = getenv("LBNO");

  cards_->AddDataCardString("tpcMaterial", "ArgonGas");
  cards_->AddDataCardDouble("tpcFidX", 1.2*CLHEP::m);
  cards_->AddDataCardDouble("tpcFidY", 1.2*CLHEP::m);
  cards_->AddDataCardDouble("tpcFidZ", 1.5*CLHEP::m);
  cards_->AddDataCardDouble("tpcDensity", 0.035*(CLHEP::g / CLHEP::cm3));
  cardsLoaded_ = false;
}

void TpcFiducialConstruction::loadDataCards() {

  matName_	= cards_->fetchValueString("tpcMaterial");
  x_  		= cards_->fetchValueDouble("tpcFidX");
  y_  		= cards_->fetchValueDouble("tpcFidY");
  z_  		= cards_->fetchValueDouble("tpcFidZ");
  density_  	= cards_->fetchValueDouble("tpcDensity");
  cardsLoaded_ 	= true;

  //turn dimensions into half lengths
  x_ = x_/2.;
  y_ = y_/2.;
  z_ = z_/2.;

  std::cout <<"\n******************************************"
	    << "\nTpcFiducialConstruction Data Card loaded."
	    <<"\n******************************************"
	    << std::endl;
}

void TpcFiducialConstruction::print(){

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
	    << "\nTpcFiducialConstruction::print() material: 	\t" << materialName
	    << "\nTpcFiducialConstruction::print() X: 		\t" << tmp_box_x* (1 / CLHEP::m) <<" m"
	    << "\nTpcFiducialConstruction::print() Y: 		\t" << tmp_box_y* (1 / CLHEP::m) <<" m"
	    << "\nTpcFiducialConstruction::print() Z: 		\t" << tmp_box_z* (1 / CLHEP::m) <<" m"
	    << "\nTpcFiducialConstruction::print() Volume:	\t" << tmp_volume_size* (1 / CLHEP::m3) <<" m3"
            << "\nTpcFiducialConstruction::print() Mass: 	\t\t" << tmp_mass* (1 / CLHEP::kg) <<" kg"
	    << "\nTpcFiducialConstruction::print() Density is: 	\t" << tmp_density * (CLHEP::m3/CLHEP::kg) <<" kgm-3"
	    << "\nTpcFiducialConstruction::print() Plate Density is:\t" << tmp_density_plate * (CLHEP::m3/CLHEP::kg) <<" kgm-3"
	    << "\n==================================================================="
	    << std::endl;

}


