//____________________________________________________________________________
/*!

\class    DetectorHit

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "DetectorHit.hh"

G4Allocator<DetectorHit> DetectorHitAllocator;

DetectorHit::DetectorHit() : G4VHit(), edep(0),nSteps(0),PDG(0), pos(TLorentzVector()){
	kinEnergy =  0.;
	trackLeftVolume = 0;
}

DetectorHit::DetectorHit(G4VPhysicalVolume* pVol) : edep(0),nSteps(0),PDG(0), pos(TLorentzVector()), prePhysVolume(pVol){
	kinEnergy =  0.;
	trackLeftVolume = 0;
}


DetectorHit::~DetectorHit()
{;}

DetectorHit::DetectorHit(const DetectorHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
  PDG = right.PDG;
  TrackID = right.TrackID;
  kinEnergy = right.kinEnergy;
  P4 = right.P4;
  trackLeftVolume = right.trackLeftVolume;
  parent = right.parent;
}

const DetectorHit& DetectorHit::operator=(const DetectorHit &right)
{
  edep = right.edep;
  pos = right.pos;
  PDG = right.PDG;
  TrackID = right.TrackID;
  kinEnergy = right.kinEnergy;
  P4 = right.P4;
  trackLeftVolume = right.trackLeftVolume;
  parent = right.parent;
  return *this;
}

G4int DetectorHit::operator==(const DetectorHit &right) const
{
  return (this==&right) ? 1 : 0;
}

std::map<G4String,G4AttDef> DetectorHit::fAttDefs;

void DetectorHit::Draw()
{

  pos3vec.setX(pos.X());
  pos3vec.setY(pos.Y());
  pos3vec.setZ(pos.Z());

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos3vec);
    circle.SetScreenSize(3.0);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1,0.0,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }

}

void DetectorHit::Draw(G4Colour colour)
{

  pos3vec.setX(pos.X());
  pos3vec.setY(pos.Y());
  pos3vec.setZ(pos.Z());

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos3vec);
    circle.SetScreenSize(3.0);
    circle.SetFillStyle(G4Circle::filled);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }

}

const std::map<G4String,G4AttDef>* DetectorHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] =
      G4AttDef("HitType","Type of hit","Physics","","G4String");
  }
  return &fAttDefs;
}

std::vector<G4AttValue>* DetectorHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back
    (G4AttValue("HitType","DetectorHit",""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

void DetectorHit::Print(){
  G4cout
     << "Edep: " << edep*(1/CLHEP::GeV)
     << " Position: (" << pos.X()*(1/CLHEP::m) << ", " << pos.Y()*(1/CLHEP::m) << ", " << pos.Z()*(1/CLHEP::m) << ") "
     << " PDG: " << PDG << " TrackID: " << TrackID
     << G4endl;

}

void DetectorHit::convertHit(SDHit * sdHit){

 sdHit->setEdep(edep*(1/CLHEP::GeV));	// set to GeV as default units
 sdHit->setNSteps(nSteps);
 sdHit->setPosition(pos);
 sdHit->setP4(P4);
 sdHit->setPDG(PDG);
 sdHit->setTrackID(TrackID);
 sdHit->setParentID(ParentID);
 sdHit->setKinEnergy(kinEnergy);
 sdHit->setTrackLeftVolume(trackLeftVolume);
 sdHit->setParent(parent);

}


