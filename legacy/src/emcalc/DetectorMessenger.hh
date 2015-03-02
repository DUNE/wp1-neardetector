
#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class EMCalcDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;


class DetectorMessenger: public G4UImessenger
{
  public:

    DetectorMessenger(EMCalcDetectorConstruction* );
   ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    EMCalcDetectorConstruction*   Detector;

    G4UIdirectory*             testemDir;
    G4UIdirectory*             detDir;
    G4UIcmdWithAString*        MaterCmd;
};

#endif

