#ifndef EMCalcPrimaryGeneratorAction_h
#define EMCalcPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;
class EMCalcDetectorConstruction;

class EMCalcPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  EMCalcPrimaryGeneratorAction(EMCalcDetectorConstruction*);
 ~EMCalcPrimaryGeneratorAction();

  G4ParticleGun* GetParticleGun() {return particleGun_;}

 public:
  void SetDefaultKinematic();
  void GeneratePrimaries(G4Event*);

  G4ParticleGun*                   particleGun_;
  EMCalcDetectorConstruction*      detector_;
};



#endif


