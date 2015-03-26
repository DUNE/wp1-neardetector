//____________________________________________________________________________
/*!

\class     utils

\brief     Utilities for string manipulation

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2014, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _gastpc_str_utils_h_
#define _gastpc_str_utils_h_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace util {

  string         TrimSpaces      (string input);
  vector<string> Tokens          (string input, string delimiter);
  string         ToLowerCase     (string data);
  
} // util
  
#endif


