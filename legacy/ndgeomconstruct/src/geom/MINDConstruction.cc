//
// Title: MIND  design
//
// Author: Tom STainer
//
// Description: MIND geometry for root in the ND geometry

#include "MINDConstruction.hh"

MINDConstruction::MINDConstruction() : totalMass(0.)
{
  initDataCards();
}

void MINDConstruction::initialize(){

  //load the data cards
  loadDataCards();  
  //volumeSize = layers_*thickness_;
}

void MINDConstruction::construct(Materials * mats){

  motherMaterial_ = (TGeoMixture*)mats->FindMaterial(materialName_.c_str());
  if( motherMaterial_ ){
  	motherMaterial_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
	motherMaterial_ = (TGeoMixture*)mats->FindMaterial("Iron");
	std::cout << "\n------------------------------------------------------------"
	    	<< "\nMINDConstruction::Construct MATERIAL NOT SET - USING IRON"
		<< "\n------------------------------------------------------------" <<std::endl;
  }
  motherMedium_ = new TGeoMedium("mindMotherMedium",1,motherMaterial_);

  double motherX = x_/2.0;
  double motherY = y_/2.0;
  double motherZ = z_/2.0;

  motherBox_ = new TGeoBBox("mindMotherBox",motherX/(CLHEP::cm),motherY/(CLHEP::cm),motherZ/(CLHEP::cm));
  motherVolume_ = new TGeoVolume("mindMotherVolume",motherBox_,motherMedium_);

  //SCINTILLATOR BARS
  material_ = (TGeoMixture*)mats->FindMaterial(scintMaterialName_.c_str(),true);
  if( material_ ){
  	material_->SetDensity(scintDensity_/(CLHEP::g/CLHEP::cm3)); 
  } 
  else{
	material_ = (TGeoMixture*)mats->FindMaterial("Scintillator");
	std::cout << "\n------------------------------------------------------------"
	    	<< "\nMINDConstruction::Construct MATERIAL NOT SET - USING Scintillator"
		<< "\n------------------------------------------------------------" <<std::endl;
  }
  //medium
  medium_ = new TGeoMedium(materialName_.c_str(),1,material_);

  //calculate the number of layers
  layers = z_/(thickness_+scintThickness_);

  for(int i=0;i<layers;i++){
    
    //shapes and volumes
    //scintillator
    tempBox_ = new TGeoBBox(Form("mindScintBoxZ_%i",i),motherX/(CLHEP::cm),motherY/(CLHEP::cm),scintThickness_/(2.0*(CLHEP::cm)));
    tempVolume_ = new TGeoVolume(Form("mindScintVolumeZ_%i",i),tempBox_,medium_);
    tempVolume_->SetLineColor(kTeal-6);
    //iron
    tempBox2_ = new TGeoBBox(Form("mindBoxZ_%i",i),motherX/(CLHEP::cm),motherY/(CLHEP::cm),thickness_/(2.0*(CLHEP::cm)));
    tempVolume2_ = new TGeoVolume(Form("mindVolumeZ_%i",i),tempBox2_,motherMedium_);
    tempVolume2_->SetLineColor(12);

    //position of the scintillator plane from z = 0 - should go iron,scint,iron,scint,iron
    double alpha = 0.5*( (2.*(i+1)*thickness_/(CLHEP::cm)) + ((2*i)+1)*scintThickness_/(CLHEP::cm) ) - z_/(2.0*(CLHEP::cm));
    double alpha2 = alpha - ((thickness_/(CLHEP::cm))+(scintThickness_/(CLHEP::cm)))/2.0;

    motherVolume_->AddNode(tempVolume_,i,
		new TGeoTranslation(0,0,alpha));
    motherVolume_->AddNode(tempVolume2_,i,
		new TGeoTranslation(0,0,alpha2));

    double tempVolumeSize = (tempBox_->GetDX() * 2.*CLHEP::cm)*(tempBox_->GetDY() * 2.*CLHEP::cm)*(tempBox_->GetDZ() * 2.*CLHEP::cm);
    double tempVolumeSize2 = (tempBox2_->GetDX() * 2.*CLHEP::cm)*(tempBox2_->GetDY() * 2.*CLHEP::cm)*(tempBox2_->GetDZ() * 2.*CLHEP::cm);
    double tempDensity = tempVolume_->GetMaterial()->GetDensity()*(CLHEP::g/CLHEP::cm3);
    double tempDensity2 = tempVolume2_->GetMaterial()->GetDensity()*(CLHEP::g/CLHEP::cm3);

    //add mass to give value for mass of whole MIND system
    totalMass += (tempVolumeSize*tempDensity) + (tempVolumeSize2*tempDensity2);

    //add iron for end
    if(i+1==layers){
      double alpha3 = alpha2 + (thickness_/(CLHEP::cm))+(scintThickness_/(CLHEP::cm));
    tempBox2_ = new TGeoBBox(Form("mindBoxZ_%i",layers),motherX/(CLHEP::cm),motherY/(CLHEP::cm),thickness_/(2.0*(CLHEP::cm)));
    tempVolume2_ = new TGeoVolume(Form("mindVolumeZ_%i",layers),tempBox2_,motherMedium_);
    tempVolume2_->SetLineColor(12);
      motherVolume_->AddNode(tempVolume2_,layers,
		new TGeoTranslation(0,0,alpha3));

	tempVolumeSize2 = (tempBox2_->GetDX() * 2.*CLHEP::cm)*(tempBox2_->GetDY() * 2.*CLHEP::cm)*(tempBox2_->GetDZ() * 2.*CLHEP::cm);
	tempDensity2 = tempVolume2_->GetMaterial()->GetDensity()*(CLHEP::g/CLHEP::cm3);
	totalMass += (tempVolumeSize2*tempDensity2);
    }
  }

  volume_ = motherVolume_;
  volume_->SetLineColor(12);
  volume_->SetVisibility(kFALSE);
}

MINDConstruction::~MINDConstruction()
{
  delete material_;
  delete motherMaterial_;
  delete medium_;
  delete motherMedium_;
  delete motherBox_;
  delete motherVolume_;
  delete volume_;
}

void MINDConstruction::setDensity(double density){
  density_ = density;
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
}

void MINDConstruction::initDataCards() {
  cards_ = DataCards::getInstance();
  //char* LBNO = getenv("LBNO");

  cards_->AddDataCardDouble("mindX", 10.0*CLHEP::m);
  cards_->AddDataCardDouble("mindY", 10.0*CLHEP::m);
  cards_->AddDataCardDouble("mindZ", 5.0*CLHEP::m);
  cards_->AddDataCardDouble("mindThicknessLayer1", 30.0*CLHEP::mm);
  cards_->AddDataCardDouble("mindThicknessLayer2", 15.0*CLHEP::mm);
  cards_->AddDataCardString("mindMaterial", "Iron");
  cards_->AddDataCardDouble("mindDensity", 7.87*(CLHEP::g/CLHEP::cm3));
  cards_->AddDataCardString("mindScintMaterial", "Scintillator");
  cards_->AddDataCardDouble("mindScintDensity", 1.18*(CLHEP::g/CLHEP::cm3));
  cardsLoaded_ = false;
}

void MINDConstruction::loadDataCards() {

  x_			= cards_->fetchValueDouble("mindX");
  y_			= cards_->fetchValueDouble("mindY");
  z_			= cards_->fetchValueDouble("mindZ");
  thickness_  		= cards_->fetchValueDouble("mindThicknessLayer1");
  scintThickness_ 	= cards_->fetchValueDouble("mindThicknessLayer2");
  materialName_  	= cards_->fetchValueString("mindMaterial");
  density_  		= cards_->fetchValueDouble("mindDensity");
  scintMaterialName_  	= cards_->fetchValueString("mindScintMaterial");
  scintDensity_ 	= cards_->fetchValueDouble("mindScintDensity");
  cardsLoaded_ = true;

  std::cout <<"\n******************************************"
	    << "\nMINDConstruction Data Card loaded."
	    <<"\n******************************************"
	    << std::endl;
}

void MINDConstruction::print(){

  //root stores densities in gcm-3
  double tmp_density = tempVolume2_->GetMaterial()->GetDensity()*(CLHEP::g/CLHEP::cm3);
  double tmp_scint_density = tempVolume_->GetMaterial()->GetDensity()*(CLHEP::g/CLHEP::cm3);
  double tmp_mother_density = motherMaterial_->GetDensity()*(CLHEP::g/CLHEP::cm3);

  std::cout << "\n==================================================================="
	    << "\nMINDConstruction::print() Number of Layers: 		" << layers
	    << "\nMINDConstruction::print() Thickness: 			" << thickness_* (1 / CLHEP::mm) <<" mm"
	    << "\nMINDConstruction::print() Scintillator Thickness: 	" << scintThickness_* (1 / CLHEP::mm) <<" mm"
	    //<< "\nMINDConstruction::print() Volume: 		" << volumeSize* (1 / CLHEP::m3) <<" m3"
	    << "\nMINDConstruction::print() Density is: 		\t" 
	    << tmp_density * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\nMINDConstruction::print() ScintDensity is: 		" 
	    << tmp_scint_density * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\nMINDConstruction::print() Total Mass is: 		" << totalMass * (1/CLHEP::kg) << " kg"
	    << "\n==================================================================="
	    << std::endl;

}
