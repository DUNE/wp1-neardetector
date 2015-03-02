//____________________________________________________________________________
/*!

\class    StackingAction

\brief    Class for the user stacking action in geant4

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update April 2014

*/
//____________________________________________________________________________
#ifndef StackingAction_hh_seen
#define StackingAction_hh_seen

#include "G4Track.hh"
#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"

/// Control which particles are actually tracked by G4.  
class StackingAction : public G4UserStackingAction {
public:
    StackingAction();
    virtual ~StackingAction();

    /// Check if a new track should be tracked.
    //virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
};
#endif
