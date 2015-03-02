using namespace std;

TGeoManager *geom;
TGeoVolume *top;
TGeoVolume *detector_gasAr;
TGeoVolume *detector_scint;
TGeoVolume *detector_MIND_downstr;
TGeoVolume *detector_MIND_top;
TGeoVolume *detector_MIND_bottom;
TGeoVolume *detector_MIND_north;
TGeoVolume *detector_MIND_south;

void makeGeometryNearDetector() {

  gSystem->Load("libGeom");
  geom = new TGeoManager("lbno", "LBNO geometry");

  makeMaterials();
  NearMINDDeff();

  top = gGeoManager->MakeBox("Top", Air, 1000., 1000., 2000.);
  gGeoManager->SetTopVolume(top);

  TGeoRotation     *rot1   = new TGeoRotation("rot1", 0., 90., 0.);  // z ,x ,z
  TGeoRotation     *rot2   = new TGeoRotation("rot2", 90., 90., 0.);  // z ,x ,z
  TGeoTranslation  *shift1 = new TGeoTranslation("shift1", 0., 0., 450.);  // x ,y ,z
  TGeoTranslation  *shift2 = new TGeoTranslation("shift2", 0., 0., 1010.);  // x ,y ,z
  TGeoTranslation  *shift3 = new TGeoTranslation("shift3", 0., -260., 400.);
  TGeoTranslation  *shift4 = new TGeoTranslation("shift4", 0., 260., 400.);
  TGeoTranslation  *shift5 = new TGeoTranslation("shift5", -260., 0., 400.);
  TGeoTranslation  *shift6 = new TGeoTranslation("shift6", 260., 0., 400.);
  TGeoTranslation  *shift7 = new TGeoTranslation("shift1", 0., 0., 400.);

  TGeoCombiTrans   *c1      = new TGeoCombiTrans(*shift3, *rot1);
  TGeoCombiTrans   *c2      = new TGeoCombiTrans(*shift4, *rot1);
  TGeoCombiTrans   *c3      = new TGeoCombiTrans(*shift5, *rot2);
  TGeoCombiTrans   *c4      = new TGeoCombiTrans(*shift6, *rot2);

  detector_scint = geom->MakeBox("detector_scint", Scint, 200., 200., 300.);
  detector_scint->SetLineColor(kBlue);
  top->AddNode(detector_scint, 1, shift1);

  detector_gasAr = geom->MakeBox("detector_gasAr", gasAr, 100., 100., 200.);
  //detector_gasAr->SetLineColor(kRed);
  top->AddNode(detector_gasAr, 2, shift7);

  detector_MIND_downstr = geom->MakeBox("detector_MIND_downstr", Air,
                                        MIND_downst_x_dim/2., MIND_downst_y_dim/2., MIND_downst_z_dim/2.);

  detector_MIND_downstr->SetLineColor(kRed);
  top->AddNode(detector_MIND_downstr, 3, shift2);
  //top->AddNode(detector_MIND_downstr, 3);

  detector_MIND_bottom = geom->MakeBox("detector_MIND_bottom", Air, MIND_side_width_tb/2., MIND_side_lenght/2., MIND_side_depth_dim/2.);
  detector_MIND_bottom->SetLineColor(kRed);
  top->AddNode(detector_MIND_bottom, 5, c1);

  detector_MIND_top = geom->MakeBox("detector_MIND_top", Air, MIND_side_width_tb/2., MIND_side_lenght/2., MIND_side_depth_dim/2.);
  detector_MIND_top->SetLineColor(kRed);
  top->AddNode(detector_MIND_top, 6, c2);

  detector_MIND_north = geom->MakeBox("detector_MIND_north", Air, MIND_side_width_sn/2., MIND_side_lenght/2., MIND_side_depth_dim/2.);
  detector_MIND_north->SetLineColor(kRed);
  top->AddNode(detector_MIND_north, 7, c3);

  detector_MIND_south = geom->MakeBox("detector_MIND_south", Air, MIND_side_width_sn/2., MIND_side_lenght/2., MIND_side_depth_dim/2.);
  detector_MIND_south->SetLineColor(kRed);
  top->AddNode(detector_MIND_south, 8, c4);

  makeGeometryMINDDetector(detector_MIND_downstr, MIND_downst_x_dim, MIND_downst_y_dim, MIND_downst_z_dim);
  makeGeometryMINDDetector(detector_MIND_bottom, MIND_side_width_tb, MIND_side_lenght, MIND_side_depth_dim);
  makeGeometryMINDDetector(detector_MIND_top, MIND_side_width_tb, MIND_side_lenght, MIND_side_depth_dim);
  makeGeometryMINDDetector(detector_MIND_north, MIND_side_width_sn, MIND_side_lenght, MIND_side_depth_dim);
  makeGeometryMINDDetector(detector_MIND_south, MIND_side_width_sn, MIND_side_lenght, MIND_side_depth_dim);

}
