//____________________________________________________________________________
/*!

\class    ScintillatorSensitiveDetector

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update April 2014

*/
//____________________________________________________________________________
#include "ScintillatorSensitiveDetector.hh"

ScintHitCollection * ScintillatorSensitiveDetector::ScintSDHitCollection = new ScintHitCollection();

ScintillatorSensitiveDetector::ScintillatorSensitiveDetector(const G4String& name,int layerID)
:G4VSensitiveDetector(name),HitsCollection_(NULL),layerNumber(layerID){

  std::stringstream ss;
  ss << layerNumber;
  G4String layerNoAsString = ss.str();

  SDBaseName = name;
  SDname = name+"_"+layerNoAsString;
  HCname = name + "HitsCollection"+"_"+layerNoAsString;

  //SDname = name;
  //HCname = hitsCollectionName;
  collectionName.insert(HCname);

  //set the layer ID for the hit
  SDHit_.setLayerNumber(layerNumber);
  //divide detector into 1 cm squares/cells
  positionResolution = 10.0*CLHEP::mm;
  //for the scintillator this is the minium amount of energy reposition that will be noticed
  // around 15 photoelectrons / 1.8 MeV
  energyDepLimit = 0.001*CLHEP::MeV;//1.8*CLHEP::MeV;
  //GasTPCDataLib definition of hits

  if(layerID==0){
  	std::cout << "\n-----------------------------------------------"
  	   	  << "\n " << name << " SensitiveDetector initializing ... "
            	  << "\n-----------------------------------------------";
  }
}

ScintillatorSensitiveDetector::~ScintillatorSensitiveDetector(){
}

//this is called every event
void ScintillatorSensitiveDetector::Initialize(G4HCofThisEvent*HCE) {

  // Create hits collection with name set by constructer - Geant Hits
  //Geant will automatically delete these hits
  //HitsCollection_ = new DetectorHitsCollection(SDname,HCname); 

  // Get the ID for this Hit Collection
  // and add this collection in hce
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
  //HCE->AddHitsCollection(HCID,HitsCollection_);

  //if(rootGeoMgr_!=NULL)World_ = rootGeoMgr_->GetVolume(0);
  //else World_ = NULL;

  NSteps_ = 0;
  Edep_ = 0.0;
  PDG_ = 0;
  ParentID_ = 0;
  TrackID_ = 0;
  kinEnergy_ = 0.;
  Pos_.SetXYZT(0.,0.,0.,0.);
  P4_.SetPxPyPzE(0.,0.,0.,0.);
  trackLeftVolume = false;
}

G4bool ScintillatorSensitiveDetector::ProcessHits(G4Step*aStep,G4TouchableHistory* touchHistory) {

  Edep_ = aStep->GetTotalEnergyDeposit();
  if(Edep_<energyDepLimit) return false;

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(thePrePoint->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

  //get the information of the original parent particle
  TrackInformation* trackInfo = (TrackInformation*)(aStep->GetTrack()->GetUserInformation());
  GeantBasicParticle parent;

  if(trackInfo){
  	parent.setTrackID(trackInfo->GetOriginalTrackID());
  	parent.setParentID(trackInfo->GetOriginalParentID());
  	parent.setPDG(trackInfo->GetOriginalPDG());
  	parent.setPosition( TLorentzVector(trackInfo->GetOriginalPosition().getX(),
				     	   trackInfo->GetOriginalPosition().getY(),
				     	   trackInfo->GetOriginalPosition().getZ(),
				     	   0.) );
  	parent.setP4( TLorentzVector(trackInfo->GetOriginalPosition().getX(),
			       	     trackInfo->GetOriginalPosition().getY(),
			       	     trackInfo->GetOriginalPosition().getZ(),
			       	     trackInfo->GetOriginalEnergy()) );
  }

  //declare hit types
  Hit_ = new DetectorHit();
  //Hit_ = new DetectorHit(thePrePV);

  PDG_ = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  TrackID_ = aStep->GetTrack()->GetTrackID();
  ParentID_ = aStep->GetTrack()->GetParentID();

  double postX = thePrePoint->GetPosition().x();//*(1/CLHEP::mm);
  double postY = thePrePoint->GetPosition().y();//*(1/CLHEP::mm);
  double postZ = thePrePoint->GetPosition().z();//*(1/CLHEP::mm);

  //set the position of the hit
  Pos_.SetXYZT(postX,postY,postZ, 0.);

  G4double dist = TMath::Sqrt( (postX*postX)+(postY*postY)+(postZ*postZ) );

  // !! add this part, to divide into cells or bars
  //if(dist<=positionResolution));

  //energy and mass
  kinEnergy_ = thePrePoint->GetKineticEnergy();
  kinEnergy_ *= (1/ CLHEP::MeV);
  G4double mass = aStep->GetTrack()->GetParticleDefinition()->GetPDGMass();
  mass *= (1/ CLHEP::MeV);
  //G4double energy =  (kinEnergy_ / CLHEP::MeV) + (mass / CLHEP::MeV);
  G4double energy =  kinEnergy_ + mass;
  //scale to actual momentum values
  G4double magMom = TMath::Sqrt(energy*energy - mass*mass);
  double momX = magMom*aStep->GetPreStepPoint()->GetMomentumDirection().x();
  double momY = magMom*aStep->GetPreStepPoint()->GetMomentumDirection().y();
  double momZ = magMom*aStep->GetPreStepPoint()->GetMomentumDirection().z();

  P4_.SetPxPyPzE(momX,momY,momZ,energy);

  //does the next step leave the geometry? 
  std::string trackVolumeName,trackNextVolumeName;
  //mother volume names
  std::string trackMotherVolumeName,trackNextMotherVolumeName; 
  G4VPhysicalVolume* 	      trackVolume 	= aStep->GetTrack()->GetVolume();
  if(trackVolume){
			      trackVolumeName	= trackVolume->GetName();
			      if(trackVolume->GetMotherLogical()){ 
				trackMotherVolumeName = trackVolume->GetMotherLogical()->GetName();
			      }
			      else trackMotherVolumeName = "null";
  }
  else {		      trackVolumeName 	= "null";
			      trackMotherVolumeName = "null";
  }

  G4VPhysicalVolume* 	      trackNextVolume 	= aStep->GetTrack()->GetNextVolume();
  if(trackNextVolume){
		              trackNextVolumeName = trackNextVolume->GetName();
			      if(trackNextVolume->GetMotherLogical()){ 
				trackNextMotherVolumeName = trackNextVolume->GetMotherLogical()->GetName();
			      }
			      else trackNextMotherVolumeName = "null";
  }
  else {
			      trackNextVolumeName = "null";
			      trackNextMotherVolumeName = "null";
  }

  trackLeftVolume = 0;
  if( (trackNextVolumeName.compare(0,SDBaseName.size(),SDBaseName)!=0) )trackLeftVolume = 1;
  
  //set the muon
  ParticleDescrRecord muon(PDG_,P4_,Pos_);

  if(PDG_ == 13){
    muon.setPDG(PDG_);
    muon.setPosition(Pos_); 
    muon.setP4(P4_);
    Hit_->setMuon(muon);
  }

  Hit_->setNSteps(NSteps_);
  Hit_->setEdep(Edep_);

  Hit_->setP4(P4_);
  Hit_->setPDG(PDG_);
  Hit_->setTrackID(TrackID_);
  Hit_->setParentID(ParentID_);
  Hit_->setKinEnergy(kinEnergy_);
  Hit_->setPos(Pos_);
  Hit_->setTrackLeftVolume(trackLeftVolume);
  Hit_->setParent(parent);
  
  Hit_->convertHit(&SDHit_);

  ScintSDHitCollection->push_back(SDHit_);
  //std::cout << "\nHit Collection address: " << HitsCollection_
//	     << "\nScintSD Hit Collection address: " << ScintSDHitCollection
//	      << "\n------------------------------" <<std::endl;
  //SDHit_.printToStream(std::cout);
  //HitsCollection_->insert(Hit_);

  //draw hit
  Hit_->Draw();

  delete Hit_;

  NSteps_++;
  return true;

}

void ScintillatorSensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE){
 
  //get the number of hits in the collection
  int nHit = 0;
  if(ScintSDHitCollection)nHit = ScintSDHitCollection->size();

  //std::cout << "\nNumber of Hits in the SD for this event is: " << nHit << std::endl;
  /*
  for ( G4int i=0; i<nHit; i++ ) {
	ScintSDHitCollection->at(i).printToStream(std::cout);
  }*/

}

void ScintillatorSensitiveDetector::clear()
{
  //free memory in vector - swap with temp empty vector is the only way to do this
  ScintHitCollection().swap(*ScintSDHitCollection);
  ScintSDHitCollection->clear();
  ScintSDHitCollection->resize(0);
  //delete ScintSDHitCollection;
} 
