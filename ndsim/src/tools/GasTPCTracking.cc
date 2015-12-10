//____________________________________________________________________________
/*!

\class    GasTPCTracking

\brief    Program to load primaries from neutrino verticies and track them
	  Only tracking, no flux or event generation

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  June 2013
\last update Oct 2015
*/
//____________________________________________________________________________
#include "GasTPCTracking.hh"
#include "GasTPCProcessorManager.hh"
#include "GeometryLoader.hh"
#include "LoadNuEventProcessor.hh"
#include "Geant4TrackingProcessor.hh"
#include "Geant4TrackingAlgorithm.hh"

#include "TXMLEngine.h"
#include "Messenger.hh"

using namespace util;

int main(int argc, char ** argv) {

  std::string datacard   = "";
  std::string inputfile  = "";
  std::string outputfile = "nuData.root";
  int nentries = -1, min_entries = -2;
  std::string treename = "";
  for(int iArg = 1; iArg < argc; iArg++) {
    if((!strcasecmp(argv[iArg],"-d"))||(!strcasecmp(argv[iArg],"-datacard"))) {
      datacard = argv[++iArg] ;
    }
    if((!strcasecmp(argv[iArg],"-i"))||(!strcasecmp(argv[iArg],"-input"))) {
      inputfile = argv[++iArg] ;
    }
    
    if((!strcasecmp(argv[iArg],"-n1"))) {
      nentries = atoi(argv[++iArg]);
      std::cout << "INFO::Generating " << nentries << " events." << std::endl;
    }
    /*
    if((!strcasecmp(argv[iArg],"-n2"))) {
      min_entries = atoi(argv[++iArg]);
      std::cout << "INFO::Generating " << nentries << " events." << std::endl;
    }
    */
    if((!strcasecmp(argv[iArg],"-o"))||(!strcasecmp(argv[iArg],"-output"))) {
      outputfile = argv[++iArg] ;
    }
    if((!strcasecmp(argv[iArg],"-t"))||(!strcasecmp(argv[iArg],"-tree"))) {
      treename = argv[++iArg] ;
    }
  }

  // Check the output genie file to setup the correct tree
  if(inputfile.find("gst") != std::string::npos)
    treename = "gst";
  else if(inputfile.find("ghep") != std::string::npos)
    treename = "gtree";
  else{
    std::cout << "Choose a valid input genie tree using the -t tree option. Or check your input file. Time to die" << std::endl;
    return 1;
  }

  // Load the xml file
  const char* gastpc_dir = getenv("GasTPC");

  if(datacard == "")
    datacard = "GeantSimulation.xml";
  std::string fdatacard = string(gastpc_dir) + "/src/config/" + datacard;

  ReadDatacard(fdatacard);

  if(fGeometryFile == "")
    fGeometryFile = "Apollon1954TPCEcalMagnetGeometry.gdml";
  std::string fgeofile = string(gastpc_dir) + "/src/config/" + fGeometryFile;
  std::cout << "Geometry file :: " << fgeofile << std::endl;

  GasTPCProcessorManager manager;
  manager.initialize(outputfile,fTrackingVerbose);
  
  //Initiate geometry loader
  //GeometryLoader * geomLoader = new GeometryLoader();
  
  // Initialize your processors here. The processors will define
  // default values for all runtime parameters.
  const double pot_per_spill = 7.5e13;
  int nspills = (int)(fPOTSimulated/pot_per_spill);
  LoadNuEventProcessor     *eventLoader  = new LoadNuEventProcessor();
  eventLoader->initialize(inputfile,treename,fgeofile,nspills);
  if(nentries <=  min_entries){
    int temp = min_entries;
    min_entries = nentries;
    nentries = temp;
  }
    
  if(nentries > 0)
    eventLoader->setNEvents(nentries);

  if(min_entries > 0)
    eventLoader->setMinEvents(min_entries);
  
  Geant4TrackingAlgorithm *g4trackingalg = new Geant4TrackingAlgorithm(fgeofile,fSimTargetVolume);
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
  //g4trackingalg->setNEcals(fSimEcalVolume);
  //g4trackingalg->doVis(false);
  //g4trackingalg->doParticleGun(false);
  
  Geant4TrackingProcessor  *g4tracking   = new Geant4TrackingProcessor(g4trackingalg);//geomLoader);
  //g4tracking->initialize();

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
  
  return 0;
}

void ReadDatacard(std::string datacard){

  LOG("GasTPCTracking",pNOTICE) << "Reading configuration from XML file [" << datacard << "]";

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
  fSimEcalVolume         = (int)util::XML2Dbl(xml,root_node,"SimEcalVolume");
  fPOTSimulated          = util::XML2Dbl(xml,root_node,"POTSimulated");

  xml->FreeDoc(xmldoc);
  delete xml;
}
