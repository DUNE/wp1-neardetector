//____________________________________________________________________________
/*!

\class    MagneticField

\brief    Class to implement magentic field in geant4

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Apr 2015

*/
//____________________________________________________________________________
#ifndef MagneticField_HH
#define MagneticField_HH

#include "G4UniformMagField.hh"
#include "G4LogicalVolume.hh"
#include "G4FieldManager.hh"

class G4FieldManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


class MagneticField: public G4UniformMagField
{
  public:
  
  MagneticField(G4ThreeVector,G4LogicalVolume * vol);  //  The value of the field
   MagneticField();               //  A zero field
  virtual ~MagneticField();  
      
   //Set the field (fieldValue,0,0)
   void SetMagFieldValue(G4double fieldValue);
   void SetMagFieldValue(G4ThreeVector fieldVector);
      
   G4ThreeVector GetConstantFieldValue();

  protected:

  // Find the global Field Manager
  G4FieldManager* GetGlobalFieldManager();   // static 

  //local field manager for detector selection
  G4FieldManager* localFieldMgr;
  G4MagneticField * magField;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
