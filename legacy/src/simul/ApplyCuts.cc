//____________________________________________________________________________
/*!

\class    ApplyCuts

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#include "ApplyCuts.hh"

ApplyCuts::ApplyCuts(G4double cutEnergy) : set(false){

  step_ = NULL;
  track_ = NULL;
  //initDataCards();
  //loadDataCards();
  
  cutEnergy_ = cutEnergy;
}

ApplyCuts::~ApplyCuts(){
}

bool ApplyCuts::process(const G4Step* aStep){
 
 step_ = aStep;
 track_ = step_->GetTrack();

 //energy cut
 if(track_->GetKineticEnergy() < cutEnergy_) set = true;
 else set = false;

 return set;

}

bool ApplyCuts::process(const G4Track* aTrack){
 
 step_ = track_->GetStep();
 track_ = aTrack;

  //energy cut
 if(track_->GetKineticEnergy() < cutEnergy_) set = true;
 else set = false;

 return set;

}
