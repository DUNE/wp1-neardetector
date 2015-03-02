#include "SteppingAction.hh"

SteppingAction::SteppingAction(TGeoManager * geoMgr,std::string fluxOutputFile_) 
: posZcount(0),negZcount(0), posYcount(0),negYcount(0),posXcount(0),negXcount(0){

  rootGeoMgr_ = geoMgr;

  //root file
  G4String rootfile = fluxOutputFile_;
  rootFile = new TFile(rootfile.c_str(),"RECREATE");
  posZPlaneTree = new TTree("posZPlane","Tree of posZTrajectories");
  negZPlaneTree = new TTree("negZPlane","Tree of negZTrajectories");
  posYPlaneTree = new TTree("posYPlane","Tree of posYTrajectories");
  negYPlaneTree = new TTree("negYPlane","Tree of negYTrajectories");
  posXPlaneTree = new TTree("posXPlane","Tree of posXTrajectories");
  negXPlaneTree = new TTree("negXPlane","Tree of negXTrajectories");
  
  posZPlaneTree->Branch("TrajTrackID",&TrajTrackID,"TrajTrackID/I");
  posZPlaneTree->Branch("TrajParentID",&TrajParentID,"TrajParentID/I");
  posZPlaneTree->Branch("TrajPDG",&TrajPDG,"TrajPDG/I");
  posZPlaneTree->Branch("TrajMomentumX",&TrajMomentumX,"TrajMomentumX/D");
  posZPlaneTree->Branch("TrajMomentumY",&TrajMomentumY,"TrajMomentumY/D");
  posZPlaneTree->Branch("TrajMomentumZ",&TrajMomentumZ,"TrajMomentumZ/D");
  posZPlaneTree->Branch("TrajMomentum",&TrajMomentum,"TrajMomentum/D");
  posZPlaneTree->Branch("TrajKineticEnergy",&TrajKineticEnergy,"TrajKineticEnergy/D");
  posZPlaneTree->Branch("TrajEnergy",&TrajEnergy,"TrajEnergy/D");
  posZPlaneTree->Branch("TrajInitialXPosition",&TrajInitialXPosition,"TrajInitialXPosition/D");
  posZPlaneTree->Branch("TrajInitialYPosition",&TrajInitialYPosition,"TrajInitialYPosition/D");
  posZPlaneTree->Branch("TrajInitialZPosition",&TrajInitialZPosition,"TrajInitialZPosition/D");
  posZPlaneTree->Branch("TrajFinalXPosition",&TrajFinalXPosition,"TrajFinalXPosition/D");
  posZPlaneTree->Branch("TrajFinalYPosition",&TrajFinalYPosition,"TrajFinalYPosition/D");
  posZPlaneTree->Branch("TrajFinalZPosition",&TrajFinalZPosition,"TrajFinalZPosition/D");

  negZPlaneTree->Branch("TrajTrackID",&TrajTrackID,"TrajTrackID/I");
  negZPlaneTree->Branch("TrajParentID",&TrajParentID,"TrajParentID/I");
  negZPlaneTree->Branch("TrajPDG",&TrajPDG,"TrajPDG/I");
  negZPlaneTree->Branch("TrajMomentumX",&TrajMomentumX,"TrajMomentumX/D");
  negZPlaneTree->Branch("TrajMomentumY",&TrajMomentumY,"TrajMomentumY/D");
  negZPlaneTree->Branch("TrajMomentumZ",&TrajMomentumZ,"TrajMomentumZ/D");
  negZPlaneTree->Branch("TrajMomentum",&TrajMomentum,"TrajMomentum/D");
  negZPlaneTree->Branch("TrajKineticEnergy",&TrajKineticEnergy,"TrajKineticEnergy/D");
  negZPlaneTree->Branch("TrajEnergy",&TrajEnergy,"TrajEnergy/D");
  negZPlaneTree->Branch("TrajInitialXPosition",&TrajInitialXPosition,"TrajInitialXPosition/D");
  negZPlaneTree->Branch("TrajInitialYPosition",&TrajInitialYPosition,"TrajInitialYPosition/D");
  negZPlaneTree->Branch("TrajInitialZPosition",&TrajInitialZPosition,"TrajInitialZPosition/D");
  negZPlaneTree->Branch("TrajFinalXPosition",&TrajFinalXPosition,"TrajFinalXPosition/D");
  negZPlaneTree->Branch("TrajFinalYPosition",&TrajFinalYPosition,"TrajFinalYPosition/D");
  negZPlaneTree->Branch("TrajFinalZPosition",&TrajFinalZPosition,"TrajFinalZPosition/D");

  posYPlaneTree->Branch("TrajTrackID",&TrajTrackID,"TrajTrackID/I");
  posYPlaneTree->Branch("TrajParentID",&TrajParentID,"TrajParentID/I");
  posYPlaneTree->Branch("TrajPDG",&TrajPDG,"TrajPDG/I");
  posYPlaneTree->Branch("TrajMomentumX",&TrajMomentumX,"TrajMomentumX/D");
  posYPlaneTree->Branch("TrajMomentumY",&TrajMomentumY,"TrajMomentumY/D");
  posYPlaneTree->Branch("TrajMomentumZ",&TrajMomentumZ,"TrajMomentumZ/D");
  posYPlaneTree->Branch("TrajMomentum",&TrajMomentum,"TrajMomentum/D");
  posYPlaneTree->Branch("TrajKineticEnergy",&TrajKineticEnergy,"TrajKineticEnergy/D");
  posYPlaneTree->Branch("TrajEnergy",&TrajEnergy,"TrajEnergy/D");
  posYPlaneTree->Branch("TrajInitialXPosition",&TrajInitialXPosition,"TrajInitialXPosition/D");
  posYPlaneTree->Branch("TrajInitialYPosition",&TrajInitialYPosition,"TrajInitialYPosition/D");
  posYPlaneTree->Branch("TrajInitialZPosition",&TrajInitialZPosition,"TrajInitialZPosition/D");
  posYPlaneTree->Branch("TrajFinalXPosition",&TrajFinalXPosition,"TrajFinalXPosition/D");
  posYPlaneTree->Branch("TrajFinalYPosition",&TrajFinalYPosition,"TrajFinalYPosition/D");
  posYPlaneTree->Branch("TrajFinalZPosition",&TrajFinalZPosition,"TrajFinalZPosition/D");

  negYPlaneTree->Branch("TrajTrackID",&TrajTrackID,"TrajTrackID/I");
  negYPlaneTree->Branch("TrajParentID",&TrajParentID,"TrajParentID/I");
  negYPlaneTree->Branch("TrajPDG",&TrajPDG,"TrajPDG/I");
  negYPlaneTree->Branch("TrajMomentumX",&TrajMomentumX,"TrajMomentumX/D");
  negYPlaneTree->Branch("TrajMomentumY",&TrajMomentumY,"TrajMomentumY/D");
  negYPlaneTree->Branch("TrajMomentumZ",&TrajMomentumZ,"TrajMomentumZ/D");
  negYPlaneTree->Branch("TrajMomentum",&TrajMomentum,"TrajMomentum/D");
  negYPlaneTree->Branch("TrajKineticEnergy",&TrajKineticEnergy,"TrajKineticEnergy/D");
  negYPlaneTree->Branch("TrajEnergy",&TrajEnergy,"TrajEnergy/D");
  negYPlaneTree->Branch("TrajInitialXPosition",&TrajInitialXPosition,"TrajInitialXPosition/D");
  negYPlaneTree->Branch("TrajInitialYPosition",&TrajInitialYPosition,"TrajInitialYPosition/D");
  negYPlaneTree->Branch("TrajInitialZPosition",&TrajInitialZPosition,"TrajInitialZPosition/D");
  negYPlaneTree->Branch("TrajFinalXPosition",&TrajFinalXPosition,"TrajFinalXPosition/D");
  negYPlaneTree->Branch("TrajFinalYPosition",&TrajFinalYPosition,"TrajFinalYPosition/D");
  negYPlaneTree->Branch("TrajFinalZPosition",&TrajFinalZPosition,"TrajFinalZPosition/D");

  posXPlaneTree->Branch("TrajTrackID",&TrajTrackID,"TrajTrackID/I");
  posXPlaneTree->Branch("TrajParentID",&TrajParentID,"TrajParentID/I");
  posXPlaneTree->Branch("TrajPDG",&TrajPDG,"TrajPDG/I");
  posXPlaneTree->Branch("TrajMomentumX",&TrajMomentumX,"TrajMomentumX/D");
  posXPlaneTree->Branch("TrajMomentumY",&TrajMomentumY,"TrajMomentumY/D");
  posXPlaneTree->Branch("TrajMomentumZ",&TrajMomentumZ,"TrajMomentumZ/D");
  posXPlaneTree->Branch("TrajMomentum",&TrajMomentum,"TrajMomentum/D");
  posXPlaneTree->Branch("TrajKineticEnergy",&TrajKineticEnergy,"TrajKineticEnergy/D");
  posXPlaneTree->Branch("TrajEnergy",&TrajEnergy,"TrajEnergy/D");
  posXPlaneTree->Branch("TrajInitialXPosition",&TrajInitialXPosition,"TrajInitialXPosition/D");
  posXPlaneTree->Branch("TrajInitialYPosition",&TrajInitialYPosition,"TrajInitialYPosition/D");
  posXPlaneTree->Branch("TrajInitialZPosition",&TrajInitialZPosition,"TrajInitialZPosition/D");
  posXPlaneTree->Branch("TrajFinalXPosition",&TrajFinalXPosition,"TrajFinalXPosition/D");
  posXPlaneTree->Branch("TrajFinalYPosition",&TrajFinalYPosition,"TrajFinalYPosition/D");
  posXPlaneTree->Branch("TrajFinalZPosition",&TrajFinalZPosition,"TrajFinalZPosition/D");

  negXPlaneTree->Branch("TrajTrackID",&TrajTrackID,"TrajTrackID/I");
  negXPlaneTree->Branch("TrajParentID",&TrajParentID,"TrajParentID/I");
  negXPlaneTree->Branch("TrajPDG",&TrajPDG,"TrajPDG/I");
  negXPlaneTree->Branch("TrajMomentumX",&TrajMomentumX,"TrajMomentumX/D");
  negXPlaneTree->Branch("TrajMomentumY",&TrajMomentumY,"TrajMomentumY/D");
  negXPlaneTree->Branch("TrajMomentumZ",&TrajMomentumZ,"TrajMomentumZ/D");
  negXPlaneTree->Branch("TrajMomentum",&TrajMomentum,"TrajMomentum/D");
  negXPlaneTree->Branch("TrajKineticEnergy",&TrajKineticEnergy,"TrajKineticEnergy/D");
  negXPlaneTree->Branch("TrajEnergy",&TrajEnergy,"TrajEnergy/D");
  negXPlaneTree->Branch("TrajInitialXPosition",&TrajInitialXPosition,"TrajInitialXPosition/D");
  negXPlaneTree->Branch("TrajInitialYPosition",&TrajInitialYPosition,"TrajInitialYPosition/D");
  negXPlaneTree->Branch("TrajInitialZPosition",&TrajInitialZPosition,"TrajInitialZPosition/D");
  negXPlaneTree->Branch("TrajFinalXPosition",&TrajFinalXPosition,"TrajFinalXPosition/D");
  negXPlaneTree->Branch("TrajFinalYPosition",&TrajFinalYPosition,"TrajFinalYPosition/D");
  negXPlaneTree->Branch("TrajFinalZPosition",&TrajFinalZPosition,"TrajFinalZPosition/D");

}

SteppingAction::~SteppingAction()
{
  //rootFile->cd();
  //negZPlaneTree->Write();
  //rootFile->Close();

  delete rootFile;
  delete posZPlaneTree;
  delete negZPlaneTree;
  delete posYPlaneTree;
  delete negYPlaneTree;
  delete posXPlaneTree;
  delete negXPlaneTree;
  //delete rootGeoMgr_;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep) { 

  G4Track* track = aStep->GetTrack();

  int steps = 0;

  //only store tracks that leave the set boundary
  preStepPos = aStep->GetPreStepPoint()->GetPosition();
  postStepPos = aStep->GetPostStepPoint()->GetPosition();

  double preX = preStepPos.x()*(1/CLHEP::mm);
  double preY = preStepPos.y()*(1/CLHEP::mm);
  double preZ = preStepPos.z()*(1/CLHEP::mm);

  double postX = postStepPos.x()*(1/CLHEP::mm);
  double postY = postStepPos.y()*(1/CLHEP::mm);
  double postZ = postStepPos.z()*(1/CLHEP::mm);

  rootGeoMgr_->SetCurrentPoint (preX,preY,preZ);

  rootGeoMgr_->FindNode();
  preVol_ = rootGeoMgr_->GetCurrentVolume();

  rootGeoMgr_->SetCurrentPoint (postX,postY,postZ);

  rootGeoMgr_->FindNode();
  postVol_ = rootGeoMgr_->GetCurrentVolume();
  
  G4String theParticleName = track->GetDefinition()->GetParticleName();
  G4String theCurrentVolumeName = aStep->GetPreStepPoint()->
                                             GetPhysicalVolume()->GetName();
  
  //do you want to track particle in the world volume i.e rock?
  //if the target volume is not top or rockVolume then ignore rock
  //if(targetVolName_ != "rockVolume" && 
  //if(theCurrentVolumeName == "rockVolume_PV"){
  //    track->SetTrackStatus(fStopAndKill);
  //    return;
  //}

  if(verbose_>0){
	std::cout <<"    " << theCurrentVolumeName
                   << ": " << theParticleName
                   << " -- Step: " << aStep->GetStepLength()/mm << " mm"
                   << " Energy Loss: "<< aStep->GetDeltaEnergy()/MeV 
                   << " MeV" << std::endl;
  }

  if (aStep->GetTrack()->GetTrackLength() > 100*m) {
      track->SetTrackStatus(fStopAndKill);
      steps = 0;
      return;
    }

  ++steps;

  if (theParticleName == "e-") {
        if (track->GetKineticEnergy() < 100.*keV){
        	track->SetTrackStatus(fStopAndKill);
		return;
	}
  }

  if (steps>50000) {
        track->SetTrackStatus(fStopAndKill);
        steps = 0;
        return;
    }

  if (track->GetTrackStatus()==fStopAndKill) {
        steps = 0;
        return;
    }

//if(preVolume->GetLogicalVolume()->GetName()== SDName.c_str() 
	//&& postVolume->GetLogicalVolume()->GetName()!=SDName.c_str()

  //std::cout << "****\nPositions****"
  //	    << "\n\tPre: (" << preX << ", "<< preY << ", " << preZ << ") "
  //	    << "\n\tPost: (" << postX << ", "<< postY << ", " << postZ << ") " << std::endl;

  //if( (preX == postX) && (preY == postY) && (preZ == postZ) )track->SetTrackStatus(fStopAndKill);
/*
  //don't track showering
  if( (track->GetDefinition()->GetPDGEncoding() == (22 || 11 || -11)) 
	&& track->GetTotalEnergy()  < 10.0 * MeV) {

   //track->SetTrackStatus(fStopButAlive);
   track->SetTrackStatus(fStopAndKill);
  }
*/
  //std::cout << "\n---PreStepPos = ( " << preStepPos.x() << ", " 
//		<< preStepPos.y() << ", "<< preStepPos.z() << ") ";
  //std::cout << "\n---PostStepPos = ( " << postStepPos.x() << ", " 
//		<< postStepPos.y() << ", "<< postStepPos.z() << ") " <<std::endl;

  //if(preVolume->GetLogicalVolume()->GetName()== SDName.c_str() 
	//&& postVolume->GetLogicalVolume()->GetName()!=SDName.c_str()

//std::cout << "\n\nPDG code is: " << trj->GetPDGEncoding();
//std::cout << "\nInitial kinetic energy is : " <<  trj->GetInitialKineticEnergy();
G4ThreeVector momDir = aStep->GetPreStepPoint()->GetMomentumDirection();
G4double kinenergy =  aStep->GetPreStepPoint()->GetKineticEnergy();
G4double mass = aStep->GetTrack()->GetParticleDefinition()->GetPDGMass();
G4double energy =  (kinenergy / CLHEP::MeV) + (mass / CLHEP::MeV);
TrajTrackID = aStep->GetTrack()->GetTrackID();
TrajParentID = aStep->GetTrack()->GetParentID();
TrajPDG = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
	      
//momentum and energy
TrajMomentumX = momDir.x();
TrajMomentumY = momDir.y();
TrajMomentumZ = momDir.z();
//TrajMomentum = std::sqrt(momDir.x()*momDir.x() 
//			+ momDir.y()*momDir.y() 
//			+ momDir.z()*momDir.z());
	     		
TrajKineticEnergy = kinenergy / CLHEP::MeV;
TrajEnergy = energy;
TrajInitialXPosition = preStepPos.x();
TrajInitialYPosition = preStepPos.y();
TrajInitialZPosition = preStepPos.z();
		      
TrajFinalXPosition = postStepPos.x();
TrajFinalYPosition = postStepPos.y();
TrajFinalZPosition = postStepPos.z();
		      
  //******************Positive Z plane*************************//
  if( preStepPos.x() / CLHEP::m <= 1.2 &&  preStepPos.x() / CLHEP::m >= -1.2
	&& preStepPos.y() / CLHEP::m <= 1.2 &&  preStepPos.y() / CLHEP::m >= -1.2
	&& preStepPos.z() / CLHEP::m <= 3.0 
	&& postStepPos.z() / CLHEP::m >= 3.0 ){

		posZPlaneTree->Fill();
		posZcount++;

	}	
	      
  //******************Negative Z plane*************************//
  if( preStepPos.x() / CLHEP::m <= 1.2 &&  preStepPos.x() / CLHEP::m >= -1.2
	&& preStepPos.y() / CLHEP::m <= 1.2 &&  preStepPos.y() / CLHEP::m >= -1.2
	&& preStepPos.z() / CLHEP::m >= 0.0 
	&& postStepPos.z() / CLHEP::m <= 0.0 ){

		negZPlaneTree->Fill();
		negZcount++;

	}		      
  //******************Positive Y plane*************************//
  if( preStepPos.x() / CLHEP::m <= 1.2 &&  preStepPos.x() / CLHEP::m >= -1.2
	&& preStepPos.y() / CLHEP::m <= 1.2 &&  postStepPos.y() / CLHEP::m >= 1.2
	&& preStepPos.z() / CLHEP::m >= 0 
	&& preStepPos.z() / CLHEP::m <= 3.0 ){

		posYPlaneTree->Fill();
		posYcount++;

	}	
  //******************Negative Y plane*************************//
  if( preStepPos.x() / CLHEP::m <= 1.2 &&  preStepPos.x() / CLHEP::m >= -1.2
	&& preStepPos.y() / CLHEP::m >= -1.2 &&  postStepPos.y() / CLHEP::m <= -1.2
	&& preStepPos.z() / CLHEP::m >= 0 
	&& preStepPos.z() / CLHEP::m <= 3.0 ){

		negYPlaneTree->Fill();
		negYcount++;

	}     		      
  //******************Positive X plane*************************//
  if( preStepPos.x() / CLHEP::m <= 1.2 &&  postStepPos.x() / CLHEP::m >= 1.2
	&& preStepPos.y() / CLHEP::m >= -1.2 &&  preStepPos.y() / CLHEP::m <= 1.2
	&& preStepPos.z() / CLHEP::m >= 0 
	&& preStepPos.z() / CLHEP::m <= 3.0 ){

		posXPlaneTree->Fill();
		posXcount++;
	}     		      
  //******************Negative X plane*************************//
  if( preStepPos.x() / CLHEP::m >= -1.2 &&  postStepPos.x() / CLHEP::m <= -1.2
	&& preStepPos.y() / CLHEP::m >= -1.2 &&  preStepPos.y() / CLHEP::m <= 1.2
	&& preStepPos.z() / CLHEP::m >= 0 
	&& preStepPos.z() / CLHEP::m <= 3.0 ){

		negXPlaneTree->Fill();
		negXcount++;
	}

/*
	World_ = rootGeoMgr_->GetVolume(0);


  	G4Track* track = aStep->GetTrack();
  	int trackID = track->GetTrackID();

  	//int parentID = track->GetParentID();
  	int pdg = track->GetDefinition()->GetPDGEncoding();


	G4ThreeVector preStep3Pos( aStep->GetPreStepPoint()->GetPosition().x(),
                          aStep->GetPreStepPoint()->GetPosition().y(),
                          aStep->GetPreStepPoint()->GetPosition().z());


	G4ThreeVector postStep3Pos( aStep->GetPostStepPoint()->GetPosition().x(),
                          aStep->GetPostStepPoint()->GetPosition().y(),
                          aStep->GetPostStepPoint()->GetPosition().z());

	TLorentzVector preStepPos( aStep->GetPreStepPoint()->GetPosition().x(),
                          aStep->GetPreStepPoint()->GetPosition().y(),
                          aStep->GetPreStepPoint()->GetPosition().z(), 0.);

	TLorentzVector postStepPos( aStep->GetPostStepPoint()->GetPosition().x(),
                          aStep->GetPostStepPoint()->GetPosition().y(),
                          aStep->GetPostStepPoint()->GetPosition().z(), 0.);

	TLorentzVector p4( track->GetMomentum().x(),
                   track->GetMomentum().y(),
                   track->GetMomentum().z(),
-                  track->GetTotalEnergy() );

 	//particles
 	//ParticleDescrRecord muon;
 	//ParticleDescrRecord fss;

 	//find where it hit in the geometry
 	rootGeoMgr_->SetCurrentPoint (preStep3Pos.x()*(1/CLHEP::mm),
					preStep3Pos.y()*(1/CLHEP::mm) ,
					preStep3Pos.z()*(1/CLHEP::mm));
 	rootGeoMgr_->FindNode();
  	preVol_ = rootGeoMgr_->GetCurrentVolume();
 	std::string preVol_name = preVol_->GetName();

 	 rootGeoMgr_->SetCurrentPoint (postStep3Pos.x()*(1/CLHEP::mm),
					postStep3Pos.y()*(1/CLHEP::mm), 
					postStep3Pos.z()*(1/CLHEP::mm));
  	rootGeoMgr_->FindNode();
  	postVol_ = rootGeoMgr_->GetCurrentVolume();
  	std::string postVol_name = postVol_->GetName();

       //if(postVol_name != "tpcFidMainVol") track->SetTrackStatus(fStopAndKill);

	if(preVol_){
		std::cout << "\n********************************"
	  		<< "\n PreVolume name is: "
	  		<< preVol_name
	  		<< "\n point (x,y,z) is: ("
	  		<< preStep3Pos.x()*(1/CLHEP::m) << ", " 
			<< preStep3Pos.y()*(1/CLHEP::m) << ", "
			<< preStep3Pos.z()*(1/CLHEP::m) << ")" 
		  	<< "\n********************************\n";
	}
	if(postVol_){
		std::cout << "\n********************************"
	  		<< "\n PostVolume name is: "
			<< postVol_name
	  		<< "\n point (x,y,z) is: ("
	  		<< postStep3Pos.x()*(1/CLHEP::m) << ", " 
			<< postStep3Pos.y()*(1/CLHEP::m) << ", "
			<< postStep3Pos.z()*(1/CLHEP::m) << ")" 
	  		<< "\n********************************\n";

	if( preVol_name == "tpcFidMainVol" && postVol_name != "tpcFidMainVol") {

		//set position
      		preStepPos.SetX(aStep->GetPreStepPoint()->GetPosition().x());
      		preStepPos.SetY(aStep->GetPreStepPoint()->GetPosition().y());
      		preStepPos.SetZ(aStep->GetPreStepPoint()->GetPosition().z());

      		//momentum and energy
      		p4.SetPx(track->GetMomentum().x());
      		p4.SetPy(track->GetMomentum().y());
      		p4.SetPz(track->GetMomentum().z());
      		p4.SetE(track->GetTotalEnergy());

  		if(track->GetDefinition()->GetPDGEncoding() == (13 || -13)) {
    		muon.setPosition(preStepPos);
    		muon.setP4(p4);
    		muon.setPDG( track->GetDefinition()->GetPDGEncoding() );
    		hit_->setMuon(muon);
  		}	
		
    		else if( track->GetDefinition()->GetPDGEncoding() < 1000000){
     		fss.setPosition(preStepPos);
     		fss.setP4(p4);
     		fss.setPDG( track->GetParticleDefinition()->GetPDGEncoding() );
     		hit_->addFss(fss);
  		}
   //hit_->setPosition(pos);

		std::cout << "\n*******************************************************"
		<< "\nEnergy of track with PDG " << fss.getPDG() 
		<< " is: " << track->GetTotalEnergy()*(1/CLHEP::GeV) 
		<< " GeV" << "\n*******************************************************";
      }

  //don't track showering
  if( (track->GetDefinition()->GetPDGEncoding() == (22 || 11 || -11)) 
	&& track->GetTotalEnergy()  < 100 * MeV) {

   track->SetTrackStatus(fStopButAlive);
  }

  bool isPrimaryLepton = (trackID == 1);
  double time = aStep->GetPreStepPoint()->GetGlobalTime();
  G4ThreeVector mom = aStep->GetPreStepPoint()->GetMomentum();
  double E = aStep->GetPreStepPoint()->GetTotalEnergy();
  G4ThreeVector pos1 = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector pos2 = aStep->GetPostStepPoint()->GetPosition();

  if (trackID == 1 && track->GetVolume()->GetName() != "Top_PV") {
    //std::cout << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << std::endl;
    std::cout << aStep << " ";
    std::cout << "Current: " << track->GetVolume()->GetName()  << " " << pos1.z();
    if (track->GetNextVolume())
      std::cout << "  Next: " << track->GetNextVolume()->GetName() << " " << pos2.z() << std::endl;
  }

  if (track->GetNextVolume()->GetName() == "Top_PV") {
    if (isPrimaryLepton && pdg == 13) {
      hasHit_ = true;
      TLorentzVector p4(mom.x(), mom.y(), mom.z(), E);
      TLorentzVector pos(pos1.x(), pos1.y(), pos1.z(), time);
      hit_->setPosition(pos);
      ParticleDescrShortRecord muon(pdg, p4);

      if(hit_)
        hit_->setMuon(muon);

      //hit_->printToStream(std::cout);
    }
    track->SetTrackStatus(fStopAndKill);
  }
*/

  
}

void SteppingAction::Write() {

 rootFile->cd();
 posZPlaneTree->Write("", TObject::kOverwrite); 
 negZPlaneTree->Write("", TObject::kOverwrite); 
 posYPlaneTree->Write("", TObject::kOverwrite); 
 negYPlaneTree->Write("", TObject::kOverwrite); 
 posXPlaneTree->Write("", TObject::kOverwrite); 
 negXPlaneTree->Write("", TObject::kOverwrite); 

}

void SteppingAction::Print() {

 std::cout << "\n\nNumber of particles out of posZPlane is: " << posZcount;
 std::cout << "\nNumber of particles out of negZPlane is: " << negZcount;
 std::cout << "\nNumber of particles out of posYPlane is: " << posYcount;
 std::cout << "\nNumber of particles out of negYPlane is: " << negYcount;
 std::cout << "\nNumber of particles out of posXPlane is: " << posXcount;
 std::cout << "\nNumber of particles out of negXPlane is: " << negXcount << std::endl;

}
