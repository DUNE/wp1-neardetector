//____________________________________________________________________________
/*!

\class    ApplyCuts

\brief    Class to for geant4 cuts

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Apr 2015

*/
//____________________________________________________________________________
#ifndef APPLYCUTS_HH
#define APPLYCUTS_HH

#include <globals.hh>
#include <G4ParticleDefinition.hh>
#include <G4Track.hh>
#include <G4Step.hh>

class ApplyCuts {

public:
  ApplyCuts(G4double);
  virtual ~ApplyCuts();

  bool process(const G4Step* aStep);
  bool process(const G4Track* aTrack);

  G4double getEnergyCut() {return cutEnergy_;};

private:

  bool set;
 
  G4double cutEnergy_;

  const G4Step * step_;
  const G4Track* track_;

};
#endif
