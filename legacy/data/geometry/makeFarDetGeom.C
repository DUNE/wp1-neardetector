{
  gROOT->ProcessLine(".L FarMINDdeff.C");
  gROOT->ProcessLine(".L makeMaterials.C");
  gROOT->ProcessLine(".L makeGeometryMINDDetector.C");
  gROOT->ProcessLine(".L makeGeometryFarDetector.C");
  gROOT->ProcessLine("makeGeometryFarDetector()");



  geom->Export("geomLBNO_far.root");
  geom->Export("geomLBNO_far.gdml");
  geom->CloseGeometry();

  //Viewer3D.DefaultDrawOption:   ogl
  geom->SetVisLevel(3);
  //geom->SetVisOption(1);
  //top->ShowAxis();
  //TCanvas c1;
  //c1.ShowAxis();
  //detector_MIND->Draw();
  top->Draw();
}
