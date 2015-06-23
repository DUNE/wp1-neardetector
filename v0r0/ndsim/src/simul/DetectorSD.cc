//____________________________________________________________________________
/*!

\class    DetectorSD

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update April 2015
*/
//____________________________________________________________________________
#include "DetectorSD.hh"
#include <G4EnergyLossTables.hh>
#include <G4ParticleTable.hh>

DetectorSD::DetectorSD(const G4String name)
:G4VSensitiveDetector(name),HitsCollection_(NULL){

  SDname = name;
  HCname = name + "HitsCollection";
  collectionName.insert(HCname);
  std::cout << "\n-----------------------------------------------"
  	    << "\n " << name << " SensitiveDetector initializing ... "
            << "\n-----------------------------------------------";

  rootGeoMgr_ = NULL;
  World_ = NULL;

  //GasTPCDataLib definition of hits
  SDHitCollection = new HitCollection();
}

DetectorSD::DetectorSD(const G4String name,const G4String &hitsCollectionName)
:G4VSensitiveDetector(name),HitsCollection_(NULL){

  SDname = name;
  HCname = hitsCollectionName;
  collectionName.insert(HCname);

  rootGeoMgr_ = NULL;
  World_ = NULL;
  SDHitCollection = NULL;
}

DetectorSD::DetectorSD(const G4String name,TGeoManager * geoMgr)
:G4VSensitiveDetector(name), HitsCollection_(NULL){

  SDname = name;
  rootGeoMgr_ = geoMgr;
  HCname="detectorCollection";
  collectionName.insert(HCname);
  std::cout << "\n-----------------------------------------------"
  	    << "\n " << name << " SensitiveDetector initializing ... "
            << "\n-----------------------------------------------";

  //GasTPCDataLib definition of hits
  SDHitCollection = new HitCollection();
}

DetectorSD::~DetectorSD(){

  delete SDHitCollection;
}

//this is called every event
void DetectorSD::Initialize(G4HCofThisEvent*HCE) {

  // Create hits collection with name set by constructer - Geant Hits
  //Geant will automatically delete these hits
  HitsCollection_ = new DetectorHitsCollection(SDname,HCname); 

  // Get the ID for this Hit Collection
  // and add this collection in hce
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
  HCE->AddHitsCollection(HCID,HitsCollection_);

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
  charge_ = 0;
}

G4bool DetectorSD::ProcessHits(G4Step*aStep,G4TouchableHistory*) {

  Edep_ = aStep->GetTotalEnergyDeposit();
  if(Edep_==0.) return false;

  //declare hit types
  Hit_ = new DetectorHit();

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  //G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

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
    parent.setP4( TLorentzVector(trackInfo->GetOriginalMomentum().getX(),
				 trackInfo->GetOriginalMomentum().getY(),
				 trackInfo->GetOriginalMomentum().getZ(),
				 trackInfo->GetOriginalEnergy()) );
  }
  
  PDG_ = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  TrackID_ = aStep->GetTrack()->GetTrackID();
  ParentID_ = aStep->GetTrack()->GetParentID();

  double postX = thePrePoint->GetPosition().x();//*(1/CLHEP::mm);
  double postY = thePrePoint->GetPosition().y();//*(1/CLHEP::mm);
  double postZ = thePrePoint->GetPosition().z();//*(1/CLHEP::mm);

  //set the position of the hit
  Pos_.SetXYZT(postX,postY,postZ, 0.);

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

  charge_ = aStep->GetTrack()->GetParticleDefinition()->GetPDGCharge();

  //does the next step leave the geometry? 
  std::string trackVolumeName,trackNextVolumeName;
  //mother volume names
  std::string trackMotherVolumeName,trackNextMotherVolumeName; 
  G4VPhysicalVolume* 	      trackVolume 	= aStep->GetTrack()->GetVolume();
  /*
  G4Material *                material          = trackVolume->GetLogicalVolume ()->GetMaterial();

  // Get the dedx tables for mu,pion,proton,electron,kaon
  G4ParticleDefinition* part = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  G4double M    = part->GetPDGMass()/CLHEP::GeV;
  G4double p    = magMom/CLHEP::GeV;
  G4double tk   = sqrt(magMom*magMom + M*M) - M;
  G4double dedx = G4EnergyLossTables::GetDEDX(part, tk, material)/(CLHEP::MeV/CLHEP::cm);
  */
  if(trackVolume){
    trackVolumeName	= trackVolume->GetName();
    if(trackVolume->GetMotherLogical()) trackMotherVolumeName = trackVolume->GetMotherLogical()->GetName();
    else trackMotherVolumeName = "null";
  }
  else {
    trackVolumeName 	= "null";
    trackMotherVolumeName = "null";
  }

  G4VPhysicalVolume* 	      trackNextVolume 	= aStep->GetTrack()->GetNextVolume();
  if(trackNextVolume){
    trackNextVolumeName = trackNextVolume->GetName();
    if(trackNextVolume->GetMotherLogical()) trackNextMotherVolumeName = trackNextVolume->GetMotherLogical()->GetName();
    else trackNextMotherVolumeName = "null";
  }
  else {
    trackNextVolumeName = "null";
    trackNextMotherVolumeName = "null";
  }

  trackLeftVolume = 0;
  if( (trackNextVolumeName.compare(0,SDname.size(),SDname)!=0) )trackLeftVolume = 1;
/*
  std::cout <<"\nTrackID: " << TrackID_ << ", PDG: " << PDG_
  	   << ", PreVolumeName: " << trackVolumeName
  	   <<", PostVolumeName: " << trackNextVolumeName << ", left volume?" << trackLeftVolume <<std::endl;

  //loop over the hits in the collection
  for(int i=0;i<nHit;i++)
  {
    Hit_ = (*HitsCollection_)[i];
    G4ThreeVector pos(Hit_->getPos().X(),Hit_->getPos().Y(),Hit_->getPos().Z());
    G4double dist2 = sqr(pos.x()-Pos_.X())
                    +sqr(pos.y()-Pos_.Y())+sqr(pos.z()-Pos_.Z());
    if(dist2<=sqr(positionResolution))
    	Hit_->addEdep(Edep_);
    	return true;
  }
*/
  double momentum = sqrt(momX*momX + momY*momY + momZ*momZ); // MeV
  if(momentum < 2.0) return false;
  /* // george
  //set the muon
  ParticleDescrRecord muon(PDG_,P4_,Pos_);
  
  if(PDG_ == 13){
    muon.setPDG(PDG_);
    muon.setPosition(Pos_); 
    muon.setP4(P4_);
    Hit_->setMuon(muon);
  }
  */
  //std::cout << PDG_ << " , TrackID = " << TrackID_ << " , momentum = " << momentum << std::endl;
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
  Hit_->setCharge(charge_);
  
  Hit_->convertHit(&SDHit_);
  
  //check the memory - if it keeps allocating new memory then it is a memory leak
  //std::cout << "\n SDHit memory address: " << &SDHit_<< ", SDHitCollection memory address: " << SDHitCollection
  //	    << "\n Hit memory address: " << Hit_<< ", HitsCollection memory address: " << HitsCollection_;

  if(SDHitCollection)
    SDHitCollection->push_back(SDHit_);
  HitsCollection_->insert(Hit_);

  //draw hit
  Hit_->Draw();

  NSteps_++;
  return true;

}

void DetectorSD::EndOfEvent(G4HCofThisEvent* HCE){
 
  //get the number of hits in the collection
  //int nHit = 0;
  //if(SDHitCollection)
  //nHit = SDHitCollection->size();

  //std::cout << "\nNumber of Hits in the SD for this event is: " << nHit << std::endl;
/*
  for ( G4int i=0; i<nHit; i++ ) {
	SDHitCollection->at(i).printToStream(std::cout);
  }
*/
}

void DetectorSD::clear()
{
  if(SDHitCollection){
    //free memory in vector - swap with temp empty vector is the only way to do this
    HitCollection().swap(*SDHitCollection);
    SDHitCollection->clear();
    SDHitCollection->resize(0);
  }
} 

