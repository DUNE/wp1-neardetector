void GasArTPC()
{
  //Argon Box
  
  //define dimensions of cylinder TPC, in meters
  Float_t R = 150.;
  Float_t L = 250.;
  //length of world
  Float_t lw = 2.5*L;  

  //load the geometry library
  gSystem->Load("libGeom");
  TGeoManager *geom = new TGeoManager("Argon Box","Argon Box geometry");
  
  //define materials
  TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
  TGeoMaterial *matCarbon = new TGeoMaterial("CARBON",12,6,2.3);
  TGeoMaterial *matAr = new TGeoMaterial("ARGON",39.95,18,0.035);
  matAr->SetUniqueID(  2);
  
  //define media 
  TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
  TGeoMedium *carbon = new TGeoMedium("Carbon",1,matCarbon);
  TGeoMedium *argon = new TGeoMedium("Argon",17,matAr);


 //make the top container volume(box)
  Double_t worldx = lw;
  Double_t worldy = lw;
  Double_t worldz = 1.5*lw;
  TGeoVolume *top = geom->MakeBox("Top",carbon ,worldx,worldy,worldz);

  //make this volume our world
  geom->SetTopVolume(top);

   //make cylinder
  Double_t tube_r = R;
  Double_t tube_l = L;
  TGeoVolume * tube = geom->MakeTube("tube",argon,0,tube_r,tube_l);
  tube->SetLineColor(kRed);

  //make ends of TPC - hemispheres
  Double_t ends_r = R;
  TGeoVolume *sphere = geom->MakeSphere("sphere",argon,0,ends_r,0,90,0,360);
  TGeoVolume *sphere2 = geom->MakeSphere("sphere2",argon,0,ends_r,90,180,0,360);
  sphere->SetLineColor(kRed);
  sphere2->SetLineColor(kRed);

  //make fiducial volume
  
  TGeoVolume * fid_tube = geom->MakeTube("fid_tube",argon,0,tube_r-0.5,tube_l-0.5);

    //define transformations
  TGeoTranslation *tr1 = new TGeoTranslation(0., 0., 2*L);
  TGeoTranslation *tr2 = new TGeoTranslation(0.,0.,3*L);
  TGeoTranslation *tr3 = new TGeoTranslation(0.0,0.0,L);

  //Rotation


  //put detector in world
  top->AddNode(tube,1,tr1);
  top->AddNode(sphere,2,tr2);
  top->AddNode(sphere2,3,tr3);
  top->AddNode(fid_tube,4,tr1);

  //close geometry
  geom->CloseGeometry();
  
  //write geometry to root file for GENIE
  geom->Export("GasArTPC.root");
  geom->Export("GasArTPC.gdml");
  
  //display the world
  top->SetLineColor(kBlue);
  geom->ViewLeaves(true);
  geom->SetVisLevel(2);
  geom->SetTopVisible(1);
  top->Draw();
}
