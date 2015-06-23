//____________________________________________________________________________
/*!

\class    

\brief    Header file containing all class types needed for software

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________

#ifndef EMCalcPhysicsList_h
#define EMCalcPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsListMessenger;
class G4VPhysicsConstructor;


class EMCalcPhysicsList: public G4VModularPhysicsList
{
  public:
    EMCalcPhysicsList();
   ~EMCalcPhysicsList();

    void ConstructParticle();
    void ConstructProcess();
    void AddPhysicsList(const G4String& name);

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);

  private:
    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
    G4double currentDefaultCut;

    G4VPhysicsConstructor*  emPhysicsList;
    G4String emName;

    PhysicsListMessenger* pMessenger;         
};


#endif

