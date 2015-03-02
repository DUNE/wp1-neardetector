//____________________________________________________________________________
/*!

\class    TpcFidSensitiveDetector

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "TpcFidSensitiveDetector.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "DataCards.hh"

#include <G4StepStatus.hh>
#include <cstring>
#include <iostream>

TpcFidSensitiveDetector::TpcFidSensitiveDetector(const G4String& name)
: DetectorSD(name) {

	//SDname = name;
	//HCname = "tpcFidHitsCollection";
  	//collectionName.insert(HCname);
  	positionResolution = 0.3*mm;
}

