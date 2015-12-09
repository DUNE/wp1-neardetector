{
  gSystem->Load("libGeom");
  TGeoManager *geom = TGeoManager::Import("../src/config/site.gdml");
 
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
