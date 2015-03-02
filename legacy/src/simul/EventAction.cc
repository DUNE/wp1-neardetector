//____________________________________________________________________________
/*!

\class    EventAction

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "EventAction.hh"

#define DISTANCE(x,y) sqrt(x*x + y*y)

EventAction::EventAction(std::string sdName,SteppingAction* SA) :
 EventID(0),nTrajectories(0) { 

  stpaction = (SteppingAction*)SA;

  detectorCollIDVect.resize(0);
  SDName = sdName;

  rootGeoMgr_ = NULL;
  World_ = NULL;

}

EventAction::EventAction(std::string sdName,TGeoManager * geoMgr,SteppingAction* SA) :
 EventID(0),nTrajectories(0) { 

  stpaction = (SteppingAction*)SA;

  detectorCollIDVect.resize(0);
  SDName = sdName;

  rootGeoMgr_ = geoMgr;
  if(rootGeoMgr_)World_ = rootGeoMgr_->GetVolume(0);
  else World_ = NULL;

}

EventAction::~EventAction() {
}

void EventAction::BeginOfEventAction(const G4Event* event) {
   /*
  HCE = NULL;
  DHC_vct.resize(0);
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  if(detectorCollIDVect.size()==0)
    {
      G4String colNam;
      detectorCollIDVect.push_back(SDman->GetCollectionID(colNam="tpcFidHitsCollection"));
      detectorCollIDVect.push_back(SDman->GetCollectionID(colNam="scintHitsCollection"));
    }

  //SDman->ListTree();
*/
}

void EventAction::EndOfEventAction(const G4Event* event) {

  //setup the navigator
  //theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

  //------------------------------------------------------------//
  //                           Hits Collection                  //
  //------------------------------------------------------------//

  //if(detectorCollID<0) return;
/*
  if(detectorCollIDVect.size()>0){
    HCE = event->GetHCofThisEvent();
  
    if(HCE){
	for(int k=0;k<detectorCollIDVect.size();k++){
	  if( detectorCollIDVect.at(k)>0 ) DHC_vct.push_back((DetectorHitsCollection*)(HCE->GetHC(detectorCollIDVect.at(k))));
	  else DHC_vct.push_back(NULL);
	}
    }
    
    for(int j=0;j<DHC_vct.size();j++){

      if(DHC_vct.at(j)==NULL) break;
      int n_hit = DHC_vct.at(j)->entries();
      
      G4double totE = 0;

      //loop over hits collection
      for(int i=0;i<n_hit;i++)
	{

       	//reset values of hitEdep
	HitEDep = 0;
       	HitChargedEDep = 0;	
	HitNeutronEDep = 0;
	HitGammaEDep = 0;

	DetectorHitsCollection* DHC = DHC_vct.at(j);

	HitEventID = EventID;
	HitPDG = (*DHC)[i]->getPDG();
	HitTrackID = (*DHC)[i]->getTrackID();
	  
	//for(int j=0;j<i;j++){
	//	if(HitTrackID==(*DHC)[j]->getTrackID()) HitFinalEnergy = (*DHC)[i]->getFinalEnergy();
	//}
	HitKinEnergy = (*DHC)[i]->getKinEnergy();
	HitPosX = ((*DHC)[i]->getPos()).X();
	HitPosY = ((*DHC)[i]->getPos()).Y();
	HitPosZ = ((*DHC)[i]->getPos()).Z();
	//HitRange = ((*DHC)[i]->getPos()).mag();

	//total energy deposited
	HitEDep = (*DHC)[i]->getEdep() / CLHEP::MeV;
	if( (HitPDG == 13) || (HitPDG == -13) 
		||(HitPDG == 11 ) || (HitPDG == -11) 
		||(HitPDG == 211 ) || (HitPDG == -211) 
		||(HitPDG == 2212) ){

		HitChargedEDep = (*DHC)[i]->getEdep() / CLHEP::MeV;
	}

	if(HitPDG == (2112))HitNeutronEDep = (*DHC)[i]->getEdep() / CLHEP::MeV;
	if( (HitPDG == 22) || (HitPDG == 111) )HitGammaEDep = (*DHC)[i]->getEdep() / CLHEP::MeV;

	  totE += (*DHC)[i]->getEdep();

	}
      
	//final energy from last hits in detector
	//loop over particles and then get last hit for that pdg
	//FinalTotalEnergy = (*DHC)[n_hit]->getFinalEnergy();
	//if(HitPDG == (13||-13)) FinalMuonEnergy = (*DHC)[n_hit]->getFinalEnergy();
	
      G4cout << "     Total energy deposition in detector : "
	     << totE / GeV << " (GeV)" << G4endl;

    }

  }
*/
  //set the stepping action
  if(stpaction){
  	stpaction->IncrementID();
  	stpaction->ResetValues();
  }

  //increment EventID
  EventID++;

}

