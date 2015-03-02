
#include "DetectorMessenger.hh"

#include "EMCalcDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


DetectorMessenger::DetectorMessenger(EMCalcDetectorConstruction * Det)
:Detector(Det)
{
  testemDir = new G4UIdirectory("/testem/");
  testemDir->SetGuidance("commands specific to this example");

  detDir = new G4UIdirectory("/testem/det/");
  detDir->SetGuidance("detector construction commands");

  MaterCmd = new G4UIcmdWithAString("/testem/det/setMat",this);
  MaterCmd->SetGuidance("Select material of the box.");
  MaterCmd->SetParameterName("choice",false);
  MaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
  delete MaterCmd;
  delete detDir;
  delete testemDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == MaterCmd )
   { Detector->SetMaterial(newValue);}
}


