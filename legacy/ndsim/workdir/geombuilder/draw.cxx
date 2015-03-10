{
  gSystem->Load("libGeom");
  TGeoManager *geom = TGeoManager::Import("geometry_2x2x2.root");
  //TGeoManager *geom = new TGeoManager("Detector geometry","Detector geometry");
  //TGeoVolume * rockVolume = geom->GetVolume("rockVolume");
  TGeoVolume * rockVolume = geom->GetTopVolume();
  //draws volume better
  rockVolume->CheckOverlaps(0.0001);
  TGeoVolume * vesselVolume = geom->GetVolume("vesselVolume");
  TGeoVolume * tpcFiducialVolume = geom->GetVolume("tpcFidVolume");
  TGeoVolume * innerVesselVolume = geom->GetVolume("innerVesselVolume");
  TGeoVolume * cavityVolume = geom->GetVolume("cavityVolume");
  TGeoVolume * magnetVolume = geom->GetVolume("magnetVolume");
  TGeoVolume * mindVolume = geom->GetVolume("mindMotherVolume");
  TGeoVolume * scintMotherVolume = geom->GetVolume("scintMotherVolume");

  double min=0, max=0;
  TGeoShape * bounding_box = tpcFiducialVolume->GetShape();
  double xrange = bounding_box->GetAxisRange(1, min, max);
  double yrange = bounding_box->GetAxisRange(2, min, max);
  double zrange = bounding_box->GetAxisRange(3, min, max);
 
  std::cout << "\nThe x axis range is: " << xrange 
 		<< "\nThe y axis range is: " << yrange
		<< "\nThe z axis range is: " << zrange << std::endl;

  //VISIBILITY OPTIONS
  //vesselVolume->SetVisibility(kFALSE);
  //innerVesselVolume->SetVisibility(kFALSE);
  //tpcFiducialVolume->SetVisibility(kFALSE);
  cavityVolume->SetVisibility(kFALSE);
  //scintMotherVolume->SetVisibility(kTRUE);
  //magnetVolume->SetVisibility(kFALSE);
  //geom->SetTopVisible(kTRUE);
  //geom->SetTopVisible(kFALSE);
  geom->SetVisOption(0);
  geom->SetVisLevel(10);
  geom->GetMasterVolume()->Draw();
  //geom->GetMasterVolume()->Draw("ogl");
  geom->SetVisOption(0);

  TLorentzVector point(0,0,10.,0);

  //put points from meters to centimeters
  // point *= 100;//CLHEP::m;

  //test points
  //find where it hit in the geometry
  geom->SetCurrentPoint (point.X(),point.Y(),point.Z()); 

  TGeoVolume *  pointVol_ = geom->GetCurrentVolume();
  std::string pointVol_name = pointVol_->GetName();
  int nDaughters = pointVol_->GetNdaughters();

  TGeoVolume * daughterVol = pointVol_;
  TGeoVolume * baseVol = daughterVol;
  std::string baseVol_name = baseVol->GetName();
  /*
  //find lowest node
  for(int i=0;i<nDaughters;i++){
    
    baseVol = daughterVol->GetNode(i)->GetVolume();
    baseVol_name = baseVol->GetName();

    if(nDaughters>0){
      
    }
    else baseVol = pointVol_;

    nDaughters = daughterVol->GetNdaughters();
  }
*/
  //pointVol_->RandomRays();
  //pointVol_->Raytrace();
 
  geom->DrawCurrentPoint(kBlack);
  pointVol = geom->FindNode(point.X(),point.Y(),point.Z())->GetVolume();

  pointVol_name =  pointVol_->GetName();

  if(pointVol_){
    std::cout << "\n********************************"
              << "\n PointVolume name is: "
              << pointVol_name 
              << "\n PointVolume material is: "
              << pointVol_->GetMaterial()->GetName()
              << "\n point (x,y,z) is: ("
              << point.X() << ", " 
              << point.Y() << ", "
              << point.Z() << ")" 
              << "\n********************************\n";
  }
/*
  //print out volumes and densities
  for(int i=0;i<gGeoManager->GetListOfVolumes()->GetLast();i++){
	if(gGeoManager->GetVolume(i)->GetMaterial()){
		//std::cout << "\nVolume: " << gGeoManager->GetVolume(i)->GetName()<< "\n\t\t\tdensity: " <<gGeoManager->GetVolume(i)->GetMaterial()->GetDensity() << " gcm-3";
	}
  }
  std::cout << "\n\n";
*/
}
