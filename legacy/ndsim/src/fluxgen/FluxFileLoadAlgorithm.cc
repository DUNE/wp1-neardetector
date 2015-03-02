//____________________________________________________________________________
/*!

\class    FluxFileLoadAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool


\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "FluxFileLoadAlgorithm.hh"

FluxFileLoadAlgorithm::FluxFileLoadAlgorithm()
: entryCount_(0), nEntries_(0), maxNuE_(0), nLoops_(0) {
  initDataCards();
  //using root random number generator 3
  //quick with good randomness
  randGen = new TRandom3(0);	//use 0 to set seed that changes with space and time
}


 FluxFileLoadAlgorithm::~FluxFileLoadAlgorithm(){
  delete randGen;
}

void FluxFileLoadAlgorithm::initDataCards() {
  cards_ = DataCards::getInstance();
  char* LBNO = getenv("LBNO");

  //default root file - should not be removed from data directory
  cards_->AddDataCardString("inputBeamFile", string(LBNO) + "/data/flux/Nu_flux_190213.root");
  cards_->AddDataCardString("inputBeamTree", "Nu_flux");
  cards_->AddDataCardDouble("nuMaxEnergy", 20.*CLHEP::GeV);

  cardsLoaded_ = false;
}

void FluxFileLoadAlgorithm::loadDataCards() {
  inputBeamFileName_ = cards_->fetchValueString("inputBeamFile");
  inputBeamTreeName_ = cards_->fetchValueString("inputBeamTree");
  nIterations_       = cards_->fetchValueInt("nIterations");
  maxNuE_            = cards_->fetchValueDouble("nuMaxEnergy");

  cardsLoaded_ = true;
}

void FluxFileLoadAlgorithm::initialize() {

  inputBeamFile_ = new TFile(inputBeamFileName_.c_str());
  if (!inputBeamFile_->IsOpen()) {
    string mesage = "File "+inputBeamFileName_+" not found.";
    string location = "void FluxFileLoadAlgorithm::initialize()";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }
  inputBeamFile_->GetObject(inputBeamTreeName_.c_str(), inputTree_);
  if ( !inputTree_ ) {
    string mesage = "TTree "+inputBeamTreeName_+" not found in file "+inputBeamFileName_+".";
    string location = "void FluxFileLoadAlgorithm::initialize()";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }
  //inputTree_->Print();
  nEntries_ = inputTree_->GetEntries();

  inputTree_->GetBranch("nuPx")->SetAddress(&nuPx_);
  inputTree_->GetBranch("nuPy")->SetAddress(&nuPy_);
  inputTree_->GetBranch("nuPz")->SetAddress(&nuPz_);
  //inputTree_->GetBranch("nuEn")->SetAddress(&nuEn_);

  inputTree_->GetBranch("nuX")->SetAddress(&parentVx_);
  inputTree_->GetBranch("nuY")->SetAddress(&parentVy_);
  inputTree_->GetBranch("nuZ")->SetAddress(&parentVz_);

  inputTree_->GetBranch("nuPID")->SetAddress(&nuPID_);
  //nuPID_ = 14;
}

void FluxFileLoadAlgorithm::close() {
  inputBeamFile_->Close();
  if(verbose_>0){
	  std::cout << "\n--------------------------------------"
	    << "\nNumber of times iterated through file " << nLoops_
	    << "\n--------------------------------------"
	    << std::endl;
  }
  delete inputBeamFile_;
}

bool FluxFileLoadAlgorithm::getNextEvent(PionDecayEvent* event) {

  //use random variable generator to get a random entry from the file
  int randInt = randGen->Integer(nEntries_);

  //inputTree_->GetEntry(entryCount_);
  if(inputTree_->GetEntry(randInt))inputTree_->GetEntry(randInt);
  else return false;
  
  entryCount_++;

  //need to alter PID codes from root file to PDG format
  // 27 == 14 (numu)
  // 28 == -14 (anumu)
  // 5 == 12 (nue)
  // 6 == -12 (anue)
  if(nuPID_ == 27)nuPDG_ = 14;
  if(nuPID_ == 28)nuPDG_ = -14;
  if(nuPID_ == 5)nuPDG_ = 12;
  if(nuPID_ == 6)nuPDG_ = -12;
  else nuPID_ = nuPDG_;

  event->setNuPDG(nuPDG_);
  event->setChLeptonPDG(0);

  //energies in **GeV**
  nuEn_ = sqrt(nuPx_*nuPx_ + nuPy_*nuPy_ + nuPz_*nuPz_);

  TLorentzVector nuP4(nuPx_*CLHEP::GeV, nuPy_*CLHEP::GeV, nuPz_*CLHEP::GeV, nuEn_*CLHEP::GeV);

  //distances from root file are given in **cm** - convert to **meters**
  //TLorentzVector decayPos(parentVx_*(CLHEP::cm/CLHEP::m), parentVy_*(CLHEP::cm/CLHEP::m), parentVz_*(CLHEP::cm/CLHEP::m), 0.);
  TLorentzVector decayPos(parentVx_*CLHEP::cm, parentVy_*CLHEP::cm, parentVz_*CLHEP::cm, 0.);

  event->setInFlight(true);
  //event->setPosition(decayPos*(CLHEP::m));
  event->setPosition(decayPos*(1/CLHEP::mm));
  event->setNuP4(nuP4);
  if(verbose_>1)event->printToStream(std::cout);

  //if it reaches end of file reset counter
  if(entryCount_ == nEntries_) {
    nLoops_++;
    entryCount_ = 0;
  }

  //std::cout << "\nEntry Count: " << entryCount_;
  //std::cout << "\nNumber of iterations: " << nIterations_;
  //std::cout << "\nNumber of loops: " << nLoops_ << std::endl;

  //if nIterations is reached stop
  //if( (entryCount_ + (nEntries_*nLoops_) - 1) == nIterations_ ) {
  //	return false;
  //}

  return true;
}


