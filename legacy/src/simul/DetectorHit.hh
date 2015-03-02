//____________________________________________________________________________
/*!

\class    DetectorHit

\brief    Class for geant4 detector hits

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef DetectorHit_h
#define DetectorHit_h 1

#include "TLorentzVector.h"

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include <G4VPhysicalVolume.hh>
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

#include "LbnoDataLib.hh"

class G4AttDef;

class DetectorHit : public G4VHit
{
  public:

      DetectorHit();
      DetectorHit(G4VPhysicalVolume* pVol);
      ~DetectorHit();
      DetectorHit(const DetectorHit &right);
      const DetectorHit& operator=(const DetectorHit &right);
      G4int operator==(const DetectorHit &right) const;

      void convertHit(SDHit * sdHit);

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      virtual void Draw();
      virtual void Draw(G4Colour col);
      virtual void Print();

      const std::map<G4String,G4AttDef>* GetAttDefs() const;
      std::vector<G4AttValue>* CreateAttValues() const;

      inline void setEdep(G4double de) { edep = de; }
      inline void addEdep(G4double de) { edep += de; }
      inline G4double getEdep() { return edep; }

      void setPos(TLorentzVector xyz) { pos = xyz; }
      TLorentzVector getPos() { return pos; }

      int getNSteps() const {return nSteps;}
      void setNSteps(int n) {nSteps = n;}

      int getPDG() const {return PDG;}
      void setPDG(int pdg) {PDG = pdg;}

      int getTrackID() const {return TrackID;}
      void setTrackID(int id) {TrackID = id;}

      int getParentID() const {return ParentID;}
      void setParentID(int id) {ParentID = id;}

      TLorentzVector getP4() const {return P4;}
      void setP4(TLorentzVector p4) {P4 = p4;}

      double getKinEnergy() const {return kinEnergy;}
      void setKinEnergy(double kinE) {kinEnergy = kinE;}

      ParticleDescrRecord getMuon() const {return muon;};
      void setMuon(ParticleDescrRecord m) {muon = m;};

      int getTrackLeftVolume() const	{return trackLeftVolume;};
      void setTrackLeftVolume(int tlv)  {trackLeftVolume = tlv;};

      void setParent(GeantBasicParticle p)     {parent = p;};
      GeantBasicParticle getParent() const     {return parent;};

  private:
      
      double edep;
      double kinEnergy;
      int nSteps;
      int PDG;
      int TrackID,ParentID;
      TLorentzVector pos,P4;
      G4ThreeVector pos3vec;
      int trackLeftVolume;

      ParticleDescrRecord muon;
      GeantBasicParticle parent;

      const G4VPhysicalVolume* prePhysVolume;
      
      static std::map<G4String,G4AttDef> fAttDefs;

};

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

extern G4Allocator<DetectorHit> DetectorHitAllocator;

inline void* DetectorHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) DetectorHitAllocator.MallocSingle();
  return aHit;
}

inline void DetectorHit::operator delete(void *aHit)
{
  DetectorHitAllocator.FreeSingle((DetectorHit*) aHit);
}

#endif


