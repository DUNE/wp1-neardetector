#ifndef MINDSD_h
#define MINDSD_h 1

#include <TVector3.h>

#include "DetectorSD.hh"
#include "LbnoDataLib.hh"

class G4Step;
class G4HCofThisEvent;

class MINDSensitiveDetector : public DetectorSD {
 public:
  //! Constructor
  MINDSensitiveDetector(const G4String&,const G4String& );

  //! Default destructor
  ~MINDSensitiveDetector();

  //! Initialise this this instance with the G4 event information
  void Initialize( G4HCofThisEvent* );

  //! process all hits in this detector
  G4bool ProcessHits( G4Step*, G4TouchableHistory* );

  //! code that is called at the end of processing of an event
  void EndOfEvent( G4HCofThisEvent* );

  void setHitPtr(MINDHit* h) {hit_ = h;}
 private:
  double Edep_;
  double path_;
  TVector3 endPos_;

  MINDHit* hit_;
  int nSteps_;
};

#endif
