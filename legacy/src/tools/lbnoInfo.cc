//____________________________________________________________________________
/*!

\class    lbnoInfo

\brief    Program to give basic information on run etx

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2013
\last update Sep 2013

*/
//____________________________________________________________________________
#include "datastruct/DataCardsRecord.hh"
#include "DataCards.hh"
#include "RunStats.hh"

#include <TTree.h>
#include <TFile.h>

#include <iostream>
#include <string>

const char* HELP =
    "Usage: lbnoinfo INFILE \n";

string optInputFileName;

int main(int argc, char** argv)
{
  // Open input file
  TFile file(argv[1], "READ");
  if (file.IsZombie()) {
    std::cout << "" << std::endl;
    exit(1);
  }

  bool noInfo = true;

  // Read dataCards
  try {
    DataCards *cards = DataCards::getInstance();
    cards->loadKeys(&file);
    cards->printToStream(cout);
  } catch (LbnoException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    file.Close();
    return 1;
  }

  // Read run stats
  RunStats* runStats = dynamic_cast<RunStats*>(file.Get("runStats"));
  if (runStats) {
    std::cout << "KEY: runStats ..." << std::endl;
    runStats->printToStream(cout);
    noInfo = false;
  }
/*
    // Read detector params
    DetectorParameters* dp = dynamic_cast<DetectorParameters*>(file.Get("detectorParams"));
    if (dp) {
        cout << "KEY: detectorParams ..." << endl;
        dp->printToStream(cout);
        noInfo = false;
    }

    // Read header generated from G4 simulation
    NfndG4SimulHeader* g4header = dynamic_cast<NfndG4SimulHeader*>(file.Get("g4header"));
    if (g4header) {
        cout << "KEY: g4header ..." << endl;
        g4header->printToStream(cout);
        noInfo = false;
    }

    // Read hits tree
    TTree *hitsTree = dynamic_cast<TTree*>(file.Get("hits"));
    if (hitsTree) {
        cout << "OBJ: hits ..." << endl;
        cout << "    nEntries = " << hitsTree->GetEntries() << endl;
        noInfo = false;
    }

    // Read digits tree
    TTree *digitsTree = dynamic_cast<TTree*>(file.Get("digits"));
    if (digitsTree) {
        cout << "OBJ: digits ..." << endl;
        cout << "    nEntries = " << hitsTree->GetEntries() << endl;
        noInfo = false;
    }
*/
  file.Close();

  return 0;
}
