
#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

#include "DataCards.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class PhysicsList: public G4VUserPhysicsList
{
public:
  PhysicsList(std::string hadlist,std::string emlist);
  ~PhysicsList();
  //  virtual ~PhysicsList();

  virtual void SetCuts();

  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();
    
  // these methods Construct physics processes and register them
  virtual void ConstructGeneral();
  virtual void ConstructEM(std::string emlist);
  virtual void ConstructHad(std::string hadlist);
  virtual void ConstructOp();

  void initDataCards();
  void loadDataCards();

  void initialize();

  virtual void AddTransportation();

private:
  G4int VerboseLevel;
  G4int OpVerbLevel;

  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4double cutForProton;
  G4double cutForAlpha;
  G4double cutForGenericIon;

  // these methods Construct particles 
  void ConstructMyBosons();
  void ConstructMyLeptons();
  void ConstructMyHadrons();
  void ConstructMyShortLiveds();

  DataCards*			cards_;
  G4bool      			cardsLoaded_;

  G4double defaultCut_;
  G4double gammaCut_;
  G4double electronCut_;
  G4double positronCut_;
  G4double neutronCut_;

  G4double cutEM_;

  std::string hadlist_;
  std::string emlist_;

};

#endif

