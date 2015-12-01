//____________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

#include "utils/UtlStr.hh"

#include <algorithm>

//___________________________________________________________________________
string util::TrimSpaces(string input)
{
  if( input.find_first_not_of(" \n") != 0)
    input.erase( 0,  input.find_first_not_of(" \n")  );

  if( input.find_last_not_of(" \n") != input.length() )
    input.erase( input.find_last_not_of(" \n")+1, input.length() );

  return input;
}
//____________________________________________________________________________
vector<string> util::Tokens (string input, string delimiter)
{
// split a string of 'delimiter' separated values and return each string
// part as a vector<string> element.

  vector<string> tokens;

  while(input.find_first_of(delimiter) < input.length()) {

    tokens.push_back( input.substr(0, input.find_first_of(delimiter)) );
    input = input.substr(input.find_first_of(delimiter)+1, input.length());
  }
  tokens.push_back(input);
  return tokens;
}
//___________________________________________________________________________

string util::ToLowerCase(string data)
{
  std::transform(data.begin(), data.end(), data.begin(), ::tolower);
  return data;
}
//___________________________________________________________________________
