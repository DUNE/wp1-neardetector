//____________________________________________________________________________
/*!

\class    PiFluxFastProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "PiFluxFastProcessor.hh"

PiFluxFastProcessor::PiFluxFastProcessor() {
  name_ = "PiFluxFastProcessor";
  initDataCards();
}

void PiFluxFastProcessor::initOutDataTree() {
  piDecay_ = new PionDecayEvent();
  // Create the data tree.
  outData_ = new TTree("NuHits", "Neutrino Hits");
  this->makeOutTreeBranches<NeutrinoHit>(neutrino_);
  if (outTreeBasketSize_)
   this->setOutTreeBasketSize<NeutrinoHit>(outTreeBasketSize_, neutrino_);
}

void PiFluxFastProcessor::initDataCards() {

  cards_->AddDataCardInt("fastFluxBasketSize", 640000);
}

void PiFluxFastProcessor::loadDataCards() {

  generator_.loadDataCards();
  projector_.loadDataCards();
  outTreeBasketSize_ = cards_->fetchValueInt("fastFluxBasketSize");
}

bool PiFluxFastProcessor::process() {

  backTracer_.resize(0);
  bool done = false;
  while (!done) {
    generator_.getNextEvent(piDecay_);
    if ( projector_.Process(piDecay_, neutrino_) ) {
      backTracer_.push_back(recordCount_);
      //std::cout << "backTracer : " << backTracer_[0] << std::endl;
      outData_->Fill();

      recordCount_++;
      done = true;
    }
  }

  return done;
}




