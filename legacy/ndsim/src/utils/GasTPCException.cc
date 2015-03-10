//____________________________________________________________________________
/*!

\class    GasTPCException

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "GasTPCException.hh"
ClassImp(GasTPCException)

GasTPCException::GasTPCException(const std::string & ErrorDescr,
                             const std::string & ErrorLocation,
                             int Svr)
: errorDescription_(ErrorDescr), errorLocation_(ErrorLocation), severity_(Svr) {}



