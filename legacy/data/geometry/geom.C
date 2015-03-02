void geom()
{
  gSystem->Load("libGeom");
  TGeoManager *geom = new TGeoManager("simple1", "Simple geometry");

  TGeoRotation     *rot1   = new TGeoRotation("rot1", 0., 45., 0.);  // z ,x ,z
  TGeoTranslation  *shift1 = new TGeoTranslation("shift1", 0., 0., 500.);  // x ,y ,z

  TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0, 0, 0);

  TGeoMixture *matScint = new TGeoMixture("Polystyrene", 2, 1.03200);    //  g/cm^3
  matScint->SetUniqueID( 101);
  matScint->DefineElement(0,1.00794,1,0.7742105E-01);
  matScint->DefineElement(1,12.011,6,0.922579);

  //   //--- define some media
  TGeoMedium *Vacuum = new TGeoMedium("Vacuum", 1, matVacuum);

  TGeoMedium *Scint = new TGeoMedium("Polystyrene", 3, matScint);

  TGeoVolume *top = gGeoManager->MakeBox("Top",Vacuum,2000.,2000.,2000.);
  gGeoManager->SetTopVolume(top);

  TGeoVolume *detector = geom->MakeTube("detector", Scint, 0., 500., 500.);
  detector->SetLineColor(kRed);

  top->AddNode(detector, 1, shift1);
  geom->Export("geom.root");
  geom->CloseGeometry();

  //Viewer3D.DefaultDrawOption:   ogl
  geom->SetVisLevel(4);
  top->Draw("ogle");
}