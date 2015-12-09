//____________________________________________________________________________
/*!

\class    Geant4TrackingAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jun 2015
*/
//____________________________________________________________________________
#include "Geant4TrackingAlgorithm.hh"

Geant4TrackingAlgorithm::Geant4TrackingAlgorithm(std::string geomFile, std::string geomVol) {
  geomFile_         = geomFile;
  targetVolName_    = geomVol;
  geoMgr_           = NULL;
  // Some default values for the simulation parameters
  defaultCut_       = 50.0; // mm
  gammaCut_         = 50.0;
  electronCut_      = 50.0;
  positronCut_      = 50.0;
  neutronCut_       = 50.0;
  magFieldStrength_ = 0.4; // Tesla
  cutEnergy_        = 5.0; // MeV
  physicsList_      = "FTFP_BERT";
  hadroniclist_     = "FTFP_BERT_HP";
  emlist_           = "EM4";
  runVisMacro_      = false;
  verbose_          = 0;
}

Geant4TrackingAlgorithm::Geant4TrackingAlgorithm(std::string geomFile, std::string geomVol, GeometryLoader *geomLoader) {
  geomFile_         = geomFile;
  targetVolName_    = geomVol;
  geoMgr_           = geomLoader->getManager();
  // Some default values for the simulation parameters
  defaultCut_       = 50.0; // mm
  gammaCut_         = 50.0;
  electronCut_      = 50.0;
  positronCut_      = 50.0;
  neutronCut_       = 50.0;
  magFieldStrength_ = 0.4; // Tesla
  cutEnergy_        = 5.0; // MeV
  physicsList_      = "FTFP_BERT";
  hadroniclist_     = "FTFP_BERT_HP";
  emlist_           = "EM4";
  runVisMacro_      = false;
  verbose_          = 0;
}

Geant4TrackingAlgorithm::~Geant4TrackingAlgorithm() {
  if(geoMgr_) delete geoMgr_;
  if(postDetConstruction_) delete postDetConstruction_;
  //if(detector_) delete detector_;

  if(physList_)delete physList_;
  if(eventAction_)delete eventAction_;
  if(runAction_)delete runAction_;
  if(stpAction_)delete stpAction_;
  if(stackAction_)delete stackAction_;
  if(pgAction_)delete pgAction_;
  if(trAction_) delete trAction_;

  //if(runManager_)delete runManager_;
  //if(detMgr) delete detMgr;
  //if(UImanager) delete UImanager;
  //if(visManager) delete visManager;
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
    postDetConstruction_ = new MyUserPostDetConstruction(targetVolName_,magFieldStrength_/*,geoMgr_*/);
    detMgr->Initialize(postDetConstruction_);
    //needed to connect to GEANT4 run manager
    detMgr->ConnectToG4();
  }
  else 
    runManager_->SetUserInitialization(detector_);

  //physics list
  physList_ = new PhysicsList(hadroniclist_,emlist_);
  physList_->initialize(defaultCut_, gammaCut_, electronCut_, positronCut_, neutronCut_);
  runManager_->SetUserInitialization(physList_);

  //G4PhysListFactory factory;
  //G4VModularPhysicsList* physics = factory.GetReferencePhysList(physicsList_);
  //physics->SetVerboseLevel(verbose);
  //runManager_->SetUserInitialization(physics);

  //PGA
  pgAction_ = new PrimaryGeneratorAction(physList_);
  pgAction_->setVerbose(verbose_);
  runManager_->SetUserAction(pgAction_);

  //Run action
  runAction_ = new RunAction();
  runAction_->SetSeed();
  runManager_->SetUserAction(runAction_);

  //Stepping Action
  if(geoMgr_)stpAction_ = new SteppingAction(geoMgr_,cutEnergy_);
  else stpAction_ = new SteppingAction(cutEnergy_);
  stpAction_->setVerbose(verbose_);
  stpAction_->setVolume(targetVolName_);
  runManager_->SetUserAction(stpAction_);

  //Event Action
  if(geoMgr_)eventAction_ = new EventAction(geoMgr_,stpAction_);
  else eventAction_ = new EventAction(stpAction_);
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
/*
bool Geant4TrackingAlgorithm::Process() {

  //ensure this is set to 1
  pgAction_->setUsingGun(1);
  this->runGunMac();
  //runManager_->BeamOn(1);
  //if(verbose_>0)stpAction_->Print();
 
  G4UIsession * session = new G4UIterminal(new G4UItcsh, false);
  //session->SessionStart();
  delete session;

  return true;
}
*/
bool Geant4TrackingAlgorithm::Process(NeutrinoEvent* event) {

  this->getPrimaryGenerator()->setNuEvent(event);

  runManager_->BeamOn(1);
  //if(verbose_>0)stpAction_->Print();
  //if (this->getSteppingAction()->hasHit())
    //return true;

  return true;
}
/*
void Geant4TrackingAlgorithm::runGunMac(){

    char* GasTPC = "";
    if(getenv("GasTPC"))GasTPC = getenv("GasTPC");
    else std::cout << "\n--NOTE: $GasTPC env varible not set---" <<std::endl;
    std::cout << "\n****Executing run script: " 
	      << "/control/execute " +  std::string(GasTPC) + "/src/simul/run.mac" <<std::endl; 
    UImanager->ApplyCommand("/control/execute " +  std::string(GasTPC) + "/src/simul/run.mac");

}
*/
void Geant4TrackingAlgorithm::runVisMac(){

  //char* GasTPC = "";
  //if(getenv("GasTPC"))GasTPC = getenv("GasTPC");
  std::string GasTPC = "";
  if(getenv("GasTPC"))
    GasTPC = std::string(getenv("GasTPC"));
  else std::cout << "\n--NOTE: $GasTPC env varible not set---" <<std::endl;

  std::cout << "\n****Executing visualisation script: " 
	    << "/control/execute " +  std::string(GasTPC) + "/src/simul/vis.mac" <<std::endl;
  UImanager->ApplyCommand("/control/execute " +  std::string(GasTPC) + "/src/simul/vis.mac");

}
