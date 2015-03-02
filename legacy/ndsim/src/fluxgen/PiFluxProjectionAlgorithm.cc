//____________________________________________________________________________
/*!

\class    PiFluxProjectionAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update May 2014

*/
//____________________________________________________________________________
#include "PiFluxProjectionAlgorithm.hh"

PiFluxProjectionAlgorithm::PiFluxProjectionAlgorithm() {
  initDataCards();
}

void PiFluxProjectionAlgorithm::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardDouble("distance", 100., "m");
  cards_->AddDataCardDouble("radiusCut", 10., "m");
  cards_->AddDataCardDouble("nuMaxEnergy", 20.*CLHEP::GeV);
  cards_->AddDataCardInt("flavour", 0);	//0 means all flavours
}

void PiFluxProjectionAlgorithm::loadDataCards() {
  distance_          = cards_->fetchValueDouble("distance");
  radiusCut_         = cards_->fetchValueDouble("radiusCut");
  maxNuE_            = cards_->fetchValueDouble("nuMaxEnergy");
  flavour_           = cards_->fetchValueInt("flavour");
}

bool PiFluxProjectionAlgorithm::Process(PionDecayEvent *piDecay, NeutrinoHit* nuHit) {

  //****************POSITIONS IN mm**********************//

  //if it decays in flight continue
  if (!piDecay->inFlight()) return false;

  // Here Z=0 is the beginning of the decay channel.
  TLorentzVector hitPos;
  TLorentzVector nuP4 = piDecay->getNuP4();  
  TLorentzVector nuPos = piDecay->getPosition(); 

  //get the pdg code for the neutrino
  int nuPDG = piDecay->getNuPDG();

  //select the particular flavour only for neutrino
  if(flavour_ !=0 && nuPDG != flavour_)return false;

  double piDecay_X = nuPos.X();
  double piDecay_Y = nuPos.Y();

  // Here Z=0 is the plane for the detector geometry for GENIE
  double piDecay_Z = distance_ - zOrigin_ - nuPos.Z();  
  double lambda = piDecay_Z/(nuP4.Vect().Unit().Z());

  // Calculate the XY coordinates of the neutrino in the detector.
  double hitPos_X = (nuP4.Vect().Unit().X() * lambda) + piDecay_X;
  double hitPos_Y = (nuP4.Vect().Unit().Y() * lambda) + piDecay_Y;
  double hitPos_Z = -zOrigin_;//CLHEP::mm;   // this should be in mm - 
					     //displace origin to set z=0 at the start of geometry
					     //shift from geometry->GetWorld()->GetZ()/2.0

  hitPos.SetXYZT(hitPos_X, hitPos_Y, hitPos_Z, 0);

  //std::cout << hitPos_X/CLHEP::cm << ":" << hitPos_Y/CLHEP::cm << std::endl;
  //std::cout << hitPos.P()/CLHEP::cm << ":" << radiusCut_/CLHEP::cm << std::endl;
  /*
  std::cout << "\n----piDecay("<< piDecay_X  << ", "<< piDecay_Y << ", "<< piDecay_Z 
	    <<"), nuPos.Z(): " <<  nuPos.Z()
	    <<", zOrigin_: " <<  zOrigin_
	    <<", distance_: " <<  distance_<< std::endl;
  */
  if ( ( ((hitPos.X()*hitPos.X()) + (hitPos.Y()*hitPos.Y())) < radiusCut_*radiusCut_) && (nuP4.E() <= maxNuE_) ) {

    nuHit->setPDG(piDecay->getNuPDG());
    nuHit->setP4(nuP4);
    nuHit->setPosition(hitPos);
    if(verbose_>0)nuHit->printToStream(std::cout);

    return true;
  }

  return false;
}


