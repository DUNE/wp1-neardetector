//____________________________________________________________________________
/*!

\class    SteppingAction

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "SteppingAction.hh"

SteppingAction::SteppingAction(G4double cutEnergy,bool trackPiZero) 
: rootGeoMgr_(NULL),eventID(0),geantID(0),particleEnterCount(0),particleLeaveCount(0),cutEnergy_(cutEnergy),trackPiZero_(trackPiZero){

  Initialise();
}

SteppingAction::SteppingAction(TGeoManager * geoMgr,G4double cutEnergy,bool trackPiZero) 
: rootGeoMgr_(geoMgr),eventID(0),geantID(0),particleEnterCount(0),particleLeaveCount(0),cutEnergy_(cutEnergy),trackPiZero_(trackPiZero){

  Initialise();
}

SteppingAction::~SteppingAction()
{
  delete cuts;
  delete primariesVtr;
  delete inParticleVtr;
  delete outParticleVtr;
}

void SteppingAction::Initialise(){

  //set all variables to zero
  ResetValues();

  //create cuts
  cuts = new ApplyCuts(cutEnergy_);

  //particle types
  primariesVtr = new std::vector<GeantPrimaryParticle>;
  inParticleVtr = new std::vector<GeantDaughterParticle>;
  outParticleVtr = new std::vector<GeantDaughterParticle>;

}

void SteppingAction::UserSteppingAction(const G4Step* theStep) { 
  
  G4Track*                    theTrack          = theStep->GetTrack();
  TrackInformation* 	      info		= (TrackInformation*)(theTrack->GetUserInformation());

  G4VPhysicalVolume* 	      trackVolume 	= theTrack->GetVolume();
  if(trackVolume){
			      trackVolumeName	= trackVolume->GetName();
			      if(trackVolume->GetMotherLogical()) trackMotherVolumeName = trackVolume->GetMotherLogical()->GetName();
			      else trackMotherVolumeName = "null";
  }
  else {		      trackVolumeName 	= "null";
			      trackMotherVolumeName = "null";
  }

  G4VPhysicalVolume* 	      trackNextVolume 	= theTrack->GetNextVolume();
  if(trackNextVolume){
		              trackNextVolumeName = trackNextVolume->GetName();
			      if(trackNextVolume->GetMotherLogical()) trackNextMotherVolumeName = trackNextVolume->GetMotherLogical()->GetName();
			      else trackNextMotherVolumeName = "null";
  }
  else {
			      trackNextVolumeName = "null";
			      trackNextMotherVolumeName = "null";
  }
 
  G4StepPoint*                thePrePoint       = theStep->GetPreStepPoint();
  G4VPhysicalVolume*          thePrePV          = thePrePoint->GetPhysicalVolume();

  G4StepPoint*                thePostPoint      = theStep->GetPostStepPoint();
  G4VPhysicalVolume*          thePostPV         = thePostPoint->GetPhysicalVolume();

  const G4VProcess*	      thePreProcess	= thePrePoint->GetProcessDefinedStep();
  std::string 		      thePreProcessName;
  if(thePreProcess)           thePreProcessName  = thePreProcess->GetProcessName();
  else 			      thePreProcessName = "";

  const G4VProcess*	      thePostProcess	 = thePostPoint->GetProcessDefinedStep();
  std::string 		      thePostProcessName;
  if(thePostProcess)          thePostProcessName = thePostProcess->GetProcessName();
  else 			      thePostProcessName = "";

/// DO NOT APPLY CUTS IF WANT FULL TRACKING HISTORY OF PARTICLES ///
  //----cuts-----
  //Apply User defined cuts first
  if (cuts->process(theStep)) {
	theTrack->SetTrackStatus(fStopAndKill);
	return;
  }

/* 
  //if it is 113 pdg then this is rho particle and crashes geant- kill this particle
  if(theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 113){
	theTrack->SetTrackStatus(fStopAndKill);
	return;
  }

  if(!thePostPV) return; //out of world
  if(trackVolumeName == "OutOfWorld") return; //out of world
  if(trackNextVolumeName == "OutOfWorld") return; //out of world
*/

  std::string rockVolumeName = "rockVolume";

  //get positions of steps
  preStepPos = thePrePoint->GetPosition();
  postStepPos = thePostPoint->GetPosition();

  preX = preStepPos.x()*(1./CLHEP::mm);
  preY = preStepPos.y()*(1./CLHEP::mm);
  preZ = preStepPos.z()*(1./CLHEP::mm);

  postX = postStepPos.x()*(1./CLHEP::mm);
  postY = postStepPos.y()*(1./CLHEP::mm);
  postZ = postStepPos.z()*(1./CLHEP::mm);

  // --- Truth variables and kinematics --- //
  trackID = theTrack->GetTrackID();
  parentID = theTrack->GetParentID();
  pdg = theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  std::string theParticleName = theStep->GetTrack()->GetParticleDefinition()->GetParticleName();

  //energy and mass
  kinenergy =  thePrePoint->GetKineticEnergy();
  kinenergy *= (1./ CLHEP::MeV);
  mass = theStep->GetTrack()->GetParticleDefinition()->GetPDGMass();
  mass *= (1./ CLHEP::MeV);
  //G4double energy =  (kinenergy / CLHEP::MeV) + (mass / CLHEP::MeV);
  energy =  kinenergy + mass;
  //momentum
  momDir = thePrePoint->GetMomentumDirection();
  //scale to actual momentum values
  magMom = TMath::Sqrt(energy*energy - mass*mass);

  //set to GeantParticle object
  GeantBasicParticle * particle = new GeantBasicParticle(trackID,parentID,pdg);
  particle->setP4( TLorentzVector(magMom*momDir.x(),magMom*momDir.y(),magMom*momDir.z(),energy) );

  //must be a primary track and not stepped over before
  if( (theTrack->GetParentID()==0) && (particleSet(primariesVtr,trackID)==false) ){
	GeantPrimaryParticle prim;
	this->setParticleKinematics(prim);

	//add to the primary vector
	primariesVtr->push_back(prim);
  }

  //particle->printToStream(std::cout);
  bool isDescendant = false;
  
  if(trackPiZero_)isDescendant = findParent(particle,info);

  //is it a daughter of a primary - ultimatly all should be descendants
  if( isDescendant && verbose_>4 ){
	std::cout << "\n --- Daughter found! --- " <<std::endl;
	particle->printToStream(std::cout);
  }

  delete particle;

  //does the trajectory enter the target volume?
  // check if particle is just entering current volume
  // don't count volumes inside target volume
  if( (thePostPoint->GetStepStatus() == fGeomBoundary) &&
      (trackVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) &&
      (trackNextVolumeName.compare(0,targetVolName_.size(),targetVolName_)==0) &&
      (trackMotherVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) &&
      (trackNextMotherVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) ){
	
	particleEnterCount++;

	//set the details from where this particle came from - primary particle details
	GeantParticle primary;
	this->setOriginalKinematics(primary,info);	

	//set the particle details
	GeantDaughterParticle inParticle;
	this->setParticleKinematics(inParticle);
	inParticle.setParent(primary);

	inParticleVtr->push_back(inParticle);

	if(verbose_>1){
		std::cout <<"\tParticle: " <<  pdg <<", entered target volume at: (" << postX <<", " << postY <<", " << postZ <<") mm " <<std::endl;
		std::cout <<"\tPrimary: " <<  primary.getPDG() <<", origin at: (" << primary.getPosition().X() <<", " 
				<< primary.getPosition().Y() <<", " << primary.getPosition().Z() <<") mm " <<std::endl;
	}

	if(verbose_ == 2){
  		G4UImanager*  UIMgr_ = G4UImanager::GetUIpointer();
  		UIMgr_->ApplyCommand("/tracking/verbose 1");
	}
  }

 //does the trajectory leave the target volume?
  // don't count volumes inside target volume
  if( (thePostPoint->GetStepStatus() == fGeomBoundary)  &&
      (trackVolumeName.compare(0,targetVolName_.size(),targetVolName_)==0) &&
      (trackNextVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) &&
      (trackMotherVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) &&
      (trackNextMotherVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) ){

	particleLeaveCount++;  

	//set the details from where this particle came from - primary particle details
	GeantParticle primary;
	this->setOriginalKinematics(primary,info);	

	//set the particle details
	GeantDaughterParticle outParticle;
	this->setParticleKinematics(outParticle);
	outParticle.setParent(primary);

	outParticleVtr->push_back(outParticle);    

	if(verbose_>1){
	  	std::cout <<"\tParticle: " <<  pdg
	        	  <<", left target volume at: (" << postX
		  	  <<", " << postY<<", " << postZ <<") mm "
			   <<std::endl;
	}
  	//show only boundary changes
  	if(verbose_ == 2){

       	 	std::cout <<"\nPreVolumeName: " << trackVolumeName
		   <<", PostVolumeName: " << trackNextVolumeName
		   //<<", TargetVolumeName: " << targetVolName_
                   << "\n\tParticle: " << theParticleName
                   << " -- Step: " << theStep->GetStepLength()/CLHEP::mm << " mm"
                   << " \n\t Kinetic Energy: "<< kinenergy/CLHEP::MeV  << " MeV"
		   << " \n\tMomentum: [MeV/c2](" << magMom*momDir.x()<< "," << magMom*momDir.y() << "," << magMom*momDir.z()<< ")" 
		   << " \n\tProcess Name: "<< thePostProcessName
                   << " \n\t--steps: " << steps << std::endl;
  	} 
  }
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// ---outputs--- ///
  //all step output
  if(verbose_ == 4){
       	 	std::cout <<"\nPreVolumeName: " << trackVolumeName
		   <<", PostVolumeName: " << trackNextVolumeName
		   //<<", TargetVolumeName: " << targetVolName_
                   << "\n\tParticle: " << theParticleName
                   << " -- Step: " << theStep->GetStepLength()/CLHEP::mm << " mm"
                   << " \n\tKinetic Energy: "<< kinenergy/CLHEP::MeV  << " MeV"
		   << " \n\tMomentum: [MeV/c2](" << magMom*momDir.x()<< "," << magMom*momDir.y() << "," << magMom*momDir.z()<< ")" 
		   << " \n\tProcess Name: "<< thePostProcessName
                   << " \n\t--steps: " << steps << std::endl;
  } 

  //show only boundary changes
  if(verbose_==5 && (thePostPoint->GetStepStatus() == fGeomBoundary)){
  		G4UImanager*  UIMgr_ = G4UImanager::GetUIpointer();
  		UIMgr_->ApplyCommand("/tracking/verbose 1");

       	 	std::cout <<"\nPreVolumeName: " << trackVolumeName
		   <<", PostVolumeName: " << trackNextVolumeName
		   //<<", TargetVolumeName: " << targetVolName_
                   << "\n\tParticle: " << theParticleName
                   << " -- Step: " << theStep->GetStepLength()/CLHEP::mm << " mm"
                   << " \n\t Kinetic Energy: "<< kinenergy/CLHEP::MeV  << " MeV"
		   << " \n\tMomentum: [MeV/c2](" << magMom*momDir.x()<< "," << magMom*momDir.y() << "," << magMom*momDir.z()<< ")" 
		   << " \n\tProcess Name: "<< thePostProcessName
                   << " \n\t--steps: " << steps << std::endl;
  } 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (steps>500000) {
        theTrack->SetTrackStatus(fStopAndKill);
	if(verbose_>0)std::cout <<"\n----Track Killed as too many steps! ";
        steps = 0;
        return;
    }
 
  ++steps;
}

bool SteppingAction::particleSet(std::vector<GeantPrimaryParticle> * particleVtr,int trackid){

  bool result = false;

  for(int i=0;i<particleVtr->size();i++){
	if(particleVtr->at(i).getTrackID() == trackid)result= true;
  }

  return result;
}

bool SteppingAction::findParent(GeantBasicParticle * p,TrackInformation * trackInfo){

  //Tracker tracker;

  //get the track and parent ids for the daughter
  int daughterTrackID = p->getTrackID();
  int daughterParentID = p->getParentID();

  //if it is primary then cannot be a daughter
  if(daughterParentID==0)return false;

  //only concerned with pi0s, photons and neutrons
   if(trackInfo->GetOriginalPDG()!=111) return false;//||
//      trackInfo->GetOriginalPDG()!=22||
//      trackInfo->GetOriginalPDG()!=2112)return false;

  //loop over primaries 
  for(int i=0;i<primariesVtr->size();i++){
	int primaryTrackID = primariesVtr->at(i).getTrackID();

// 	std::vector<GeantBasicParticle> * descendants = primariesVtr->at(i).getDaughterVtrPtr();
// 	int numberOfDaughters = descendants->size();
// 	bool IsParticleDescendant = tracker.IsDescendant(primaryTrackID,p,*descendants);

 	if( (trackInfo->GetOriginalTrackID() == primaryTrackID) ){//|| IsParticleDescendant){
	  primariesVtr->at(i).addDaughter(*p);	
	  //p.printToStream(std::cout);
	  return true;  
	}
  }

  return false;

/*
  bool trackExists = false;
  bool isDescendant = false;

  //loop over primaries daughters to check if track exists already
  // Primaries first
  for(int i=0;i<primariesVtr->size();i++){
	int numberOfDaughters = primariesVtr->at(i).getDaughterVtr().size();
	
	if( (numberOfDaughters==0) && (primariesVtr->at(i).getTrackID()==daughterParentID) ){
	  primariesVtr->at(i).addDaughter(p);	  
	  return true;
	}

	//then daughters
	for(int j=0;j<numberOfDaughters;j++){

	  //get the daughter in vector
	  GeantBasicParticle *daughterInVtr = &primariesVtr->at(i).getDaughterVtr().at(j);
	  int trackIDInVtr = daughterInVtr->getTrackID();

	  //if a matching trackID is already found in the vector
	  // it must already be a daughter and the first kinematics are already recorded
	  if( trackIDInVtr == daughterTrackID ){
		//trackExists = true;
		return false;
	  }

	  if( trackIDInVtr == daughterParentID || primariesVtr->at(i).getTrackID()==daughterParentID ){
		isDescendant = true;
	  }
	}

	//if it matches the parent id of the particle to be identified
	// then it must be a daughter of the primary
	if( isDescendant==true ){
		
		//p.printToStream(std::cout);
		//primariesVtr->at(i).addDaughter(p);
		//do pi0s only
		if(primariesVtr->at(i).getPDG()==111)primariesVtr->at(i).addDaughter(p);  
		return true;
	}	
  }

  return false;
*/
/*  
  //loop over primaries daughters to check if track exists already
  // Primaries first
  for(int i=0;i<primariesVtr->size();i++){
	std::vector<GeantBasicParticle> descendants = primariesVtr->at(i).getDaughterVtr();
	int numberOfDaughters = descendants.size();
	
	if( numberOfDaughters==0 ){
	  if(primariesVtr->at(i).getTrackID()==daughterParentID) {
	  	primariesVtr->at(i).addDaughter(p);	  
	  	return true;
	  }
	  //else continue;
	}

	//then daughters
	bool IsParticleDescendant = this->IsDescendant(primariesVtr->at(i).getTrackID(),p,descendants);
	if( IsParticleDescendant || primariesVtr->at(i).getTrackID()==daughterParentID ){
	  primariesVtr->at(i).addDaughter(p);  
	  return true;
	}
  }

  return false;
 */
}

void SteppingAction::setParticleKinematics(GeantParticle &p){
	p.setTrackID(trackID);
	p.setEventID(eventID);
	p.setParentID(parentID);
	p.setStepNumber(steps);
	p.setMass(mass);
	p.setPDG(pdg);
	p.setPosition( TLorentzVector(preX,preY,preZ,0.) );
	p.setP4( TLorentzVector(magMom*momDir.x(),magMom*momDir.y(),magMom*momDir.z(),energy) );
}

void SteppingAction::setOriginalKinematics(GeantParticle &p,TrackInformation * trackInfo){

  if(!trackInfo)return;

  p.setTrackID(trackInfo->GetOriginalTrackID());
  p.setEventID(eventID);
  p.setParentID(trackInfo->GetOriginalParentID());
  p.setStepNumber(0);
  p.setMass(trackInfo->GetOriginalMass());
  p.setPDG(trackInfo->GetOriginalPDG());
  p.setPosition( TLorentzVector(trackInfo->GetOriginalPosition().getX(),
				trackInfo->GetOriginalPosition().getY(),
				trackInfo->GetOriginalPosition().getZ(),
				0.) );
  p.setP4( TLorentzVector(trackInfo->GetOriginalPosition().getX(),
			trackInfo->GetOriginalPosition().getY(),
			trackInfo->GetOriginalPosition().getZ(),
			trackInfo->GetOriginalEnergy()) );
}

void SteppingAction::IncrementID(){

  eventID++;
  if(particleEnterCount>0)geantID++;

}

void SteppingAction::ClearTrackVtrs(){
  primariesVtr->resize(0);
  inParticleVtr->resize(0);
  outParticleVtr->resize(0);  
}

void SteppingAction::ResetValues(){

  //particleEnterCount = 0;
  //particleLeaveCount = 0;
  steps = 0;
  condition = 0;
}

void SteppingAction::Print() {

 std::cout << "\nNumber of particles reaching " << targetVolName_ << ": " << particleEnterCount 
	   << "\nNumber of particles leaving " << targetVolName_ 
	   << ": "  << particleLeaveCount << std::endl;

}
