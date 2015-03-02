#include "PhysicsList.hh"
#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip>  

#include "G4UserLimits.hh"


// Constructor /////////////////////////////////////////////////////////////
PhysicsList::PhysicsList(std::string hadlist,std::string emlist) : G4VUserPhysicsList() 
{

  hadlist_ = hadlist;
  emlist_ = emlist;

  this->initDataCards();

  //G4LossTableManager::Instance();

}


// Destructor //////////////////////////////////////////////////////////////
PhysicsList::~PhysicsList() 
{;}

void PhysicsList::initialize(){


  defaultCutValue = defaultCut_;
  cutForGamma     = gammaCut_;
  cutForElectron  = electronCut_;
  cutForPositron  = positronCut_;

  VerboseLevel = 1;
  OpVerbLevel = 0;

  SetVerboseLevel(VerboseLevel);
  this->SetCuts();
}

// Construct Particles /////////////////////////////////////////////////////
void PhysicsList::ConstructParticle() 
{

  ConstructMyBosons();
  ConstructMyLeptons();
  ConstructMyHadrons();
  ConstructMyShortLiveds();

}


// construct Bosons://///////////////////////////////////////////////////
void PhysicsList::ConstructMyBosons()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
  // gamma
  G4Gamma::GammaDefinition();

  //OpticalPhotons
  G4OpticalPhoton::OpticalPhotonDefinition();

}


// construct Leptons://///////////////////////////////////////////////////
void PhysicsList::ConstructMyLeptons()
{
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}


#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

// construct Hadrons://///////////////////////////////////////////////////
void PhysicsList::ConstructMyHadrons()
{
 //  mesons
  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();

 //  baryons
  G4BaryonConstructor bConstructor;
  bConstructor.ConstructParticle();

 //  ions
  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();

}


// construct Shortliveds://///////////////////////////////////////////////////
void PhysicsList::ConstructMyShortLiveds()
{
  // ShortLiveds
}

// Construct Processes //////////////////////////////////////////////////////
void PhysicsList::ConstructProcess() 
{

  AddTransportation();
  ConstructEM(emlist_);
  //ConstructOp();
  ConstructHad(hadlist_);
  ConstructGeneral();

}


// Transportation ///////////////////////////////////////////////////////////
#include "G4StepLimiter.hh"

void PhysicsList::AddTransportation() {
  
  G4VUserPhysicsList::AddTransportation();
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle     = theParticleIterator->value();
    G4ProcessManager*     pmanager     = particle->GetProcessManager();
    G4String              particleName = particle->GetParticleName();

    // time cuts for ONLY neutrons:
    //if(particleName == "neutron") 
    //  pmanager->AddDiscreteProcess(new MaxTimeCuts());
    // Energy cuts to kill charged (embedded in method) particles:
    //pmanager->AddDiscreteProcess(new MinEkineCuts());
    
    // Step limit applied to all particles:
    pmanager->AddProcess(new G4StepLimiter,       -1,-1,1);
    
  }		      
}


// Electromagnetic Processes ------------------------------------------------------------
// all charged particles

// gamma
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"

#include "G4LowEPComptonModel.hh"

// e-
#include "G4eMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

// e+
#include "G4eIonisation.hh" 
#include "G4eBremsstrahlung.hh" 
#include "G4eplusAnnihilation.hh"


// alpha and GenericIon and deuterons, triton, He3:
//hIonisation #include "G4hLowEnergyIonisation.hh" -> moved to G4hIonisation
#include "G4EnergyLossTables.hh"
// hLowEnergyIonisation uses Ziegler 1988 as the default


//muon:
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCaptureAtRest.hh"

//OTHERS:
#include "G4hIonisation.hh" 
#include "G4hMultipleScattering.hh"
#include "G4hBremsstrahlung.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"

//em process options to allow msc step-limitation to be switched off
#include "G4EmProcessOptions.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"

void PhysicsList::ConstructEM(std::string emlist){
  
  if(emlist.find("EM0") != string::npos || emlist.find("emstandard") != string::npos){
    G4EmStandardPhysics* EmStandardPhysics = new G4EmStandardPhysics();
    EmStandardPhysics->ConstructProcess();
  }
  else if(emlist.find("EM1") != string::npos || emlist.find("emstandard_opt1") != string::npos){
    G4EmStandardPhysics_option1* EmStandardPhysics = new G4EmStandardPhysics_option1();
    EmStandardPhysics->ConstructProcess();
  }
  else if(emlist.find("EM2") != string::npos || emlist.find("emstandard_opt2") != string::npos){
    G4EmStandardPhysics_option2* EmStandardPhysics = new G4EmStandardPhysics_option2();
    EmStandardPhysics->ConstructProcess();
  }
  else if(emlist.find("EM3") != string::npos || emlist.find("emstandard_opt3") != string::npos){
    G4EmStandardPhysics_option3* EmStandardPhysics = new G4EmStandardPhysics_option3();
    EmStandardPhysics->ConstructProcess();
  }
  else if(emlist.find("EM4") != string::npos || emlist.find("emstandard_opt4") != string::npos){
    G4EmStandardPhysics_option4* EmStandardPhysics = new G4EmStandardPhysics_option4();
    EmStandardPhysics->ConstructProcess();
  }
  else{
    //set a finer grid of the physic tables in order to improve precision
    //former LowEnergy models have 200 bins up to 100 GeV
    G4EmProcessOptions opt;
    opt.SetMaxEnergy(100*GeV);
    opt.SetDEDXBinning(200);
    opt.SetLambdaBinning(200);
    
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();
      G4String particleType = particle->GetParticleType();
      G4double charge = particle->GetPDGCharge();
      
      if (particleName == "gamma") 
	{
	  //gamma
	  G4RayleighScattering* theRayleigh = new G4RayleighScattering();
	  theRayleigh->SetEmModel(new G4LivermoreRayleighModel());  //not strictly necessary
	  pmanager->AddDiscreteProcess(theRayleigh);
	  
	  G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
	  thePhotoElectricEffect->SetEmModel(new G4LivermorePhotoElectricModel());
	  pmanager->AddDiscreteProcess(thePhotoElectricEffect);
	  
	  G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
	  theComptonScattering->SetEmModel(new G4LivermoreComptonModel());
	  //theComptonScattering->SetEmModel(new G4LowEPComptonModel());
	  pmanager->AddDiscreteProcess(theComptonScattering);

	  G4GammaConversion* theGammaConversion = new G4GammaConversion();
	  theGammaConversion->SetEmModel(new G4LivermoreGammaConversionModel());
	  pmanager->AddDiscreteProcess(theGammaConversion);
	  
	} 
      else if (particleName == "e-") 
	{
	  //electron
	  // process ordering: AddProcess(name, at rest, along step, post step)
	  // Multiple scattering
	  G4eMultipleScattering* msc = new G4eMultipleScattering();
	  //xioni	msc->SetStepLimitType(fUseDistanceToBoundary);
	  //xmsc
	  pmanager->AddProcess(msc,-1, 1, 1);
	  
	  // Ionisation
	  G4eIonisation* eIonisation = new G4eIonisation();
	  //xioni	eIonisation->SetEmModel(new G4LivermoreIonisationModel());
	  //xioni	eIonisation->SetStepFunction(0.2, 100*um); //improved precision in tracking  
	  pmanager->AddProcess(eIonisation,-1, 2, 2);
	  
	  // Bremsstrahlung
	  G4eBremsstrahlung* eBremsstrahlung = new G4eBremsstrahlung();
	  eBremsstrahlung->SetEmModel(new G4LivermoreBremsstrahlungModel());
	  pmanager->AddProcess(eBremsstrahlung, -1,-3, 3);
	} 
      else if (particleName == "e+") 
	{
	  //positron	
	  G4eMultipleScattering* msc = new G4eMultipleScattering();
	  msc->SetStepLimitType(fUseDistanceToBoundary);
	  pmanager->AddProcess(msc,-1, 1, 1);
	  
	  // Ionisation
	  G4eIonisation* eIonisation = new G4eIonisation();
	  //xioni	eIonisation->SetStepFunction(0.2, 100*um); //     
	  pmanager->AddProcess(eIonisation,                 -1, 2, 2);
	  
	  //Bremsstrahlung (use default, no low-energy available)
	  pmanager->AddProcess(new G4eBremsstrahlung(), -1,-1, 3);
	  
	  //Annihilation
	  pmanager->AddProcess(new G4eplusAnnihilation(),0,-1, 4);      
	} 
      else if( particleName == "mu+" || 
	       particleName == "mu-"    ) 
	{
	  //muon  
	  pmanager->AddProcess(new G4eMultipleScattering,           -1, 1, 1);
	  pmanager->AddProcess(new G4MuIonisation(),          -1, 2, 2);
	  pmanager->AddProcess(new G4MuBremsstrahlung(),      -1,-1, 3);
	  pmanager->AddProcess(new G4MuPairProduction(),      -1,-1, 4);
	  if( particleName == "mu-" )
	    pmanager->AddProcess(new G4MuonMinusCaptureAtRest(), 0,-1,-1);
	} 
      else if (particleName == "proton" || 
	       particleName == "pi+"    || 
	       particleName == "pi-")
	{
	  //multiple scattering
	  //xmsc
	  pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
	  
	  //ionisation
	  G4hIonisation* hIonisation = new G4hIonisation();
	  //xioni	hIonisation->SetStepFunction(0.2, 50*um);
	  pmanager->AddProcess(hIonisation,                     -1, 2, 2);      
	  
	  //bremmstrahlung
	  pmanager->AddProcess(new G4hBremsstrahlung,     -1,-3, 3);
	}
      else if(particleName  == "alpha"      ||
	      particleName == "deuteron"   ||
	      particleName == "triton"     ||
	      particleName == "He3")
	{
	  //multiple scattering
	  //xmsc
	  pmanager->AddProcess(new G4hMultipleScattering,-1,1,1);
	  
	  //ionisation
	  G4ionIonisation* ionIoni = new G4ionIonisation();
	  //xioni	ionIoni->SetStepFunction(0.1, 20*um);
	  pmanager->AddProcess(ionIoni,                   -1, 2, 2);
	}
      else if (particleName == "GenericIon")
	{
	  // OBJECT may be dynamically created as either a GenericIon or nucleus
	  // G4Nucleus exists and therefore has particle type nucleus
	  // genericIon:
	  
	  //multiple scattering
	  //xmsc
	  pmanager->AddProcess(new G4hMultipleScattering,-1,1,1);
	  
	  //ionisation
	  G4ionIonisation* ionIoni = new G4ionIonisation();
	  //xioni	ionIoni->SetEmModel(new G4IonParametrisedLossModel());
	  //xioni	ionIoni->SetStepFunction(0.1, 20*um);
	  pmanager->AddProcess(ionIoni,                   -1, 2, 2);
	} 
      
      else if ((!particle->IsShortLived()) &&
	       (charge != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) 
	{
	  //all others charged particles except geantino
	  G4hMultipleScattering* aMultipleScattering = new G4hMultipleScattering();
	  G4hIonisation* ahadronIon = new G4hIonisation();
	  
	  //multiple scattering
	  //xmsc
	  pmanager->AddProcess(aMultipleScattering,-1,1,1);
	  
	  //ionisation
	  pmanager->AddProcess(ahadronIon,       -1,2,2);      
	}
      
    }
  
    // turn off msc step-limitation - especially as electron cut 1nm
    //xmsc
    //xioni  opt.SetMscStepLimitation(fMinimal);
    
  }
}


// Optical Processes ----------------------------------------------------
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
//#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4OpWLS.hh"

void PhysicsList::ConstructOp() 
{
  // default scintillation process
  G4Scintillation* theScintProcessDef = new G4Scintillation("Scintillation");
  // theScintProcessDef->DumpPhysicsTable();
  theScintProcessDef->SetTrackSecondariesFirst(true);
  theScintProcessDef->SetScintillationYieldFactor(1.0); //

  G4double fastcomponent_frac = 0.5;     
  theScintProcessDef->SetScintillationExcitationRatio(fastcomponent_frac); // Fast/Total ratio (according to geant4 documentation
  theScintProcessDef->SetVerboseLevel(OpVerbLevel);

  // scintillation process for alpha:
  G4Scintillation* theScintProcessAlpha = new G4Scintillation("Scintillation");
  // theScintProcessNuc->DumpPhysicsTable();
  theScintProcessAlpha->SetTrackSecondariesFirst(true);
  theScintProcessAlpha->SetScintillationYieldFactor(1.0);
  //  theScintProcessAlpha->SetScintillationExcitationRatio(1.0); 
  theScintProcessAlpha->SetScintillationExcitationRatio(0.25); 
  theScintProcessAlpha->SetVerboseLevel(OpVerbLevel);

  // scintillation process for gamma:
  G4Scintillation* theScintProcessGamma = new G4Scintillation("Scintillation");
  // theScintProcessNuc->DumpPhysicsTable();
  theScintProcessGamma->SetTrackSecondariesFirst(true);
  theScintProcessGamma->SetScintillationYieldFactor(1.0);
  theScintProcessGamma->SetScintillationExcitationRatio(0.5); 
  theScintProcessGamma->SetVerboseLevel(OpVerbLevel);

  // scintillation process for heavy nuclei
  G4Scintillation* theScintProcessNuc = new G4Scintillation("Scintillation");
  // theScintProcessNuc->DumpPhysicsTable();
  theScintProcessNuc->SetTrackSecondariesFirst(true);
  theScintProcessNuc->SetScintillationYieldFactor(1.0); // for Helium no evidence of quenching, unlike with Xenon...
  theScintProcessNuc->SetScintillationExcitationRatio(fastcomponent_frac); //
  theScintProcessNuc->SetVerboseLevel(OpVerbLevel);

  // optical processes
  G4OpAbsorption* theAbsorptionProcess = new G4OpAbsorption();
  //  G4OpRayleigh* theRayleighScatteringProcess = new G4OpRayleigh();
  G4OpBoundaryProcess* theBoundaryProcess = new G4OpBoundaryProcess();
  //  theAbsorptionProcess->DumpPhysicsTable();
  //  theRayleighScatteringProcess->DumpPhysicsTable();
  theAbsorptionProcess->SetVerboseLevel(OpVerbLevel);
  // theRayleighScatteringProcess->SetVerboseLevel(OpVerbLevel);
  theBoundaryProcess->SetVerboseLevel(OpVerbLevel);
  G4OpticalSurfaceModel themodel = unified;
  //theBoundaryProcess->SetModel(themodel);

  G4OpWLS* theWLSProcess = new G4OpWLS("OpWLS");
  //  theWLSProcess->UseTimeProfile("delta"); //delta decay time
  //  theWLSProcess->UseTimeProfile("exponential"); //exponential default

  theParticleIterator->reset();
  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();
      if (theScintProcessDef->IsApplicable(*particle)) {
	//      if(particle->GetPDGMass() > 5.0*GeV) 
	if(particle->GetParticleName() == "GenericIon") {
	  pmanager->AddProcess(theScintProcessNuc); // AtRestDiscrete
	  pmanager->SetProcessOrderingToLast(theScintProcessNuc,idxAtRest);
	  pmanager->SetProcessOrderingToLast(theScintProcessNuc,idxPostStep);
	}	  
	else if(particle->GetParticleName() == "alpha") {
	  pmanager->AddProcess(theScintProcessAlpha);
	  pmanager->SetProcessOrderingToLast(theScintProcessAlpha,idxAtRest);
	  pmanager->SetProcessOrderingToLast(theScintProcessAlpha,idxPostStep);
	}

	else if(particle->GetParticleName() == "gamma") {
	  pmanager->AddProcess(theScintProcessAlpha);
	  pmanager->SetProcessOrderingToLast(theScintProcessGamma,idxAtRest);
	  pmanager->SetProcessOrderingToLast(theScintProcessGamma,idxPostStep);
	}
	else {
	  pmanager->AddProcess(theScintProcessDef);
	  pmanager->SetProcessOrderingToLast(theScintProcessDef,idxAtRest);
	  pmanager->SetProcessOrderingToLast(theScintProcessDef,idxPostStep);
	}	  
      }
      
      if (particleName == "opticalphoton") {
	pmanager->AddDiscreteProcess(theAbsorptionProcess);
	//	pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
	pmanager->AddDiscreteProcess(theBoundaryProcess);
	pmanager->AddDiscreteProcess(theWLSProcess);
      }
    }
}


// Hadronic processes ////////////////////////////////////////////////////////

// Elastic processes:
#include "G4HadronElasticProcess.hh"

// Inelastic processes:
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"

// Low-energy Models: < 20GeV
#include "G4LElastic.hh"
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"
#include "G4HadronCaptureProcess.hh"
// High-energy Models: >20 GeV
#include "G4HEPionPlusInelastic.hh"
#include "G4HEPionMinusInelastic.hh"
#include "G4HEKaonPlusInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonMinusInelastic.hh"
#include "G4HEProtonInelastic.hh"
#include "G4HEAntiProtonInelastic.hh"
#include "G4HENeutronInelastic.hh"
#include "G4HEAntiNeutronInelastic.hh"

// Neutron high-precision models: <20 MeV
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4LCapture.hh"


// neutron-induced fission (new implementation)
#include "G4HadronFissionProcess.hh"
#include "G4FissionLibrary.hh"
#include "G4FissLib.hh"


// Stopping processes
#include "G4PiMinusAbsorptionAtRest.hh"
#include "G4KaonMinusAbsorptionAtRest.hh"
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4AntiNeutronAnnihilationAtRest.hh"

#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
//#include "G4QStoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "HadronPhysicsQGSP_BIC_HP.hh"
#include "HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronInelasticQBBC.hh"

// ConstructHad()
// Makes discrete physics processes for the hadrons, at present limited
// to those particles with GHEISHA interactions (INTRC > 0).
// The processes are: Elastic scattering and Inelastic scattering.
// F.W.Jones  09-JUL-1998
void PhysicsList::ConstructHad(std::string hadlist) 
{

  if(hadlist.find("QGSP_BERT_HP") != string::npos){
    G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
    G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic", VerboseLevel,true);
    //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
    G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ion");
    G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
    HadronPhysicsQGSP_BERT_HP* HadronPhysicsBert = new HadronPhysicsQGSP_BERT_HP("hadron",true);

    EmExtraPhysics->ConstructProcess();
    HadronElasticPhysics->ConstructProcess();
    //QStoppingPhysics->ConstructProcess();
    IonBinaryCascadePhysics->ConstructProcess();
    NeutronTrackingCut->ConstructProcess();
    HadronPhysicsBert->ConstructProcess();
  }
  else if(hadlist.find("QGSP_BIC_HP") != string::npos){
    G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
    G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic", VerboseLevel,true);
    //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
    G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ionBIC");
    G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
    HadronPhysicsQGSP_BIC_HP*  HadronPhysicsBic = new HadronPhysicsQGSP_BIC_HP("hadron",true);

    EmExtraPhysics->ConstructProcess();
    HadronElasticPhysics->ConstructProcess();
    //QStoppingPhysics->ConstructProcess();
    IonBinaryCascadePhysics->ConstructProcess();
    NeutronTrackingCut->ConstructProcess();
    HadronPhysicsBic->ConstructProcess();
  }
  else if(hadlist.find("QBBC_HP") != string::npos){
    G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
    G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic", VerboseLevel,true);
    //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
    G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ionBIC");
    G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
    G4HadronInelasticQBBC* HadronInelasticQBBC = new G4HadronInelasticQBBC("QBBC",VerboseLevel,false,false,false,true,true);

    EmExtraPhysics->ConstructProcess();
    HadronElasticPhysics->ConstructProcess();
    //QStoppingPhysics->ConstructProcess();
    IonBinaryCascadePhysics->ConstructProcess();
    NeutronTrackingCut->ConstructProcess();
    HadronInelasticQBBC->ConstructProcess();
  }
  else if(hadlist.find("FTFP_BERT_HP") != string::npos){
    G4EmExtraPhysics* EmExtraPhysics = new G4EmExtraPhysics("extra EM");
    G4HadronElasticPhysics*  HadronElasticPhysics = new G4HadronElasticPhysics("elastic", VerboseLevel,true);
    //G4QStoppingPhysics* QStoppingPhysics =  new  G4QStoppingPhysics("stopping",VerboseLevel);
    G4IonBinaryCascadePhysics* IonBinaryCascadePhysics = new G4IonBinaryCascadePhysics("ionBIC");
    G4NeutronTrackingCut* NeutronTrackingCut = new G4NeutronTrackingCut("nTackingCut",VerboseLevel);
    HadronPhysicsFTFP_BERT_HP*  HadronPhysicsFtfp = new HadronPhysicsFTFP_BERT_HP("hadron",true);

    EmExtraPhysics->ConstructProcess();
    HadronElasticPhysics->ConstructProcess();
    //QStoppingPhysics->ConstructProcess();
    IonBinaryCascadePhysics->ConstructProcess();
    NeutronTrackingCut->ConstructProcess();
    HadronPhysicsFtfp->ConstructProcess();
  }
  else{
    G4HadronElasticProcess* theElasticProcess = new G4HadronElasticProcess;
    G4LElastic* theElasticModel = new G4LElastic;
    theElasticProcess->RegisterMe(theElasticModel);
    
    theParticleIterator->reset();
    while ((*theParticleIterator)()) 
      {
	G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4String particleName = particle->GetParticleName();
	
	if (particleName == "pi+") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4PionPlusInelasticProcess* theInelasticProcess = 
	      new G4PionPlusInelasticProcess("inelastic");
	    G4LEPionPlusInelastic* theLEInelasticModel = 
	      new G4LEPionPlusInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEPionPlusInelastic* theHEInelasticModel = 
	      new G4HEPionPlusInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  } 
	
	else if (particleName == "pi-") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4PionMinusInelasticProcess* theInelasticProcess = 
	      new G4PionMinusInelasticProcess("inelastic");
	    G4LEPionMinusInelastic* theLEInelasticModel = 
	      new G4LEPionMinusInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEPionMinusInelastic* theHEInelasticModel = 
	      new G4HEPionMinusInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	    G4String prcNam;
	    pmanager->AddRestProcess(new G4PiMinusAbsorptionAtRest, ordDefault);
	  }
	
	else if (particleName == "kaon+") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4KaonPlusInelasticProcess* theInelasticProcess = 
	      new G4KaonPlusInelasticProcess("inelastic");
	    G4LEKaonPlusInelastic* theLEInelasticModel = 
	      new G4LEKaonPlusInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEKaonPlusInelastic* theHEInelasticModel = 
	      new G4HEKaonPlusInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "kaon0S") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4KaonZeroSInelasticProcess* theInelasticProcess = 
	      new G4KaonZeroSInelasticProcess("inelastic");
	    G4LEKaonZeroSInelastic* theLEInelasticModel = 
	      new G4LEKaonZeroSInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEKaonZeroInelastic* theHEInelasticModel = 
	      new G4HEKaonZeroInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "kaon0L") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4KaonZeroLInelasticProcess* theInelasticProcess = 
	      new G4KaonZeroLInelasticProcess("inelastic");
	    G4LEKaonZeroLInelastic* theLEInelasticModel = 
	      new G4LEKaonZeroLInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEKaonZeroInelastic* theHEInelasticModel = 
	      new G4HEKaonZeroInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "kaon-") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4KaonMinusInelasticProcess* theInelasticProcess = 
	      new G4KaonMinusInelasticProcess("inelastic");
	    G4LEKaonMinusInelastic* theLEInelasticModel = 
	      new G4LEKaonMinusInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEKaonMinusInelastic* theHEInelasticModel = 
	      new G4HEKaonMinusInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	    pmanager->AddRestProcess(new G4KaonMinusAbsorptionAtRest, ordDefault);
	  }
	
	else if (particleName == "proton") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4ProtonInelasticProcess* theInelasticProcess = 
	      new G4ProtonInelasticProcess("inelastic");
	    G4LEProtonInelastic* theLEInelasticModel = new G4LEProtonInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEProtonInelastic* theHEInelasticModel = new G4HEProtonInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "anti_proton") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4AntiProtonInelasticProcess* theInelasticProcess = 
	      new G4AntiProtonInelasticProcess("inelastic");
	    G4LEAntiProtonInelastic* theLEInelasticModel = 
	      new G4LEAntiProtonInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEAntiProtonInelastic* theHEInelasticModel = 
	      new G4HEAntiProtonInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "neutron") {
	  // elastic scattering
	  G4HadronElasticProcess* theNeutronElasticProcess = 
	    new G4HadronElasticProcess;
	  G4LElastic* theElasticModel1 = new G4LElastic;
	  G4NeutronHPElastic * theElasticNeutron = new G4NeutronHPElastic;
	  theNeutronElasticProcess->RegisterMe(theElasticModel1);
	  theElasticModel1->SetMinEnergy(19*MeV);
	  theNeutronElasticProcess->RegisterMe(theElasticNeutron);
	  G4NeutronHPElasticData * theNeutronData = new G4NeutronHPElasticData;
	  theNeutronElasticProcess->AddDataSet(theNeutronData);
	  pmanager->AddDiscreteProcess(theNeutronElasticProcess);
	  // inelastic scattering
	  G4NeutronInelasticProcess* theInelasticProcess =
	    new G4NeutronInelasticProcess("inelastic");
	  G4LENeutronInelastic* theInelasticModel = new G4LENeutronInelastic;
	  theInelasticModel->SetMinEnergy(19*MeV);
	  theInelasticProcess->RegisterMe(theInelasticModel);
	  G4NeutronHPInelastic * theLENeutronInelasticModel =
	    new G4NeutronHPInelastic;
	  theInelasticProcess->RegisterMe(theLENeutronInelasticModel);
	  G4NeutronHPInelasticData * theNeutronData1 = 
	    new G4NeutronHPInelasticData;
	  theInelasticProcess->AddDataSet(theNeutronData1);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	  // capture
	  G4HadronCaptureProcess* theCaptureProcess =
	    new G4HadronCaptureProcess;
	  G4LCapture* theCaptureModel = new G4LCapture;
	  theCaptureModel->SetMinEnergy(19*MeV);
	  theCaptureProcess->RegisterMe(theCaptureModel);
	  G4NeutronHPCapture * theLENeutronCaptureModel = new G4NeutronHPCapture;
	  theCaptureProcess->RegisterMe(theLENeutronCaptureModel);
	  G4NeutronHPCaptureData * theNeutronData3 = new G4NeutronHPCaptureData;
	  theCaptureProcess->AddDataSet(theNeutronData3);
	  pmanager->AddDiscreteProcess(theCaptureProcess);
	  
	  
	  // Fission library model
	  G4HadronFissionProcess *theFissionProcess = new G4HadronFissionProcess("lll_fission");
	  G4FissLib* theFissionModel = new G4FissLib;
	  theFissionProcess->RegisterMe(theFissionModel);
	  pmanager->AddDiscreteProcess(theFissionProcess);
	  
	  
	  
	  //  G4ProcessManager* pmanager = G4Neutron::Neutron->GetProcessManager();
	  //  pmanager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
      }
	else if (particleName == "anti_neutron") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4AntiNeutronInelasticProcess* theInelasticProcess = 
	      new G4AntiNeutronInelasticProcess("inelastic");
	    G4LEAntiNeutronInelastic* theLEInelasticModel = 
	      new G4LEAntiNeutronInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    G4HEAntiNeutronInelastic* theHEInelasticModel = 
	      new G4HEAntiNeutronInelastic;
	    theInelasticProcess->RegisterMe(theHEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "deuteron") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4DeuteronInelasticProcess* theInelasticProcess = 
	      new G4DeuteronInelasticProcess("inelastic");
	    G4LEDeuteronInelastic* theLEInelasticModel = 
	      new G4LEDeuteronInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "triton") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4TritonInelasticProcess* theInelasticProcess = 
	      new G4TritonInelasticProcess("inelastic");
	    G4LETritonInelastic* theLEInelasticModel = 
	      new G4LETritonInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
	
	else if (particleName == "alpha") 
	  {
	    pmanager->AddDiscreteProcess(theElasticProcess);
	    G4AlphaInelasticProcess* theInelasticProcess = 
	      new G4AlphaInelasticProcess("inelastic");
	    G4LEAlphaInelastic* theLEInelasticModel = 
	      new G4LEAlphaInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	    pmanager->AddDiscreteProcess(theInelasticProcess);
	  }
      }	
  }
}


// Decays ///////////////////////////////////////////////////////////////////
#include "G4Decay.hh"
#include "G4RadioactiveDecay.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"
//xScore 
#include "G4ParallelWorldScoringProcess.hh"


void PhysicsList::ConstructGeneral() {

//xScore
#ifdef ARKTIS_SCORING
  G4ParallelWorldScoringProcess* theParallelWorldScoringProcess
    = new G4ParallelWorldScoringProcess("ParaWorldScoringProc");
  theParallelWorldScoringProcess->SetParallelWorld("ParallelScoringWorld");
#endif

  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      
      if (theDecayProcess->IsApplicable(*particle) && !particle->IsShortLived()) 
	{ 
	  pmanager ->AddProcess(theDecayProcess);
	  // set ordering for PostStepDoIt and AtRestDoIt
	  pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
	  pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
	}

//xScore
#ifdef ARKTIS_SCORING
      pmanager->AddProcess(theParallelWorldScoringProcess);
      pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess, idxAtRest);
      pmanager->SetProcessOrdering(theParallelWorldScoringProcess, idxAlongStep, 1);
      pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess, idxPostStep);
#endif      
      
    }


  // Declare radioactive decay to the GenericIon in the IonTable.
  const G4IonTable *theIonTable = 
    G4ParticleTable::GetParticleTable()->GetIonTable();
  G4RadioactiveDecay *theRadioactiveDecay = new G4RadioactiveDecay();

  for (G4int i=0; i<theIonTable->Entries(); i++) 
    {
      G4String particleName = theIonTable->GetParticle(i)->GetParticleName();
      G4String particleType = theIonTable->GetParticle(i)->GetParticleType();
      
      if (particleName == "GenericIon") 
	{
	  G4ProcessManager* pmanager = 
	    theIonTable->GetParticle(i)->GetProcessManager();
	  pmanager->SetVerboseLevel(VerboseLevel);
	  pmanager ->AddProcess(theRadioactiveDecay);
	  pmanager ->SetProcessOrdering(theRadioactiveDecay, idxPostStep);
	  pmanager ->SetProcessOrdering(theRadioactiveDecay, idxAtRest);
	} 
    }
}

// Cuts /////////////////////////////////////////////////////////////////////
void PhysicsList::SetCuts() 
{
  
  if (verboseLevel >1)
    G4cout << "PhysicsList::SetCuts:";
  
  if (verboseLevel>0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " 
	   << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  //special for low energy physics
  G4double lowlimit=250*eV;  
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowlimit,100.*GeV);

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma 
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  
  if (verboseLevel>0) DumpCutValuesTable();
}


// Cards /////////////////////////////////////////////////////////////////////
void PhysicsList::initDataCards() {
  cards_ = DataCards::getInstance();

  cards_->AddDataCardDouble("cutEM", 0.0001*MeV);
  cards_->AddDataCardDouble("defaultCut",0.0001*cm);
  cards_->AddDataCardDouble("gammaCut",0.0001*cm);
  cards_->AddDataCardDouble("electronCut",0.0001*cm);
  cards_->AddDataCardDouble("positronCut",0.0001*cm);
  cards_->AddDataCardDouble("neutronCut",0.0001*cm);
  //cards_->AddDataCardString("decayList","decays");
  cardsLoaded_ = false;
}

void PhysicsList::loadDataCards() {
  defaultCut_       = cards_->fetchValueDouble("defaultCut");
  gammaCut_         = cards_->fetchValueDouble("gammaCut");
  electronCut_      = cards_->fetchValueDouble("electronCut");
  positronCut_      = cards_->fetchValueDouble("positronCut");
  neutronCut_       = cards_->fetchValueDouble("neutronCut");
  cutEM_            = cards_->fetchValueDouble("cutEM");
  //decayList_        = cards_->fetchValueString("decayList");
  cardsLoaded_      = true;
}
