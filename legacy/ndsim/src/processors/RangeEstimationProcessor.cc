//____________________________________________________________________________
/*!

\class    RangeEstimationProcessor

\brief    Processor class to implement the estimation of the muon range

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva


\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "RangeEstimationProcessor.hh"

RangeEstimationProcessor::RangeEstimationProcessor() {
  name_ = "RangeEstimationProcessor";
}

bool RangeEstimationProcessor::process() {

  inData_->GetEntry(precessCount_);
  precessCount_++;

  g4ranger_.Process(MChits_, range_);

  //MChits_->printToStream(std::cout);
  //range_->printToStream(std::cout);

  outData_->Fill();
}

void RangeEstimationProcessor::loadInDataTree() {
  this->setInTreeBranches<tpcFidHit>(MChits_);
}

void RangeEstimationProcessor::loadDataCards() {
  g4ranger_.loadDataCards();
  g4ranger_.initialize();
}

void RangeEstimationProcessor::initOutDataTree() {
  TFile *file = TFile::CurrentFile();
  if (file->IsOpen()) {
    TTree* tree;
    file->GetObject("MuRange", tree);
    if (tree)
      tree->Delete("all");
  }

  outData_ = new TTree("MuRange", "Muon Range in MIND");
  this->makeOutTreeBranches<MuonRange>(range_);
  if (outTreeBasketSize_)
   this->setOutTreeBasketSize<MuonRange>(outTreeBasketSize_, range_);
}
