//____________________________________________________________________________
/*!

\class    MagneticField

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update May 2014

*/
//____________________________________________________________________________
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4LogicalVolume.hh"
#include "G4UniformMagField.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ClassicalRK4.hh"
#include "DetectorConstruction.hh"
#include "MagneticField.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::MagneticField()
  : G4UniformMagField(G4ThreeVector())
{
  GetGlobalFieldManager()->SetDetectorField(this);
  GetGlobalFieldManager()->CreateChordFinder(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::MagneticField(G4ThreeVector fieldVector,G4LogicalVolume *vol)
  : G4UniformMagField(fieldVector)
{

  magField = new G4UniformMagField(fieldVector);

  if(vol->GetName()=="rockVolume")
  {
      GetGlobalFieldManager()->SetDetectorField(this);    
      GetGlobalFieldManager()->CreateChordFinder(this);
  }
  
  else 
    {
      localFieldMgr = new G4FieldManager(magField);
      // Create an equation of motion for this field
      G4EqMagElectricField* fEquation = new G4EqMagElectricField(magField);
      //G4Mag_UsualEqRhs* fEquationMagneticField = new G4Mag_UsualEqRhs(magField);	
	
      G4int nvar = 8;
      //G4MagIntegratorStepper* fStepper = new G4ClassicalRK4( fEquation, nvar ); 
      localFieldMgr->SetDetectorField(magField);
      localFieldMgr->CreateChordFinder(magField);
      vol->SetFieldManager(localFieldMgr,true);

      G4double fMinStep     = 0.010*mm ; // minimal step of 10 microns

      //fIntgrDriver = new G4MagInt_Driver(fMinStep, 
       //                                  fStepper, 
       //                                  fStepper->GetNumberOfVariables() );

      //set electic field for drifting electrons as well
      // must be parallel to B field so that it does not affect momentum measurements
      //G4UniformElectricField* myField = new G4UniformElectricField(G4ThreeVector(200.*kilovolt/cm,0, 0));
      //G4EqMagElectricField *fEquationEl = new G4EqMagElectricField(elcField);

    }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::~MagneticField()
{
	delete magField;
	delete localFieldMgr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Set the value of the Global Field to fieldValue along X
//
void MagneticField::SetMagFieldValue(G4double fieldValue)
{
   //input should be in tesla
   SetMagFieldValue(G4ThreeVector(fieldValue*tesla,0,0));
   std::cout << "\n\n\n!!!!!!! fieldValue: " << fieldValue*tesla << "!!!!\n\n\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Set the value of the Global Field
//
void MagneticField::SetMagFieldValue(G4ThreeVector fieldVector)
{
  // Find the Field Manager for the global field
  G4FieldManager* fieldMgr = GetGlobalFieldManager();
    
  if(fieldVector!=G4ThreeVector(0.,0.,0.))
    { 
      SetFieldValue(fieldVector);
      fieldMgr->SetDetectorField(this);
    } 

  else 
    {
      // If the new field's value is Zero, then it is best to
      //  insure that it is not used for propagation.
      G4MagneticField* magField = 0;
      fieldMgr->SetDetectorField(magField);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4FieldManager*  MagneticField::GetGlobalFieldManager()
{
  return G4TransportationManager::GetTransportationManager()->GetFieldManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
