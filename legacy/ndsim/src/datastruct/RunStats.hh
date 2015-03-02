//____________________________________________________________________________
/*!

\class    RunStats

\brief    Class for statistics on run

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef DATASTRUCT_RUNSTATS_H
#define DATASTRUCT_RUNSTATS_H 1

#include "Units.hh"
#include "Misc.hh"

#include <TObject.h>

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <locale>

using namespace std;

class RunStats : public TObject
{
 public :
  RunStats();
  ~RunStats();

  void setStat(const string& name, const string& value);
  void setStat(const string& name, int value);
  void setStat(const string& name, double value);

  map<string,string,ci_less> getInfoMap(){return stats;};

  const std::string getStringStat();
  const int getIntStat();
  const double getDoubleStat();
  /*
  // Overriden methods from TObject
  void Copy(TObject& object) const;
  void Clear(Option_t* option = "");
  void Print(Option_t* option = "") const;
  */

  // Utility methods
  void printToStream(ostream& stream) const;


 private :
  map<string,string,ci_less> stats;

 ClassDef(RunStats, 1);
};

#endif
