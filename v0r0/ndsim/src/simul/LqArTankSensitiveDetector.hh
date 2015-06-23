//____________________________________________________________________________
/*!

\class    LqArTankSensitiveDetector

\brief    Class to implement magentic field in geant4

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update May 2014

*/
//____________________________________________________________________________

#ifndef LQARTANKSD_h
#define LQARTANKSD_h 1

#include "GasTPCDataLib.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class LqArTankSensitiveDetector : public G4VSensitiveDetector {
 public:
  //! Constructor
  LqArTankSensitiveDetector(const G4String&);

  //! Default destructor
  ~LqArTankSensitiveDetector();

  //! Initialise this this instance with the G4 event information
  void Initialize( G4HCofThisEvent* );

  //! process all hits in this detector
  G4bool ProcessHits( G4Step*, G4TouchableHistory* );

  //! code that is called at the end of processing of an event
  void EndOfEvent( G4HCofThisEvent* );

  void setHitPtr(LiqArTankHit* h) {hit_ = h;}

 private:

  int nSteps_;
  LiqArTankHit* hit_;

};

#endif



