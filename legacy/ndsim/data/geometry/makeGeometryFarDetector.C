using namespace std;

TGeoManager *geom;
TGeoVolume *top;
TGeoVolume *detector_liqAr;
TGeoVolume *detector_MIND;

double liqAr_r = 1850.;
double liqAr_h = 1000.;

void makeGeometryFarDetector() {

  gSystem->Load("libGeom");
  geom = new TGeoManager("lbno", "LBNO geometry");

  makeMaterials();
  FarMINDDeff();

  top = gGeoManager->MakeBox("Top", Air, 3500., 2500., 6000.);
  gGeoManager->SetTopVolume(top);
/*
  TGeoVolume *flux = geom->MakeTube("fluxSurface", Air, 0., 2500., 10.);
  flux->SetLineColor(kRed);
  top->AddNode(flux, 1001);
*/
  TGeoRotation     *rot1   = new TGeoRotation("rot1", 0., 100., 0.);
  TGeoRotation     *rot2   = new TGeoRotation("rot2", 0., 10., 0.);
  TGeoTranslation  *shift1 = new TGeoTranslation("shift1", 0., 0., 2200.);  // x ,y ,z
  TGeoTranslation  *shift2 = new TGeoTranslation("shift2", 0., 0., 4200.+MIND_downst_z_dim/2.);
  TGeoTranslation  *shift3 = new TGeoTranslation("shift3", 0., 0., 10.);
  TGeoTranslation  *shift4 = new TGeoTranslation("shift4", 0., 0., liqAr_h+10.75);
  TGeoTranslation  *shift5 = new TGeoTranslation("shift5", 0., 0., -liqAr_h-10.75);
  
  TGeoCombiTrans   *c1     = new TGeoCombiTrans(*shift1, *rot1);
  TGeoCombiTrans   *c2     = new TGeoCombiTrans(*shift2, *rot2);

  detector_liqAr = geom->MakeTube("detector_liqAr", Air, 0., liqAr_r+0.5, liqAr_h+11.);
  TGeoVolume *liqAr = geom->MakeTube("liqAr", liqAr, 0., liqAr_r, liqAr_h);
  TGeoVolume *liqArTank = geom->MakeTube("liqArTank", Iron, liqAr_r, liqAr_r+0.5, liqAr_h+10.);
  TGeoVolume *liqArTank_cap = geom->MakeTube("liqArTank_cap", Iron, liqAr_r, liqAr_r+0.5, .5);

  liqAr->SetLineColor(kBlue);
  detector_liqAr->AddNode(liqAr, 2, shift3);
  detector_liqAr->AddNode(liqArTank_cap, 3, shift4);
  detector_liqAr->AddNode(liqArTank_cap, 4, shift5);
  detector_liqAr->AddNode(liqArTank, 5);

  top->AddNode(detector_liqAr, 1, c1);
  //top->AddNode(detector_liqAr, 1, rot1);

  detector_MIND = geom->MakeBox("detector_MIND", Air, MIND_downst_x_dim/2., MIND_downst_y_dim/2., MIND_downst_z_dim/2.);
  detector_MIND->SetLineColor(kRed);
  top->AddNode(detector_MIND, 10, c2);

  makeGeometryMINDDetector(detector_MIND, MIND_downst_x_dim, MIND_downst_y_dim, MIND_downst_z_dim);
}

