//____________________________________________________________________________
/*!

\class    TrackingAction

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2013

*/
//____________________________________________________________________________
#include "TrackingAction.hh"

TrackingAction::TrackingAction() {
}

TrackingAction::~TrackingAction() {
} 

void TrackingAction::PreUserTrackingAction(const G4Track* track) {
  
  //record information on the primary
  if(track->GetParentID()==0){
        TrackInformation* info = new TrackInformation(track);
        G4Track* theTrack = (G4Track*)track;
        theTrack->SetUserInformation(info);
  }

  //store all trajectories 
  fpTrackingManager->SetStoreTrajectory(true);

  //preX = track->GetPosition().x();
  //preY = track->GetPosition().y();
  //preZ = track->GetPosition().z();

}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {

  //postX = track->GetPosition().x();
  //postY = track->GetPosition().y();
  //postZ = track->GetPosition().z();

  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
  {
    TrackInformation* anInfo = (TrackInformation*)(track->GetUserInformation());
    size_t nSeco = secondaries->size();

    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        TrackInformation* infoNew = new TrackInformation(anInfo);
        (*secondaries)[i]->SetUserInformation(infoNew);
	fpTrackingManager->SetStoreTrajectory(true);
	 
      }
    }
  }
}
