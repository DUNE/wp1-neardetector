//____________________________________________________________________________
/*!

\class    ApplyCuts

\brief    Class to for geant4 cuts

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#ifndef APPLYCUTS_HH
#define APPLYCUTS_HH

#include <globals.hh>
#include <G4ParticleDefinition.hh>
#include <G4Track.hh>
#include <G4Step.hh>

#include "DataCards.hh"

class ApplyCuts {

public:
  ApplyCuts(G4double);
  ~ApplyCuts();

  bool process(const G4Step* aStep);
  bool process(const G4Track* aTrack);

  void initDataCards();
  void loadDataCards();

  G4double getEnergyCut() {return cutEnergy_;};

private:

  bool set;
 
  G4double cutEnergy_;

  const G4Step * step_;
  const G4Track* track_;

  DataCards *cards_;
  bool      cardsLoaded_;

};
#endif
