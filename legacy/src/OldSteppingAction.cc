#include "SteppingAction.hh"

SteppingAction::SteppingAction(std::string fluxOutputFile_,G4double cutEnergy_) 
: writeToFile(1),rootGeoMgr_(NULL),eventID(0),geantID(0),particleEnterCount(0),particleLeaveCount(0),prevPrimTrackID(-1){

  //set all variables to zero
  ResetValues();

  if(writeToFile==1)InitRootFile(fluxOutputFile_);

  //create cuts
  cuts = new ApplyCuts(cutEnergy_);
}

SteppingAction::SteppingAction(TGeoManager * geoMgr,std::string fluxOutputFile_,G4double cutEnergy_) 
: writeToFile(1),rootGeoMgr_(geoMgr),eventID(0),geantID(0),particleEnterCount(0),particleLeaveCount(0),prevPrimTrackID(-1){

  //set all variables to zero
  ResetValues();

  if(writeToFile==1)InitRootFile(fluxOutputFile_);

  //create cuts
  cuts = new ApplyCuts(cutEnergy_);
}

SteppingAction::~SteppingAction()
{
/*
  if(writeToFile==1){
	delete fROOTfile;
  	delete fROOTPrimaryTree;
	delete fROOTInFluxTree;
	delete fROOTOutFluxTree;
  }
*/
  fROOTfile->Close();
  delete cuts;
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

  //----cuts-----
  //Apply User defined cuts first
  if (cuts->process(theStep)) {
	theTrack->SetTrackStatus(fStopAndKill);
	return;
  }

  //if it is 113 pdg then this is rho particle and crashes geant- kill this particle
  if(theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 113){
	theTrack->SetTrackStatus(fStopAndKill);
	return;
  }

  if(!thePostPV) return; //out of world
  if(trackVolumeName == "OutOfWorld") return; //out of world
  if(trackNextVolumeName == "OutOfWorld") return; //out of world

  std::string rockVolumeName = "rockVolume";

  //get positions of steps
  preStepPos = thePrePoint->GetPosition();
  postStepPos = thePostPoint->GetPosition();
/*
  double preX = preStepPos.x()*(1/CLHEP::cm);
  double preY = preStepPos.y()*(1/CLHEP::cm);
  double preZ = preStepPos.z()*(1/CLHEP::cm);
*/
  double postX = postStepPos.x()*(1/CLHEP::cm);
  double postY = postStepPos.y()*(1/CLHEP::cm);
  double postZ = postStepPos.z()*(1/CLHEP::cm);

  //*******trajectory properties***********
  //set the primaries from the PGA or track info
  fPrimTrackID = (int)info->GetOriginalTrackID();

  //must be a primary track and not stepped over before
  if( (theTrack->GetParentID()==0) && (fPrimTrackID != prevPrimTrackID) && (info->GetOriginalPDG()!=0) ){
	prevTrajFinalKineticEnergy = (double)info->GetOriginalKineticEnergy()/MeV;
	fPrimEventID = eventID;
	fPrimGeantID = geantID;
	fPrimPDG = (int)info->GetOriginalPDG();
	fPrimEnergy = (double)info->GetOriginalEnergy();
	fPrimKinEnergy = (double)info->GetOriginalKineticEnergy();
	fPrimPx = (double)info->GetOriginalMomentum().getX();
	fPrimPy = (double)info->GetOriginalMomentum().getY();
	fPrimPz = (double)info->GetOriginalMomentum().getZ();
	fPrimPosX = (double)info->GetOriginalPosition().getX();
	fPrimPosY = (double)info->GetOriginalPosition().getY();
	fPrimPosZ = (double)info->GetOriginalPosition().getZ();

	//fill in tree
	if(writeToFile==1){
		fROOTfile->cd();
		fROOTPrimaryTree->Fill();	
 		//fROOTfile->Close();
	}
  }

  prevPrimTrackID = fPrimTrackID;

  //does the trajectory enter the target volume?
  // check if particle is just entering current volume
  // don't count volumes inside target volume
  if( (thePostPoint->GetStepStatus() == fGeomBoundary) &&
      (trackVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) &&
      (trackNextVolumeName.compare(0,targetVolName_.size(),targetVolName_)==0) &&
      (trackMotherVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) &&
      (trackNextMotherVolumeName.compare(0,targetVolName_.size(),targetVolName_)!=0) ){
		
	if(verbose_ == 2){
  		G4UImanager*  UIMgr_ = G4UImanager::GetUIpointer();
  		UIMgr_->ApplyCommand("/tracking/verbose 1");
	}

	particleEnterCount++;

    	//*******track properties***********
	fInEventID = eventID;
	fInGeantID = geantID;
	fInTrackID = theStep->GetTrack()->GetTrackID();
	fInPDG = theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
	//energy and mass
	G4double kinenergy =  thePostPoint->GetKineticEnergy();
  	kinenergy *= (1/ CLHEP::MeV);
  	G4double mass = theStep->GetTrack()->GetParticleDefinition()->GetPDGMass();
  	mass *= (1/ CLHEP::MeV);
  	//G4double energy =  (kinenergy / CLHEP::MeV) + (mass / CLHEP::MeV);
  	G4double energy =  kinenergy + mass;
	fInEnergy = energy;
	fInKEAtVertex = kinenergy;
  	//momentum
  	G4ThreeVector momDir = thePostPoint->GetMomentumDirection();
  	//scale to actual momentum values
  	G4double magMom = TMath::Sqrt(energy*energy - mass*mass);
  	fInMomX = magMom*momDir.x();
  	fInMomY = magMom*momDir.y();
  	fInMomZ = magMom*momDir.z();
	//position
	fInPosX = postX;
	fInPosY = postY;
	fInPosZ = postZ;

	if(verbose_>1){
		std::cout <<"\tParticle: " <<  fInPDG <<", entered target volume at: (" << fInPosX <<", " << fInPosY <<", " << fInPosZ <<") cm " <<std::endl;
	}

	fInNoParticle = particleEnterCount;

	//fill in tree
	if(writeToFile==1){
		fROOTfile->cd();
		fROOTInFluxTree->Fill();	
 		//fROOTfile->Close();
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

    	//*******track properties***********
	fOutEventID = eventID;
	fOutGeantID = geantID;
	fOutTrackID = theStep->GetTrack()->GetTrackID();
	fOutPDG = theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  	theParticleName = theStep->GetTrack()->GetParticleDefinition()->GetParticleName();
	//energy and mass
	G4double kinenergy =  thePostPoint->GetKineticEnergy();
  	kinenergy *= (1/ CLHEP::MeV);
  	G4double mass = theStep->GetTrack()->GetParticleDefinition()->GetPDGMass();
  	mass *= (1/ CLHEP::MeV);
  	//G4double energy =  (kinenergy / CLHEP::MeV) + (mass / CLHEP::MeV);
  	G4double energy =  kinenergy + mass;
	fOutEnergy = energy;
	fOutKEAtVertex = kinenergy;
  	//momentum
  	G4ThreeVector momDir = thePostPoint->GetMomentumDirection();
  	//scale to actual momentum values
  	G4double magMom = TMath::Sqrt(energy*energy - mass*mass);
  	fOutMomX = magMom*momDir.x();
  	fOutMomY = magMom*momDir.y();
  	fOutMomZ = magMom*momDir.z();
	//position
	fOutPosX = postX;
	fOutPosY = postY;
	fOutPosZ = postZ;

	fOutNoParticle = particleLeaveCount;

	//fill in tree
	if(writeToFile==1){
		fROOTfile->cd();
		fROOTOutFluxTree->Fill();		
 		//fROOTfile->Close();
	}
      
  	//show only boundary changes
  	if(verbose_ == 2){

       	 	std::cout <<"\nPreVolumeName: " << trackVolumeName
		   <<", PostVolumeName: " << trackNextVolumeName
		   //<<", TargetVolumeName: " << targetVolName_
                   << "\n\tParticle: " << theParticleName
                   << " -- Step: " << theStep->GetStepLength()/mm << " mm"
                   << " \n\tPrevious Kinetic Energy: "<< prevTrajFinalKineticEnergy/MeV  << " MeV"
                   << ", Final Kinetic Energy: "<< fOutKEAtVertex/MeV  << " MeV"
                   << ", Accumulated Energy Loss: "<< fInEnergy - fOutEnergy/MeV
                   << " MeV" 
		   << " \n\tMomentum: [MeV/c2](" << fOutMomX << "," << fOutMomY << "," << fOutMomZ << ")" 
		   << " \n\tProcess Name: "<< thePostProcessName
                   << " \n\t--steps: " << steps << std::endl;
  	} 
    		
	if(verbose_>1){
	  	std::cout <<"\tParticle: " <<  fOutPDG
	        	  <<", left target volume at: (" << fOutPosX
		  	  <<", " << fOutPosY<<", " << fOutPosZ <<") cm "
			   <<std::endl;
	}
  }
  	
  //---outputs---
  //all step output
  if(verbose_ == 4){
       	 	std::cout <<"\nPreVolumeName: " << trackVolumeName
		   <<", PostVolumeName: " << trackNextVolumeName
		   //<<", TargetVolumeName: " << targetVolName_
                   << "\n\tParticle: " << theParticleName
                   << " -- Step: " << theStep->GetStepLength()/mm << " mm"
                   << " \n\tPrevious Kinetic Energy: "<< prevTrajFinalKineticEnergy/MeV  << " MeV"
                   << ", Final Kinetic Energy: "<< fOutKEAtVertex/MeV  << " MeV"
                   << ", Accumulated Energy Loss: "<< fInEnergy - fOutEnergy/MeV
                   << " MeV" 
		   << " \n\tMomentum: [MeV/c2](" << fOutMomX << "," << fOutMomY << "," << fOutMomZ << ")" 
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
                   << " -- Step: " << theStep->GetStepLength()/mm << " mm"
                   << " \n\tPrevious Kinetic Energy: "<< prevTrajFinalKineticEnergy/MeV  << " MeV"
                   << ", Final Kinetic Energy: "<< fOutKEAtVertex/MeV  << " MeV"
                   << ", Accumulated Energy Loss: "<< fInEnergy - fOutEnergy/MeV
                   << " MeV" 
		   << " \n\tMomentum: [MeV/c2](" << fOutMomX << "," << fOutMomY << "," << fOutMomZ << ")" 
		   << " \n\tProcess Name: "<< thePostProcessName
                   << " \n\t--steps: " << steps << std::endl;
  } 

  if (steps>500000) {
        theTrack->SetTrackStatus(fStopAndKill);
	if(verbose_>0)std::cout <<"\n----Track Killed as too many steps! ";
        steps = 0;
        return;
    }


  ++steps;

}

void SteppingAction::InitRootFile(std::string rootfile){

  const int basket = 32000;

  //create the file
  fROOTfile = new TFile(rootfile.c_str(),"RECREATE");
  fROOTfile->cd();

  //create the trees
  fROOTPrimaryTree = new TTree("Primaries","Primary Tree");
  fROOTInFluxTree = new TTree("InFlux","InFlux Tree");
  fROOTOutFluxTree = new TTree("OutFlux","OutFlux Tree");
 
  //--------------------Primary Tree-------------------------
  fROOTPrimaryTree->Branch("fPrimEventID",&fPrimEventID,"fPrimEventID/I",basket);
  fROOTPrimaryTree->Branch("fPrimGeantID",&fPrimGeantID,"fPrimGeantID/I",basket);
  fROOTPrimaryTree->Branch("fPrimPDG",&fPrimPDG,"fPrimPDG/I", basket );
  fROOTPrimaryTree->Branch("fPrimEnergy",&fPrimEnergy,"fPrimEnergy/D", basket );
  fROOTPrimaryTree->Branch("fPrimKinEnergy",&fPrimKinEnergy,"fPrimKinEnergy/D", basket );
  fROOTPrimaryTree->Branch("fPrimPx",&fPrimPx,"fPrimPx/D", basket );
  fROOTPrimaryTree->Branch("fPrimPy",&fPrimPy,"fPrimPy/D", basket );
  fROOTPrimaryTree->Branch("fPrimPz",&fPrimPz,"fPrimPz/D", basket );
  fROOTPrimaryTree->Branch("fPrimPosX",&fPrimPosX,"fPrimPosX/D", basket );
  fROOTPrimaryTree->Branch("fPrimPosY",&fPrimPosY,"fPrimPosY/D", basket );
  fROOTPrimaryTree->Branch("fPrimPosZ",&fPrimPosZ,"fPrimPosZ/D", basket );

  //----------------------InFlux Tree-----------------------------
  fROOTInFluxTree->Branch("fInEventID",&fInEventID, "fInEventID/I",basket);
  fROOTInFluxTree->Branch("fInGeantID",&fInGeantID,"fInGeantID/I",basket);
  fROOTInFluxTree->Branch("fInTrackID",&fInTrackID,"fInTrackID/I",basket);
  fROOTInFluxTree->Branch("fInDetectorID",&fInDetectorID, "fInDetectorID/I",basket);
  fROOTInFluxTree->Branch("fInNoParticle",&fInNoParticle, "fInNoParticle/I",basket);
  fROOTInFluxTree->Branch("fInPDG",&fInPDG, "fInPDG/I",basket);
  fROOTInFluxTree->Branch("fInMomX",&fInMomX, "fInMomX/D",basket);
  fROOTInFluxTree->Branch("fInMomY",&fInMomY, "fInMomY/D",basket);
  fROOTInFluxTree->Branch("fInMomZ",&fInMomZ, "fInMomZ/D",basket);
  fROOTInFluxTree->Branch("fInEnergy",&fInEnergy, "fInEnergy/D",basket);
  fROOTInFluxTree->Branch("fInKEAtVertex",&fInKEAtVertex, "fInKEAtVertex/D",basket);
  fROOTInFluxTree->Branch("fInPosX",&fInPosX, "fInPosX/D",basket);
  fROOTInFluxTree->Branch("fInPosY",&fInPosY, "fInPosY/D",basket);
  fROOTInFluxTree->Branch("fInPosZ",&fInPosZ, "fInPosZ/D",basket);
  fROOTInFluxTree->Branch("fInSolAngA",&fInSolAngA,"fInSolAngA/D", basket );
  fROOTInFluxTree->Branch("fInSolAngB",&fInSolAngB,"fInSolAngB/D", basket );

  //----------------------OutFlux Tree-----------------------------
  fROOTOutFluxTree->Branch("fOutEventID",&fOutEventID, "fOutEventID/I",basket);
  fROOTOutFluxTree->Branch("fOutGeantID",&fOutGeantID,"fOutGeantID/I",basket);
  fROOTOutFluxTree->Branch("fOutTrackID",&fOutTrackID,"fOutTrackID/I",basket);
  fROOTOutFluxTree->Branch("fOutDetectorID",&fOutDetectorID, "fOutDetectorID/I",basket);
  fROOTOutFluxTree->Branch("fOutNoParticle",&fOutNoParticle, "fOutNoParticle/I",basket);
  fROOTOutFluxTree->Branch("fOutPDG",&fOutPDG, "fOutPDG/I",basket);
  fROOTOutFluxTree->Branch("fOutPrimScat",&fOutPrimScat, "fOutPrimScat/I",basket);
  fROOTOutFluxTree->Branch("fOutMomX",&fOutMomX, "fOutMomX/D",basket);
  fROOTOutFluxTree->Branch("fOutMomY",&fOutMomY, "fOutMomY/D",basket);
  fROOTOutFluxTree->Branch("fOutMomZ",&fOutMomZ, "fOutMomZ/D",basket);
  fROOTOutFluxTree->Branch("fOutEnergy",&fOutEnergy, "fOutEnergy/D",basket);
  fROOTOutFluxTree->Branch("fOutKEAtVertex",&fOutKEAtVertex, "fOutKEAtVertex/D",basket);
  fROOTOutFluxTree->Branch("fOutTotEDep",&fOutTotEDep, "fOutTotEDep/D",basket);
  fROOTOutFluxTree->Branch("fOutPosX",&fOutPosX, "fOutPosX/D",basket);
  fROOTOutFluxTree->Branch("fOutPosY",&fOutPosY, "fOutPosY/D",basket);
  fROOTOutFluxTree->Branch("fOutPosZ",&fOutPosZ, "fOutPosZ/D",basket);

  //fROOTfile->Close();

}

void SteppingAction::Write() {

 fROOTfile->cd();

 fROOTPrimaryTree->Write("", TObject::kOverwrite);
 fROOTInFluxTree->Write("", TObject::kOverwrite);
 fROOTOutFluxTree->Write("", TObject::kOverwrite);

 //fROOTfile->Close();

}

void SteppingAction::IncrementID(){

  eventID++;
  if(particleEnterCount>0)geantID++;

}

void SteppingAction::ResetValues(){

  interactionPointX = 0;
  interactionPointY = 0;
  interactionPointZ = 0;
  TrajTrackID = 0;
  TrajParentID = 0;
  TrajPDG = 0;
  TrajMomentumX = 0;
  TrajMomentumY = 0;
  TrajMomentumZ = 0;
  TrajMomentum = 0;
  TrajKineticEnergy = 0;
  TrajEnergy = 0;
  TrajInitialXPosition = 0;
  TrajInitialYPosition = 0;
  TrajInitialZPosition = 0;
  TrajFinalXPosition = 0;
  TrajFinalYPosition = 0;
  TrajFinalZPosition = 0;
  TrajFinalKineticEnergy = 0;
  prevTrajFinalKineticEnergy = 0;

  fPrimPDG = 0;
  fPrimTrackID = 0;
  prevPrimTrackID = -1;
  fPrimEnergy = 0;
  fPrimKinEnergy = 0;
  fPrimPx = 0;
  fPrimPy = 0;
  fPrimPz = 0;
  fPrimPosX = 0;
  fPrimPosY = 0;
  fPrimPosZ = 0;

  fInDetectorID = 0;
  fInNoParticle = 0;
  fInPDG = 0;
  fInTrackID = 0;
  fInPrimScat = 0;
  fInMomX = 0;
  fInMomY = 0;
  fInMomZ = 0;
  fInEnergy = 0;
  fInKEAtVertex = 0;
  fInTotEDep = 0;
  fInPosX = 0;
  fInPosY =0;
  fInPosZ = 0;
  fInSolAngA = 0;
  fInSolAngB = 0;

  fOutDetectorID = 0;
  fOutNoParticle = 0;
  fOutPDG = 0;
  fOutTrackID = 0;
  fOutPrimScat = 0;
  fOutMomX = 0;
  fOutMomY = 0;
  fOutMomZ = 0;
  fOutEnergy = 0;
  fOutKEAtVertex = 0;
  fOutTotEDep = 0;
  fOutPosX = 0;
  fOutPosY = 0;
  fOutPosZ = 0;

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
