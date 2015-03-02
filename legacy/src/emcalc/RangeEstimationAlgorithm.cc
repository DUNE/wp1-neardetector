#include "RangeEstimationAlgorithm.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

RangeEstimationAlgorithm::RangeEstimationAlgorithm() {
  this->initDataCards();
}

RangeEstimationAlgorithm::~RangeEstimationAlgorithm() {}

void RangeEstimationAlgorithm::initialize() {
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes
  detector_ = new EMCalcDetectorConstruction();
  detector_->SetMaterial(material_);
  runManager->SetUserInitialization(detector_);

  physList_ = new EMCalcPhysicsList;
  runManager->SetUserInitialization(physList_);

  pgAction_ = new EMCalcPrimaryGeneratorAction(detector_);
  runManager->SetUserAction(pgAction_);

  // set user action classes
  runAction_ = new RunAction(detector_, pgAction_);
  runManager->SetUserAction(runAction_);

  char* LBNO = getenv("LBNO");
  G4String command = "/control/execute " + std::string(LBNO) + "/src/emcalc/EMCalc.in";
  std::cout << "going to run " << command << std::endl;
  G4UImanager::GetUIpointer()->ApplyCommand(command); 

}

void RangeEstimationAlgorithm::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardString("rangeEstimationMaterial", "MIND_mixture");
  cardsLoaded_ = false;
}

void RangeEstimationAlgorithm::loadDataCards() {
  material_        = cards_->fetchValueString("rangeEstimationMaterial");
  cardsLoaded_     = true;
}

bool RangeEstimationAlgorithm::Process(tpcFidHit* event, MuonRange* range) {

  G4ParticleGun* particleGun = pgAction_->GetParticleGun();

  int pdg = event->getMuon().getPDG();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(pdg);
  particleGun->SetParticleDefinition(particle);

  double mu_mass = particle->GetPDGMass();
  double kinE = event->getMuon().getP4().E() - mu_mass;
/*
  double p = event->getMuon().getP4().Vect().Mag();
  std::cout << " \n \n E = " << event->getMuon().getP4().E() * (1./CLHEP::GeV) << " GeV"
  << "   P = " << p * (1./CLHEP::GeV) << " GeV"
  << "   Ekin = " << kinE  * (1./CLHEP::GeV) << " GeV" << std::endl;
*/
  TVector3 direction = event->getMuon().getP4().Vect().Unit();
  particleGun->SetParticleEnergy(kinE);

  G4UImanager::GetUIpointer()->ApplyCommand("/run/beamOn");
  range->setRange(runAction_->getRange());

  //TVector3 displ = direction*(runAction_->getRange()) * (1./CLHEP::m);
  //std::cout << "displ[m]: (" << displ.X() << ", " << displ.Y() << ", " << displ.Z() << ")" << std::endl;

  TVector3 pos = event->getPosition().Vect() + direction*(runAction_->getRange());
  range->setPosition( TLorentzVector(pos.X(), pos.Y(), pos.Z(), 0.) );
  return true;
}











