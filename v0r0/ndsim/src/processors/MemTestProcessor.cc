//____________________________________________________________________________
/*!

\class    MemTestProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2013
\last update Jun 2015
*/
//____________________________________________________________________________
#include "MemTestProcessor.hh"

MemTestProcessor::MemTestProcessor() {
  name_ = "MemTestProcessor";
  entries = 100;
}

void MemTestProcessor::initOutDataTree() {
  outData_ = new TTree("MemTest", "Memory allocation test");
  this->makeOutTreeBranches(event_);
  //   if (outTreeBasketSize_)
  //     this->setOutTreeBasketSize(outTreeBasketSize_, event_);
}

bool MemTestProcessor::process() {
//  std::cout << "This is MemTestProcessor::process()" << std::endl;
  backTracer_.resize(0);
  for(int i=0; i<entries; i++){
    event_->setData( rand_.Uniform(-100., 100.) );
    backTracer_.push_back(precessCount_);
    outData_->Fill();
  }

  int nEntries = outData_->GetEntries();
  if( nEntries%10000 == 0 ) {
    gSystem->GetProcInfo(&info_);
    printf( "MemTestProcessor: nEntries=%d, MemRes=%dKB, MemVirt=%dKB\n",
             nEntries, info_.fMemResident, info_.fMemVirtual);
  }

  precessCount_++;
  return true;
}

