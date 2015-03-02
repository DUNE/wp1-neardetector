//____________________________________________________________________________
/*!

\class    PiDecayProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "PiDecayProcessor.hh"

PiDecayProcessor::PiDecayProcessor()
: generator_() {
  name_ = "PiDecayProcessor";
  initDataCards();
}

void PiDecayProcessor::initOutDataTree() {
  outData_ = new TTree("PiDecays", "Pion Decays");
  this->makeOutTreeBranches<PionDecayEvent>(event_);
  if (outTreeBasketSize_)
   this->setOutTreeBasketSize<PionDecayEvent>(outTreeBasketSize_, event_);
}

void PiDecayProcessor::initDataCards() {
 cards_->AddDataCardInt("piDecayBasketSize", 640000);
}

void PiDecayProcessor::loadDataCards() {
  outTreeBasketSize_ = cards_->fetchValueInt("piDecayBasketSize");
  generator_.loadDataCards();
}

bool PiDecayProcessor::process() {
//  std::cout << "This is PiDecayProcessor::process()" << std::endl;
  backTracer_.resize(0);
  generator_.getNextEvent(event_);
  backTracer_.push_back(precessCount_);
  outData_->Fill();
  precessCount_++;
  return true;
}

