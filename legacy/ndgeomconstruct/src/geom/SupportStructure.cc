//
// Title: Support Structures
//
// Author: Tom STainer
//
// Description: Support Structures for inner vessel geometry for root in the ND geometry

#include "SupportStructure.hh"

SupportStructure::SupportStructure()
{
  initDataCards();
}

void SupportStructure::initialize(){

  //load the data cards
  loadDataCards();
}

void SupportStructure::construct(Materials * mats,TpcFiducialConstruction * tpc){

  materialName = "Steel";

  //TpcVessel
  if( mats->FindMaterial(materialName.c_str()) ){
  	material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  	material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
	material_ = (TGeoMixture*)mats->FindMaterial("Aluminium");
	std::cout << "\n------------------------------------------------------------"
	    	<< "\nSupportStructure::Construct MATERIAL NOT SET - using Aluminium as default"
		<< "\n------------------------------------------------------------" <<std::endl;
  }
  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  tube_ = new TGeoTube("vesselCenterTube",0,radius_/(CLHEP::cm),tubeLength_/(2.0*(CLHEP::cm)));
  centerVolume_ = new TGeoVolume("vesselCenterVolume",tube_,medium_);

  //vessel - end distance -- must be equal to or less than the radius
  Float_t radiusEndMax = ratio_*radius_;
  //theta
  Float_t thetaEnd = 
	(180/TMath::Pi())*TMath::ASin( (2*radius_*radiusEndMax) / 
	(radius_*radius_ + radiusEndMax*radiusEndMax));

  theta_ = thetaEnd;

  //radius
  Float_t radiusEnd = (radius_*radius_ + radiusEndMax*radiusEndMax) / (2*radiusEndMax);
  nearEnd_ = new TGeoSphere("vesselNearEnd",0,radiusEnd/(CLHEP::cm),0,thetaEnd,0,360);
  farEnd_ = new TGeoSphere("vesselFarEnd",0,radiusEnd/(CLHEP::cm),180-thetaEnd,180,0,360);

  //end displacement
  Float_t endDisplacement = radiusEnd*TMath::Cos(TMath::Pi()*thetaEnd/180.0);
  //Inner vessel dimensions
  //cylinder
  Float_t innerVesselR = radius_ - thickness_;
  innerRadius_ = innerVesselR;
  Float_t innerVesselL = tubeLength_;

  //inner vesssel material should match tpc - argon, as only wan thin shell for vessel
  //innerMaterial_ = tpc->getMaterial();
  innerMedium_ = tpc->getMedium();

  innerTube_ = new TGeoTube("innerVesselCenterTube",0,innerVesselR/(CLHEP::cm),innerVesselL/(2.0*(CLHEP::cm)));
  innerCenterVolume_ = new TGeoVolume("innerVesselCenterVolume",tube_,innerMedium_);

  //end distance 
  Float_t innerVesselEndMaxR = radiusEndMax - thickness_;
  //theta
  Float_t innerVesselEndTheta = 
	(180/TMath::Pi())*TMath::ASin( (2*innerVesselR*innerVesselEndMaxR) / 
	(innerVesselR*innerVesselR + innerVesselEndMaxR*innerVesselEndMaxR));
  
  innerTheta_ = innerVesselEndTheta;
  //radius
  Float_t innerVesselEndRadius = (innerVesselR*innerVesselR + innerVesselEndMaxR*innerVesselEndMaxR) 
				  / (2*innerVesselEndMaxR);

  innerNearEnd_ = new TGeoSphere("innerVesselNearEnd",0,innerVesselEndRadius/(CLHEP::cm),0,innerVesselEndTheta,0,360);
  innerFarEnd_ = new
	 TGeoSphere("innerVesselFarEnd",0,innerVesselEndRadius/(CLHEP::cm),180-innerVesselEndTheta,180,0,360);

  //displacement
  Float_t innerVesselEndDisplacement = 
		innerVesselEndRadius*TMath::Cos(TMath::Pi()*innerVesselEndTheta/180.0);

  endLength_ = innerVesselEndRadius - innerVesselEndDisplacement;


  //translations
  TGeoTranslation * t1 = 
	new TGeoTranslation("t1",0,0, - tubeLength_/(2.0*(CLHEP::cm)) + endDisplacement/(CLHEP::cm));
  t1->RegisterYourself();
  TGeoTranslation * t2 = 
	new TGeoTranslation("t2",0,0,tubeLength_/(2.0*(CLHEP::cm)) - endDisplacement/(CLHEP::cm));
  t2->RegisterYourself();
  TGeoTranslation * t3 = 
	new TGeoTranslation("t3",0.,0.,
		(innerVesselL/(2.0*(CLHEP::cm))) - innerVesselEndDisplacement/(CLHEP::cm));
  t3->RegisterYourself();
  TGeoTranslation * t4= 
	new TGeoTranslation("t4",0.,0.,
		innerVesselEndDisplacement/(CLHEP::cm) - (innerVesselL/(2.0*(CLHEP::cm))));
  t4->RegisterYourself();

  //total volume - center + ends
  TGeoCompositeShape *compositeShape_ = 
	new TGeoCompositeShape("(vesselCenterTube + (vesselNearEnd:t2) + (vesselFarEnd:t1))" );
  TGeoCompositeShape *innerCompositeShape_ = 
     new TGeoCompositeShape("(innerVesselCenterTube + (innerVesselNearEnd:t3) + (innerVesselFarEnd:t4))" );

  volume_ = new TGeoVolume("vesselVolume",compositeShape_,medium_);
  innerVolume_ = new TGeoVolume("innerVesselVolume",innerCompositeShape_,innerMedium_);
  
  innerVolume_->SetLineColor(kOrange);
  volume_->AddNode(innerVolume_,1,new TGeoTranslation(0,0,0));

  innerCenterVolume_->SetLineColor(kOrange);
  centerVolume_->AddNode(innerCenterVolume_,1,new TGeoTranslation(0,0,0));
}

SupportStructure::~SupportStructure()
{
  delete material_;
  delete medium_;
  delete tube_;
  delete centerVolume_;
  delete nearEnd_;
  delete farEnd_;
  delete compositeShape_;
  delete volume_;
}

void SupportStructure::setDensity(double density){
  density_ = density;
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
}

void SupportStructure::initDataCards() {
  cards_ = DataCards::getInstance();
  //char* LBNO = getenv("LBNO");

  cards_->AddDataCardString("tpcVesselMaterial", "Aluminium");
  cards_->AddDataCardDouble("tpcVesselRadius", 2.0*CLHEP::m);
  cards_->AddDataCardDouble("tpcVesselLength", 4.0*CLHEP::m);
  cards_->AddDataCardDouble("tpcVesselThickness", 10.0*CLHEP::mm);
  cards_->AddDataCardDouble("tpcVesselDensity", 2.7*(CLHEP::g/CLHEP::cm3));
  cards_->AddDataCardDouble("tpcVesselRadiusRatio", 0.5);
  cardsLoaded_ = false;
}

void SupportStructure::loadDataCards() {

  matName_	= cards_->fetchValueString("tpcVesselMaterial");
  radius_  	= cards_->fetchValueDouble("tpcVesselRadius");
  length_  	= cards_->fetchValueDouble("tpcVesselLength");
  thickness_  	= cards_->fetchValueDouble("tpcVesselThickness");
  density_  	= cards_->fetchValueDouble("tpcVesselDensity");
  ratio_  	= cards_->fetchValueDouble("tpcVesselRadiusRatio");
  cardsLoaded_ 	= true;

  std::cout <<"\n******************************************"
	    << "\nSupportStructure Data Card loaded."
	    <<"\n******************************************"
	    << std::endl;
}

void SupportStructure::print(){

  std::cout << "\n==================================================================="
	    << "\nSupportStructure::print() Radius: 	\t\t" << radius_* (1 / CLHEP::m) <<" m"
	    << "\nSupportStructure::print() Length: 	\t\t" << length_* (1 / CLHEP::m) <<" m"
	    << "\nSupportStructure::print() Thickness: 	\t" << thickness_* (1 / CLHEP::mm) <<" mm"
	    << "\nSupportStructure::print() Volume: 	\t\t" << volumeSize* (1 / CLHEP::m3) <<" m3"
	    << "\nSupportStructure::print() Density is: 	\t" << density_ * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\n==================================================================="
	    << std::endl;

}
