void phase1()
{
  //Phase 1 detector geometry

  //load the geometry library and manager
  gSystem->Load("libGeom");
  TGeoManager *geom = new TGeoManager("Phase1 detector","Phase1 detector geometry");

  //define densities of materials/mixtures in gcm-3
  //----------------------------------------------------------------------
  Double_t argonDensity = 0.035;
  Double_t rockDensity = 2.323e-9;
  Double_t airDensity = 1.2e-3;
  Double_t scintDensity = 1.18;
 
  //define Materials/Mixtures
  //----------------------------------------------------------------------
  //TGeoMaterial *matVacuum = new TGeoMaterial("matVacuum",0,0,1e-9);
  TGeoMaterial *matArgon = new TGeoMaterial("matArgon",39.95,18,argonDensity);
  TGeoMaterial *matCarbon = new TGeoMaterial("matCarbon",12.1,6,2.265);
  TGeoMaterial *matOxygen = new TGeoMaterial("matOxygen",16,8,1);
  TGeoMaterial *matSilicon = new TGeoMaterial("matSilicon",28.09,14,1);
  TGeoMaterial *matHydrogen = new TGeoMaterial("matHydrogen",1.01,1,0.0708);
  TGeoMaterial *matNitrogen = new TGeoMaterial("matNitrogen",14,7,1);
  TGeoMaterial *matAluminium = new TGeoMaterial("matAluminium",26.98,13,2.7);

  TGeoMixture *matRock = new TGeoMixture("matRock",1,rockDensity);
  matRock->AddElement(matOxygen,0.53);
  matRock->AddElement(matSilicon,0.47);

  TGeoMixture *matAir = new TGeoMixture("matAir",2,airDensity);
  matAir->AddElement(matOxygen,0.22);
  matAir->AddElement(matNitrogen,0.78);

  TGeoMixture *matScint = new TGeoMixture("matScint",3,scintDensity);
  matScint->AddElement(matCarbon,0.85);
  matScint->AddElement(matHydrogen,0.15);

  //define Media
  //----------------------------------------------------------------------
  //TGeoMedium *vacuum = new TGeoMedium("Vacuum",1, matVacuum);
  TGeoMedium *argon = new TGeoMedium("Argon",2,matArgon);
  TGeoMedium *carbon = new TGeoMedium("Carbon",3,matCarbon);
  TGeoMedium *rock = new TGeoMedium("Rock",4,matRock);
  TGeoMedium *air = new TGeoMedium("Air",5,matAir);
  TGeoMedium *aluminium = new TGeoMedium("Aluminium",6,matAluminium);
  TGeoMedium *hydrogen = new TGeoMedium("Hydrogen",7,matHydrogen);
  TGeoMedium *scintillator = new TGeoMedium("Scintillator",8,matScint);

  //define dimensions in centimeters
  //----------------------------------------------------------------------
  //world cylinder - the rock
  Float_t worldR = 1500.;
  Float_t worldL = 3000.;

  Float_t scintThickness = 0.5;
  Int_t noOfScintLayers = 10;

  //vessel - cylinder
  Float_t vesselR = 200.;
  Float_t vesselL = 400.;

  //vessel - end distance -- must be equal to or less than the radius
  Float_t vesselEndMaxR = vesselR/2.0;
  //theta
  Float_t vesselEndTheta = 
	(180/TMath::Pi())*TMath::ASin( (2*vesselR*vesselEndMaxR) / 
	(vesselR*vesselR + vesselEndMaxR*vesselEndMaxR));
  //radius
  Float_t vesselEndRadius = (vesselR*vesselR + vesselEndMaxR*vesselEndMaxR) / (2*vesselEndMaxR);
  //displacement
  Float_t vesselEndDisplacement = vesselEndRadius*TMath::Cos(TMath::Pi()*vesselEndTheta/180.0);
  //((vesselR*vesselR + vesselEndMaxR*vesselEndMaxR - 2) / (2*vesselEndMaxR));

  //std::cout << "\ntheta is: " << vesselEndTheta << " deg"<<std::endl;
  //std::cout << "\ncos theta is: " << TMath::Cos(TMath::Pi()*vesselEndTheta/180.0) <<std::endl;
  //std::cout << "\ndisplacement is: " << vesselEndDisplacement << " cm" << std::endl;

  //vessel - thickness
  Float_t vesselThickness = 10.0;

  //fiducial cut thickness
  Float_t fiducialCutThickness = 100.0;
  
  //Inner vessel dimensions
  //cylinder
  Float_t innerVesselR = vesselR - vesselThickness;
  Float_t innerVesselL = vesselL;

  //end distance 
  Float_t innerVesselEndMaxR = vesselEndMaxR - vesselThickness;
  //theta
  Float_t innerVesselEndTheta = 
	(180/TMath::Pi())*TMath::ASin( (2*innerVesselR*innerVesselEndMaxR) / 
	(innerVesselR*innerVesselR + innerVesselEndMaxR*innerVesselEndMaxR));
  //radius
  Float_t innerVesselEndRadius = (innerVesselR*innerVesselR + innerVesselEndMaxR*innerVesselEndMaxR) 
				  / (2*innerVesselEndMaxR);
  //displacement
  Float_t innerVesselEndDisplacement = innerVesselEndRadius*TMath::Cos(TMath::Pi()*innerVesselEndTheta/180.0);

  //TPC dimensions
  Float_t tpcFidX = 120.0;//vesselEndMaxR - vesselThickness - fiducialCutThickness;
  Float_t tpcFidY = 120.0;//vesselEndMaxR - vesselThickness - fiducialCutThickness;
  Float_t tpcFidZ = 150.0;//vesselL - vesselThickness - fiducialCutThickness;


  //Define Volumes
  //-----------------------------------------------------------------------------------//
  //                              //
  //             world            //
  //                              //

  TGeoVolume *worldVol = geom->MakeTube("worldVol",rock,0,worldR,worldL);
  //make this volume our world
  geom->SetTopVolume(worldVol);

  //                              //
  //             vessel           //
  //                              //
  TGeoVolume *vesselMainVol = geom->MakeTube("vesselMainVol",aluminium,0,vesselR,vesselL/2.0);
  //make ends
  TGeoVolume *vesselEndFarVol =
	 geom->MakeSphere("vesselEndFarVol",aluminium,0,vesselEndRadius,0,vesselEndTheta,0,360);
  TGeoVolume *vesselEndNearVol =
	 geom->MakeSphere("vesselEndNearVol",aluminium,0,vesselEndRadius,180-vesselEndTheta,180,0,360);

  //                              //
  //           TPC Outer          //
  //                              //
  TGeoVolume * tpcOuterMainVol = geom->MakeTube("tpcOuterMainVol",argon,0,innerVesselR,innerVesselL/2.0);
  //make ends
  TGeoVolume * tpcOuterEndFarVol =
	 geom->MakeSphere("tpcOuterEndFarVol",argon,0,innerVesselEndRadius,0,innerVesselEndTheta,0,360);
  TGeoVolume * tpcOuterEndNearVol =
	 geom->MakeSphere("tpcOuterEndNearVol",argon,0,innerVesselEndRadius,
							180-innerVesselEndTheta,180,0,360);

  //                              //
  //         TPC Fiducial         //
  //                              //
  TGeoVolume * tpcFidMainVol = geom->MakeBox("tpcFidMainVol",argon,tpcFidX,tpcFidY,tpcFidZ);
  //                              //
  TGeoVolume * motherVol = geom->MakeBox("motherVol",argon,tpcFidX,tpcFidY,tpcFidZ);

  //                              //
  //         TPC Readout Plane    //
  //                              //
  //TGeoVolume * tpcFidPlaneVol = geom->MakeBox("tpcFidPlaneVol",argon,tpcFidX,tpcFidY,tpcFidZ);

  //                              //
  //         Scintillator bars    //
  //                              //

  // TGeoVolume * scintBarVol = geom->MakeBox("scintBarVol",scint,tpcFidX,tpcFidY,tpcFidZ);

  //define transformations
  //--------------------------------------------------------------------------------
  TGeoTranslation *origin = new TGeoTranslation(0.,0.,0.);
  TGeoTranslation *vesselMainPos = new TGeoTranslation(0., 0., vesselEndMaxR + vesselL/2.0);
  TGeoTranslation *vesselEndFarPos = new TGeoTranslation(0.,0.,(vesselL) + vesselEndMaxR - vesselEndDisplacement);
  TGeoTranslation *vesselEndNearPos = new TGeoTranslation(0.0,0.0,vesselEndRadius );
  TGeoTranslation *tpcOuterEndFarPos = new TGeoTranslation(0.,0.,
					(innerVesselL/2.0) - innerVesselEndDisplacement);
  TGeoTranslation *tpcOuterEndNearPos= new TGeoTranslation(0.,0.,
					innerVesselEndDisplacement - (innerVesselL/2.0));
 
  //-------For placement of volumes- Non overlapping nodes get prioity and higher level nodes----//

  //put detector in world
  worldVol->AddNode(tpcFidMainVol,1,new TGeoTranslation(0,0,tpcFidZ) );//vesselMainPos);
  //worldVol->AddNode(vesselMainVol,1,vesselMainPos); 
  //worldVol->AddNode(motherVol,1, new TGeoTranslation(0.,0.,2000.));

  //worldVol->AddNodeOverlap(vesselEndFarVol,2,vesselEndFarPos);
  //worldVol->AddNodeOverlap(vesselEndNearVol,3,vesselEndNearPos);
  
  // vesselMainVol->AddNode(tpcOuterMainVol,4,origin);
  //vesselEndNearVol->AddNode(tpcOuterEndNearVol,5,origin);
  //vesselEndFarVol->AddNode(tpcOuterEndFarVol,6,origin);

  // vesselMainVol->AddNodeOverlap(tpcOuterEndFarVol,5,tpcOuterEndFarPos);
  //vesselMainVol->AddNodeOverlap(tpcOuterEndNearVol,6,tpcOuterEndNearPos);

  //tpcOuterMainVol->AddNode(tpcFidMainVol,7,origin);

  //define colours for each layer
  worldVol->SetLineColor(kBlue);

  vesselMainVol->SetLineColor(kRed);
  vesselEndNearVol->SetLineColor(kRed);
  vesselEndFarVol->SetLineColor(kRed);

  tpcOuterMainVol->SetLineColor(kGreen);
  tpcOuterEndFarVol->SetLineColor(kGreen);
  tpcOuterEndNearVol->SetLineColor(kGreen);

  tpcFidMainVol->SetLineColor(kOrange);

  TLorentzVector preStep3Pos(0,0,-151,0);
  TLorentzVector postStep3Pos(0,0,150,0);

  //test points
  //find where it hit in the geometry
  geom->SetCurrentPoint (preStep3Pos.X(),
			preStep3Pos.Y() ,
			preStep3Pos.Z());
  geom->FindNode();
  TGeoVolume *  preVol_ = geom->GetCurrentVolume();
  std::string preVol_name = preVol_->GetName();

  geom->SetCurrentPoint (postStep3Pos.X(),
			postStep3Pos.Y(), 
			 postStep3Pos.Z());
  geom->FindNode();
  postVol_ = geom->GetCurrentVolume();
  std::string postVol_name = postVol_->GetName();

  if(preVol_){
    std::cout << "\n********************************"
	      << "\n PreVolume name is: "
	      << preVol_name 
	      << "\n PreVolume material is: "
	      << preVol_->GetMaterial()->GetName()
	      << "\n point (x,y,z) is: ("
	      << preStep3Pos.X() << ", " 
	      << preStep3Pos.Y() << ", "
	      << preStep3Pos.Z() << ")" 
	      << "\n********************************\n";
  }

  if(postVol_){
    std::cout << "\n********************************"
	      << "\n PostVolume name is: "
	      << postVol_name
	      << "\n PostVolume material is: "
	      << postVol_->GetMaterial()->GetName()
	      << "\n point (x,y,z) is: ("
	      << postStep3Pos.X() << ", " 
	      << postStep3Pos.Y() << ", "
	      << postStep3Pos.Z() << ")" 
	      << "\n********************************\n";
	}
  
  //close geometry
  geom->CloseGeometry();
  
  //write geometry to root file for GENIE
  geom->Export("phase1.root");
  geom->Export("phase1.gdml");
  
  //display the world
  worldVol->SetLineColor(kBlue);
  //geom->ViewLeaves(true);
  geom->SetVisLevel(-1);
  geom->SetTopVisible(0);
  geom->SetVisOption(0);
  worldVol->Draw();
  geom->SetVisOption(0);

}
