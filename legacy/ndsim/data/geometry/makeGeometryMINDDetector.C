

void makeGeometryMINDDetector(TGeoVolume *detector_MIND, double x_dim, double y_dim, double z_dim) {

  int n_layers = (int)(z_dim/5.);
  int n_hor_slabs = (int)(y_dim/3.5);
  int n_ver_slabs = (int)(x_dim/3.5);

  string suffix = string( detector_MIND->GetName() );
  string name = "layer_" + suffix;
  TGeoVolume *layer = geom->MakeBox(name.c_str() , Air , x_dim/2., y_dim/2., MIND_layer_thickness/2.);

  name = "iron_plane_" + suffix;
  TGeoVolume *iron_plane = geom->MakeBox(name.c_str(), Iron, x_dim/2., y_dim/2., iron_plane_thickness/2.);

  name = "scint_plane_" + suffix;
  TGeoVolume *scint_plane = geom->MakeBox(name.c_str(), Scint, x_dim/2., y_dim/2., scint_slab_thickness);

  layer->AddNode(iron_plane, 0,  new TGeoTranslation(0., 0., -MIND_layer_thickness/2. + iron_plane_thickness/2.));
  layer->AddNode(scint_plane, 1,  new TGeoTranslation(0., 0., MIND_layer_thickness/2. - scint_slab_thickness));

  name = "scint_slab_v_" + suffix;
  TGeoVolume *scint_slab_v = geom->MakeBox(name.c_str(), Scint, scint_slab_width/2., y_dim/2., scint_slab_thickness/2.);

  name = "scint_slab_h_" + suffix;
  TGeoVolume *scint_slab_h = geom->MakeBox(name.c_str(), Scint, x_dim/2., scint_slab_width/2., scint_slab_thickness/2.);

  double slab_pos_y = -y_dim/2. + scint_slab_width/2.;
  int slab_h_id = 0;
  while (slab_h_id < n_hor_slabs) {
    //cout << slab_h_id << " " << slab_pos_y << endl;
    scint_plane->AddNode(scint_slab_h, slab_h_id, new TGeoTranslation(0., slab_pos_y, scint_slab_thickness/2.));
    slab_pos_y += scint_slab_width;
    slab_h_id++;
  }

  double slab_pos_x = -x_dim/2. + scint_slab_width/2.;
  int slab_v_id = 0;
  while (slab_v_id < n_ver_slabs) {
    //cout << slab_h_id << " " << slab_pos_y << endl;
    scint_plane->AddNode(scint_slab_v, slab_v_id*1000, new TGeoTranslation(slab_pos_x, 0., -scint_slab_thickness/2.));
    slab_pos_x += scint_slab_width;
    slab_v_id++;
  }

  double layer_pos_z = -z_dim/2.  + MIND_layer_thickness/2.;
  int layer_id = 0;
  while (layer_id < n_layers) {
    detector_MIND->AddNode(layer, layer_id, new TGeoTranslation(0., 0., layer_pos_z));
    layer_id++;
    layer_pos_z += MIND_layer_thickness;
  }
}