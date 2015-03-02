//____________________________________________________________________________
/*!

\class    RunAction

\brief    Class for the user run action in geant4

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef RunAction_h
#define RunAction_h 1

#include <G4UserRunAction.hh>
#include <globals.hh>

#include "Randomize.hh"

#include <CLHEP/Random/Random.h>

class G4Run;

class RunAction : public G4UserRunAction {
 public:
  RunAction();
  ~RunAction();
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  // Set the seed to a new value.  This takes a long since the low-level
  // random generate expects a long seed.
  void SetSeed();
  void SetSeed(long);
  
  // Get the seed that started the low level random generator.
  long GetSeed(void) const;

};

#endif





