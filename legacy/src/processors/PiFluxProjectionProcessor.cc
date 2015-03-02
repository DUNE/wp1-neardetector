//____________________________________________________________________________
/*!

\class    PiFluxProjectionProcessor

\brief    Processor class to implement pion forward projection

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "PiFluxProjectionProcessor.hh"

PiFluxProjectionProcessor::PiFluxProjectionProcessor() {
  name_ = "PiFluxProjectionProcessor";
  initDataCards();
}

void PiFluxProjectionProcessor::initOutDataTree() {
  outData_ = new TTree("NuHits", "Neutrino Hits");
  this->makeOutTreeBranches<NeutrinoHit>(neutrino_);
  if (outTreeBasketSize_)
   this->setOutTreeBasketSize<NeutrinoHit>(outTreeBasketSize_, neutrino_);
}

void PiFluxProjectionProcessor::loadInDataTree() {
  this->setInTreeBranches<PionDecayEvent>(piDecay_);
}

void PiFluxProjectionProcessor::initDataCards() {
  projector.initDataCards();
  cards_->AddDataCardInt("fluxProjectionBasketSize", 640000);
}

void PiFluxProjectionProcessor::loadDataCards() {
  projector.loadDataCards();
  outTreeBasketSize_ = cards_->fetchValueInt("fluxProjectionBasketSize");
}

bool PiFluxProjectionProcessor::process() {

  inData_->GetEntry(precessCount_);
  precessCount_++;

  if ( projector.Process(piDecay_, neutrino_) ) {
    backTracer_.push_back(recordCount_);
    //std::cout  << backTracer_.size() << " : " << backTracer_[0] << "," << backTracer_[1] << std::endl;
    outData_->Fill();

    recordCount_++;
  }

  return true;
}




