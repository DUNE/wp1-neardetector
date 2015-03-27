{
  gSystem->Load("libGeom");
  TGeoManager *geom = TGeoManager::Import("HPGasTPCGeometry.root");
 
  TGeoVolume * vesselVolume = geom->GetVolume("vesselVolume");
  TGeoVolume * tpcVolume = geom->GetVolume("tpcFidVolume");
  TGeoVolume * innerVesselVolume = geom->GetVolume("innerVesselVolume");
  // Print out some useful information
  double min=0, max=0;
  TGeoShape * bounding_box = tpcVolume->GetShape();
  double xrange = bounding_box->GetAxisRange(1, min, max);
  double yrange = bounding_box->GetAxisRange(2, min, max);
  double zrange = bounding_box->GetAxisRange(3, min, max);
 
  std::cout << "\nTPC x axis range is: " << xrange 
	    << "\nTPC y axis range is: " << yrange
	    << "\nTPC z axis range is: " << zrange << std::endl;

  bounding_box = innerVesselVolume->GetShape();
  xrange = bounding_box->GetAxisRange(1, min, max);
  yrange = bounding_box->GetAxisRange(2, min, max);
  zrange = bounding_box->GetAxisRange(3, min, max);

  std::cout << "\nInner Vessel x axis range is: " << xrange 
	    << "\nInner Vessel y axis range is: " << yrange
	    << "\nInner Vessel z axis range is: " << zrange << std::endl;

  bounding_box = vesselVolume->GetShape();
  xrange = bounding_box->GetAxisRange(1, min, max);
  yrange = bounding_box->GetAxisRange(2, min, max);
  zrange = bounding_box->GetAxisRange(3, min, max);

  std::cout << "\nVessel x axis range is: " << xrange 
	    << "\nVessel y axis range is: " << yrange
	    << "\nVessel z axis range is: " << zrange << std::endl;

  //VISIBILITY OPTIONS
  //vesselVolume->SetVisibility(kFALSE);
  //innerVesselVolume->SetVisibility(kFALSE);
  //tpcVolume->SetVisibility(kFALSE);
  geom->SetTopVisible(kTRUE);
  geom->SetVisOption(0);
  geom->SetVisLevel(10);
  TCanvas *c90 = new TCanvas("innerVesselVolume","innerVesselVolume");
  innerVesselVolume->Draw();
  TCanvas *c91 = new TCanvas("tpcVolume","tpcVolume");
  tpcVolume->Draw();
  TCanvas *c92 = new TCanvas("vesselVolume","vesselVolume");
  vesselVolume->Draw("ogl");
 
  const char* vol_name = vesselVolume->GetName();
  Int_t n_daughters = vesselVolume->GetNdaughters();
  std::cout << vol_name << " has " << n_daughters << " daughters: " << std::endl;
  TObjArray* nodes = vesselVolume->GetNodes();
  if(nodes) nodes->Print();

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

  //print out volumes and densities
  for(int i=0;i<gGeoManager->GetListOfVolumes()->GetLast();i++){
    if(gGeoManager->GetVolume(i)->GetMaterial()){
      std::cout << "\nVolume: " << gGeoManager->GetVolume(i)->GetName()<< "\n\t\t\tdensity: " <<gGeoManager->GetVolume(i)->GetMaterial()->GetDensity() << " gcm-3";
    }
  }

}
