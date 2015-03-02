//
// Title: Scintillator bars design
//
// Author: Tom STainer
//
// Description: Scintillator bars geometry for root in the ND geometry

#include "ScintillatorConstruction.hh"

ScintillatorConstruction::ScintillatorConstruction() : additionalLayers_(0), theta(0)
{
  initDataCards();
  totalMass = 0.;
}

ScintillatorConstruction::~ScintillatorConstruction()
{
  delete tempBox_;
  delete tempVolume_;
  delete tempVolume2_;
  delete material_;
  delete motherMaterial_;
  delete medium_;
  delete motherMedium_;
  delete motherBox_;
  delete motherVolume_;
  delete motherTubeTop_;
  delete motherVolumeTop_;
  delete motherFarEnd_;
  delete t3;
  delete volume_;
}

void ScintillatorConstruction::initialize(){

  //load the data cards
  loadDataCards();  
  //volumeSize = layers_*thickness_;
}

void ScintillatorConstruction::construct(Materials * mats,TpcFiducialConstruction * tpc,TpcVesselConstruction * vessel){
  
  //set a pointer to the vessel object and tpc
  tpcPtr_ = tpc;
  vesselPtr_ = vessel;

  //create a mother volume to hold all nodes
  //material should match the same as the tpc
  materialName = tpc->getMatName();

  motherMaterial_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if( motherMaterial_ ){
  	motherMaterial_->SetDensity(tpc->getMaterial()->GetDensity()); //it should match the density of the tpc
  } 
  else{
	motherMaterial_ = (TGeoMixture*)mats->FindMaterial("ArgonGas");
	std::cout << "\n------------------------------------------------------------"
	    	<< "\nScintillatorConstruction::Construct MATERIAL NOT SET - USING VACUUM"
		<< "\n------------------------------------------------------------" <<std::endl;
  }
  motherMedium_ = new TGeoMedium("motherMedium",1,motherMaterial_);

  double motherX = tpc->getX() + (layers_*thickness_) +((layers_-1)*gap_);
  double motherY = tpc->getY() + (layers_*thickness_) +((layers_-1)*gap_);
  double motherZ = tpc->getZ() + (layers_*thickness_) +((layers_-1)*gap_);

  double lengthDiff = motherZ - vessel->getTubeLength()/2.0;
  double maxHeight = vessel->getInnerRadius();

  // if the tpc falls into the curved ends of the vessel then need to change the max height

  if(lengthDiff > 0)maxHeight = 
		TMath::Sqrt( (vessel->getInnerSphereRadius()*vessel->getInnerSphereRadius())
		 - (vessel->getInnerSphereRadius() - vessel->getEndLength() + lengthDiff)*
		   (vessel->getInnerSphereRadius() - vessel->getEndLength() + lengthDiff) );

  //make sure the box is limited to dimensions of vessel - restricted by sqrt(2) ~1.42
  while( (maxHeight*maxHeight) <= ( (motherX*motherX)+(motherY*motherY) ) || 
	 ((motherX*motherX)+(motherY*motherY) > vessel->getInnerRadius()*vessel->getInnerRadius()) ){
	/*
	std::cout << "\nTpc dims: (" << tpc->getX() << "," << tpc->getY() << "," << tpc->getZ() << ")";
	std::cout << "\nMother dims: (" << motherX << "," << motherY << "," << motherZ << ")";
	std::cout << "\nRadius: " << vessel->getInnerRadius();
	std::cout << "\nSphereRadius: " << vessel->getInnerSphereRadius();
	std::cout << "\nEnd length: " << vessel->getEndLength();
	std::cout << "\nTube length: " << vessel->getTubeLength();
	std::cout << "\nLength diff: " << lengthDiff;
	std::cout << "\nMax height: " << maxHeight;
	std::cout << "\nSqrt(x2 + y2): " << TMath::Sqrt((motherX*motherX)+(motherY*motherY));
	std::cout << "\nLayers: " << layers_;
	std::cout << "\n---------------------";
	*/
	//reduce the number of layers in order to fit
	layers_ -= 1;

	motherX = tpc->getX() + (layers_*thickness_) +((layers_-1)*gap_);
	motherY = tpc->getY() + (layers_*thickness_) +((layers_-1)*gap_);
	motherZ = tpc->getZ() + (layers_*thickness_) +((layers_-1)*gap_);

 	lengthDiff = motherZ - vessel->getTubeLength()/2.0;

  	if(lengthDiff > 0)maxHeight = 
				TMath::Sqrt( (vessel->getInnerSphereRadius()*vessel->getInnerSphereRadius())
				 - (vessel->getInnerSphereRadius() - vessel->getEndLength() + lengthDiff)*
				   (vessel->getInnerSphereRadius() - vessel->getEndLength() + lengthDiff) );
	else maxHeight = vessel->getInnerRadius();		
  }
  //set the ratio of x to y expressed in theta - in degrees
  theta = (180./TMath::Pi())*TMath::ATan(motherY/motherX);

  //main section
  motherBox_ = new TGeoBBox("motherBox",motherX/(CLHEP::cm),motherY/(CLHEP::cm),motherZ/(CLHEP::cm));
  //motherVolume_ = new TGeoVolume("motherVolume",motherBox_,motherMedium_);
  //top Y section
  motherTubeTop_ = new TGeoTubeSeg("motherTubeTop",0,
	vesselPtr_->getInnerRadius()/(CLHEP::cm),vesselPtr_->getTubeLength()/(2.*CLHEP::cm),
	theta,180+theta);
  motherVolumeTop_ = new TGeoVolume("motherVolumeTop",motherTubeTop_,motherMedium_);
  //bottom y section
  motherTubeBottom_ = new TGeoTubeSeg("motherTubeBottom",0,
	vesselPtr_->getInnerRadius()/(CLHEP::cm),vesselPtr_->getTubeLength()/(2.*CLHEP::cm),
	180+theta,360-theta);
  motherVolumeBottom_ = new TGeoVolume("motherVolumeBottom",motherTubeBottom_,motherMedium_);
  //left hand side X
  motherTubeLeft_ = new TGeoTubeSeg("motherTubeLeft",0,
	vesselPtr_->getInnerRadius()/(CLHEP::cm),vesselPtr_->getTubeLength()/(2.*CLHEP::cm),
	180-theta,180+theta);
  motherVolumeLeft_ = new TGeoVolume("motherVolumeLeft",motherTubeLeft_,motherMedium_);
  //right hand side X
  motherTubeRight_ = new TGeoTubeSeg("motherTubeRight",0,
	vesselPtr_->getInnerRadius()/(CLHEP::cm),vesselPtr_->getTubeLength()/(2.*CLHEP::cm),
	360-theta,theta);
  motherVolumeRight_ = new TGeoVolume("motherVolumeRight",motherTubeRight_,motherMedium_);
  motherFarEnd_ = new TGeoSphere(*(vesselPtr_->getInnerNearEndSphereShape()));
  t3 = new TGeoTranslation(*(vesselPtr_->getInnerNearEndSpherePosition()));

  TGeoCompositeShape *compositeShape_ = 
	new TGeoCompositeShape("(motherBox + (motherTubeTop) + (motherTubeBottom) + (motherTubeRight) + (motherTubeLeft) + (innerVesselNearEnd:t3))" );
  motherVolume_ = new TGeoVolume("scintMotherVolume",compositeShape_,motherMedium_);

  //SCINTILLATOR BARS
  materialName = "Scintillator";

  material_ = (TGeoMixture*)mats->FindMaterial(materialName.c_str());
  if( material_ ){
  	material_->SetDensity(density_/(CLHEP::g/CLHEP::cm3)); //it should match the density of the tpc
  } 
  else{
	material_ = (TGeoMixture*)mats->FindMaterial("Vacuum");
	std::cout << "\n------------------------------------------------------------"
	    	<< "\nScintillatorConstruction::Construct MATERIAL NOT SET - USING VACUUM"
		<< "\n------------------------------------------------------------" <<std::endl;
  }
  //medium
  medium_ = new TGeoMedium(materialName.c_str(),1,material_);

  double lastX = tpc->getX();
  double lastY = tpc->getY();
  double basePosZ = tpc->getZ()+thickness_/2.0;

  //first layer - add tiny space between tpc outer and first layer
  double space = 0.1*(CLHEP::cm);

  //make sure mother is bigger than scint panels
  //layers_ -= 1;

  //-------------------------------------------------------------------//
  //---------------------- Z PLANES -----------------------------------//
  //-------------------------------------------------------------------//
  double lastZScintPosition = 0;
  double lastXWidth = 0.;
  double lastYWidth = 0.;
  for(int i=0;i<layers_;i++){
    //for bars perp to z axis
    if(i==0){
	x_ = tpc->getX();
    	y_ = tpc->getY();
    }

    else{
    	//n th layer
    	x_ = lastX + thickness_ + gap_;
    	y_ = lastY + thickness_ + gap_;
    }
    
    lastX = x_;
    lastY = y_;

    //shapes and volumes
    tempBox_ = new TGeoBBox(Form("scintBoxZ_%i",i),x_/(CLHEP::cm),y_/(CLHEP::cm),thickness_/(2.0*(CLHEP::cm)));
    tempVolume_ = new TGeoVolume(Form("scintVolumeZ_%i",i),tempBox_,medium_);
    tempVolume2_ = new TGeoVolume(Form("scintVolume-Z_%i",i),tempBox_,medium_);
    //divide the box into slices
    double step = 2.0*(CLHEP::cm);
    int N = (2*x_/(CLHEP::cm))/(step/(CLHEP::cm));
    //TGeoVolume * tempDivX_ = tempVolume_->Divide("scintVolumeZsliceX", 1, 6,-x_/(CLHEP::cm),step);
    //tempDivX_->SetLineColor(1);

    tempVolume_->SetLineColor(kTeal-6);
    tempVolume2_->SetLineColor(kTeal-6);

    if(i==0){
      motherVolume_->AddNode(tempVolume_,i,
		new TGeoTranslation(0,0,basePosZ/(CLHEP::cm) + space/(CLHEP::cm)));
      motherVolume_->AddNode(tempVolume2_,i+layers_,
		new TGeoTranslation(0,0,-basePosZ/(CLHEP::cm) - space/(CLHEP::cm)));
    }
    else{
      motherVolume_->AddNode(tempVolume_,i,
		new TGeoTranslation(0,0,basePosZ/(CLHEP::cm) + i*(thickness_+gap_)/(CLHEP::cm)));
      motherVolume_->AddNode(tempVolume2_,i+layers_,
		new TGeoTranslation(0,0,-basePosZ/(CLHEP::cm) - i*(thickness_+gap_)/(CLHEP::cm)));
    }

    lastZScintPosition = basePosZ/(CLHEP::cm) + i*(thickness_+gap_)/(CLHEP::cm);
    lastXWidth = lastX;
    lastYWidth = lastY;

    //add mass to give value for mass of whole scintillator system
    totalMass += 2.*calculateMass(tempBox_);	//times by two as two planes either side

  }

  addExtraScintInZ(lastZScintPosition,lastXWidth/(CLHEP::cm),lastYWidth/(CLHEP::cm));

  //-------------------------------------------------------------------//
  //---------------------- Y PLANES -----------------------------------//
  //-------------------------------------------------------------------//

  lastX = tpc->getX();
  double basePosY = tpc->getY()+thickness_/2.0;
  double lastZ = tpc->getZ();
  double lastYScintPosition = 0;

  for(int i=0;i<layers_;i++){
    //for bars perp to y axis
    //first layer
    if(i==0){
	x_ = tpc->getX();
    	z_ = tpc->getZ() + thickness_;
    }

    else{
    	//n th layer
    	x_ = lastX + thickness_ + gap_;
    	z_ = lastZ + thickness_ + gap_;
    }
    
    lastX = x_;
    lastZ = z_;

    //shapes and volumes
    tempBox_ = new TGeoBBox(Form("scintBoxY_%i",i),x_/(CLHEP::cm),thickness_/(2.0*(CLHEP::cm)),z_/(CLHEP::cm));
    tempVolume_ = new TGeoVolume(Form("scintVolumeY_%i",i),tempBox_,medium_);
    tempVolume2_ = new TGeoVolume(Form("scintVolume-Y_%i",i),tempBox_,medium_);

    tempVolume_->SetLineColor(kTeal-6);
    tempVolume2_->SetLineColor(kTeal-6);

    if(i==0){
      motherVolume_->AddNode(tempVolume_,i,
	new TGeoTranslation(0,basePosY/(CLHEP::cm) + space/(CLHEP::cm),0));
      motherVolume_->AddNode(tempVolume2_,i+layers_,
	new TGeoTranslation(0,-basePosY/(CLHEP::cm) - space/(CLHEP::cm),0));
    }
    else {
      motherVolume_->AddNode(tempVolume_,i,
	new TGeoTranslation(0,basePosY/(CLHEP::cm) + i*(thickness_ + gap_)/(CLHEP::cm),0));
      motherVolume_->AddNode(tempVolume2_,i+layers_,
	new TGeoTranslation(0,-basePosY/(CLHEP::cm) - i*(thickness_+ gap_)/(CLHEP::cm),0));
    }

    lastYScintPosition = basePosY/(CLHEP::cm) + i*(thickness_ + gap_)/(CLHEP::cm);

    totalMass += 2.*calculateMass(tempBox_);	//times by two as two planes either side
  }

  addExtraScintInY(lastYScintPosition);

  //-------------------------------------------------------------------//
  //---------------------- X PLANES -----------------------------------//
  //-------------------------------------------------------------------//

  double basePosX = tpc->getX()+thickness_/2.0;
  lastX = tpc->getX();
  lastZ = tpc->getZ();
  double lastXScintPosition = 0;

  for(int i=0;i<layers_;i++){
    //for bars perp to x axis
    //first layer
    if(i==0){
	y_ = tpc->getY() + thickness_;
    	z_ = tpc->getZ() + thickness_;
    }

    else{
    	//n th layer
    	y_ = lastY + thickness_ + gap_;
    	z_ = lastZ + thickness_ + gap_;
    }
    
    lastY = y_;
    lastZ = z_;

    //shapes and volumes
    tempBox_ = new TGeoBBox(Form("scintBoxX_%i",i),thickness_/(2.0*(CLHEP::cm)),y_/(CLHEP::cm),z_/(CLHEP::cm));
    tempVolume_ = new TGeoVolume(Form("scintVolumeX_%i",i),tempBox_,medium_);
    tempVolume2_ = new TGeoVolume(Form("scintVolume-X_%i",i),tempBox_,medium_);

    tempVolume_->SetLineColor(kTeal-6);
    tempVolume2_->SetLineColor(kTeal-6);

    if(i==0){
      motherVolume_->AddNode(tempVolume_,i,
	new TGeoTranslation(basePosX/(CLHEP::cm) + space/(CLHEP::cm),0,0));
      motherVolume_->AddNode(tempVolume2_,i+layers_,
			new TGeoTranslation(-basePosX/(CLHEP::cm) - space/(CLHEP::cm),0,0));
    }
    else{
      motherVolume_->AddNode(tempVolume_,i,
	new TGeoTranslation(basePosX/(CLHEP::cm) + i*(thickness_+gap_)/(CLHEP::cm),0,0));
      motherVolume_->AddNode(tempVolume2_,i+layers_,
			new TGeoTranslation(-basePosX/(CLHEP::cm) - i*(thickness_+gap_)/(CLHEP::cm),0,0));
    }
    tempVolume_->SetLineColor(kTeal-6);
    tempVolume2_->SetLineColor(kTeal-6);
    motherVolume_->SetLineColor(kTeal-6);

    lastXScintPosition = basePosX/(CLHEP::cm) + i*(thickness_ + gap_)/(CLHEP::cm);

    totalMass += 2.*calculateMass(tempBox_);	//times by two as two planes either side
  }

  addExtraScintInX(lastXScintPosition);

  volume_ = motherVolume_;
  volume_->SetLineColor(kTeal-6);
  volume_->SetVisibility(kFALSE);
}

void ScintillatorConstruction::addExtraScintInY(double lastYScintPosition){

  //More scintillator in Y planes at curved ends
  // TOP Level
  //create an extra volume for the curved tops for scintillator
  double lastYScintPositionPositive = lastYScintPosition;
  
  double heightOfPrevLayer = lastYScintPositionPositive + (0.5*thickness_/(CLHEP::cm));
  double heightOfNewLayer = lastYScintPositionPositive + (1.5*thickness_/(CLHEP::cm));

  double newScintWidth = 0.0;

  int i = 1;
  int layerIndex = layers_;
  double rho = heightOfNewLayer / (TMath::Cos((90 - theta)*TMath::Pi()/180. ));
  double rhoLower = heightOfPrevLayer / (TMath::Cos((90 - theta)*TMath::Pi()/180. ));

  while( heightOfNewLayer < vesselPtr_->getInnerRadius()/(CLHEP::cm)){
	
	double lowerWidth = (heightOfPrevLayer*TMath::Tan((90 - theta)*TMath::Pi()/180. ));
	double topWidth = TMath::Sqrt( (vesselPtr_->getInnerRadius()/(CLHEP::cm)
	 	*vesselPtr_->getInnerRadius() /(CLHEP::cm))  - (heightOfNewLayer*heightOfNewLayer));

	if(layersRequested_ <=layerIndex)break;

  	if( rhoLower <= vesselPtr_->getInnerRadius()/(CLHEP::cm) && 
	    rho >= vesselPtr_->getInnerRadius()/(CLHEP::cm) ){
		if( lowerWidth < topWidth)newScintWidth = lowerWidth;
		else newScintWidth = topWidth;	
	}
	else if(rhoLower <= vesselPtr_->getInnerRadius()/(CLHEP::cm)) {
	    newScintWidth = lowerWidth;
	}
	else newScintWidth = topWidth;

  	tempBox_ = new
	 	TGeoBBox(Form("scintBoxY_%i",layerIndex),newScintWidth,
				thickness_/(2.0*(CLHEP::cm)),vesselPtr_->getTubeLength()/(2.*CLHEP::cm));
    	tempVolume_ = new TGeoVolume(Form("scintVolumeY_%i",layerIndex),tempBox_,medium_);
	tempVolume_->SetLineColor(kTeal-6);

	motherVolume_->AddNode(tempVolume_,layerIndex,
		new TGeoTranslation(0,lastYScintPositionPositive + (thickness_+ gap_)/(CLHEP::cm),0));

	lastYScintPositionPositive = lastYScintPositionPositive + thickness_/(CLHEP::cm);
	heightOfPrevLayer = heightOfPrevLayer + thickness_/(CLHEP::cm);
	heightOfNewLayer = heightOfNewLayer + thickness_/(CLHEP::cm);

	rho = heightOfNewLayer / (TMath::Cos((90 - theta)*TMath::Pi()/180. ));
	rhoLower = heightOfPrevLayer / (TMath::Cos((90 - theta)*TMath::Pi()/180. ));
    
	i++;
	layerIndex++;  

    	totalMass += calculateMass(tempBox_);
  }

  additionalLayers_ = layerIndex - layers_;

  // Bottom Level
  //create an extra volume for the curved tops for scintillator  
  double lastYScintPositionNegative = -lastYScintPosition;
  
  heightOfPrevLayer = lastYScintPositionNegative - (0.5*thickness_/(CLHEP::cm));
  heightOfNewLayer = lastYScintPositionNegative - (1.5*thickness_/(CLHEP::cm));

  newScintWidth = 0.0;

  i = 1;
  layerIndex = layers_;
  rho = TMath::Abs(heightOfNewLayer) / (TMath::Cos((90 - theta)*TMath::Pi()/180. ));
  rhoLower = TMath::Abs(heightOfPrevLayer) / (TMath::Cos((90 - theta)*TMath::Pi()/180. ));

  while( TMath::Abs(heightOfNewLayer) < vesselPtr_->getInnerRadius()/(CLHEP::cm)) {

	double lowerWidth = TMath::Abs(heightOfPrevLayer*TMath::Tan((90 - theta)*TMath::Pi()/180. ));
	double topWidth = TMath::Sqrt( (vesselPtr_->getInnerRadius()/(CLHEP::cm)
	 	*vesselPtr_->getInnerRadius() /(CLHEP::cm))  - (heightOfNewLayer*heightOfNewLayer));

	if(layersRequested_ <=layerIndex)break;

  	if( rhoLower <= vesselPtr_->getInnerRadius()/(CLHEP::cm) && 
	    rho >= vesselPtr_->getInnerRadius()/(CLHEP::cm) ){
		if( lowerWidth < topWidth)newScintWidth = lowerWidth;
		else newScintWidth = topWidth;	
	}
	else if(rhoLower <= vesselPtr_->getInnerRadius()/(CLHEP::cm)) {
	    newScintWidth = lowerWidth;
	}
	else newScintWidth = topWidth;

  	tempBox_ = new
	 	TGeoBBox(Form("scintBox-Y_%i",layerIndex),newScintWidth,
				thickness_/(2.0*(CLHEP::cm)),vesselPtr_->getTubeLength()/(2.*CLHEP::cm));
    	tempVolume_ = new TGeoVolume(Form("scintVolume-Y_%i",layerIndex),tempBox_,medium_);
	tempVolume_->SetLineColor(kTeal-6);

	motherVolume_->AddNode(tempVolume_,layerIndex+additionalLayers_,
		new TGeoTranslation(0,lastYScintPositionNegative - (thickness_+ gap_)/(CLHEP::cm),0));

	lastYScintPositionNegative = lastYScintPositionNegative - thickness_/(CLHEP::cm);
	heightOfPrevLayer = heightOfPrevLayer - thickness_/(CLHEP::cm);
	heightOfNewLayer = heightOfNewLayer - thickness_/(CLHEP::cm);
	
	rho = TMath::Abs(heightOfNewLayer) / (TMath::Cos((90 - theta)*TMath::Pi()/180. )) ;
	rhoLower = TMath::Abs(heightOfPrevLayer) / (TMath::Cos((90 - theta)*TMath::Pi()/180. ));
    
	i++;  
	layerIndex++;  

    	totalMass += calculateMass(tempBox_);
  }

}

void ScintillatorConstruction::addExtraScintInX(double lastXScintPosition){

  //More scintillator in X planes at curved ends
  // Left Level
  //create an extra volume for the curved tops for scintillator
  double lastXScintPositionPositive = lastXScintPosition;

  double heightOfPrevLayer = lastXScintPositionPositive + (0.5*thickness_/(CLHEP::cm));
  double heightOfNewLayer = lastXScintPositionPositive + (1.5*thickness_/(CLHEP::cm));

  double newScintWidth = 0.0;

  int i = 1;
  int layerIndex = layers_;
  double rho = heightOfNewLayer / (TMath::Cos(theta*TMath::Pi()/180.) );
  double rhoLower = heightOfPrevLayer / (TMath::Cos(theta*TMath::Pi()/180.) );

  while( heightOfNewLayer < vesselPtr_->getInnerRadius()/(CLHEP::cm) ){

	double lowerWidth = (heightOfPrevLayer*TMath::Tan(theta*TMath::Pi()/180.));
	double topWidth = TMath::Sqrt( (vesselPtr_->getInnerRadius()/(CLHEP::cm)
	 	*vesselPtr_->getInnerRadius() /(CLHEP::cm))  - (heightOfNewLayer*heightOfNewLayer));

	if(layersRequested_ <=layerIndex)break;

  	if( rhoLower <= vesselPtr_->getInnerRadius()/(CLHEP::cm) && 
	    rho >= vesselPtr_->getInnerRadius()/(CLHEP::cm) ){
		if( lowerWidth < topWidth)newScintWidth = lowerWidth;
		else newScintWidth = topWidth;	
	}
	else if(rhoLower <= vesselPtr_->getInnerRadius()/(CLHEP::cm)) {
	    newScintWidth = lowerWidth;
	}
	else newScintWidth = topWidth;

  	tempBox_ = new
	 	TGeoBBox(Form("scintBoxX_%i",layerIndex),thickness_/(2.0*(CLHEP::cm)),
			newScintWidth,vesselPtr_->getTubeLength()/(2.*CLHEP::cm));
    	tempVolume_ = new TGeoVolume(Form("scintVolumeX_%i",layerIndex),tempBox_,medium_);
	tempVolume_->SetLineColor(kTeal-6);

	motherVolume_->AddNode(tempVolume_,layerIndex+ 2*additionalLayers_,
		new TGeoTranslation(lastXScintPositionPositive + (thickness_+ gap_)/(CLHEP::cm),0,0));

	lastXScintPositionPositive = lastXScintPositionPositive + thickness_/(CLHEP::cm);
	
	heightOfPrevLayer = heightOfPrevLayer + thickness_/(CLHEP::cm);
	heightOfNewLayer = heightOfNewLayer + thickness_/(CLHEP::cm);

	rho = heightOfNewLayer / (TMath::Cos(theta*TMath::Pi()/180. ));
	rhoLower = heightOfPrevLayer / (TMath::Cos(theta*TMath::Pi()/180. ));
    
	i++;
	layerIndex++;  

    	totalMass += calculateMass(tempBox_);
  }

  // Right Level
  //create an extra volume for the curved tops for scintillator  
  double lastXScintPositionNegative = -lastXScintPosition;

  heightOfPrevLayer = lastXScintPositionNegative - (0.5*thickness_/(CLHEP::cm));
  heightOfNewLayer = lastXScintPositionNegative - (1.5*thickness_/(CLHEP::cm));

  newScintWidth = 0.0;

  i = 1;
  layerIndex = layers_;
  rho = TMath::Abs(heightOfNewLayer) / (TMath::Cos(theta*TMath::Pi()/180.) );
  rhoLower = heightOfPrevLayer / (TMath::Cos(theta*TMath::Pi()/180.) );

  while( TMath::Abs(heightOfNewLayer) <  vesselPtr_->getInnerRadius()/(CLHEP::cm)) {

	double lowerWidth = (TMath::Abs(heightOfPrevLayer)*TMath::Tan(theta*TMath::Pi()/180.));
	double topWidth = TMath::Sqrt( (vesselPtr_->getInnerRadius()/(CLHEP::cm)
	 	*vesselPtr_->getInnerRadius() /(CLHEP::cm))  - (heightOfNewLayer*heightOfNewLayer));

	if(layersRequested_ <=layerIndex)break;

  	if( rhoLower <= vesselPtr_->getInnerRadius()/(CLHEP::cm) && 
	    rho >= vesselPtr_->getInnerRadius()/(CLHEP::cm) ){
		if( lowerWidth < topWidth)newScintWidth = lowerWidth;
		else newScintWidth = topWidth;	
	}
	else if(rhoLower <= vesselPtr_->getInnerRadius()/(CLHEP::cm)) {
	    newScintWidth = lowerWidth;
	}
	else newScintWidth = topWidth;

  	tempBox_ = new
	 	TGeoBBox(Form("scintBox-X_%i",layerIndex),thickness_/(2.0*(CLHEP::cm)),
				newScintWidth,vesselPtr_->getTubeLength()/(2.*CLHEP::cm));
    	tempVolume_ = new TGeoVolume(Form("scintVolume-X_%i",layerIndex),tempBox_,medium_);
	tempVolume_->SetLineColor(kTeal-6);

	motherVolume_->AddNode(tempVolume_,layerIndex+ 3*additionalLayers_,
		new TGeoTranslation(lastXScintPositionNegative - (thickness_+ gap_)/(CLHEP::cm),0,0));

	lastXScintPositionNegative = lastXScintPositionNegative - thickness_/(CLHEP::cm);
	
	heightOfPrevLayer = heightOfPrevLayer - thickness_/(CLHEP::cm);
	heightOfNewLayer = heightOfNewLayer - thickness_/(CLHEP::cm);

	rho = TMath::Abs(heightOfNewLayer) / (TMath::Cos(theta*TMath::Pi()/180.) );
	rhoLower = TMath::Abs(heightOfPrevLayer) / (TMath::Cos(theta*TMath::Pi()/180.) );
    
	i++;  
	layerIndex++;  

    	totalMass += calculateMass(tempBox_);
  }

}
void ScintillatorConstruction::addExtraScintInZ(double lastZScintPosition,double lastXWidth,double lastYWidth){

  // Right Level
  //create an extra volume for the curved tops for scintillator  
  double lastZScintPositionPositive = lastZScintPosition;
  double zOffset = vesselPtr_->getInnerFarEndSphereZOffset();//(CLHEP::cm);

  double heightOfPrevLayer = lastZScintPositionPositive + 
		(0.5*thickness_/(CLHEP::cm));// + zOffset/(CLHEP::cm);
  double heightOfNewLayer = lastZScintPositionPositive + 
		(1.5*thickness_/(CLHEP::cm));// + zOffset/(CLHEP::cm);

  double newScintRadius = 0.0;
  double newScintXWidth = 0.0;
  double newScintYWidth = 0.0;

  int i = 1;
  int layerIndex = layers_;

  while( TMath::Abs(heightOfNewLayer) < vesselPtr_->getLength()/(2.0*CLHEP::cm)) {

	if(layersRequested_ <=layerIndex)break;
	
	//is it in the tube part of curved end?
  	if( TMath::Abs(heightOfNewLayer) < vesselPtr_->getTubeLength()/(2.0*CLHEP::cm) ){
		newScintXWidth = lastXWidth + thickness_/(CLHEP::cm);
		newScintYWidth = lastYWidth + thickness_/(CLHEP::cm);

		if(newScintXWidth < thickness_/(CLHEP::cm) || 
	   	newScintYWidth < thickness_/(CLHEP::cm))break;

  		tempBox_ = new
	 	  TGeoBBox(Form("scintBox-Z_%i",layerIndex),newScintXWidth,
				newScintYWidth,thickness_/(2.0*(CLHEP::cm)));
    		tempVolume_ = new TGeoVolume(Form("scintVolume-Z_%i",layerIndex),tempBox_,medium_);
	}
	else{
		double lSqrd = (vesselPtr_->getInnerSphereRadius()/(CLHEP::cm)
	 	*vesselPtr_->getInnerSphereRadius() /(CLHEP::cm))  - 
		(heightOfNewLayer + zOffset)*(heightOfNewLayer + zOffset);

		if(lSqrd < 0.0)break;

		newScintRadius = TMath::Sqrt(lSqrd) - thickness_/(2.0*CLHEP::cm);
		//newScintXWidth = TMath::Sqrt(1/2.)*TMath::Sqrt(lSqrd) - thickness_/(2.0*CLHEP::cm);
		//newScintYWidth = TMath::Sqrt(1/2.)*TMath::Sqrt(lSqrd) - thickness_/(2.0*CLHEP::cm);
  		tempTube_ = new
	 	  TGeoTube(Form("scintBox-Z_%i",layerIndex),0,
				newScintRadius,thickness_/(2.0*(CLHEP::cm)));
    		tempVolume_ = new TGeoVolume(Form("scintVolume-Z_%i",layerIndex),tempTube_,medium_);
	}

	tempVolume_->SetLineColor(kTeal-6);

	motherVolume_->AddNode(tempVolume_,layerIndex+ 3*additionalLayers_,
		new TGeoTranslation(0,0,
			lastZScintPositionPositive + (thickness_+ gap_)/(CLHEP::cm)));

	lastZScintPositionPositive = lastZScintPositionPositive + thickness_/(CLHEP::cm);
	
	heightOfPrevLayer = heightOfPrevLayer + thickness_/(CLHEP::cm);
	heightOfNewLayer = heightOfNewLayer + thickness_/(CLHEP::cm);
    
	i++;  
	layerIndex++;  

    	totalMass += calculateMass(tempBox_);
  }

}

void ScintillatorConstruction::setDensity(double density){
  density_ = density;
  volume_->GetMedium()->GetMaterial()->SetDensity(density_);
}

double ScintillatorConstruction::calculateMass(TGeoBBox * box){

    //add mass to give value for mass of whole scintillator system
    double tempVolumeSize = (box->GetDX() * 2.*CLHEP::cm)*(box->GetDY() * 2.*CLHEP::cm)*(box->GetDZ() * 2.*CLHEP::cm);
    double tempDensity = material_->GetDensity()*(CLHEP::g/CLHEP::cm3);
    //totalMass += tempVolumeSize*tempDensity;

    return tempVolumeSize*tempDensity;
}

void ScintillatorConstruction::initDataCards() {
  cards_ = DataCards::getInstance();
  //char* LBNO = getenv("LBNO");

  layersRequested_ = 10;
  cards_->AddDataCardInt("scintLayers", layersRequested_);
  cards_->AddDataCardDouble("scintGap", 2.0*CLHEP::mm);
  cards_->AddDataCardDouble("scintThickness", 5.0*CLHEP::mm);
  cards_->AddDataCardDouble("scintDensity", 1.18*(CLHEP::g/CLHEP::cm3));
  cardsLoaded_ = false;
}

void ScintillatorConstruction::loadDataCards() {

  layers_  = cards_->fetchValueInt("scintLayers");
  layersRequested_ = layers_;
  thickness_  = cards_->fetchValueDouble("scintThickness");
  gap_  = cards_->fetchValueDouble("scintGap");
  density_  = cards_->fetchValueDouble("scintDensity");
  cardsLoaded_ = true;

  std::cout <<"\n******************************************"
	    << "\nScintillatorConstruction Data Card loaded."
	    <<"\n******************************************"
	    << std::endl;
}

void ScintillatorConstruction::print(){

  //root stores densities in gcm-3
  double tmp_density = material_->GetDensity()*(CLHEP::g/CLHEP::cm3);
  double tmp_mother_density = motherMaterial_->GetDensity()*(CLHEP::g/CLHEP::cm3);

  std::cout << "\n==================================================================="
	    << "\nScintillatorConstruction::print() Number of Layers: 	" << layers_ + additionalLayers_
	    << "\nScintillatorConstruction::print() Thickness: 		" << thickness_* (1 / CLHEP::mm) <<" mm"
	    << "\nScintillatorConstruction::print() Gap: 		\t" << gap_* (1 / CLHEP::mm) <<" mm"
	    //<< "\nScintillatorConstruction::print() Volume: 		" << volumeSize* (1 / CLHEP::m3) <<" m3"
	    << "\nScintillatorConstruction::print() Density is: 	\t" 
	    << tmp_density * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\nScintillatorConstruction::print() MotherDensity is: 	" 
	    << tmp_mother_density * (CLHEP::m3 / CLHEP::kg) <<" kgm-3"
	    << "\nScintillatorConstruction::print() Total Mass is: 	" << totalMass * (1/CLHEP::kg) << " kg"
	    << "\n==================================================================="
	    << std::endl;

}
