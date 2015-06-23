//____________________________________________________________________________
/*!

\class    PhysicsList

\brief    Class for the physics used in geant4

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
#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "G4ParallelWorldScoringProcess.hh"
#include "globals.hh"

class PhysicsList: public G4VUserPhysicsList
{
  public:
     PhysicsList(std::string hadlist,std::string emlist);
    ~PhysicsList();

     virtual void SetCuts();

  //void loadDataCards();
  void initialize(G4double defaultCut,G4double gammaCut,G4double electronCut,G4double positronCut,G4double neutronCut);
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
 
    std::string hadlist_;
    std::string emlist_;

};

#endif







