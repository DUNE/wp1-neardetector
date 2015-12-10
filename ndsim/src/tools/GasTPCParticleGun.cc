//____________________________________________________________________________
/*!

\class    GasTPCGun: Not the classical way to run a particle gun.
                     The way the simulation is setup is much easier to
                     generate each particle's kinematics and then propagate
                     into geant

\brief    Program to fire single particles using particle gun.
	  Only tracking, no flux or event generation

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author	  Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Oct 2014
\last update Jun 2015
*/
//____________________________________________________________________________
#include "GasTPCProcessorManager.hh"
#include "GeometryLoader.hh"
//#include "ParticleGunProcessor.hh"
#include "Geant4TrackingAlgorithm.hh"
#include "GasTPCParticleGun.hh"

#include "TXMLEngine.h"
#include "Messenger.hh"

using namespace util;

int main(int argc, char ** argv) {

  // Load the xml file
  std::string datacard   = "";
  std::string outputfile = "PGData.root";
  int nentries = 100;
  for(int iArg = 1; iArg < argc; iArg++) {
    if((!strcasecmp(argv[iArg],"-d"))||(!strcasecmp(argv[iArg],"-datacard"))) {
      datacard = argv[++iArg] ;
    }
    if((!strcasecmp(argv[iArg],"-n"))) {
      nentries = atoi(argv[++iArg]);
      std::cout << "INFO::Generating " << nentries << " events." << std::endl;
    }
    if((!strcasecmp(argv[iArg],"-o"))||(!strcasecmp(argv[iArg],"-output"))) {
      outputfile = argv[++iArg] ;
    }
  }

  // Load the xml file
  const char* gastpc_dir = getenv("GasTPC");
  if(datacard == "")
    datacard = "GeantSimulation.xml";
  std::string fdatacard = string(gastpc_dir) + "/src/config/" + datacard;

  ReadDatacard(fdatacard);

  // Define the input tree
  TTree *pg = new TTree("pg","pg");

  double Px,Py,Pz,E,X,Y,Z,M;
  int pdg;

  pg->Branch("Px",    &Px,  "Px/D");
  pg->Branch("Py",    &Py,  "Py/D");
  pg->Branch("Pz",    &Pz,  "Pz/D");
  pg->Branch("E",     &E,   "E/D");
  pg->Branch("X",     &X,   "X/D");
  pg->Branch("Y",     &Y,   "Y/D");
  pg->Branch("Z",     &Z,   "Z/D");
  pg->Branch("M",     &M,   "M/D");
  pg->Branch("pdg",   &pdg, "pdg/I");

  // Fill the tree
  for(int i=0; i< nentries; i++){
    // Determine the mass
    double fMass = 105.66; // Muon as default
    pdg = fPDG;
    if(pdg == 22)
      fMass = 0;
    else if(abs(pdg) == 11)
      fMass = 0.511;
    else if(abs(pdg) == 13)
      fMass = 105.66;
    else if(abs(pdg) == 211)
      fMass = 139.57;
    else if(abs(pdg) == 321)
      fMass = 493.67;
    else if(abs(pdg) == 2212)
      fMass = 938.27;
    else if(abs(pdg) == 2112)
      fMass = 939.57;
    else{
      cout << "Choose the correct pdg to simulate. 11,-11,13,-13,22,211,-211,321,-321,2212,2112" << endl;
      break;
    }


    //determine monoenergetic or energy spectrum 
    if(fMinEnergy == fMaxEnergy){
      E = fMaxEnergy;
    }
    else{
      TRandom rand;
      rand.SetSeed(i+999);
      if(fgauss == 0)
	E = rand.Uniform(fMinEnergy,fMaxEnergy);
      else
	E = rand.Gaus(fMaxEnergy);
    }
    
    //if all 3 momentum are 0 then indicates to use Gaussian
    Px = fMomentumX;
    Py = fMomentumY;
    Pz = fMomentumZ;
    while( (Px == 0) && (Py == 0 ) && ( Pz == 0) ){
      TRandom3 rand;
      rand.SetSeed(i+999);
      Px = rand.Gaus();
      Py = rand.Gaus();
      Pz = rand.Gaus(); 
    }

    //normalise momentum to unit vectors and multiply by energy
    double Mom = TMath::Sqrt( Px*Px + Py*Py + Pz*Pz);
    //turn them into unit vectors and multiply by true momentum
    //M = fMass;
    Px *= (TMath::Sqrt( E*E - M*M ))/Mom;
    Py *= (TMath::Sqrt( E*E - M*M ))/Mom;
    Pz *= (TMath::Sqrt( E*E - M*M ))/Mom;
    
    X = fPositionX;
    Y = fPositionY;
    Z = fPositionZ;
    //pdg = fPDG;

    pg->Fill();
  }

  //try {
  // Create Processor Manage.
  GasTPCProcessorManager manager;
  manager.initialize(outputfile,fTrackingVerbose);

  //Initiate geometry loader
  //GeometryLoader * geomLoader = new GeometryLoader();
  
  // Initialize your processors here. The processors will define
  // default values for all runtime parameters.
  //ParticleGunProcessor   *g4tracking = new ParticleGunProcessor(geomLoader);
  
  // Get command line arguments if any. 
  // This will modify the default values for the runtime parameters.
  //manager.getRunTimeArguments(argc, argv);
  //manager.loadDataCards();

  // Initialize your processors here. The processors will define
  // default values for all runtime parameters.
  LoadNuEventProcessor     *eventLoader  = new LoadNuEventProcessor();
  eventLoader->initialize("particlegun",pg,fGeometryFile,1);
  if(nentries > 0)
    eventLoader->setNEvents(nentries);
  
  Geant4TrackingAlgorithm *g4trackingalg = new Geant4TrackingAlgorithm(fGeometryFile,fSimTargetVolume);
  //g4trackingalg->setGeometryFile(fGeometryFile);
  //g4trackingalg->setTargetVolumeName(fSimTargetVolume);
  g4trackingalg->setPhysicsList(fEMPhysicsList,fHadPhysicsList);
  g4trackingalg->setMagneticField(fMagneticFieldStrength);
  g4trackingalg->setEnergyCut(fMinEnergyCut);
  g4trackingalg->setDefaultCut(fGammaCut);
  g4trackingalg->setElectronCut(fElectronCut);
  g4trackingalg->setPositronCut(fPositronCut);
  g4trackingalg->setGammaCut(fGammaCut);
  g4trackingalg->setNeutronCut(fNeutronCut);
  g4trackingalg->setVerbose(fTrackingVerbose);
  //g4trackingalg->doVis(false);
  //g4trackingalg->doParticleGun(false);

  Geant4TrackingProcessor  *g4tracking   = new Geant4TrackingProcessor(g4trackingalg);
  /*
  ParticleGunProcessor   *g4tracking = new ParticleGunProcessor(g4trackingalg);
  g4tracking->SetPosition(fPositionX,fPositionY,fPositionZ);
  g4tracking->SetMomentum(fMomentumX,fMomentumY,fMomentumZ);
  g4tracking->SetMinMaxEnergy(fMinEnergy,fMaxEnergy);
  g4tracking->SetMass(fMass);
  g4tracking->SetPdg(fPDG);
  */
  // Now add your processors to the manages. 
  manager.addProcessor(eventLoader);
  manager.addProcessor(g4tracking);
  
  //reinitiate the geometry loader
  //geomLoader->initialize(eventLoader->get);
  
  manager.printToStream(cout);
  
  // Run your processors.
  manager.go();
  
  // Store the result.
  manager.write();
  
  //close the geometry
  //geomLoader->close();
  /*
    } catch (GasTPCException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
    }
  */
  return 0;
}

void ReadDatacard(std::string datacard){

  LOG("GeometryConstruction",pNOTICE) << "Reading configuration from XML file [" << datacard << "]";

  TXMLEngine* xml = new TXMLEngine(); 
  XMLDocPointer_t xmldoc = xml->ParseFile(datacard.c_str());
  if (xmldoc==0) {
    std::cerr << "Exp >> " << "Can not parse input: " << datacard << std::endl;
    delete xml;
    return;  
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);
  fGeometryFile          = util::XML2String(xml,root_node,"GeometryFile");
  fSimTargetVolume       = util::XML2String(xml,root_node,"SimTargetVolume");
  fMinEnergyCut          = util::XML2Dbl(xml,root_node,"ParticleThresholds/MinEnergyCut");
  fGammaCut              = util::XML2Dbl(xml,root_node,"ParticleThresholds/GammaCut");
  fElectronCut           = util::XML2Dbl(xml,root_node,"ParticleThresholds/ElectronCut");
  fPositronCut           = util::XML2Dbl(xml,root_node,"ParticleThresholds/PositronCut");
  fNeutronCut            = util::XML2Dbl(xml,root_node,"ParticleThresholds/NeutronCut");
  fRunVisMacro           = util::XML2Bool(xml,root_node,"RunVisMacro");
  fPhysicsList           = util::XML2String(xml,root_node,"PhysicsList");
  fEMPhysicsList         = util::XML2String(xml,root_node,"EMPhysicsList");
  fHadPhysicsList        = util::XML2String(xml,root_node,"HadPhysicsList");
  fMagneticFieldStrength = util::XML2Dbl(xml,root_node,"MagneticFieldStrength");
  fTrackingVerbose       = (int)util::XML2Dbl(xml,root_node,"TrackingVerbose");
  // Particle gun options
  fPositionX             = util::XML2Dbl(xml,root_node,"ParticleGun/PositionX");
  fPositionY             = util::XML2Dbl(xml,root_node,"ParticleGun/PositionY");
  fPositionZ             = util::XML2Dbl(xml,root_node,"ParticleGun/PositionZ");
  fMomentumX             = util::XML2Dbl(xml,root_node,"ParticleGun/MomentumX");
  fMomentumY             = util::XML2Dbl(xml,root_node,"ParticleGun/MomentumY");
  fMomentumZ             = util::XML2Dbl(xml,root_node,"ParticleGun/MomentumZ");
  fMinEnergy             = util::XML2Dbl(xml,root_node,"ParticleGun/MinEnergy");
  fMaxEnergy             = util::XML2Dbl(xml,root_node,"ParticleGun/MaxEnergy");
  fPDG                   = util::XML2Dbl(xml,root_node,"ParticleGun/PDG");
  fgauss                 = util::XML2Dbl(xml,root_node,"ParticleGun/Gauss");
}

