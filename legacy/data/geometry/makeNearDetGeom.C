{
  gROOT->ProcessLine(".L NearMINDdeff.C");
  gROOT->ProcessLine(".L makeMaterials.C");
  gROOT->ProcessLine(".L makeGeometryMINDDetector.C");
  gROOT->ProcessLine(".L makeGeometryNearDetector.C");
  gROOT->ProcessLine("makeGeometryNearDetector()");

  geom->Export("geomLBNO_near.root");
  geom->Export("geomLBNO_near.gdml");

  //geom->Export("test.root");
  //geom->Export("test.gdml");

  geom->CloseGeometry();

  //Viewer3D.DefaultDrawOption:   ogl
  geom->SetVisLevel(1);
  //geom->SetVisOption(1);
  //top->ShowAxis();
  //TCanvas c1;
  //c1.ShowAxis();
  //detector_MIND->Draw();
  top->Draw();
}