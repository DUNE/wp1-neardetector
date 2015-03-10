//____________________________________________________________________________
/*!

\class    Geant4TrackingProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#include "Geant4TrackingProcessor.hh"

Geant4TrackingProcessor::Geant4TrackingProcessor() {
  name_ = "Geant4TrackingProcessor";
  geomLoader_ = NULL;
  this->initDataCards();
}

Geant4TrackingProcessor::Geant4TrackingProcessor(GeometryLoader * geomLoader) {
  name_ = "Geant4TrackingProcessor";
  geomLoader_ = geomLoader;
  this->initDataCards();
}

Geant4TrackingProcessor::~Geant4TrackingProcessor(){
  geomLoader_ = NULL;
  //if(outData_)delete outData_;
}

void Geant4TrackingProcessor::initDataCards() {
  cards_->AddDataCardInt("Geant4TrackingBasketSize", 64000);
  cards_->AddDataCardBool("qeccOnly", false);
}

void Geant4TrackingProcessor::loadDataCards() {
  outTreeBasketSize_ = cards_->fetchValueInt("Geant4TrackingBasketSize");
  qeccOnly_	     = cards_->fetchValueBool("qeccOnly");
  g4tracking_.loadDataCards();
  verbose_ = g4tracking_.getVerbose();
  if(geomLoader_)g4tracking_.setManager(geomLoader_->getManager());
  g4tracking_.initialize();
  //set if gun is on or not
  setGunOption(g4tracking_.isGunOn());
}

void Geant4TrackingProcessor::initOutDataTree() {
  file_->cd();
  outData_   = new TTree("Tracking", "MC simulation tracking and hits");

  this->makeOutTreeBranches<SimulData>(MChits_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<SimulData>(outTreeBasketSize_, MChits_);
  this->makeOutTreeBranches<GeantTrackingTruth>(trackingRecord_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<GeantTrackingTruth>(outTreeBasketSize_, trackingRecord_);
}

void Geant4TrackingProcessor::loadInDataTree() {
  this->setInTreeBranches<NeutrinoEvent>(event_);
}

bool Geant4TrackingProcessor::process() {if(gunOn_!=1){
	inData_->GetEntry(precessCount_);
  	precessCount_++;

  	bool good = false;
  	good = filter_.Process(event_,qeccOnly_);
  	if (good) {
    		if (g4tracking_.Process(event_) ) {
      		this->setHitCollections();
  		this->setTracking();
      		if(g4tracking_.getVerbose()>0)MChits_->printToStream(std::cout);
      		//g4ranger_.Process(g4tracking_.getSteppingAction()->getHit());
		//g4tracking_.getSteppingAction()->unsetHit();
      		backTracer_.push_back(recordCount_);
     		//std::cout << "backTracer : " << backTracer_[0] << ", " << backTracer_[1]
     		//<< ", " << backTracer_[2] << std::endl;
     		//outBranch_->Fill();
		file_->cd();
     		outData_->Fill();
		if(recordCount_ % 100 ==0){
			//Long64_t bytesWritten = outData_->AutoSave("FlushBaskets");
			int bytesWritten2 = outBranch_->FlushBaskets();
			if(g4tracking_.getVerbose()>0)std::cout << "\nBytes written to file: " << bytesWritten2 <<std::endl;
			//outBranch_->Print();
		}
     		recordCount_++;
		MChits_->clearSDHits();
 		trackingRecord_->clearTracks();
    		}
	}
  }
  else {
	if (g4tracking_.Process() ) {
      		this->setHitCollections();
		//MChits_->setSDHit(*SD_hits_); 
      		if(g4tracking_.getVerbose()>0)MChits_->printToStream(std::cout);
		file_->cd();
     		outData_->Fill();
     		recordCount_++;
		MChits_->clearSDHits();
	}
  }
  return true;/*
  if(gunOn_!=1){
	inData_->GetEntry(precessCount_);
  	precessCount_++;

  	bool good = false;
  	good = filter_.Process(event_,qeccOnly_);
  	if (good) setAndFill();
  }
  else setAndFill();

  return true;*/
}

void Geant4TrackingProcessor::setAndFill(){

  //set data for output to file
  this->setHitCollections();
  this->setTracking();

  if(gunOn_!=1)backTracer_.push_back(recordCount_);
  //std::cout << "backTracer : " << backTracer_[0] << ", " << backTracer_[1]
  //<< ", " << backTracer_[2] << std::endl;

  //outputs
  MChits_->printToStream(std::cout);
  //trackingRecord_->printToStream(std::cout);

  // fill tree
  file_->cd();
  outData_->Fill();
  if(recordCount_ % 100 ==0){
	//Long64_t bytesWritten = outData_->AutoSave("FlushBaskets");
	int bytesWritten2 = outBranch_->FlushBaskets();
	std::cout << "\nBytes written to file: " << bytesWritten2 <<std::endl;
	//outBranch_->Print();
  }
  recordCount_++;
  MChits_->clearSDHits();
  trackingRecord_->clearTracks();

}

void Geant4TrackingProcessor::setHitCollections(){

  MChits_->setEventID(recordCount_);

  if(g4tracking_.getDetectorConstruction()->GetValueUsingRootGeom() == 0) { 
    	if(g4tracking_.getDetectorConstruction()->getTpcHitCollection()){
		MChits_->setTpcFidHits(*g4tracking_.getDetectorConstruction()->getTpcHitCollection());
	}
 	if(g4tracking_.getDetectorConstruction()->getScintHitCollection()){
		MChits_->setScintHits(*g4tracking_.getDetectorConstruction()->getScintHitCollection());
	}
	//clean up the hits after
	g4tracking_.getDetectorConstruction()->clearHitPtrs();
	}
/*
  else if(g4tracking_.getDetectorConstruction()->GetValueUsingRootGeom() == 1) {     
    	//g4tracking_.getPostDetectorConstruction()->setSDHitPtr(SD_hits_);
  }*/
  else {
	std::cout << "\n-----------------------------------------------------"
		  << "\nNo sensitive detector set! No SD Hits to be recorded."
		  << "\n-----------------------------------------------------" << std::endl;
  }

}

void Geant4TrackingProcessor::setTracking(){

  if(g4tracking_.getSteppingAction()->getPrimariesVtrPtr()){
	trackingRecord_->setPrimaries(*g4tracking_.getSteppingAction()->getPrimariesVtrPtr());
  }
  if(g4tracking_.getSteppingAction()->getInParticlesVtrPtr()){
	trackingRecord_->setInParticles(*g4tracking_.getSteppingAction()->getInParticlesVtrPtr());
  }
  if(g4tracking_.getSteppingAction()->getOutParticlesVtrPtr()){
	trackingRecord_->setOutParticles(*g4tracking_.getSteppingAction()->getOutParticlesVtrPtr());
  }

  g4tracking_.getSteppingAction()->ClearTrackVtrs();
}


