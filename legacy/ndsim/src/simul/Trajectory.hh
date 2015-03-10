//____________________________________________________________________________
/*!

\class    Trajectory

\brief    Class to record trajectory information in geant4

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update Sep 2013

*/
//____________________________________________________________________________
#ifndef Trajectory_h
#define Trajectory_h 1

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>
#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include "globals.hh" 
#include "G4ParticleDefinition.hh" 
#include "G4TrajectoryPoint.hh"   
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;

typedef std::vector<G4VTrajectoryPoint*> TrajectoryPointContainer;

class Trajectory : public G4VTrajectory
{
 public:
   Trajectory();
   Trajectory(const G4Track* aTrack);
   Trajectory(Trajectory &);
   virtual ~Trajectory();

   inline void* operator new(size_t);
   inline void  operator delete(void*);
   inline int operator == (const Trajectory& right) const
   {return (this==&right);} 

   inline G4int GetTrackID() const
   { return fTrackID; }
   inline G4int GetParentID() const
   { return fParentID; }
   inline G4String GetParticleName() const
   { return ParticleName; }
   inline G4double GetCharge() const
   { return PDGCharge; }
   inline G4int GetPDGEncoding() const
   { return PDGEncoding; }
   inline G4ThreeVector GetInitialMomentum() const
   { return momentum; }
   inline const G4ThreeVector& GetVertexPosition() const
   { return vertexPosition; }
   inline G4double GetGlobalTime() const
   { return globalTime; }
   inline G4double GetKineticEnergy() const
   { return kineticEnergy; }
   virtual int GetPointEntries() const
   { return positionRecord->size(); }
   virtual G4VTrajectoryPoint* GetPoint(G4int i) const 
   { return (*positionRecord)[i]; }

   virtual void ShowTrajectory() const;
   virtual void DrawTrajectory(G4int i_mode=0) const;
   virtual void AppendStep(const G4Step* aStep);
   virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

   G4ParticleDefinition* GetParticleDefinition();

 private:
   TrajectoryPointContainer* positionRecord;
   G4int                        fTrackID;
   G4int                        fParentID;
   G4ParticleDefinition*        fpParticleDefinition;
   G4String                     ParticleName;
   G4double                     PDGCharge;
   G4int                        PDGEncoding;
   G4ThreeVector                momentum;
   G4ThreeVector                vertexPosition;
   G4double                     globalTime;
   G4double                     kineticEnergy;

};

extern G4Allocator<Trajectory> myTrajectoryAllocator;

inline void* Trajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)myTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void Trajectory::operator delete(void* aTrajectory)
{
  myTrajectoryAllocator.FreeSingle((Trajectory*)aTrajectory);
}

#endif
