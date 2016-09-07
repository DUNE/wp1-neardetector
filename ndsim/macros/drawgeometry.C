{
  gSystem->Load("libGeom");
  // Choose the file to read
  // Full DUNE ND detector geometry including the cavern
  TGeoManager *geom = TGeoManager::Import("../src/config/site.gdml");
  // Only HPTPC
  //TGeoManager *geom = TGeoManager::Import("../src/config/GasTPCDetector.gdml");
 
  //print out volumes and densities
  for(int i=0;i<=gGeoManager->GetListOfVolumes()->GetLast();i++){
    if(gGeoManager->GetVolume(i)->GetMaterial()){
      std::string name = gGeoManager->GetVolume(i)->GetName();
      if(name.find("Vessel")  != std::string::npos)
	std::cout << "\nVolume: " << gGeoManager->GetVolume(i)->GetName()<< "\n\t\t\tdensity: " <<gGeoManager->GetVolume(i)->GetMaterial()->GetDensity() << " gcm-3";
    }
  }
 
  TGeoVolume * worldVolume = geom->GetTopVolume();

  geom->SetTopVisible(kTRUE);
  geom->SetVisOption(0);
  geom->SetVisLevel(10);
  TCanvas *c90 = new TCanvas("innerVesselVolume","innerVesselVolume");
  worldVolume->Draw("ogl");
 

}
