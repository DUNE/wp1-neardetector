//____________________________________________________________________________
/*!

\class    PhysicsList

\brief    Class for the physics used in geant4

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update April 2014

*/
//____________________________________________________________________________
#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "G4ParallelWorldScoringProcess.hh"
#include "globals.hh"

#include "DataCards.hh"

class PhysicsList: public G4VUserPhysicsList
{
  public:
     PhysicsList(std::string hadlist,std::string emlist);
    ~PhysicsList();

     virtual void SetCuts();

     void loadDataCards();
     void initialize();
     bool IsDeffined(G4int pid);

    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();

    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);


    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructIons();
    void ConstructShortLived();

    void ConstructEM();
    void ConstructEM(std::string emlist);
    void ConstructHad(std::string hadlist);
    void ConstructGeneral();

    void AddTransportation();
    void AddVirtualScoringProcess();

  private:

    G4int VerboseLevel;
    G4int OpVerbLevel;

    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
    G4double cutForNeutron;
    G4double cutForProton;
    G4double cutForAlpha;
    G4double cutForGenericIon;
  
    G4double defaultCut_;
    G4double gammaCut_;
    G4double electronCut_;
    G4double positronCut_;
    G4double neutronCut_;

    G4double cutEM_;

    std::string hadlist_;
    std::string emlist_;
    G4String               fEmName;

    DataCards*			cards_;
    G4bool      		cardsLoaded_;
};

#endif







