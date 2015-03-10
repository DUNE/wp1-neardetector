//____________________________________________________________________________
/*!

\class    GenieNuEventAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "GenieNuEventAlgorithm.hh"

#include <fstream>
#include <TSystem.h>

#include <TGeoManager.h>

GenieNuEventAlgorithm::GenieNuEventAlgorithm()
: geom_anal_(0), genie_event_(0), ievent_(0), interactionCount(0),condition_(false){

  resetVolumeCounts();
  initDataCards();

}

GenieNuEventAlgorithm::~GenieNuEventAlgorithm() {

  //delete xspl_;
  if ( GasTPC_geom_driver_ )    {delete GasTPC_geom_driver_;};
  if ( root_geom_ )           {delete root_geom_;};
  if ( GasTPC_flux_driver_ )    {delete GasTPC_flux_driver_;};
  if ( mcj_driver_ )          {delete mcj_driver_;};
  if ( ntpw_ ){ 
	ntpw_->Save(); 
	delete ntpw_;
	}
  if ( mcjmonitor_ )          {delete mcjmonitor_;};
  if ( genie_event_ )         {delete genie_event_;};

}

void GenieNuEventAlgorithm::initialize() {

  this->initRandGen();
  this->initGenieMessenger();
  this->initSplines();
  this->initGenieGeomDriver();
  this->initGenieFluxDriver();
  //this->initGenieHstFluxDriver("../data/nufl_-002300_GLOBES_100Km.txt");
  this->initGenieMCjobDriver();
  this->initGenieNtpWriter();         //--this will only create a genie-mc-status file, not needed
  this->initGenieMCJMonitor();

  std::cout << "\nInitializing...\n"; 

}

void GenieNuEventAlgorithm::initDataCards() {

  cards_ = DataCards::getInstance();
  cards_->AddDataCardInt("runNumber", 0);
  cards_->AddDataCardInt("eventVerbose", 1);
  cards_->AddDataCardBool("wantGntp",0);
  char* GasTPC = getenv("GasTPC");
  cards_->AddDataCardString("GenieXSecSplines", string(GasTPC) + "/data/splines/gxspl-freenuc-v2.8.4.xml");

  cardsLoaded_ = false;

  cout << "\nInitializing data cards...\n";
}

void GenieNuEventAlgorithm::loadDataCards() {

  runNumber_       = cards_->fetchValueInt("runNumber");
  verbose_         = cards_->fetchValueInt("eventVerbose");
  xSecSplines_     = cards_->fetchValueString("GenieXSecSplines");
  wantGntp_        = cards_->fetchValueBool("wantGntp");

  cardsLoaded_ = true;

  cout << "\nLoading data cards...\n";
}

bool GenieNuEventAlgorithm::GenieNuEventAlgorithm::Process(NeutrinoHit* hit,
                                                           NeutrinoEvent* event) {
  if(!cardsLoaded_) {
    this->loadDataCards();
    this->initialize();
  }

  this->getFluxDriver()->setNeutrino(hit);

  EventRecord* gEvent = makeGenieNuEvent();

  if (gEvent) {

    this->convert(gEvent, event);
    return true;
  }

  return false;
}

void GenieNuEventAlgorithm::initSplines() {

  //utils::app_init::XSecTable(xSecSplines_, false);

  xspl_ = XSecSplineList::Instance();
  //xspl_->AutoLoad();
  xspl_->LoadFromXml(xSecSplines_,false);

  cout << "\nInitializing splines...\n";
}

void GenieNuEventAlgorithm::initRandGen() {

  // Set seed if not set by GSEED
  randGen_ = RandomGen::Instance();  // get random generator initialized

  randInt_ = rand() % 100000 + 1; //generate random number for seed
  if ( gSystem->Getenv("GSEED") == 0 ) randGen_->SetSeed(randInt_);   //set to some random number

  std::cout << "\nInitializing random generator seed...\n";
}

void GenieNuEventAlgorithm::initGenieGeomDriver() {

  GasTPC_geom_driver_ = new GasTPCGenieGeom(geoMgr_);
  GasTPC_geom_driver_->initialize();
  geom_anal_ = GasTPC_geom_driver_->getGeomAnalyzer();

  std::cout << "\nLoading geometry driver...\n";
}

void GenieNuEventAlgorithm::initGenieFluxDriver() {

  GasTPC_flux_driver_ = new GasTPCGenieFlux();

  std::cout << "\nLoading flux driver...\n";
}

void GenieNuEventAlgorithm::initGenieHstFluxDriver(std::string fluxFile) {

  double E, nu_e, nu_mu, nu_t, anu_e, anu_mu, anu_t;
  std::ifstream stream(fluxFile.c_str());

  nu_e_h = new TH1D("nu_e_flux", "nu_e flux", 501, 0.05, 50.15);
  anu_e_h = new TH1D("anu_e_flux", "anu_e flux", 501, 0.05, 50.15);
  nu_mu_h = new TH1D("nu_mu_flux", "nu_mu flux", 501, 0.05, 50.15);
  anu_mu_h = new TH1D("anu_mu_flux", "anu_mu flux", 501, 0.05, 50.15);

  if (!stream) {
    string mesage = "Flux file " + fluxFile + " not found.";
    string location = "GenieNuEventAlgorithm::initGenieHstFluxDriver";
    throw GasTPCException(mesage, location, GasTPCException::FATAL);
  }

  while (!stream.eof()) {
    stream >> E >> nu_e >> nu_mu >> nu_t >> anu_e >> anu_mu >> anu_t;
    nu_e_h->Fill(E, nu_e);
    nu_mu_h->Fill(E, nu_mu);
    anu_e_h->Fill(E, anu_e);
    anu_mu_h->Fill(E, anu_mu);
  }

  // creating & configuring a generic GCylindTH1Flux flux driver
  hst_flux_driver_ = 0;

  TVector3 bdir (0,0,1); // dir along +z
  TVector3 bspot(0,0,0);
  hst_flux_driver_ = new flux::GCylindTH1Flux;
  hst_flux_driver_->SetNuDirection      (bdir);
  hst_flux_driver_->SetBeamSpot         (bspot);
  hst_flux_driver_->SetTransverseRadius (-1);

  hst_flux_driver_->AddEnergySpectrum(12, nu_e_h);
  hst_flux_driver_->AddEnergySpectrum(-12, anu_e_h);
  hst_flux_driver_->AddEnergySpectrum(14, nu_mu_h);
  hst_flux_driver_->AddEnergySpectrum(-14, anu_mu_h);

  // casting to the GENIE flux driver interface
  flux_driver_ = dynamic_cast<GFluxI *> (hst_flux_driver_);
}

void GenieNuEventAlgorithm::initGenieMCjobDriver() {

  std::cout << "\nInitilizing GenieMCjobDriver...\n";

  // Create the monte carlo job driver
  mcj_driver_ = new GMCJDriver;
  mcj_driver_->UseFluxDriver(GasTPC_flux_driver_);
  //mcj_driver_->UseFluxDriver(hst_flux_driver_);
  mcj_driver_->UseGeomAnalyzer(geom_anal_);
  mcj_driver_->Configure();
  mcj_driver_->UseSplines();
  mcj_driver_->ForceSingleProbScale();
}

void GenieNuEventAlgorithm::initGenieNtpWriter() {

  if(!wantGntp_){
	ntpw_ = NULL;
 	return;
  }

  //-- initialize an Ntuple Writer
  ntpw_ = new NtpWriter(kNFGHEP, runNumber_);
  ntpw_->Initialize();

  std::cout << "\nInitilizing ntpWriter...\n";
}

void GenieNuEventAlgorithm::initGenieMCJMonitor() {
//-- create an MC Job Monitor
  mcjmonitor_ = NULL;
  std::cout << "\nInitilizing GenieMCJMonitor...\n";
  mcjmonitor_ = new GMCJMonitor(runNumber_);
}

void GenieNuEventAlgorithm::initGenieMessenger() {
  //-- get a messanger instance and set the 
  Messenger *msg = Messenger::Instance();
  msg->SetPriorityLevel("GHEP", pERROR);
  msg->SetPriorityLevel("XSecSplLst", pERROR);
}

EventRecord* GenieNuEventAlgorithm::makeGenieNuEvent() {

  if(genie_event_) {
    delete genie_event_;
    genie_event_ = NULL;
  }

  //regenerate seed from random number
  randInt_ = rand() % 100000 + 1; 
  //std::cout << "\nSetting seed from random number: " << randInt_ << std::endl;
  randGen_->SetSeed(randInt_);   

  //if(verbose_>3)std::cout << "\nGenerating genie event number: " << ievent_ << std::endl;
 
  genie_event_ = mcj_driver_->GenerateEvent();

  // add event at the output ntuple, refresh the mc job monitor & clean-up
  if (genie_event_){
	 if(wantGntp_){
		ntpw_->AddEventRecord(ievent_, genie_event_); //-- do you also want the gntp genie file?
  	 	//mcjmonitor_->Update(ievent_, genie_event_); //--this will only create a genie-mc-status file
	 }
	 interactionCount++;
	 if(verbose_>1)std::cout << "\nNeutrino Interaction!";
  	 ievent_++;
	 condition_ = true;
  }
  else {

	 //if(verbose_>3)std::cout << "\nNULL event - no interaction, trying again...";
	 condition_ = false;
  } 

  return genie_event_;
}

void GenieNuEventAlgorithm::convert(EventRecord* gEvent, NeutrinoEvent* event) {

  const ProcessInfo& procInfo = gEvent->Summary()->ProcInfo();
  const InitialState& initState = gEvent->Summary()->InitState();

  int scTypeId  = procInfo.ScatteringTypeId();
  int intTypeId = procInfo.InteractionTypeId();

  bool qelcc = procInfo.IsQuasiElastic() && procInfo.IsWeakCC();

  if(verbose_>2)gEvent->Print(std::cout);

  event->setQELCC(qelcc);
  event->setScatteringType(scTypeId);
  event->setInteractionType(intTypeId);
  event->setNuEnergy(gEvent->Probe()->Energy()*CLHEP::GeV);
  //string scType_s  = procInfo.ScatteringTypeAsString();
  //string intType_s = procInfo.InteractionTypeAsString();

/*
  nInteractions_++;
  if (procInfo.IsWeakCC())  nInteractionsCC_++;
  if (procInfo.IsWeakNC())  nInteractionsNC_++;
  if (procInfo.IsWeakMix()) nInteractionsMix_++;
*/
  TLorentzVector vertex = *( gEvent->Vertex() );
  vertex *= CLHEP::m;

  std::string nodeName("");

  //std::cout << "Neutrino Interaction Vertex (cm) " << vertex.X()/CLHEP::cm << " " << vertex.Y()/CLHEP::cm
  //<< " " << vertex.Z()/CLHEP::cm << std::endl;
  
  TGeoNode * node = geoMgr_->FindNode(vertex.X()/CLHEP::cm,
                                   vertex.Y()/CLHEP::cm,
                                   vertex.Z()/CLHEP::cm);

  if(node)nodeName = node->GetName();
  //std::cout << "In Node " << nodeName << std::endl << std::endl;

  //check which volume it is in.
  geomVolumeCounter.findVolume(nodeName);

  event->setNodeName(nodeName);
  event->setPosition(vertex);
 
  
  if ( gEvent->FinalStatePrimaryLepton()->Pdg() != 0 ) {
    TLorentzVector fspl_p4 = *( gEvent->FinalStatePrimaryLepton()->P4() );
    fspl_p4 *= CLHEP::GeV;
    int fspl_pdg = gEvent->FinalStatePrimaryLepton()->Pdg();
    ParticleDescrShortRecord fspl(fspl_pdg, fspl_p4);
    double fspl_mass = ( gEvent->FinalStatePrimaryLepton()->Mass());
    fspl_mass *= CLHEP::GeV;
    fspl.setMass(fspl_mass);
    event->setFspl(fspl);
  }

  if( gEvent->HitNucleon() ) {
    TLorentzVector nuc_p4 = *( gEvent->HitNucleon()->P4() );
    nuc_p4 *= CLHEP::GeV;
    int nuc_pdg = gEvent->HitNucleon()->Pdg();
    ParticleDescrShortRecord nuc(nuc_pdg, nuc_p4);
    event->setHitNucleon(nuc);
  }

  //clear previous fss vector
  event->clearFssVector();
  
  int Npart = gEvent->GetEntries();
  int ipart=0;
  
  //loop over particles generated for each neutrino event
  while (ipart<Npart) {
    GHepParticle *gParticle = gEvent->Particle(ipart);
    int status = gParticle->Status();
    
    //only count final state particles
    if (status != kIStStableFinalState) {
      ipart++;
      continue;
    }

    //dont count fspl twice
    if(ipart == gEvent->FinalStatePrimaryLeptonPosition()){
      ipart++;
      continue;
    }

    int pdg = gParticle->Pdg();
    TLorentzVector p4 = *( gParticle->P4() );
    p4 *= CLHEP::GeV;
    ParticleDescrShortRecord p(pdg, p4);
    double fss_mass = gParticle->Mass();
    fss_mass *= CLHEP::GeV;
    p.setMass(fss_mass);
    
    event->addFss(p);
    ipart++;
  }

  //print to stream
  if(verbose_>0)event->printToStream(std::cout);

  //volume region output
  if(verbose_>1)geomVolumeCounter.printToStream(std::cout);
}
void GenieNuEventAlgorithm::resetVolumeCounts(){

  geomVolumeCounter.resetCounts();
}
