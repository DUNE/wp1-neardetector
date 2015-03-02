
#include "RunAction.hh"
#include "EMCalcDetectorConstruction.hh"
#include "EMCalcPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4ProcessManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "G4Electron.hh"

#include <vector>

RunAction::RunAction(EMCalcDetectorConstruction* det, EMCalcPrimaryGeneratorAction* kin)
:detector(det), primary(kin) {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {

  //instanciate EmCalculator
  G4EmCalculator emCal;
  //  emCal.SetVerbose(2);

  // get particle 
  G4ParticleDefinition* particle = primary->GetParticleGun()
                                          ->GetParticleDefinition();
  G4double energy   = primary->GetParticleGun()->GetParticleEnergy();

  // get material
  G4Material* material = detector->GetMaterial();
  G4String matName     = material->GetName();

   //get range from full dedx
  G4double Range1  = emCal.GetRange(energy,particle,material);
  G4double density = material->GetDensity();
  G4double Range2  = Range1*density;

  //get range from restricted dedx
  G4double range1 = emCal.GetRangeFromRestricteDEDX(energy, particle, material);
  G4double range2 = range1*density;

  G4double radl        = material->GetRadlen();

  range_ = Range1;
  //print range
  //double EGeV = energy * (1./CLHEP::GeV);
  if (0) {
  std::cout << "\n" << particle->GetParticleName () << " E = " << G4BestUnit(energy,"Energy");
  std::cout << "\n material: " << matName << "  radiation length: " << G4BestUnit(radl,   "Length");
  std::cout << "\n  range from restrict dE/dx: " 
         << "\t" << std::setw(8) << G4BestUnit(range1,"Length")
         << " (" << std::setw(8) << G4BestUnit(range2,"Mass/Surface") << ")";

  std::cout << "\n  range from full dE/dx    : " 
         << "\t" << std::setw(8) << G4BestUnit(Range1,"Length")
         << " (" << std::setw(8) << G4BestUnit(Range2,"Mass/Surface") << ")" << std::endl;
  }
/*
  G4String partName = particle->GetParticleName();
  G4double charge   = particle->GetPDGCharge();

  std::cout << "\n " << partName << " ("
            << G4BestUnit(energy,"Energy") << ") in " 
            << material->GetName() << " (density: " 
            << G4BestUnit(density,"Volumic Mass") << ";   radiation length: "
            << G4BestUnit(radl,   "Length")       << ")" << std::endl;

  // max energy transfert
  if (charge != 0.) {
  G4double Mass_c2 = particle->GetPDGMass();
  G4double moverM = electron_mass_c2/Mass_c2;
  G4double gamM1 = energy/Mass_c2, gam = gamM1 + 1., gamP1 = gam + 1.;
  G4double Tmax = 
            (2*electron_mass_c2*gamM1*gamP1)/(1.+2*gam*moverM+moverM*moverM);
  G4double range = emCal.GetCSDARange(Tmax,G4Electron::Electron(),material);

  std::cout << "\n  Max_energy _transferable  : " << G4BestUnit(Tmax,"Energy")
         << " (" << G4BestUnit(range,"Length") << ")" << std::endl;
  }

  // get processList and extract EM processes (but not MultipleScattering)
  G4ProcessVector* plist = particle->GetProcessManager()->GetProcessList();
  G4String procName;
  G4double cut;
  std::vector<G4String> emName;
  std::vector<G4double> enerCut;
  size_t length = plist->size();
  for (size_t j=0; j<length; j++) {
     procName = (*plist)[j]->GetProcessName();
     cut = energyCut[1];
     if ((procName == "eBrem")||(procName == "muBrems")) cut = energyCut[0];
     if (((*plist)[j]->GetProcessType() == fElectromagnetic) &&
         (procName != "msc")) {
       emName.push_back(procName);
       enerCut.push_back(cut);
     }
  }

  // print list of processes
  std::cout << "\n  processes :                ";
  for (size_t j=0; j<emName.size();j++)
    std::cout << "\t" << std::setw(13) << emName[j] << "\t";
  std::cout << "\t" << std::setw(13) <<"total";
*/
}


void RunAction::EndOfRunAction(const G4Run* ) {}


#include "G4ProductionCutsTable.hh"

void RunAction::GetCuts() {
  G4ProductionCutsTable* theCoupleTable =
        G4ProductionCutsTable::GetProductionCutsTable();

  size_t numOfCouples = theCoupleTable->GetTableSize();
  const G4MaterialCutsCouple* couple = 0;
  G4int index = 0;
  for (size_t i=0; i<numOfCouples; i++) {
     couple = theCoupleTable->GetMaterialCutsCouple(i);
     if (couple->GetMaterial() == detector->GetMaterial()) {index = i; break;}
  }

  rangeCut[0] =
         (*(theCoupleTable->GetRangeCutsVector(idxG4GammaCut)))[index];
  rangeCut[1] =      
         (*(theCoupleTable->GetRangeCutsVector(idxG4ElectronCut)))[index];
  rangeCut[2] =      
         (*(theCoupleTable->GetRangeCutsVector(idxG4PositronCut)))[index]; 

  energyCut[0] =
         (*(theCoupleTable->GetEnergyCutsVector(idxG4GammaCut)))[index];
  energyCut[1] =      
         (*(theCoupleTable->GetEnergyCutsVector(idxG4ElectronCut)))[index];
  energyCut[2] =      
         (*(theCoupleTable->GetEnergyCutsVector(idxG4PositronCut)))[index];

}

void RunAction::CriticalEnergy() {
  // compute e- critical energy (Rossi definition) and Moliere radius.
  // Review of Particle Physics - Eur. Phys. J. C3 (1998) page 147
  //
  G4EmCalculator emCal;

  const G4Material* material = detector->GetMaterial();
  const G4double radl = material->GetRadlen();
  G4double ekin = 5*MeV;
  G4double deioni;
  G4double err  = 1., errmax = 0.001;
  G4int    iter = 0 , itermax = 10;  
  while (err > errmax && iter < itermax) {
    iter++;          
    deioni  = radl*
              emCal.ComputeDEDX(ekin,G4Electron::Electron(),"eIoni",material);
    err = std::abs(deioni - ekin)/ekin;
    ekin = deioni;
  }
  std::cout << "\n \n  critical energy (Rossi)  : " 
         << "\t" << std::setw(8) << G4BestUnit(ekin,"Energy");

  //Pdg formula (only for single material)
  G4double pdga[2] = { 610*MeV, 710*MeV };
  G4double pdgb[2] = { 1.24, 0.92 };
  G4double EcPdg = 0.;

  if (material->GetNumberOfElements() == 1) {
    G4int istat = 0;
    if (material->GetState() == kStateGas) istat = 1;  
    G4double Zeff = material->GetZ() + pdgb[istat];
    EcPdg = pdga[istat]/Zeff;
    std::cout << "\t\t\t (from Pdg formula : " 
           << std::setw(8) << G4BestUnit(EcPdg,"Energy") << ")";    
  }

 const G4double Es = 21.2052*MeV;
 G4double rMolier1 = Es/ekin, rMolier2 = rMolier1*radl;
 std::cout << "\n  Moliere radius           : "
        << "\t" << std::setw(8) << rMolier1 << " X0 "   
        << "= " << std::setw(8) << G4BestUnit(rMolier2,"Length");
	
 if (material->GetNumberOfElements() == 1) {
    G4double rMPdg = radl*Es/EcPdg;
    std::cout << "\t (from Pdg formula : " 
           << std::setw(8) << G4BestUnit(rMPdg,"Length") << ")";    
  }
}


