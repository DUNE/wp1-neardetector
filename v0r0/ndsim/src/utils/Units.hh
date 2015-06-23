//____________________________________________________________________________
/*!

\class    Units

\brief    Units here

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  June 2014
\last update June 2014

*/
//____________________________________________________________________________

#ifndef UTILS_UNITS_H
#define UTILS_UNITS_H 1

#include <CLHEP/Units/SystemOfUnits.h>

#include <string>
#include <iostream>
#include <cstdlib>

double stringToUnit(const std::string& str);

#endif
