//____________________________________________________________________________
/*!

\class    Geant4TrackingAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#include "Geant4TrackingAlgorithm.hh"

Geant4TrackingAlgorithm::Geant4TrackingAlgorithm() {
  physList_ = new PhysicsList(hadroniclist_,emlist_);
  this->initDataCards();
}

Geant4TrackingAlgorithm::~Geant4TrackingAlgorithm() {
  if(physList_)delete physList_;
  if(eventAction_)delete eventAction_;
  if(runAction_)delete runAction_;
  if(stpAction_)delete stpAction_;
  if(stackAction_)delete stackAction_;
  if(detector_)delete detector_;
  if(postDetConstruction_)delete postDetConstruction_;
  if(pgAction_)delete pgAction_;
  //if(runManager_)delete runManager_;

}

void Geant4TrackingAlgorithm::initialize() {

  //detector geometry
  if(geoMgr_)detector_ = new DetectorConstruction(geomFile_,magFieldStrength_,geoMgr_);
  else detector_ = new DetectorConstruction(geomFile_,magFieldStrength_);

  bool tgeo = detector_->GetValueUsingRootGeom();

  if(tgeo)detMgr = detector_->getRootMgr();
 
  //set run manager
  runManager_ = new G4RunManager();

  postDetConstruction_ = NULL;
  if(tgeo){
    postDetConstruction_ = new MyUserPostDetConstruction(SDName_,magFieldStrength_,geoMgr_);
    detMgr->Initialize(postDetConstruction_);
    //needed to connect to GEANT4 run manager
    detMgr->ConnectToG4();
  }
  else runManager_->SetUserInitialization(detector_);

  //physics list
  physList_ = new PhysicsList(hadroniclist_,emlist_);
  physList_->loadDataCards();
  physList_->initialize();
  runManager_->SetUserInitialization(physList_);

  //G4PhysListFactory factory;
  //G4VModularPhysicsList* physics = factory.GetReferencePhysList(physicsList_);
  //physics->SetVerboseLevel(verbose);
  //runManager_->SetUserInitialization(physics);

  //PGA
  pgAction_ = new PrimaryGeneratorAction(physList_);
  pgAction_->setVerbose(verbose_);
  pgAction_->setUsingGun(runGunMacro_);
  runManager_->SetUserAction(pgAction_);

  //Run action
  runAction_ = new RunAction();
  runAction_->SetSeed();
  runManager_->SetUserAction(runAction_);

  //Stepping Action
  if(geoMgr_)stpAction_ = new SteppingAction(geoMgr_,cutEnergy_,piZeroTracker_);
  else stpAction_ = new SteppingAction(cutEnergy_,piZeroTracker_);
  stpAction_->setVerbose(verbose_);
  stpAction_->setVolume(targetVolName_);
  runManager_->SetUserAction(stpAction_);

  //Event Action
  if(geoMgr_)eventAction_ = new EventAction(SDName_,geoMgr_,stpAction_);
  else eventAction_ = new EventAction(SDName_,stpAction_);
  runManager_->SetUserAction(eventAction_);

  //Stacking Action
  stackAction_ = new StackingAction();
  runManager_->SetUserAction(stackAction_);

  //Tracking Action
  trAction_ = new TrackingAction();
  runManager_->SetUserAction(trAction_);

  //Initialize
  runManager_->Initialize();

  UImanager = G4UImanager::GetUIpointer();

  visManager = new G4VisExecutive;
  visManager->Initialize();

  if(verbose_>2){
	UImanager->ApplyCommand("/tracking/verbose 1");
  	UImanager->ApplyCommand("/stepping/verbose 1");
  	UImanager->ApplyCommand("/event/verbose 1");
  	UImanager->ApplyCommand("/process/verbose 1");
  	UImanager->ApplyCommand("/run/verbose 1");
  	UImanager->ApplyCommand("/control/verbose 1");
  }
  else {
	UImanager->ApplyCommand("/tracking/verbose 0");
  	UImanager->ApplyCommand("/stepping/verbose 0");
  	UImanager->ApplyCommand("/event/verbose 0");
  	UImanager->ApplyCommand("/process/verbose 0");
  	UImanager->ApplyCommand("/run/verbose 0");
  	UImanager->ApplyCommand("/control/verbose 0");
  }

  if (runVisMacro_)this->runVisMac();
  //if (runGunMacro_)this->runGunMac();
}

bool Geant4TrackingAlgorithm::Process() {

  //ensure this is set to 1
  pgAction_->setUsingGun(1);
  this->runGunMac();
  //runManager_->BeamOn(1);
  if(verbose_>0)stpAction_->Print();
 
  G4UIsession * session = new G4UIterminal(new G4UItcsh, false);
  //session->SessionStart();
  delete session;

  return true;
}

bool Geant4TrackingAlgorithm::Process(NeutrinoEvent* event) {

  this->getPrimaryGenerator()->setNuEvent(event);

  runManager_->BeamOn(1);
  if(verbose_>0)stpAction_->Print();
  //if (this->getSteppingAction()->hasHit())
    //return true;

  return true;
}

void Geant4TrackingAlgorithm::runGunMac(){

    char* LBNO = "";
    if(getenv("LBNO"))LBNO = getenv("LBNO");
    else std::cout << "\n--NOTE: $LBNO env varible not set---" <<std::endl;
    std::cout << "\n****Executing run script: " 
	      << "/control/execute " +  string(LBNO) + "/src/simul/run.mac" <<std::endl; 
    UImanager->ApplyCommand("/control/execute " +  string(LBNO) + "/src/simul/run.mac");

}

void Geant4TrackingAlgorithm::runVisMac(){

    char* LBNO = "";
    if(getenv("LBNO"))LBNO = getenv("LBNO");
    else std::cout << "\n--NOTE: $LBNO env varible not set---" <<std::endl;
    std::cout << "\n****Executing visualisation script: " 
	      << "/control/execute " +  string(LBNO) + "/src/simul/vis.mac" <<std::endl;
    UImanager->ApplyCommand("/control/execute " +  string(LBNO) + "/src/simul/vis.mac");

}

void Geant4TrackingAlgorithm::initDataCards() {
  cards_ = DataCards::getInstance();
  char* LBNO = getenv("LBNO");
  cards_->AddDataCardString("simGeomFileName", string(LBNO) + "/geom_construction/workdir/geometry.gdml");
  cards_->AddDataCardString("physicsList","FTFP_BERT");
  cards_->AddDataCardDouble("magFieldStrength",0.5);
  cards_->AddDataCardBool("runVisMacro", false);
  cards_->AddDataCardBool("runGunMacro", false);  
  cards_->AddDataCardString("hadroniclist", "FTFP_BERT_HP");
  cards_->AddDataCardString("emlist", "EM4");
  cards_->AddDataCardInt("trackingVerbose",1);
  cards_->AddDataCardString("simTargetVolume", "tpcFidVolume");
  cards_->AddDataCardDouble("simEnergyCut",100.0*keV);
  cards_->AddDataCardBool("trackPiZero",0);
  //physicsList Cuts
  cards_->AddDataCardDouble("physicsDefaultCut",0.01*cm);
  cards_->AddDataCardDouble("gammaCut",0.01*cm);
  cards_->AddDataCardDouble("electronCut",0.01*cm);
  cards_->AddDataCardDouble("positronCut",0.01*cm);
  cards_->AddDataCardDouble("neutronCut",0.01*cm);
  cards_->AddDataCardDouble("cutEM",0.01*cm);

  cardsLoaded_ = false;
}

void Geant4TrackingAlgorithm::loadDataCards() {
  geomFile_         = cards_->fetchValueString("simGeomFileName");
  runVisMacro_      = cards_->fetchValueBool("runVisMacro");
  runGunMacro_      = cards_->fetchValueBool("runGunMacro");
  physicsList_      = cards_->fetchValueString("physicsList");
  magFieldStrength_ = cards_->fetchValueDouble("magFieldStrength");  
  hadroniclist_     = cards_->fetchValueString("hadroniclist");
  emlist_           = cards_->fetchValueString("emlist");
  verbose_ 	    = cards_->fetchValueInt("trackingVerbose"); 
  targetVolName_    = cards_->fetchValueString("simTargetVolume");
  cutEnergy_        = cards_->fetchValueDouble("simEnergyCut");
  piZeroTracker_    = cards_->fetchValueBool("trackPiZero");

  cardsLoaded_      = true;
}


