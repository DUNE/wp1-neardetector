//____________________________________________________________________________
/*!

\class    RunAction

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#include "RunAction.hh"

RunAction::RunAction() {
  //set the seed engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
}

RunAction::~RunAction() {
}

void RunAction::BeginOfRunAction(const G4Run* /*aRun*/) {
}

void RunAction::EndOfRunAction(const G4Run* /*aRun*/) {
}

void RunAction::SetSeed() {
    //set random seed with system time
    G4long seed = time(NULL);
    if (seed<0) seed = -seed;
    G4cout << "### Random seed number set to: " << seed<< G4endl;
    CLHEP::HepRandom::setTheSeed(seed);
}

void RunAction::SetSeed(long seed) {
    if (seed<0) seed = -seed;
    G4cout << "### Random seed number set to: "<< seed << G4endl;
    CLHEP::HepRandom::setTheSeed(seed);
}

long RunAction::GetSeed(void) const {
    return CLHEP::HepRandom::getTheSeed();
}

