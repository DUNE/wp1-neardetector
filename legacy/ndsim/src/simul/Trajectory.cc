//____________________________________________________________________________
/*!

\class    Trajectory

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2013

*/
//____________________________________________________________________________
#include "Trajectory.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"

G4Allocator<Trajectory> myTrajectoryAllocator;

Trajectory::Trajectory()
{
   fpParticleDefinition = 0;
   ParticleName = "";
   PDGCharge = 0;
   PDGEncoding = 0;
   fTrackID = 0;
   fParentID = 0;
   positionRecord = 0;
   momentum = G4ThreeVector(0.,0.,0.);
   vertexPosition = G4ThreeVector(0.,0.,0.);
   globalTime = 0.;
   kineticEnergy = 0.;
}

Trajectory::Trajectory(const G4Track* aTrack)
{
   fpParticleDefinition = aTrack->GetDefinition();
   ParticleName = fpParticleDefinition->GetParticleName();
   PDGCharge = fpParticleDefinition->GetPDGCharge();
   PDGEncoding = fpParticleDefinition->GetPDGEncoding();
   fTrackID = aTrack->GetTrackID();
   fParentID = aTrack->GetParentID();
   positionRecord = new TrajectoryPointContainer();
   positionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
   momentum = aTrack->GetMomentum();
   vertexPosition = aTrack->GetPosition();
   globalTime = aTrack->GetGlobalTime();
   kineticEnergy = aTrack->GetVertexKineticEnergy();
}

Trajectory::Trajectory(Trajectory & right)
{
  ParticleName = right.ParticleName;
  fpParticleDefinition = right.fpParticleDefinition;
  PDGCharge = right.PDGCharge;
  PDGEncoding = right.PDGEncoding;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  positionRecord = new TrajectoryPointContainer();
  for(int i=0;i<right.positionRecord->size();i++)
  {
    G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }
   momentum = right.momentum;
   vertexPosition = right.vertexPosition;
   globalTime = right.globalTime;
   kineticEnergy = right.kineticEnergy;
}

Trajectory::~Trajectory()
{
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();

  delete positionRecord;
}

void Trajectory::ShowTrajectory() const
{
   G4cout << G4endl << "TrackID =" << fTrackID 
        << " : ParentID=" << fParentID << G4endl;
   G4cout << "Particle name : " << ParticleName 
        << "  Charge : " << PDGCharge << G4endl;
   G4cout << "Original momentum : " <<
        G4BestUnit(momentum,"Energy") << G4endl;
   G4cout << "Original energy : " <<
        G4BestUnit(kineticEnergy,"Energy") << G4endl;
   G4cout << "Vertex : " << G4BestUnit(vertexPosition,"Length")
        << "  Global time : " << G4BestUnit(globalTime,"Time") << G4endl;
   G4cout << "  Current trajectory has " << positionRecord->size() 
        << " points." << G4endl;

   for( size_t i=0 ; i < positionRecord->size() ; i++){
       G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
       G4cout << "Point[" << i << "]" 
            << " Position= " << aTrajectoryPoint->GetPosition() << G4endl;
   }
}

void Trajectory::DrawTrajectory(G4int i_mode) const
{

   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   G4ThreeVector pos;

   G4Polyline pPolyline;
   for (int i = 0; i < positionRecord->size() ; i++) {
     G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
     pos = aTrajectoryPoint->GetPosition();
     pPolyline.push_back( pos );
   }

   G4Colour colour(0.2,0.2,0.2);
   if(fpParticleDefinition==G4Gamma::GammaDefinition())
      colour = G4Colour(1.,1.,1.);
   else if(fpParticleDefinition==G4Neutron::NeutronDefinition())
      colour = G4Colour(0.,0.,0.7);
   else if(fpParticleDefinition==G4Electron::ElectronDefinition()
         ||fpParticleDefinition==G4Positron::PositronDefinition())
      colour = G4Colour(1.,1.,0.);
   else if(fpParticleDefinition==G4MuonMinus::MuonMinusDefinition()
         ||fpParticleDefinition==G4MuonPlus::MuonPlusDefinition())
      colour = G4Colour(0.,1.,0.);
   else if(fpParticleDefinition->GetParticleType()=="meson")
   {
      if(PDGCharge!=0.)
         colour = G4Colour(1.,0.,0.);
      else
         colour = G4Colour(0.5,0.,0.);
   }
   else if(fpParticleDefinition->GetParticleType()=="baryon")
   {
      if(PDGCharge!=0.)
         colour = G4Colour(0.,1.,1.);
      else
         colour = G4Colour(0.,0.5,0.5);
   }

   G4VisAttributes attribs(colour);
   pPolyline.SetVisAttributes(attribs);
   if(pVVisManager) pVVisManager->Draw(pPolyline);
}

void Trajectory::AppendStep(const G4Step* aStep)
{
   positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->
                                 GetPosition() ));
}
  
G4ParticleDefinition* Trajectory::GetParticleDefinition()
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}

void Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  Trajectory* seco = (Trajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  {
    positionRecord->push_back((*(seco->positionRecord))[i]);
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();

}
