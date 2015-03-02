void geom()
{

   gSystem->Load("libGeom");
   TGeoManager *geom = new TGeoManager("simple1", "Simple geometry");

   TGeoRotation     *rot1   = new TGeoRotation("rot1", 0., 45., 0.);  // z ,x ,z
   TGeoTranslation  *shift1 = new TGeoTranslation("shift1", 0., 0., 500.);  // x ,y ,z

   TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);

   //TGeoMaterial *matAl = new TGeoMaterial("Al", 26.98,13,2.7);


   TGeoMixture *matScint = new TGeoMixture("Polystyrene",2,   1.03200    );    //  g/cm^3
   matScint->SetUniqueID( 101);
   matScint->DefineElement(0,1.00794,1,0.7742105E-01);
   matScint->DefineElement(1,12.011,6,0.922579);

//  TGeoMaterial*  polystyrene = new TGeoMaterial("Polystyrene",13.01f,7,1.032f,42.4f,81.9f);

   static const double ROCK_Z = 11;
   static const double ROCK_A = 22;
   static const double ROCK_DENSITY = 2.65; // *g/cm3;

   TGeoMaterial *matRock = new TGeoMaterial("Rock", ROCK_A, ROCK_Z, ROCK_DENSITY);

//   //--- define some media
   TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);

   //TGeoMedium *Al = new TGeoMedium("Root Material",2, matAl);
   TGeoMedium *Rock = new TGeoMedium("Standard Rock Material",4, matAl);
   TGeoMedium *Scint = new TGeoMedium("Polystyrene",3,matScint);

   TGeoVolume *top = gGeoManager->MakeBox("Top",Vacuum,2000.,2000.,2000.);
   gGeoManager->SetTopVolume(top);

   TGeoVolume *detector = geom->MakeTube("detector",Rock , 0., 150., 500.);
   detector->SetLineColor(kRed);

   top->AddNode(detector, 1,shift1);
   geom->Export("geom.root");
   geom->CloseGeometry();


   //Viewer3D.DefaultDrawOption:   ogl

   geom->SetVisLevel(4);
   top->Draw("ogle");
}

