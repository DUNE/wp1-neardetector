  //--- define some media
TGeoMedium *Vacuum;
TGeoMedium *Air;
TGeoMedium *Scint;
TGeoMedium *gasAr;
TGeoMedium *liqAr;
TGeoMedium *Water;
TGeoMedium *Iron;

void makeMaterials() {

  TGeoElementTable *table = gGeoManager->GetElementTable();

  TGeoElement *H  = table->FindElement("Hydrogen");
  TGeoElement *Ar = table->FindElement("Argon");
  TGeoElement *N  = table->FindElement("Nitrogen");
  TGeoElement *O  = table->FindElement("Oxygen");
  TGeoElement *Fe = table->FindElement("Iron");
  TGeoElement *C = table->FindElement("Carbon");


  TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0, 0, 0);
  matVacuum->SetUniqueID(100);

  TGeoMixture *matAir = new TGeoMixture("matAir", 2, 0.00129);
  matAir->DefineElement(0, O, .3288);
  matAir->DefineElement(1, N, .6712);
  matAir->SetUniqueID(101);

  TGeoMaterial *matGasAr = new TGeoMaterial("matGasArgon", Ar, 0.00167);
  matGasAr->SetUniqueID(102);

  TGeoMaterial *matLiqAr = new TGeoMaterial("matLiquidArgon", Ar, 0.13928);
  matLiqAr->SetUniqueID(103);

  TGeoMaterial *matIron = new TGeoMaterial("matIron", Fe, 7.874);
  matLiqAr->SetUniqueID(104);

  TGeoMixture *matScint = new TGeoMixture("Polystyrene", 2, 1.03200);    //  g/cm^3
  matScint->DefineElement(0, H, 0.7742105E-01);
  matScint->DefineElement(1, C, 0.922579);
  matScint->SetUniqueID( 105);

  TGeoMixture *matWater = new TGeoMixture("matWater", 2, 1.0);
  matWater->DefineElement(1, O, .8880);
  matWater->DefineElement(0, H, .1120);
  matWater->SetUniqueID(106);


  //--- define some media
  Vacuum = new TGeoMedium("Vacuum", 1, matVacuum);

  Air = new TGeoMedium("Air", 3, matAir);

  Scint = new TGeoMedium("Polystyrene", 3, matScint);

  gasAr = new TGeoMedium("gasArgon", 4, matGasAr);

  liqAr = new TGeoMedium("liquidArgon", 5, matLiqAr);

  Water = new TGeoMedium("Water", 2, matWater);

  Iron = new TGeoMedium("Iron", 2, matIron);

}
