//____________________________________________________________________________
/*!

\class    ScintillatorSensitiveDetector

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
         University of Liverpool

\created  Sep 2012
\last update Nov 2015
*/
//____________________________________________________________________________
#include "ScintillatorSensitiveDetector.hh"

ScintHitCollection * ScintillatorSensitiveDetector::ScintSDHitCollection = new ScintHitCollection();

ScintillatorSensitiveDetector::ScintillatorSensitiveDetector(const G4String name)
:G4VSensitiveDetector(name),HitsCollection_(NULL){

  SDname = name;
  HCname = name + "HitsCollection";
  collectionName.insert(HCname);

  //divide detector into 1 cm squares/cells
  //positionResolution = 10.0*CLHEP::mm;
  //for the scintillator this is the minium amount of energy reposition that will be noticed
  // around 15 photoelectrons / 1.8 MeV
  //energyDepLimit = 0.001*CLHEP::MeV;//1.8*CLHEP::MeV;

  std::cout << "\n-----------------------------------------------"
	    << "\n " << name << "Ecal SensitiveDetector initializing ... "
	    << "\n-----------------------------------------------";

  //GasTPCDataLib definition of hits
  ScintSDHitCollection = new ScintHitCollection();
}

ScintillatorSensitiveDetector::ScintillatorSensitiveDetector(const G4String name,const G4String &hitsCollectionName)
:G4VSensitiveDetector(name),HitsCollection_(NULL){

  SDname = name;
  HCname = hitsCollectionName;
  collectionName.insert(HCname);

  std::cout << "\n-----------------------------------------------"
	    << "\n " << name << "Ecal SensitiveDetector initializing ... "
	    << "\n-----------------------------------------------";

  //GasTPCDataLib definition of hits
  ScintSDHitCollection = new ScintHitCollection();
}

ScintillatorSensitiveDetector::~ScintillatorSensitiveDetector(){
  if(ScintSDHitCollection)
    delete ScintSDHitCollection;
}

void ScintillatorSensitiveDetector::Initialize(G4HCofThisEvent*HCE) {

  // Create hits collection with name set by constructer - Geant Hits
  //Geant will automatically delete these hits
  HitsCollection_ = new DetectorHitsCollection(SDname,HCname);

  // Get the ID for this Hit Collection
  // and add this collection in hce
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
  HCE->AddHitsCollection(HCID,HitsCollection_);

  NSteps_ = 0;
  Edep_ = 0.0;
  PDG_ = 0;
  ParentID_ = 0;
  TrackID_ = 0;
  kinEnergy_ = 0.;
  Pos_.SetXYZT(0.,0.,0.,0.);
  P4_.SetPxPyPzE(0.,0.,0.,0.);
  charge_ = 0;
  trackLeftVolume = false;
}

G4bool ScintillatorSensitiveDetector::ProcessHits(G4Step *aStep , G4TouchableHistory*) {

  Edep_ = aStep->GetTotalEnergyDeposit();
  if(Edep_==0.) return false;

  //declare hit types
  Hit_ = new DetectorHit();
  //Hit_ = new DetectorHit(thePrePV);

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  //G4TouchableHistory* theTouchable = (G4TouchableHistory*)(thePrePoint->GetTouchable());
  //G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();
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
				       trackInfo->GetOriginalTime()) );
    parent.setP4( TLorentzVector(trackInfo->GetOriginalPosition().getX(),
				 trackInfo->GetOriginalPosition().getY(),
				 trackInfo->GetOriginalPosition().getZ(),
				 trackInfo->GetOriginalEnergy()) );
  }
  
  PDG_ = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  TrackID_ = aStep->GetTrack()->GetTrackID();
  ParentID_ = aStep->GetTrack()->GetParentID();
  
  Pos_.SetXYZT(thePrePoint->GetPosition().x(),thePrePoint->GetPosition().y(),thePrePoint->GetPosition().z(), thePrePoint->GetGlobalTime());
 
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
  G4VPhysicalVolume*  trackVolume  = aStep->GetTrack()->GetVolume();
  if(trackVolume){
    trackVolumeName	= trackVolume->GetName();
    if(trackVolume->GetMotherLogical()){ 
      trackMotherVolumeName = trackVolume->GetMotherLogical()->GetName();
    }
    else trackMotherVolumeName = "null";
  }
  else{
    trackVolumeName 	= "null";
    trackMotherVolumeName = "null";
  }
  
  G4VPhysicalVolume* trackNextVolume = aStep->GetTrack()->GetNextVolume();
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

  if( (trackNextVolumeName.compare(0,SDname.size(),SDname)!=0) )
    trackLeftVolume = true;

  int detector = -1;
  std::string sdetector = trackVolume->GetName();
  if(sdetector.find("ScintZ1") != std::string::npos)
    detector = 1;
  else if(sdetector.find("ScintZ2") != std::string::npos)
    detector = 2;
  else if(sdetector.find("ScintY1") != std::string::npos)
    detector = 3;
  else if(sdetector.find("ScintY2") != std::string::npos)
    detector = 4;
  else if(sdetector.find("ScintX1") != std::string::npos)
    detector = 5;
  else if(sdetector.find("ScintX2") != std::string::npos)
    detector = 6;

  int layer = -1;
  if(sdetector.find("Scint") != std::string::npos){
    std::string str2 = sdetector.substr(12,2);
    if(str2.find("V") != std::string::npos)
      str2 = sdetector.substr(12,1);
    layer = atoi(str2.c_str()); 
  }

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

  SDHit_.setEcalNumber(detector);
  SDHit_.setLayerNumber(layer);

  if(ScintSDHitCollection)
    ScintSDHitCollection->push_back(SDHit_);
 
  //SDHit_.printToStream(std::cout);
  HitsCollection_->insert(Hit_);

  //draw hit
  Hit_->Draw();

  NSteps_++;
  return true;

}

void ScintillatorSensitiveDetector::EndOfEvent(G4HCofThisEvent* /*HCE*/){
 
  //get the number of hits in the collection
  //int nHit = 0;
  //if(ScintSDHitCollection)nHit = ScintSDHitCollection->size();
}

void ScintillatorSensitiveDetector::clear()
{
  if(ScintSDHitCollection){
    //free memory in vector - swap with temp empty vector is the only way to do this
    ScintHitCollection().swap(*ScintSDHitCollection);
    ScintSDHitCollection->clear();
    ScintSDHitCollection->resize(0);
  }
} 
