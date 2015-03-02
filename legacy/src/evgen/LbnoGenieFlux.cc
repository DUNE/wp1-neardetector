//____________________________________________________________________________
/*!

\class    LbnoGenieFlux

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "LbnoGenieFlux.hh"

LbnoGenieFlux::LbnoGenieFlux()
: maxEnergy_(0), count_(0), neutrino_(0) {

  double maxEnergy = DataCards::getInstance()->fetchValueDouble("nuMaxEnergy");
  addAllNeutrinos();

  //int chargeSign     = cards_rec->fetchValueInt("piSign");
  //addFluxParticle(-chargeSign*14);
  //addFluxParticle(chargeSign*12);

  setMaxEnergy(maxEnergy);
}

bool LbnoGenieFlux::GenerateNext() {

  // Check that the neutrino_ points to the next event to be processed.
  if (End())
    return false;
  
  //neutrino_->printToStream(std::cout);
  pdgCode_ = neutrino_->getPDG();
  momentum_ = neutrino_->getP4() * (1/CLHEP::GeV);
  position_ = neutrino_->getPosition() * (1/CLHEP::m);
  count_++;

  // Unset the neutrino_ pointer.
  // Someone has to set it again using LbnoGenieFlux:setNeutrino() before 
  // the next call of this function.
  neutrino_ = NULL;

  return true;
}

bool LbnoGenieFlux::End() {
  if (!neutrino_) {
    return true;
  }

  return false;
}

