//___________________________________________________________________________
/*!

\class    GasTPCProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "GasTPCProcessor.hh"

GasTPCProcessor::GasTPCProcessor()
: precessCount_(0), recordCount_(0),  gunOn_(0){
  backTracerBuff_ = &backTracer_;
  cards_ = DataCards::getInstance();
}
