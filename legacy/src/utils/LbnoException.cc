//____________________________________________________________________________
/*!

\class    lbnoException

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "LbnoException.hh"
ClassImp(LbnoException)

LbnoException::LbnoException(const std::string & ErrorDescr,
                             const std::string & ErrorLocation,
                             int Svr)
: errorDescription_(ErrorDescr), errorLocation_(ErrorLocation), severity_(Svr) {}



