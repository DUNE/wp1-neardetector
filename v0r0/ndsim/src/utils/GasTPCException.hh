//____________________________________________________________________________
/*!

\class    GasTPCException

\brief    GasTPC Exception Class

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef UTILS_GasTPCEXCEPTION_H
#define UTILS_GasTPCEXCEPTION_H 1

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <exception>

#include <TObject.h>

class GasTPCException {

 public:
  enum {
    WARNING=0,
    SERIOUS=1,
    FATAL=2
  };

  GasTPCException(const std::string &ErrorDescr = "", 
                const std::string &ErrorLocation = "",
                const int Svr = SERIOUS );

  // Default destructor has nothing to do
  // except be declared virtual and non-throwing.
  ~GasTPCException() throw() {}

  std::string GetDescription() {return "*** " + errorDescription_ + " ***";}
  std::string GetLocation()    {return "ERROR in " + errorLocation_;}


 protected: // sometimes derived classes want to manipulate this one
  std::string errorDescription_;
  std::string errorLocation_;
  int severity_;

  ClassDef(GasTPCException, 1);
};

#endif

