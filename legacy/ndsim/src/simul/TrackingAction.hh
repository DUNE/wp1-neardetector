//____________________________________________________________________________
/*!

\class    TrackingAction

\brief    Class to implement user tracking action in geant4

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update Sep 2013

*/
//____________________________________________________________________________
#ifndef SIMUL_TRACKINGACTION_H 
#define SIMUL_TRACKINGACTION_H 1

#include "TrackInformation.hh"

#include <G4Track.hh>
#include <G4TrackingManager.hh>

#include <iostream>

#include "G4UserTrackingAction.hh"

class TrackingAction : public G4UserTrackingAction {
 public: 
  TrackingAction(); 
  virtual ~TrackingAction();
  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);

private:

  double preX,preY,preZ;
  double postX,postY,postZ;

};

#endif
