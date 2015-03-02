//____________________________________________________________________________
/*!

\class    MemTestProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2013
\last update Sep 2013

*/
//____________________________________________________________________________
#include "MemTestProcessor.hh"

MemTestProcessor::MemTestProcessor() {
  name_ = "MemTestProcessor";
  this->initDataCards();
}

void MemTestProcessor::initOutDataTree() {
  outData_ = new TTree("MemTest", "Memory allocation test");
  this->makeOutTreeBranches(event_);
//   if (outTreeBasketSize_)
//     this->setOutTreeBasketSize(outTreeBasketSize_, event_);
}

void MemTestProcessor::initDataCards() {
  cards_->AddDataCardInt("memTestBasketSize", 640000);
  cards_->AddDataCardInt("seed", 0);
}

void MemTestProcessor::loadDataCards() {
  outTreeBasketSize_ = cards_->fetchValueInt("memTestBasketSize");
  rand_.SetSeed(cards_->fetchValueInt("seed"));
}

bool MemTestProcessor::process() {
//  std::cout << "This is MemTestProcessor::process()" << std::endl;
  backTracer_.resize(0);

  event_->setData( rand_.Uniform(-100., 100.) );
  backTracer_.push_back(precessCount_);
  outData_->Fill();

  int nEntries = outData_->GetEntries();
  if( nEntries%10000 == 0 ) {
    gSystem->GetProcInfo(&info_);
    printf( "MemTestProcessor: nEntries=%d, MemRes=%dKB, MemVirt=%dKB\n",
             nEntries, info_.fMemResident, info_.fMemVirtual);
  }

  precessCount_++;
  return true;
}

