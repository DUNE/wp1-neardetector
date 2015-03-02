{
  TGeoManager::Import("geomLBNO_near.gdml");
  //TGeoManager::Import("mytest.gdml");
  gGeoManager->ViewLeaves(true);
  gGeoManager->SetVisLevel(3);
  gGeoManager->GetTopVolume()->Draw(); 
}
