//
// Title: Ecal Environment for the enclosure around the ND
//
// Author: Tom Stainer, Georgios Christodoulou
//
// Description: Ecal geometry for root in the ND geometry

#include "EcalConstruction.hh"

EcalConstruction::EcalConstruction()// : TubeConstruction()
{
}

void EcalConstruction::initialize(bool had){
  density_      = density_*(CLHEP::g / CLHEP::cm3);
  thickness_    = thickness_*(CLHEP::cm);
  absthickness_ = absthickness_*(CLHEP::cm);
  absdensity_   = absdensity_*(CLHEP::g / CLHEP::cm3);
  gap_          = gap_*(CLHEP::cm);
  mass_         = 0.;
  buildhadecal_ = had;
  if(layers_ < 1)
    layers_ = 34;
}

void EcalConstruction::construction(Materials * mats,TpcVesselConstruction *vessel){

  // SD
  materialName = matName_;
  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if( material_ ){
    material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    materialName = "Scintillator";
    material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
    std::cout << "\n------------------------------------------------------------"
	      << "\nEcalConstruction::Construct MATERIAL NOT SET - using " << materialName << " as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }

  double innerRadius_ = vessel->getRadius()/(CLHEP::cm);
  double radius_ = innerRadius_ +  gap_/(CLHEP::cm);
  length_ = vessel->getTubeLength()/(CLHEP::cm) + gap_/(CLHEP::cm);
  double totalthickness_ = layers_*thickness_/(CLHEP::cm) +  (layers_+1)*absthickness_/(CLHEP::cm);
  
  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  medium_->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));

  double origin[3] = {0.0,0.0,0.0};
  double x1 = 0.0;
  double y1 = 0.0;
  double z1 = 0.0;

   origin[2] = (length_-totalthickness_/2);
   x1 = radius_;
   y1 = radius_;
   z1 = totalthickness_/2;
   volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
   mass_ += (volumeSize)*(density_);
   TGeoBBox *motherBoxZ1_ = new TGeoBBox("EcalMotherBoxZ1",x1,y1,z1,origin);

   origin[2] = -(length_-totalthickness_/2);
   x1 = radius_;
   y1 = radius_ ;
   z1 = totalthickness_/2;
   volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
   mass_ += (volumeSize)*(density_);
   TGeoBBox *motherBoxZ2_ = new TGeoBBox("EcalMotherBoxZ2",x1,y1,z1,origin);

   origin[2] = 0.0;
   origin[1] = radius_+totalthickness_/2;
   y1 = totalthickness_/2;
   x1 = radius_ + totalthickness_;
   z1 = length_;
   volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
   mass_ += (volumeSize)*(density_);
   TGeoBBox *motherBoxY1_ = new TGeoBBox("EcalMotherBoxY1",x1,y1,z1,origin);

   origin[1] = -(radius_+totalthickness_/2);
   y1 = totalthickness_/2;
   x1 = radius_ + totalthickness_;
   z1 = length_;
   volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
   mass_ += (volumeSize)*(density_);
   TGeoBBox *motherBoxY2_ = new TGeoBBox("EcalMotherBoxY2",x1,y1,z1,origin);

   origin[1] = 0.0;
   origin[0] = radius_+totalthickness_/2;
   x1 = totalthickness_/2;
   y1 = radius_;
   z1 = length_;
   volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
   mass_ += (volumeSize)*(density_);
   TGeoBBox *motherBoxX1_ = new TGeoBBox("EcalMotherBoxX1",x1,y1,z1,origin);

   origin[0] = -(radius_+totalthickness_/2);
   x1 = totalthickness_/2;
   y1 = radius_;
   z1 = length_;
   volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
   mass_ += (volumeSize)*(density_);
   TGeoBBox *motherBoxX2_ = new TGeoBBox("EcalMotherBoxX2",x1,y1,z1,origin);

   TGeoCompositeShape *compositeShape_ = new TGeoCompositeShape("EcalMotherBoxZ1 + EcalMotherBoxZ2 + EcalMotherBoxY1 + EcalMotherBoxY2 + EcalMotherBoxX1 + EcalMotherBoxX2");
   volume_ = new TGeoVolume("EcalVolume",compositeShape_,medium_);
   
}

void EcalConstruction::construct(Materials * mats,TpcVesselConstruction *vessel, std::string ecaldirection, bool buildlayers, int layernumber){
  // SD
  materialName = matName_;
  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if( material_ ){
    material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    materialName = "Scintillator";
    material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
    std::cout << "\n------------------------------------------------------------"
	      << "\nEcalConstruction::Construct MATERIAL NOT SET - using " << materialName << " as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  
  // Absorber
  absmaterialName = absmatName_;
  absmaterial_ = (TGeoMixture*)mats->FindMaterial(absmaterialName.c_str());
  if( absmaterial_ ){
    absmaterial_->SetDensity(absdensity_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    absmatName_ = "Lead";
    absmaterial_ = (TGeoMixture*)mats->FindMaterial(absmaterialName.c_str());
    std::cout << "\n------------------------------------------------------------"
	      << "\nEcalConstruction::Construct Absorber MATERIAL NOT SET - using " <<  absmaterialName << " as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  //double gap = 5.0*(CLHEP::cm);
  double innerRadius_ = vessel->getRadius()/(CLHEP::cm);
  double radius_ = innerRadius_ +  gap_/(CLHEP::cm);//vessel->getThickness()/(CLHEP::cm);
  length_ = vessel->getTubeLength()/(CLHEP::cm) /*+ vessel->getThickness()/(CLHEP::cm)*/ + gap_/(CLHEP::cm); //(thickness_/4)/(CLHEP::cm);
  double totalthickness_ = layers_*thickness_/(CLHEP::cm) +  (layers_+1)*absthickness_/(CLHEP::cm);
  //totalthickness_ = totalthickness_/(CLHEP::cm);
  //thickness_ = thickness_/(CLHEP::cm);

  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  medium_->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));

  double origin[3] = {0.0,0.0,0.0};
  double x1 = 0.0;
  double y1 = 0.0;
  double z1 = 0.0;

  if(ecaldirection == "+z"){
    origin[2] = (length_-totalthickness_/2);
    x1 = radius_;
    y1 = radius_;
    z1 = totalthickness_/2;
    volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
    mass_ = (volumeSize)*(density_);
    if(layernumber < 0){
      TGeoBBox *motherBoxZ1_ = new TGeoBBox("EcalMotherBoxZ1",x1,y1,z1,origin);
      if(buildhadecal_)
	volume_ = new TGeoVolume("Ecal7Volume",motherBoxZ1_,medium_);
      else
	volume_ = new TGeoVolume("Ecal1Volume",motherBoxZ1_,medium_);
    }
    // Now add the layers
    if(buildlayers && layernumber < 0){
      for(int i=0;i<layers_;i++){
	origin[2] = (length_-i*thickness_/(CLHEP::cm));
	x1 = radius_;
	y1 = radius_;
	z1 = thickness_/2;
	TGeoBBox *BoxZ1_ = new TGeoBBox(Form("EcalBoxZ1_%i",i),x1,y1,z1,origin);
	TGeoVolume *VolZ1 = new TGeoVolume(Form("Ecal1_%iVolume",i),BoxZ1_,medium_);
	volume_->AddNodeOverlap(VolZ1,1);
      }
    }
    else if(buildlayers && layernumber >= 0){
      origin[2] = (length_-layernumber*thickness_/(CLHEP::cm));
      x1 = radius_;
      y1 = radius_;
      z1 = thickness_/2;
      TGeoBBox *BoxZ1_ = new TGeoBBox("EcalBoxZ1",x1,y1,z1,origin);
      volume_ = new TGeoVolume(Form("Ecal1_%iVolume",layernumber),BoxZ1_,medium_);    
    }
  }
  else if(ecaldirection == "-z"){
    origin[2] = -(length_-totalthickness_/2);
    x1 = radius_;
    y1 = radius_ ;
    z1 = totalthickness_/2;
    volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
    mass_ = (volumeSize)*(density_);
    if(layernumber < 0){
      TGeoBBox *motherBoxZ2_ = new TGeoBBox("EcalMotherBoxZ2",x1,y1,z1,origin);
      if(buildhadecal_)
	volume_ = new TGeoVolume("Ecal8Volume",motherBoxZ2_,medium_);
      else
	volume_ = new TGeoVolume("Ecal2Volume",motherBoxZ2_,medium_);
    }
    // Now add the layers
    if(buildlayers && layernumber < 0){
      for(int i=0;i<layers_;i++){
	origin[2] = -(length_-i*thickness_/(CLHEP::cm));
	x1 = radius_;
	y1 = radius_;
	z1 = thickness_/2;
	TGeoBBox *BoxZ2_ = new TGeoBBox(Form("EcalBoxZ2_%i",i),x1,y1,z1,origin);
	TGeoVolume *VolZ2 = new TGeoVolume(Form("Ecal2_%iVolume",i),BoxZ2_,medium_);
	volume_->AddNodeOverlap(VolZ2,1);
      }
    }
    else if(buildlayers && layernumber >= 0){
      origin[2] = -(length_-layernumber*thickness_/(CLHEP::cm));
      x1 = radius_;
      y1 = radius_;
      z1 = thickness_/2;
      TGeoBBox *BoxZ2_ = new TGeoBBox("EcalBoxZ2",x1,y1,z1,origin);
      volume_ = new TGeoVolume(Form("Ecal2_%iVolume",layernumber),BoxZ2_,medium_); 
    }
  }
  else if(ecaldirection == "+y"){
    origin[1] = radius_+totalthickness_/2;
    y1 = totalthickness_/2;
    x1 = radius_ + totalthickness_;
    z1 = length_;
    volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
    mass_ = (volumeSize)*(density_);
    if(layernumber < 0){
      TGeoBBox *motherBoxY1_ = new TGeoBBox("EcalMotherBoxY1",x1,y1,z1,origin);
      if(buildhadecal_)
	volume_ = new TGeoVolume("Ecal11Volume",motherBoxY1_,medium_);
      else
	volume_ = new TGeoVolume("Ecal5Volume",motherBoxY1_,medium_);
    }
    // Now add the layers
    if(buildlayers && layernumber < 0){
      for(int i=0;i<layers_;i++){
	origin[1] = radius_+i*thickness_/(CLHEP::cm);
	y1 = thickness_/2;
	x1 = radius_ + totalthickness_;
	z1 = length_;
	TGeoBBox *BoxY1_ = new TGeoBBox(Form("EcalBoxY1_%i",i),x1,y1,z1,origin);
	TGeoVolume *VolY1 = new TGeoVolume(Form("Ecal5_%iVolume",i),BoxY1_,medium_);
	volume_->AddNodeOverlap(VolY1,1);
      }
    }
    else if(buildlayers && layernumber >= 0){
      origin[1] = radius_+layernumber*thickness_/(CLHEP::cm);
      y1 = thickness_/2;
      x1 = radius_ + totalthickness_;
      z1 = length_;
      TGeoBBox *BoxY1_ = new TGeoBBox("EcalBoxY1",x1,y1,z1,origin);
      volume_ = new TGeoVolume(Form("Ecal5_%iVolume",layernumber),BoxY1_,medium_);
    }
  }
  else if(ecaldirection == "-y"){
    origin[1] = -(radius_+totalthickness_/2);
    y1 = totalthickness_/2;
    x1 = radius_ + totalthickness_;
    z1 = length_;
    volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
    mass_ = (volumeSize)*(density_);
    if(layernumber < 0){
      TGeoBBox *motherBoxY2_ = new TGeoBBox("EcalMotherBoxY2",x1,y1,z1,origin);
      if(buildhadecal_)
	volume_ = new TGeoVolume("Ecal12Volume",motherBoxY2_,medium_);
      else
	volume_ = new TGeoVolume("Ecal6Volume",motherBoxY2_,medium_);
    }
    // Now add the layers
    if(buildlayers && layernumber < 0){
      for(int i=0;i<layers_;i++){
	origin[1] = -(radius_+i*thickness_/(CLHEP::cm));
	y1 = thickness_/2;
	x1 = radius_ + totalthickness_;
	z1 = length_;
	TGeoBBox *BoxY2_ = new TGeoBBox(Form("EcalBoxY2_%i",i),x1,y1,z1,origin);
	TGeoVolume *VolY2 = new TGeoVolume(Form("Ecal6_%iVolume",i),BoxY2_,medium_);
	volume_->AddNodeOverlap(VolY2,1);
      }
    }
    else if(buildlayers && layernumber >= 0){
      origin[1] = -(radius_+layernumber*thickness_/(CLHEP::cm));
      y1 = thickness_/2;
      x1 = radius_ + totalthickness_;
      z1 = length_;
      TGeoBBox *BoxY2_ = new TGeoBBox("EcalBoxY2",x1,y1,z1,origin);
      volume_ = new TGeoVolume(Form("Ecal6_%iVolume",layernumber),BoxY2_,medium_);
    }
  }
  else if(ecaldirection == "+x"){
    origin[0] = radius_+totalthickness_/2;
    x1 = totalthickness_/2;
    y1 = radius_;
    z1 = length_;
    volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
    mass_ = (volumeSize)*(density_);
    if(layernumber < 0){
      TGeoBBox *motherBoxX1_ = new TGeoBBox("EcalMotherBoxX1",x1,y1,z1,origin);
      if(buildhadecal_)
	volume_ = new TGeoVolume("Ecal9Volume",motherBoxX1_,medium_);
      else
	volume_ = new TGeoVolume("Ecal3Volume",motherBoxX1_,medium_);
    }
    if(buildlayers && layernumber < 0){
      for(int i=0;i<layers_;i++){
	origin[0] = radius_+i*thickness_/(CLHEP::cm);
	x1 = thickness_/2;
	y1 = radius_;
	z1 = length_;
	TGeoBBox *BoxX1_ = new TGeoBBox(Form("EcalBoxX1_%i",i),x1,y1,z1,origin);
	TGeoVolume *VolX1 = new TGeoVolume(Form("Ecal3_%iVolume",i),BoxX1_,medium_);
	volume_->AddNodeOverlap(VolX1,1);
      }
    }
    else if(buildlayers && layernumber >= 0){
      origin[0] = radius_+layernumber*thickness_/(CLHEP::cm);
      x1 = thickness_/2;
      y1 = radius_;
      z1 = length_;
      TGeoBBox *BoxX1_ = new TGeoBBox("EcalBoxX1",x1,y1,z1,origin);
      volume_ = new TGeoVolume(Form("Ecal3_%iVolume",layernumber),BoxX1_,medium_);
    }
  }
  else if(ecaldirection == "-x"){
    origin[0] = -(radius_+totalthickness_/2);
    x1 = totalthickness_/2;
    y1 = radius_;
    z1 = length_;
    volumeSize = x1* 2*CLHEP::cm*y1* 2*CLHEP::cm*z1* 2*CLHEP::cm;
    mass_ = (volumeSize)*(density_);
    if(layernumber < 0){
      TGeoBBox *motherBoxX2_ = new TGeoBBox("EcalMotherBoxX2",x1,y1,z1,origin);
      if(buildhadecal_)
	volume_ = new TGeoVolume("Ecal10Volume",motherBoxX2_,medium_);
      else
	volume_ = new TGeoVolume("Ecal4Volume",motherBoxX2_,medium_);
    }
    if(buildlayers && layernumber < 0){
      for(int i=0;i<layers_;i++){
	origin[0] = -(radius_+i*thickness_/(CLHEP::cm));
	x1 = thickness_/2;
	y1 = radius_;
	z1 = length_;
	TGeoBBox *BoxX2_ = new TGeoBBox(Form("EcalBoxX2_%i",i),x1,y1,z1,origin);
	TGeoVolume *VolX2 = new TGeoVolume(Form("Ecal4_%iVolume",i),BoxX2_,medium_);
	volume_->AddNodeOverlap(VolX2,1);
      }
    }
    else if(buildlayers && layernumber >= 0){
      origin[0] = -(radius_+layernumber*thickness_/(CLHEP::cm));
      x1 = thickness_/2;
      y1 = radius_;
      z1 = length_;
      TGeoBBox *BoxX2_ = new TGeoBBox("EcalBoxX2",x1,y1,z1,origin);
      volume_ = new TGeoVolume(Form("Ecal4_%iVolume",layernumber),BoxX2_,medium_);
    }
  }
  else{
    std::cout << "You didn't specify the correct ecal direction. Choose from +x,-x,+y,-y,+z,-z." << std::endl;
  }

  volume_->SetLineColor(4);
  /*
  x1 = x1 * 2*CLHEP::cm;
  y1 = y1 * 2*CLHEP::cm;
  z1 = z1 * 2*CLHEP::cm;
  volumeSize = x1*y1*z1;
  mass_ = (volumeSize)*(density_);
  */
}

void EcalConstruction::construct(Materials * mats,TpcVesselConstruction *vessel){
  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if( material_ ){
    material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3));
  } 
  else{
    materialName = "Scintillator";
    material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
    std::cout << "\n------------------------------------------------------------"
	      << "\nEcalConstruction::Construct MATERIAL NOT SET - using " << materialName << " as default"
	      << "\n------------------------------------------------------------" <<std::endl;
  }
  
  double innerRadius_ = vessel->getRadius()/(CLHEP::cm);
  double radius_ = innerRadius_ + vessel->getThickness()/(CLHEP::cm);//thickness_;
  length_ = vessel->getTubeLength()/(CLHEP::cm) + vessel->getThickness()/(CLHEP::cm);
  thickness_ = thickness_/(CLHEP::cm);

  medium_ = new TGeoMedium(materialName.c_str(),1,material_);
  medium_->GetMaterial()->SetDensity(density_/(CLHEP::g/CLHEP::cm3));

  // Build Ecal along z-direction
  double origin[3] = {0,0,length_+thickness_/2};
  double x1 = radius_+ thickness_;
  double y1 = radius_ + thickness_;
  double z1 = thickness_/2;
  
  TGeoBBox *motherBoxZ1_ = new TGeoBBox("EcalMotherBoxZ1",x1,y1,z1,origin);
  //TGeoVolume *motherVolZ1_ = new TGeoVolume("EcalMotherVolZ1",motherBoxZ1_,medium_);
  volume_ = new TGeoVolume("Ecal1Volume",motherBoxZ1_,medium_);
  //volume_->GetMedium()->GetMaterial()->SetDensity(density_);

  origin[2] = -(length_+thickness_/2); 
  TGeoBBox *motherBoxZ2_ = new TGeoBBox("EcalMotherBoxZ2",x1,y1,z1,origin);
  TGeoVolume *motherVolZ2_ = new TGeoVolume("Ecal2Volume",motherBoxZ2_,medium_);
  
  // Build Ecal along x-direction
  origin[0] = radius_+thickness_/2, origin[1] = 0, origin[2] = 0;
  x1 = thickness_/2;
  y1 = radius_;
  z1 = length_;
  TGeoBBox *motherBoxX1_ = new TGeoBBox("EcalMotherBoxX1",x1,y1,z1,origin);
  TGeoVolume *motherVolX1_ = new TGeoVolume("Ecal3Volume",motherBoxX1_,medium_);

  origin[0] = -(radius_+thickness_/2);
  TGeoBBox *motherBoxX2_ = new TGeoBBox("EcalMotherBoxX2",x1,y1,z1,origin);
  TGeoVolume *motherVolX2_ = new TGeoVolume("Ecal4Volume",motherBoxX2_,medium_);

  // Build Ecal along y-direction
  origin[0] = 0, origin[1] = radius_+thickness_/2, origin[2] = 0;
  y1 = thickness_/2;
  x1 = radius_ + thickness_;
  z1 = length_;
  TGeoBBox *motherBoxY1_ = new TGeoBBox("EcalMotherBoxY1",x1,y1,z1,origin);
  TGeoVolume *motherVolY1_ = new TGeoVolume("Ecal5Volume",motherBoxY1_,medium_);

  origin[1] = -(radius_+thickness_/2);
  TGeoBBox *motherBoxY2_ = new TGeoBBox("EcalMotherBoxY2",x1,y1,z1,origin);
  TGeoVolume *motherVolY2_ = new TGeoVolume("Ecal6Volume",motherBoxY2_,medium_);

  // Add all the Ecal volumes
  //TGeoCompositeShape *compositeShape_ = new TGeoCompositeShape("(EcalMotherBoxX1 + EcalMotherBoxX2 +  EcalMotherBoxY1 + EcalMotherBoxY2 + EcalMotherBoxZ1 + EcalMotherBoxZ2)");

  //volume_ = new TGeoVolume("EcalVolume",compositeShape_,medium_);
  //volume_->GetMedium()->GetMaterial()->SetDensity(density_);
 
  volume_->AddNode(motherVolZ2_,1);
  volume_->AddNode(motherVolX1_,1);
  volume_->AddNode(motherVolX2_,1);
  volume_->AddNode(motherVolY1_,1);
  volume_->AddNode(motherVolY2_,1);
  //volume_->GetMedium()->GetMaterial()->SetDensity(density_);
}

EcalConstruction::~EcalConstruction()
{
  delete material_;
  delete medium_;
  delete tube_;
  delete volume_;
}

void EcalConstruction::print(){

  std::cout << "\n==================================================================="
	    << "\nEcalConstruction::print() Thickness: 	\t\t" << thickness_* (1 / CLHEP::m) <<" m"
	    << "\nEcalConstruction::print() Length: 		\t" << length_* (1 / CLHEP::m) <<" m"
    //<< "\nEcalConstruction::print() Volume: 		\t" << volumeSize* (1 / CLHEP::m3) <<" m3"
	    << "\nEcalConstruction::print() Density is: 	\t" << density_ * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\nEcalConstruction::print() Mass is: 	 	\t" << mass_ * (1 / CLHEP::kg) <<" kg"
	    << "\nEcalConstruction::print() Material is:  	\t" << materialName.c_str()
	    << "\n==================================================================="
	    << std::endl;

}
