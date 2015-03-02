//____________________________________________________________________________
/*!

\class    TpcFidSensitiveDetector

\brief    Class for the sensitive detector of the TPC

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef TPCFIDSENSITIVEDETECTOR_hh
#define TPCFIDSENSITIVEDETECTOR_hh 1

#include "LbnoDataLib.hh"

#include "DetectorSD.hh"

class G4Step;
class G4HCofThisEvent;

class TpcFidSensitiveDetector : public DetectorSD {
 public:
  //! Constructor
  TpcFidSensitiveDetector(const G4String&);

  //! Default destructor
  //~TpcFidSensitiveDetector();

  //void setHitPtr(tpcFidHit* h) {hit_ = h;}

 private:

  //tpcFidHit* hit_;

};

#endif



