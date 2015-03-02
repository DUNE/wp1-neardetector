//____________________________________________________________________________
/*!

\class    Misc

\brief    Add misc stuff here

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef UTILS_MISC_H
#define UTILS_MISC_H 1

#include <map>
#include <string>

// case-independent (ci) string less_than
// returns true if s1 < s2
struct ci_less : std::binary_function <std::string, std::string, bool> {
  // case-independent (ci) compare_less binary function
  struct nocase_compare : public std::binary_function<unsigned char,unsigned char,bool> {
    bool operator() (const unsigned char& c1, const unsigned char& c2) const {
      return tolower(c1) < tolower(c2);
    }
  };

  bool operator() (const std::string & s1, const std::string & s2) const {
    return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), nocase_compare());
  };
};

#endif
