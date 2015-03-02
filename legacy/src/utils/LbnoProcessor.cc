//___________________________________________________________________________
/*!

\class    LbnoProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "LbnoProcessor.hh"

LbnoProcessor::LbnoProcessor()
: precessCount_(0), recordCount_(0),  gunOn_(0){
  backTracerBuff_ = &backTracer_;
  cards_ = DataCards::getInstance();
}
