//____________________________________________________________________________
/*!

\class    MuDecayAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "MuDecayAlgorithm.hh"

double funcP1(double* x, double* par) {
  return ((3 - 2 * x[0]) + x[1] * par[0] * (1 - 2 * x[0])) * x[0] * x[0];
}

double funcP2(double* x, double* par) {
  return (1 - x[0]) * (1 + x[1] * par[0]) * x[0] * x[0];
}

MuDecayAlgorithm::MuDecayAlgorithm() {
  initialize();
  initDataCards();
}

MuDecayAlgorithm::~MuDecayAlgorithm() {
  delete func1_;
  delete func2_;
}

void MuDecayAlgorithm::initialize() {

  nDecay_ = -1;
  event_ = new MuonDecayEvent();
  neutrino_ = new NeutrinoHit();
    // Create the data tree.
  dataTree_ = new TTree("dataTree", "NFFlux");
  dataTree_->Branch("nDecay", &nDecay_, "nDecay/I");
  dataTree_->Branch("theNeutrino", &neutrino_);

  func1_ = new TF2("func1", funcP1, 0, 1, -1, 1, 1);
  func1_->SetParameter(0, -muSign_ * muPolarization_);
  TF12 *f1pi = new TF12("f11", func1_, 1., "x");
  TF12 *f1mpi = new TF12("f12", func1_, -1., "x");
  double max11 = f1pi->GetMaximum();
  double max12 = f1mpi->GetMaximum();
  max1_ = (max11 > max12) ? max11 : max12;

  func2_ = new TF2("func2", funcP2, 0, 1, -1, 1, 1);
  func2_->SetParameter(0, -muSign_ * muPolarization_);
  TF12 *f2pi = new TF12("f21", func2_, 1., "x");
  TF12 *f2mpi = new TF12("f22", func2_, -1., "x");
  double max21 = f2pi->GetMaximum();
  double max22 = f2mpi->GetMaximum();
  max2_ = (max21 > max22) ? max21 : max22;

  delete f1pi;
  delete f1mpi;
  delete f2pi;
  delete f2mpi;
}

void MuDecayAlgorithm::initDataCards() {
  cards_ = cards_->getInstance();
  cards_->AddDataCardInt("muSign", -1);
  cards_->AddDataCardInt("muPol", 0);
  cards_->AddDataCardDouble("muEnergy", 25., "GeV");
  cards_->AddDataCardDouble("muEnergySigma", 80.,"MeV");
  cards_->AddDataCardDouble("distance", 100., "m");
  cards_->AddDataCardDouble("radiusCut", -1., "m");
  cards_->AddDataCardDouble("strSec", 600., "m");
  cards_->AddDataCardDouble("muThetaSigma", 0.5e-3, "rad");
  cards_->AddDataCardInt("seed", 0);
  cardsLoaded_ = false;
}

void MuDecayAlgorithm::loadDataCards() {

  muSign_          = cards_->fetchValueInt("muSign");
  muPolarization_  = cards_->fetchValueInt("muPol");
  muEnergy_        = cards_->fetchValueDouble("muEnergy");
  muEnergySigma_   = cards_->fetchValueDouble("muEnergySigma");
  muThetaSigma_    = cards_->fetchValueDouble("muThetaSigma");
  straightSection_ = cards_->fetchValueDouble("strSec");
  distance_        = cards_->fetchValueDouble("distance");
  radiusCut_       = cards_->fetchValueDouble("radiusCut");
  rand_.SetSeed(cards_->fetchValueInt("seed"));

  cards_->AddDataCardDouble("nuMaxEnergy", muEnergy_ + muEnergySigma_*5.);
  cardsLoaded_ = true;
}

void MuDecayAlgorithm::Process() {

  if(!cardsLoaded_)
    this->loadDataCards();

  getNextEvent();
/*
  if (event_->getPosition().P() < radiusCut_) {
    neutrino_->setPDG( event_->getNuMuPDG() );
    neutrino_->setPosition( event_->getPosition() );
    neutrino_->setP4( event_->getNuMuP4() );

    dataTree_->Fill();
  }

  if (event_->getPosition().P() < radiusCut_) {
    neutrino_->setPDG( event_->getNuEPDG() );
    neutrino_->setPosition( event_->getPosition() );
    neutrino_->setP4( event_->getNuEP4() );

    dataTree_->Fill();
  }
*/
}

void MuDecayAlgorithm::getNextEvent() {
  nDecay_++;
  // Get the decay position in the straight section (relative to detector z=0)
  double decayZ = -(rand_.Uniform(0, straightSection_) + distance_);

  event_->setNuMuPDG(-muSign_*14);
  event_->setNuEPDG(muSign_*12);

  TLorentzVector nuMuP4, nuMuPos, nuEP4, nuEPos;

  MakeNeutrino(func1_, max1_, decayZ, nuMuP4, nuMuPos);
  event_->setPosition(nuMuPos);
  event_->setNuMuP4(nuMuP4);

  MakeNeutrino(func2_, max2_, decayZ, nuEP4, nuEPos);
  event_->setPosition(nuEPos);
  event_->setNuEP4(nuEP4);
}

void MuDecayAlgorithm::MakeNeutrino(TF2* func, double funcMax, double decayZ,
                                        TLorentzVector& nuP4, TLorentzVector& nuPos) {
  // pick CosThCM and x from distribution with pdf 'func'
  double CosThCM, x, probe;
  do {
    probe = rand_.Uniform(0, funcMax);
    x = rand_.Uniform(0, 1); //normalized neutrino energy
    CosThCM = rand_.Uniform(-1, 1); //neutrino cosTheta in CM system
  } while (func->Eval(x, CosThCM) <= probe);

  double EMu = rand_.Gaus(muEnergy_, muEnergySigma_);
  double sinThCM = sqrt(1 - CosThCM * CosThCM);
  double pbeamMu = sqrt(EMu * EMu - MUON_MASS * MUON_MASS);
  double EmaxNuCM = MUON_MASS / 2; //maximum neutrino energy in CM system
  double ENuCM = x * EmaxNuCM; // Neutrino energy in CM system

  TLorentzVector beamMu(0, 0, pbeamMu, EMu);
  double Beta_Mu = beamMu.Beta();

  double phi = rand_.Uniform(-CLHEP::pi, CLHEP::pi);
  double px = ENuCM * sinThCM * cos(phi);
  double py = ENuCM * sinThCM * sin(phi);
  double pz = ENuCM * CosThCM;
  nuP4.SetPxPyPzE(px, py, pz, ENuCM); // Neutrino momentum in CM system

  double axisPhi = rand_.Uniform(-CLHEP::pi, CLHEP::pi);
  double axisX = cos(axisPhi);
  double axisY = sin(axisPhi);
  double ThMu = rand_.Gaus(0, muThetaSigma_);
  TVector3 axis(axisX, axisY, 0);
  TVector3 BetaMu(0, 0, Beta_Mu);
  BetaMu.Rotate(ThMu, axis);
  nuP4.Boost(BetaMu);

  double Phi_Nu_LS = nuP4.Phi();
  double Th_Nu_LS = nuP4.Theta();
  double posx = -tan(Th_Nu_LS) * cos(Phi_Nu_LS) * decayZ;
  double posy = -tan(Th_Nu_LS) * sin(Phi_Nu_LS) * decayZ;
  nuPos.SetXYZT(posx, posy, 0, 0);
}


