//____________________________________________________________________________
/*!

\class    RunStats

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "RunStats.hh"
ClassImp(RunStats)

RunStats::RunStats() {}

RunStats::~RunStats() {}

const std::string RunStats::getStringStat() {
}
const int RunStats::getIntStat() {
}
const double RunStats::getDoubleStat() {
}

void RunStats::setStat(const string& name, const string& value) {
  stats[name] = value;
}

void RunStats::setStat(const string& name, int value) {
  stringstream ss;
  ss << value;
  stats[name] = ss.str();
}

void RunStats::setStat(const string& name, double value) {
  stringstream ss;
  ss.precision(2);
  ss << scientific << value;
  stats[name] = ss.str();
}

void RunStats::printToStream(ostream& stream) const {
  map<string,string>::const_iterator it;
  stream << "========================= RunStats =========================" << endl;
  for (it = stats.begin(); it != stats.end(); it++) {
    stream << " - " << it->first << " = " << it->second << endl;
  }
  stream << "============================================================" << endl;
}
