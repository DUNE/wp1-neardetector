//____________________________________________________________________________
/*!

\class    MuDecayProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "MuDecayProcessor.hh"

MuDecayProcessor::MuDecayProcessor()
: generator_() {
  name_ = "MuDecayProcessor";
  outData_ = generator_.getDataTree();
}

bool MuDecayProcessor::process() {
  // std::cout << "This is MuDecayProcessor::process()" << std::endl;
  generator_.Process();
}

