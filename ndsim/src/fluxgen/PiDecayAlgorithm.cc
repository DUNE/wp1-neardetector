//____________________________________________________________________________
/*!

\class    PiDecayAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2014

*/
//____________________________________________________________________________
#include "PiDecayAlgorithm.hh"
#include "MuDecayAlgorithm.hh"

PiDecayAlgorithm::PiDecayAlgorithm() {
  initialize();
  initDataCards();
}

void PiDecayAlgorithm::initialize() {
  Energy_NuMu_CM_ = (PION_MASS * PION_MASS - MUON_MASS * MUON_MASS)/(2.*PION_MASS);
}

void PiDecayAlgorithm::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardInt("piSign", -1);
  cards_->AddDataCardDouble("piMomentum", 8., "GeV");
  cards_->AddDataCardDouble("piMomentumSigma", 5.,"GeV");
  //cards_->AddDataCardDouble("distance", 100., "m");
  //cards_->AddDataCardDouble("radiusCut", 10., "m");
  cards_->AddDataCardDouble("decaySec", 100., "m");
  cards_->AddDataCardDouble("piThetaSigma", 0.5e-3, "rad");
  cards_->AddDataCardInt("seed", 0);

  cardsLoaded_ = false;
}

void PiDecayAlgorithm::loadDataCards() {

  piSign_          = cards_->fetchValueInt("piSign");
  piMomentum_      = cards_->fetchValueDouble("piMomentum");
  piMomentumSigma_ = cards_->fetchValueDouble("piMomentumSigma");
  piThetaSigma_    = cards_->fetchValueDouble("piThetaSigma");
  decaySection_    = cards_->fetchValueDouble("decaySec");
  //distance_        = cards_->fetchValueDouble("distance");
  //radiusCut_       = cards_->fetchValueDouble("radiusCut");
  rand_.SetSeed(cards_->fetchValueInt("seed"));

  double nuMaxEnergy = sqrt(pow(piMomentum_ + piMomentumSigma_*5., 2) + pow(PION_MASS, 2));
  cards_->AddDataCardDouble("nuMaxEnergy", nuMaxEnergy);
  cardsLoaded_ = true;
}
/*
void PiDecayAlgorithm::Process() {

  if(!cardsLoaded_)
    this->loadDataCards();

  getNextEvent();

  if (event_->getPosition().P() < radiusCut_) {
    neutrino_->setPDG( event_->getNuPDG() );
    neutrino_->setPosition( event_->getPosition() );
    neutrino_->setP4( event_->getNuP4() );

    dataTree_->Fill();
  }

}
*/
bool PiDecayAlgorithm::getNextEvent(PionDecayEvent* event) {

  if(!cardsLoaded_)
    this->loadDataCards();

  event->setNuPDG(-piSign_ * 14);
  event->setChLeptonPDG(piSign_ * 13);

  TLorentzVector nuP4;
  TLorentzVector decayPos;
  double cosThCM, Energy_NuMu_CM_;
  cosThCM = rand_.Uniform(-1, 1); //neutrino cosTheta in CM system.
  Energy_NuMu_CM_ = (PION_MASS * PION_MASS - MUON_MASS * MUON_MASS)/(2.*PION_MASS);

  // Generate random momentum spread for the pions.
  double P_pi;
  do {
    P_pi= rand_.Gaus(piMomentum_, piMomentumSigma_);
    //std::cout <<"P_pi = " << P_pi/CLHEP::GeV << std::endl;
  } while (P_pi<0.);

  double sinThCM = sqrt(1 - cosThCM * cosThCM);
  double E_pi = sqrt(P_pi * P_pi + PION_MASS * PION_MASS);

  TLorentzVector beamPi(0, 0, P_pi, E_pi);
  double Beta_Pi = beamPi.Beta();                   // pion beta
  TVector3 BetaPi(0, 0, Beta_Pi);
  double Gamma_Pi = beamPi.Gamma();                    // pion gamma
  //std::cout << Beta_Pi << " " << Gamma_Pi << std::endl;
  
  // Get the neutrino 4vector in CM system.
  double phi = rand_.Uniform(-CLHEP::pi, CLHEP::pi);
  double px = Energy_NuMu_CM_ * sinThCM * cos(phi);
  double py = Energy_NuMu_CM_ * sinThCM * sin(phi);
  double pz = Energy_NuMu_CM_ * cosThCM;
  nuP4.SetPxPyPzE(px, py, pz, Energy_NuMu_CM_); // Neutrino momentum in CM system

  // Generate random divergency for the pion beam. 
  double axisPhi = rand_.Uniform(-CLHEP::pi, CLHEP::pi);
  double axisX = cos(axisPhi);
  double axisY = sin(axisPhi);
  double ThPi = rand_.Gaus(0, piThetaSigma_);

  // Pion decay simulation.
  double u = rand_.Uniform(0,1);
  double time_pi = -Gamma_Pi*PION_LIFETIME*log(u);     // lifetime of the pion.
  double distance_pi = time_pi*Beta_Pi*CLHEP::c_light; // distance travelled.
  /*
  std::cout << "distance_pi = " << distance_pi/CLHEP::m 
  << "   time_pi = " << time_pi/CLHEP::second << "    " 
  << Gamma_Pi*PION_LIFETIME/CLHEP::second << "    "
  << Beta_Pi*Gamma_Pi*PION_LIFETIME*CLHEP::c_light/CLHEP::m << std::endl;
  */

  // Calculate the coordinates of the pion decay (relative to end of decaySection z=0).
  double decayPosX ,decayPosY, decayPosZ;
  if (distance_pi < decaySection_) {
    // The pion will decay in flight.
    //std::cout << "pion decay in flight" << std::endl;
    event->setInFlight(true);
    // Boost the neutrino CM 4vector using the generated 
    // random energy spread and angylar beam divergency.
    TVector3 axis(axisX, axisY, 0);
    BetaPi.Rotate(ThPi, axis);
    nuP4.Boost(BetaPi);
    //std::cout << "E_nu = " << nuP4.E()/CLHEP::GeV << std::endl;
    decayPosX = BetaPi.Unit().X() * distance_pi;
    decayPosY = BetaPi.Unit().Y() * distance_pi;
    decayPosZ = (BetaPi.Unit().Z() * distance_pi) - decaySection_;

  } else {
    // The pion will decay at rest at the end of the decay tunnel.
    //std::cout << "pion decay at rest" << std::endl;
    //std::cout << "E_nu = " << nuP4.E()/CLHEP::GeV << std::endl;
    event->setInFlight(false);
    decayPosX = BetaPi.Unit().X() * decaySection_;
    decayPosY = BetaPi.Unit().Y() * decaySection_;
    decayPosZ = 0.;
  }
/*
  // Calculate the XY coordinates of the neutrino in the detector.
  double posX = nuP4.Vect().Unit().X() * decayPosZ + decayPosX; // m
  double posY = nuP4.Vect().Unit().Y() * decayPosZ + decayPosY; // m
  //std::cout << posX/CLHEP::m << ":" << posY/CLHEP::m << std::endl;
  nuPos.SetXYZT(posX, posY, 0, 0);
*/
  decayPos.SetXYZT(decayPosX, decayPosY, decayPosZ, 0);
  event->setPosition(decayPos);
  event->setNuP4(nuP4);
  return true;
}


