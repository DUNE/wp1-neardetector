//____________________________________________________________________________
/*!

\class    NuEventFileLoadAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#include "NuEventFileLoadAlgorithm.hh"

NuEventFileLoadAlgorithm::NuEventFileLoadAlgorithm()
: entryCount_(0), maxNuE_(0), nuHits_nEntries(0), nuEvents_nEntries(0),exposure(0){

  backTracerBuff_ = &backTracer_;
  initDataCards();
}


 NuEventFileLoadAlgorithm::~NuEventFileLoadAlgorithm(){
  delete randGen;
}

void NuEventFileLoadAlgorithm::initDataCards() {
  cards_ = DataCards::getInstance();
  char* LBNO = getenv("LBNO");

  //default root file - should not be removed from data directory
  cards_->AddDataCardString("inputNuEventFile", string(LBNO) + "/workdir/primaries.root");
  cards_->AddDataCardString("inputNuEventTree", "NuInteractions");
  cards_->AddDataCardString("inputGeomFile", "");	//leave blank for it to be read from the data card in the input file
  cards_->AddDataCardDouble("nuMaxEnergy", 30.*CLHEP::GeV);
  cards_->AddDataCardInt("runNumber",-1);

  cardsLoaded_ = false;
}

void NuEventFileLoadAlgorithm::loadDataCards() {
  inputFileName_ 	= cards_->fetchValueString("inputNuEventFile");
  inputTreeName_ 	= cards_->fetchValueString("inputNuEventTree");
  inputGeom_ 		= cards_->fetchValueString("inputGeomFile");
  runNumber_		= cards_->fetchValueInt("runNumber");
  maxNuE_           	= cards_->fetchValueDouble("nuMaxEnergy");

  cardsLoaded_ = true;
}

void NuEventFileLoadAlgorithm::initialize() {

  std::stringstream ss;
  ss << runNumber_;
  std::string runNumberAsstring = ss.str();
  std::string runStatsName = "runStats";
  std::string dataCardName = "dataCards";

  //load the file to read
  inputFile_ = new TFile(inputFileName_.c_str());
  if (!inputFile_->IsOpen()) {
    string mesage = "File "+inputFileName_+" not found.";
    string location = "void NuEventFileLoadAlgorithm::initialize()";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }
 
  if(runNumber_>=0){
	inputTreeName_ += "_run" + runNumberAsstring;
	runStatsName += "_run" + runNumberAsstring;
	dataCardName += "_run" + runNumberAsstring;
  }

  inputFile_->GetObject(inputTreeName_.c_str(), inputTree_);
  if ( !inputTree_ ) {
    string mesage = "TTree "+inputTreeName_+" not found in file "+inputFileName_+".";
    string location = "void NuEventFileLoadAlgorithm::initialize()";
    throw LbnoException(mesage, location, LbnoException::FATAL);
  }
  this->setDataAddress<NeutrinoEvent>(nuEvent_, inputTree_);
  nuEvents_nEntries = inputTree_->GetEntries();

  //get run statistics
  if(inputFile_->Get(runStatsName.c_str())){
	stats_ = dynamic_cast<RunStats*>(inputFile_->Get(runStatsName.c_str()));
	stats_->printToStream(std::cout);
  	map<string,string,ci_less> statsInfo = stats_->getInfoMap();
  	map<string,string>::const_iterator it;

	//loop over the stats entries
	for (it = statsInfo.begin(); it != statsInfo.end(); it++) {
	  std::string statName = it->first;
	  std::size_t foundExposure = statName.find("Real Exposure");
	  if(foundExposure!=std::string::npos){
		std::stringstream ss_scientific(it->second);
  		ss_scientific.precision(2);
  		ss_scientific>> scientific >> realExposure_;
	  }
	  std::size_t foundProbScale = statName.find("Global Prob Scale");
	  if(foundProbScale!=std::string::npos){
		std::stringstream ss_scientific(it->second);
  		ss_scientific.precision(2);
  		ss_scientific>> scientific >> probScale_;
	  }
	}
    }
    else{
	stats_ = NULL;
	probScale_ = 1;
	realExposure_ = 0;
    }
  
 //get data cards
  if(inputFile_->Get(dataCardName.c_str())){
	dataCard_ = dynamic_cast<DataCardsRecord*>(inputFile_->Get(dataCardName.c_str()));
        if(verbose_>2)dataCard_->printToStream(std::cout);
	exposure = dataCard_->fetchValueDouble("exposure");
        if(inputGeom_ == "")inputGeom_ = dataCard_->fetchValueString("geomFileName");
        
  }
  else{
        dataCard_ = NULL;
	exposure = 0;
  }

}

void NuEventFileLoadAlgorithm::close() {
  inputFile_->Close();
  if(verbose_>0){
	  std::cout << "\n--------------------------------------"
	    << "\nClosing " << inputFileName_ << " file"
	    << "\n--------------------------------------"
	    << std::endl;
  }
}

NeutrinoEvent* NuEventFileLoadAlgorithm::getEvent(int event) {

  if(inputTree_->GetEntry(event)){
	inputTree_->GetEntry(event);
	if(verbose_>0){
	  std::cout << "\n=================================="
	    << "\nEvent: " << event
	    << "\n=================================="
	    << std::endl;
	}
	if(verbose_>2)nuEvent_->printToStream(std::cout);
	return nuEvent_;
  }
  else return NULL;

}

bool NuEventFileLoadAlgorithm::getNextEvent(){

  if(entryCount_>=nuEvents_nEntries)return false;

  if(inputTree_->GetEntry(entryCount_)){
	inputTree_->GetEntry(entryCount_);
	entryCount_++;
	return true;
  }
  else return false;
}

