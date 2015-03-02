#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class EMCalcDetectorConstruction;
class EMCalcPrimaryGeneratorAction;

class RunAction : public G4UserRunAction {
 public:
  RunAction(EMCalcDetectorConstruction*, EMCalcPrimaryGeneratorAction*);
  ~RunAction();

  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);

  void GetCuts();
  void CriticalEnergy();

  double getRange() const {return range_;}

 private:
  EMCalcDetectorConstruction*   detector;
  EMCalcPrimaryGeneratorAction* primary;

  G4double  rangeCut[3];
  G4double energyCut[3];

  double range_;
};


#endif

