//
// Title: TPC Vessel
//
// Author: Tom STainer, Georgios Christodoulou
//
// Description: TPC Vessel geometry for root in the ND geometry

#include "TpcVesselConstruction.hh"

TpcVesselConstruction::TpcVesselConstruction()
{
}

void TpcVesselConstruction::initialize(){
  length_ = length_*CLHEP::m;
  radius_ = radius_*CLHEP::m;
  thickness_ = thickness_*CLHEP::m;
  density_ = density_*(CLHEP::g / CLHEP::cm3);
  tubeLength_ = length_ - 2*ratio_*radius_;
  volumeSize = (3.14159*tubeLength_*radius_*radius_) + 0.;//fill in other volumes
}

void TpcVesselConstruction::initialize(double length, double radius, double thickness, double density){
  length_ = length*CLHEP::m;
  radius_ = radius*CLHEP::m;
  thickness_ = thickness*CLHEP::m;
  density_ = density*(CLHEP::g / CLHEP::cm3);
  tubeLength_ = length_ - 2*ratio_*radius_;
  volumeSize = (3.14159*tubeLength_*radius_*radius_) + 0.;//fill in other volumes
}

void TpcVesselConstruction::construct(Materials * mats,TpcConstruction * tpc, bool plates){
  TGeoMedium *innermedium = tpc->getMedium();
  this->construct(mats,innermedium,plates);
}

void TpcVesselConstruction::construct(Materials * mats,std::string gas, double gasden, bool plates){
  gasden = gasden*(CLHEP::g / CLHEP::cm3);
  TGeoMixture *gasmaterial = (TGeoMixture*)mats->FindMaterial(gas.c_str());
  if(gasmaterial){
    gasmaterial->SetDensity(gasden/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    gasmaterial = (TGeoMixture*)mats->FindMaterial("T2KArgonGas");
    gasmaterial->SetDensity(gasden/(CLHEP::g/CLHEP::cm3));
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::TpcConstruction::Construct MATERIAL NOT SET - using gas Argon as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  
  TGeoMedium *innermedium = new TGeoMedium(gas.c_str(),1,gasmaterial);
  this->construct(mats,innermedium,plates);

}

void TpcVesselConstruction::construct(Materials * mats,TGeoMedium * innerMedium_, bool plates){
  materialName = matName_;
  
  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if( material_ ){
    material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    material_ = (TGeoMixture*)mats->FindMaterial("Aluminium");
    std::cout << "\n------------------------------------------------------------"
	      << "\nINFO::TpcVesselConstruction::Construct MATERIAL NOT SET - using Aluminium as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  tube_ = new TGeoTube("vesselCenterTube",0,radius_/(CLHEP::cm),tubeLength_/(2.0*(CLHEP::cm)));
  //centerVolume_ = new TGeoVolume("vesselCenterVolume",tube_,medium_);


  volume_ = new TGeoVolume("innerVesselVolume",tube_,innerMedium_);



  /*
  //vessel - end distance -- must be equal to or less than the radius
  Float_t radiusEndMax = ratio_*radius_;
  //theta
  Float_t thetaEnd = (180/TMath::Pi())*TMath::ASin( (2*radius_*radiusEndMax) / (radius_*radius_ + radiusEndMax*radiusEndMax));
  theta_ = thetaEnd;
  
  //radius
  Float_t radiusEnd = (radius_*radius_ + radiusEndMax*radiusEndMax) / (2*radiusEndMax);
  sphereRadius_ = radiusEnd;
  nearEnd_ = new TGeoSphere("vesselNearEnd",0,radiusEnd/(CLHEP::cm),0,thetaEnd,0,360);
  farEnd_ = new TGeoSphere("vesselFarEnd",0,radiusEnd/(CLHEP::cm),180-thetaEnd,180,0,360);
  
  //end displacement
  Float_t endDisplacement = radiusEnd*TMath::Cos(TMath::Pi()*thetaEnd/180.0);
  //Inner vessel dimensions
  //cylinder
  Float_t innerVesselR = radius_ - thickness_;
  innerRadius_ = innerVesselR;
  
  innerTube_ = new TGeoTube("innerVesselCenterTube",0,innerVesselR/(CLHEP::cm),tubeLength_/(2.0*(CLHEP::cm)));
  //innerCenterVolume_ = new TGeoVolume("innerVesselCenterVolume",tube_,innerMedium_);
  //innerCenterVolume_ = new TGeoVolume("innerVesselCenterVolume",innerTube_,innerMedium_);
  
  //end distance 
  Float_t innerVesselEndMaxR = radiusEndMax - thickness_;
  //theta
  Float_t innerVesselEndTheta = (180/TMath::Pi())*TMath::ASin( (2*innerVesselR*innerVesselEndMaxR) / (innerVesselR*innerVesselR + innerVesselEndMaxR*innerVesselEndMaxR));
  
  innerTheta_ = innerVesselEndTheta;
  //radius
  Float_t innerVesselEndRadius = (innerVesselR*innerVesselR + innerVesselEndMaxR*innerVesselEndMaxR) / (2*innerVesselEndMaxR);
  innerSphereRadius_ = innerVesselEndRadius;

  innerNearEnd_ = new TGeoSphere("innerVesselNearEnd",0,innerVesselEndRadius/(CLHEP::cm),0,innerVesselEndTheta,0,360);
  innerFarEnd_  = new TGeoSphere("innerVesselFarEnd",0,innerVesselEndRadius/(CLHEP::cm),180-innerVesselEndTheta,180,0,360);

  //displacement
  Float_t innerVesselEndDisplacement = innerVesselEndRadius*TMath::Cos(TMath::Pi()*innerVesselEndTheta/180.0);
  endLength_ = innerVesselEndRadius - innerVesselEndDisplacement;

  //translations
  TGeoTranslation * t1 = new TGeoTranslation("t1",0,0, - tubeLength_/(2.0*(CLHEP::cm)) + endDisplacement/(CLHEP::cm));
  t1->RegisterYourself();
  TGeoTranslation * t2 = new TGeoTranslation("t2",0,0,tubeLength_/(2.0*(CLHEP::cm)) - endDisplacement/(CLHEP::cm));
  t2->RegisterYourself();
  InnerNearEndSphereZOffSet_ = (tubeLength_/(2.0*(CLHEP::cm))) - innerVesselEndDisplacement/(CLHEP::cm);
  t3 = new TGeoTranslation("t3",0.,0.,InnerNearEndSphereZOffSet_);
  t3->RegisterYourself();
  InnerFarEndSphereZOffSet_ = innerVesselEndDisplacement/(CLHEP::cm) - (tubeLength_/(2.0*(CLHEP::cm)));
  //InnerFarEndSphereZOffSet_ *= (CLHEP::cm);
  t4 = new TGeoTranslation("t4",0.,0.,InnerFarEndSphereZOffSet_);
  t4->RegisterYourself();
  
  //total volume - center + ends
  TGeoCompositeShape *compositeShape_ = new TGeoCompositeShape("(vesselCenterTube + (vesselNearEnd:t2) + (vesselFarEnd:t1))" );
  TGeoCompositeShape *innerCompositeShape_ = new TGeoCompositeShape("(innerVesselCenterTube + (innerVesselNearEnd:t3) + (innerVesselFarEnd:t4))" );

  volume_ = new TGeoVolume("vesselVolume",compositeShape_,medium_);
  //volume_->GetMedium()->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  innerVolume_ = new TGeoVolume("innerVesselVolume",innerCompositeShape_,innerMedium_);


*/


  //--------------- Add Anode and Cathode plates ------------------//
  if(plates){
    std::string anodePlateMaterialName = "G10Roha";

    TGeoMixture *anodePlateMaterial = (TGeoMixture*)mats->FindMaterial(anodePlateMaterialName.c_str());
    if(!anodePlateMaterial){
      anodePlateMaterialName = "Aluminium";
      anodePlateMaterial = (TGeoMixture*)mats->FindMaterial(anodePlateMaterialName.c_str());
    }
    
    double plateDensity = 0.2877*(CLHEP::g / CLHEP::cm3);
    anodePlateMaterial->SetDensity(plateDensity/(CLHEP::g/CLHEP::cm3));

    std::string cathodePlateMaterialName = anodePlateMaterialName;
    TGeoMixture *cathodePlateMaterial = anodePlateMaterial;
    
    TGeoMedium *cathodeMedium = new TGeoMedium(cathodePlateMaterialName.c_str(),2,cathodePlateMaterial);
    TGeoMedium *anodeMedium   = new TGeoMedium(anodePlateMaterialName.c_str(),3,anodePlateMaterial);
    
    //x is the thickness
    double cathodeThickness = 13.2*CLHEP::mm;
    double anodeThickness = cathodeThickness;
    
    double gap = innerRadius_/4;
    double cathodeY = innerRadius_ - 1.25*gap;
    double cathodeZ = tubeLength_/2;
    
    TGeoBBox *cathodeBox    = new TGeoBBox("cathodeBox",cathodeThickness/(CLHEP::cm),cathodeY/(CLHEP::cm),cathodeZ/(CLHEP::cm));
    TGeoBBox *anodeBox      = new TGeoBBox("anodeBox",anodeThickness/(CLHEP::cm),cathodeY/(CLHEP::cm),cathodeZ/(CLHEP::cm));

    TGeoVolume *cathodeVolume = new TGeoVolume("cathodeVolume",cathodeBox,cathodeMedium);
    TGeoVolume *anodeVolume   = new TGeoVolume("anodeVolume",anodeBox,anodeMedium);
    
    cathodeVolume->SetLineColor(2);
    anodeVolume->SetLineColor(3);

    //place the cathode and anode in the tpc
    volume_->AddNode(cathodeVolume,1, new TGeoTranslation(innerRadius_/(CLHEP::cm) - cathodeThickness/(1.0*(CLHEP::cm)) - (gap)/(CLHEP::cm),0.0,0.0));
    volume_->AddNode(anodeVolume,  2, new TGeoTranslation(- innerRadius_/(CLHEP::cm) + anodeThickness/(1.0*(CLHEP::cm)) + (gap)/(CLHEP::cm),0.0,0.0));
    //middle cathode
    volume_->AddNode(cathodeVolume,3, new TGeoTranslation(0.0,0.0,0.0));

  }

  /*
  innerVolume_->SetLineColor(kOrange+2);
  volume_->AddNode(innerVolume_,1,new TGeoTranslation(0,0,0));
  volume_->SetLineColor(kOrange+12);
  */


  //innerCenterVolume_->SetLineColor(kOrange+8);
  //centerVolume_->AddNode(innerCenterVolume_,1,new TGeoTranslation(0,0,0));
  //centerVolume_->SetLineColor(kOrange+8);
}



TpcVesselConstruction::~TpcVesselConstruction()
{
  if(material_)
    delete material_;
  if(medium_)
    delete medium_;
  if(tube_)
    delete tube_;
  //if(centerVolume_)
  //delete centerVolume_;
  if(nearEnd_)
    delete nearEnd_;
  if(farEnd_)
    delete farEnd_;
  if(compositeShape_)
    delete compositeShape_;
  if(volume_)
    delete volume_;
}

void TpcVesselConstruction::print(){

  std::cout << "\n==================================================================="
	    << "\nTpcVesselConstruction::print() Radius: 	\t\t" << radius_* (1 / CLHEP::m) <<" m"
	    << "\nTpcVesselConstruction::print() Length: 	\t\t" << length_* (1 / CLHEP::m) <<" m"
	    << "\nTpcVesselConstruction::print() Thickness: 	\t" << thickness_* (1 / CLHEP::mm) <<" mm"
	    << "\nTpcVesselConstruction::print() Volume: 	\t\t" << volumeSize* (1 / CLHEP::m3) <<" m3"
	    << "\nTpcVesselConstruction::print() Density is: 	\t" << density_ * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\n==================================================================="
	    << std::endl;

}
