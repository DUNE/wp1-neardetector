void geom()
{

   gSystem->Load("libGeom");
   TGeoManager *geom = new TGeoManager("simple1", "Simple geometry");

   TGeoRotation     *rot1   = new TGeoRotation("rot1", 0., 45., 0.);  // z ,x ,z
   TGeoTranslation  *shift1 = new TGeoTranslation("shift1", 0., 0., 500.);  // x ,y ,z

   TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);

   TGeoElementTable *table = gGeoManager->GetElementTable();
   TGeoElement *H = table->GetElement( 1 );
   TGeoElement *C = table->GetElement( 6 );
   TGeoElement *O = table->GetElement( 8 );
   TGeoElement *Na = table->GetElement( 11 );
   TGeoElement *Mg = table->GetElement( 12 );
   TGeoElement *Al = table->GetElement( 13 );
   TGeoElement *Si = table->GetElement( 14 );
   TGeoElement *K = table->GetElement( 19 );
   TGeoElement *Ca = table->GetElement( 20 );
   TGeoElement *Fe = table->GetElement( 26 );

   TGeoMixture *matConcrete = new TGeoMixture("Concrete",10,   2.3 );    //  g/cm^3
   matConcrete->AddElement(H, 0.01);
   matConcrete->AddElement(C, 0.001);
   matConcrete->AddElement(O, 0.529107);
   matConcrete->AddElement(Na, 0.016);
   matConcrete->AddElement(Mg, 0.002);
   matConcrete->AddElement(Al, 0.033872);
   matConcrete->AddElement(Si, 0.337021);
   matConcrete->AddElement(K, 0.013);
   matConcrete->AddElement(Ca, 0.044);
   matConcrete->AddElement(Fe, 0.014);

//   //--- define some media
   TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);

   TGeoMedium *Concrete = new TGeoMedium("Concrete",2, matConcrete);

   TGeoVolume *top = gGeoManager->MakeBox("Top",Vacuum,2000.,2000.,2000.);
   gGeoManager->SetTopVolume(top);

   TGeoVolume *detector = geom->MakeTube("detector", Concrete, 0., 300., 300.);
   detector->SetLineColor(kRed);

   top->AddNode(detector, 1,shift1);
   geom->Export("geomConcrete.root");
   geom->CloseGeometry();


   //Viewer3D.DefaultDrawOption:   ogl

   geom->SetVisLevel(4);
   top->Draw("ogle");
}
