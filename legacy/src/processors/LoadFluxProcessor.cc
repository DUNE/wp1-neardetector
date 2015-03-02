//____________________________________________________________________________
/*!

\class    LoadFluxProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#include "LoadFluxProcessor.hh"

LoadFluxProcessor::LoadFluxProcessor(GeometryLoader * geomLoader) : nEntries(0), iterationCount(0),piDecay_(NULL){
  name_ = "LoadFluxProcessor";
  geomLoader_ = geomLoader;
  initDataCards();
  //for memory check
  totbytes=0;
}

LoadFluxProcessor::~LoadFluxProcessor(){

  delete piDecay_;
}

void LoadFluxProcessor::initOutDataTree() {
  piDecay_ = new PionDecayEvent();
  file_->cd();
  // Create the data tree.
  outData_ = new TTree("NuHits", "Neutrino Hits");
  //outData_->SetAutoSave(3000000);
  this->makeOutTreeBranches<NeutrinoHit>(neutrino_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<NeutrinoHit>(outTreeBasketSize_, neutrino_);

}

void LoadFluxProcessor::initDataCards() {

  cards_->AddDataCardInt("loadFluxBasketSize", 640000);
  cards_->AddDataCardInt("fluxVerbose", 1);
  cards_->AddDataCardDouble("fluxFileExposure", 212000.0);	//number of p.o.t for flux file
}

void LoadFluxProcessor::loadDataCards() {

  verbose_           = cards_->fetchValueInt("fluxVerbose");
  outTreeBasketSize_ = cards_->fetchValueInt("loadFluxBasketSize");
  fileExposure_      = cards_->fetchValueDouble("fluxFileExposure");

  loader_.loadDataCards();
  loader_.initialize();
  loader_.setVerbose(verbose_);
  //set number of entries from input flux file
  nEntries = loader_.getEntries();
  projector_.loadDataCards();
  projector_.setOrigin( geomLoader_->getZRange()/2.0 );
  projector_.setVerbose(verbose_);
}

bool LoadFluxProcessor::process() {

  backTracer_.resize(0);
  bool done = false;
  bool result = false;

  if ( loader_.getNextEvent(piDecay_) ){ 
	iterationCount++;
	if( projector_.Process(piDecay_, neutrino_) ){
        	//std::cout << "entry " << outData_->GetEntries() << std::endl;
    		backTracer_.push_back(recordCount_);
        	outData_->Fill();
		//std::cout << "backTracer : " << backTracer_[0] << std::endl;
    		recordCount_++;
    
   		result = true;
	}
   	result = true;
  }
  else result = false;
/*
  int writeStatus = 0;
  if(recordCount_ % 10000){
	//outData_->Write("", TObject::kOverwrite);
	writeStatus = outData_->FlushBaskets();
	//outData_->DropBaskets();
  }
*/
  return result;

}




