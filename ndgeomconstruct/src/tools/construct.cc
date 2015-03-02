//
// Title: Root and gdml Geometry for the Near Detector
//
// Description: to be written
//
// Author: Tom Stainer
//

#include "construct.hh"

//___________________________________________________________________
int main(int argc, char ** argv)
{

  //load the geometry library
  TSystem * system = new TSystem("system");
  system->Load("libGeom"); 
  
  TGeoManager * geom = new TGeoManager("geom","Near Detector");

  //initiate data cards
  initDataCards();

  //construct detector parts here
  RockConstruction * rock = new RockConstruction();
  CavityConstruction * cavity = new CavityConstruction();
  MagnetConstruction * magnet = new MagnetConstruction();
  TpcVesselConstruction *vessel = new TpcVesselConstruction();
  TpcFiducialConstruction * tpc = new TpcFiducialConstruction();
  ScintillatorConstruction * scint = new ScintillatorConstruction();
  MINDConstruction * mind = new MINDConstruction();
  //SupportStructureConstruction * support = new SupportStructureConstruction();

  //get the data card
  getRunTimeArguments(argc,argv);

  //load data cards and initialize all parts of the detector
  loadDataCards();
  //initialise here
  rock->initialize();
  cavity->initialize();
  magnet->initialize();
  vessel->initialize();
  tpc->initialize();
  scint->initialize();
  mind->initialize();
  //support->initialize();

  //material list constructor will create all necessary materials
  Materials * materialList = new Materials();
  //construct()
  rock->construct(materialList);
  tpc->construct(materialList);
  vessel->construct(materialList,tpc);
  magnet->construct(materialList,vessel);
  cavity->construct(materialList,vessel,magnet,rock,detectorYAxisOffset_);
  scint->construct(materialList,tpc,vessel);
  mind->construct(materialList);
  //support->construct(materialList,scint,vessel);

  //set the world volume
  world = rock->getVolume();

  //set rock as the top volume
  geom->SetTopVolume(world);

  //set colours of volumes for drawing
  rock->getVolume()->SetLineColor(42);
  cavity->getVolume()->SetLineColor(42);
  magnet->getVolume()->SetLineColor(12);
  vessel->getVolume()->SetLineColor(kOrange+2);
  tpc->getVolume()->SetLineColor(11);
  mind->getVolume()->SetLineColor(12);
   
  //---- Placements ----//
  TGeoTranslation* cavityPlacementTrans = 
	new TGeoTranslation(0.0,rock->getY()/(2.0*CLHEP::cm) - cavity->getLength()/(2.0*CLHEP::cm),0.0);

  TGeoRotation *cavityPlacementRot = new TGeoRotation();
  cavityPlacementRot->SetAngles(0, 90, 0);
  TGeoCombiTrans *cavityPlacementCombi = new TGeoCombiTrans(*cavityPlacementTrans, *cavityPlacementRot);
  
  //now the axis are rotated so need to allow for this in  daughter positions
  // y axis is now the z axis
  TGeoTranslation* magnetPlacementTrans = 
	new TGeoTranslation(0.0,0.0,cavity->getLength()/(2.0*CLHEP::cm) - magnet->getRadius()/(CLHEP::cm));
  TGeoRotation * magnetPlacementRot = new TGeoRotation();
  magnetPlacementRot->SetAngles(0, -90, 0);
  TGeoCombiTrans *magnetPlacementCombi = new TGeoCombiTrans(*magnetPlacementTrans, *magnetPlacementRot);

  TGeoTranslation* origin = new TGeoTranslation(0,0,0);

  world->AddNode(cavity->getVolume(),1,cavityPlacementCombi);
  //world->AddNode(vessel->getVolume(),1,origin);
  cavity->getVolume()->AddNodeOverlap(magnet->getVolume(),2,magnetPlacementCombi);
  cavity->getVolume()->AddNodeOverlap(vessel->getVolume(),3,magnetPlacementCombi);
  //magnet->getVolume()->AddNodeOverlap(vessel->getVolume(),3,origin);
  vessel->getInnerVolume()->AddNodeOverlap(scint->getVolume(),4,
  				new TGeoTranslation(0,0,tpcPlacementZ_/CLHEP::cm));
  scint->getVolume()->AddNode(tpc->getVolume(),9,origin);

  //MIND placement
  TGeoTranslation* mindPlacementTrans = 
	new TGeoTranslation(0.0,0.0,vessel->getLength()/(2.0*CLHEP::cm) + gap_/(CLHEP::cm) + mind->getZ()/(2.0*CLHEP::cm));
  world->AddNode(mind->getVolume(),1,mindPlacementTrans);

  //print statements
  rock->print();
  cavity->print();
  magnet->print();
  vessel->print();
  scint->print();
  tpc->print();
  mind->print();

  //close geometry
  geom->CloseGeometry();

  geom->Export(rootFilename_.c_str());
  geom->Export(gdmlFilename_.c_str());

  delete geom;

  std::cout << "\nRoot file created for geometry.\nOutput file is: "<< rootFilename_.c_str()
	    << "\nGDML file created for geometry.\nOutput file is: "<< gdmlFilename_.c_str() << "\n\n";

  return 0;
}

void getRunTimeArguments(int argc, char ** argv) {
  for (int i=1;i<argc;i++)
    if (argv[i]) {
      std::string cardsfile(argv[i]);
      cards_->readKeys(cardsfile);
    }
    cards_->printToStream(std::cout);
}

void initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardString("rootFile","geometry.root");
  cards_->AddDataCardString("gdmlFile","geometry.gdml");

  cards_->AddDataCardDouble("tpcPlacementZ", 0.0*CLHEP::m);
  cards_->AddDataCardDouble("tpcVesselPlacementZ", 0.0*CLHEP::m);
  cards_->AddDataCardDouble("detectorYAxisOffset", 0.0*CLHEP::m);
  cards_->AddDataCardDouble("mindGap", 0.5*CLHEP::m);
  //cards_->AddDataCardInt("scintLayers", 10);

}

void loadDataCards() {
  rootFilename_ = cards_->fetchValueString("rootFile");
  gdmlFilename_ = cards_->fetchValueString("gdmlFile");

  tpcPlacementZ_  = cards_->fetchValueDouble("tpcPlacementZ");
  tpcVesselPlacementZ_  = cards_->fetchValueDouble("tpcVesselPlacementZ");
  detectorYAxisOffset_  = cards_->fetchValueDouble("detectorYAxisOffset");
  gap_  		= cards_->fetchValueDouble("mindGap");
  //scintLayers_  = cards_->fetchValueInt("scintLayers");

}
