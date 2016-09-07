//___________________________________________________________________________
/*!

\class    GasTPCProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2016
*/
//____________________________________________________________________________
#include "GasTPCProcessor.hh"

GasTPCProcessor::GasTPCProcessor()
  : precessCount_(0), recordCount_(0), backTracer_(0){
  //backTracerBuff_ = &backTracer_;
  fNtpMCEventRecord = new genie::NtpMCEventRecord(); // protection when running the particle gun
}

GasTPCProcessor::~GasTPCProcessor(){
  delete fNtpMCEventRecord;
}
