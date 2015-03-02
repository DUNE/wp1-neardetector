//____________________________________________________________________________
/*!

\class    DetectorSD

\brief    Base class for geant4 sensitive detector

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef DetectorSD_h
#define DetectorSD_h 1

#include <CLHEP/Units/SystemOfUnits.h>

#include "G4VSensitiveDetector.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4THitsCollection.hh"
#include "G4TransportationManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4TouchableHandle.hh"

#include "TGeoManager.h"
#include "TGeoVolume.h"

#include "TrackInformation.hh"
#include "LbnoDataLib.hh"

#include "DetectorHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class DetectorSD : public G4VSensitiveDetector
{
  public:
      DetectorSD(const G4String name);
      DetectorSD(const G4String name,const G4String &hitsCollectionName);
      DetectorSD(const G4String name,TGeoManager * geoMgr);
      virtual ~DetectorSD();

      virtual void setHitCollectionPtr(HitCollection* h) {SDHitCollection = h;};
      virtual HitCollection * getHitCollectionPtr() {if(SDHitCollection)return SDHitCollection;else return NULL;};

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

      virtual void EndOfEvent(G4HCofThisEvent*HCE);
      virtual void clear();
      //virtual void DrawAll();
     // virtual void PrintAll();

      virtual G4String GetName() {return SDname;};
      virtual G4String GetHCName() {return HCname;};
  
  protected:
      
      int NSteps_;
      int PDG_;
      int TrackID_,ParentID_;
      G4double kinEnergy_;
      G4double Edep_;
      TLorentzVector Pos_,P4_;
      bool trackLeftVolume;

      G4double positionResolution;

      G4String SDname;
      G4String HCname;

      DetectorHit* Hit_;
      DetectorHitsCollection* HitsCollection_;

      SDHit SDHit_;
      HitCollection * SDHitCollection;

  private:
      G4Navigator * theNavigator;

      G4VPhysicalVolume * preVolume;
      G4VPhysicalVolume * postVolume;

      TGeoManager * rootGeoMgr_;
      TGeoVolume * World_;
      TGeoVolume * preVol_;
      TGeoVolume * postVol_;

      std::string GeomFile;

};

#endif

