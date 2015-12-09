//____________________________________________________________________________
/*!

\class    Geant4TrackingProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jun 2015
*/
//____________________________________________________________________________
#include "Geant4TrackingProcessor.hh"

Geant4TrackingProcessor::Geant4TrackingProcessor(Geant4TrackingAlgorithm *g4tracking) {
  name_ = "Geant4TrackingProcessor";
  outTreeBasketSize_ = 64000;
  g4tracking_ = g4tracking;
  // Initialize
  g4tracking_->initialize();
}

Geant4TrackingProcessor::~Geant4TrackingProcessor(){
  //if(outData_)delete outData_;
  delete event_;
  delete MChits_;
  delete trackingRecord_;
  delete g4tracking_;
}

void Geant4TrackingProcessor::initOutDataTree() {
  //file_->cd();
  outData_   = new TTree("Tracking", "MC simulation tracking and hits");
  
  this->makeOutTreeBranches<SimulData>(MChits_);
  if (outTreeBasketSize_)
    this->setOutTreeBasketSize<SimulData>(outTreeBasketSize_, MChits_);

  this->makeOutTreeBranches<GeantTrackingTruth>(trackingRecord_);
  if (outTreeBasketSize_)
    this->setOutTreeBasketSize<GeantTrackingTruth>(outTreeBasketSize_, trackingRecord_);

  // line needed to avoid memory-resident Tree error
  outData_->SetDirectory(0);
}

void Geant4TrackingProcessor::loadInDataTree() {
  this->setInTreeBranches<NeutrinoEvent>(event_);
}

bool Geant4TrackingProcessor::process() {
  inData_->GetEntry(precessCount_);
  precessCount_++;

  if (g4tracking_->Process(event_) ) {
    this->setHitCollections();
    this->setTracking();

    if(g4tracking_->getVerbose()>0)
      MChits_->printToStream(std::cout);

    //g4ranger_.Process(g4tracking_.getSteppingAction()->getHit());
    //g4tracking_.getSteppingAction()->unsetHit();

    //backTracer_.push_back(recordCount_);

    outData_->Fill();
    
    recordCount_++;
    MChits_->clearSDHits();
    trackingRecord_->clearTracks();
  }

  return true;
}

void Geant4TrackingProcessor::setHitCollections(){
  MChits_->setEventID(recordCount_);

  if(g4tracking_->getDetectorConstruction()->GetValueUsingRootGeom() == 0) {
    // TPC
    if(g4tracking_->getDetectorConstruction()->getTpcHitCollection()){
      MChits_->setTpcFidHits(*g4tracking_->getDetectorConstruction()->getTpcHitCollection());
    }
    // ECAL
    if(g4tracking_->getDetectorConstruction()->getScintHitCollection()){
      MChits_->setScintHits(*g4tracking_->getDetectorConstruction()->getScintHitCollection());
    }
    /*
    int ecalhitsets =  g4tracking_->getDetectorConstruction()->getNumberOfScintHitCollection();
    for(int i=0;i<ecalhitsets;i++){
      if(g4tracking_->getDetectorConstruction()->getScintHitCollection(i)){
	MChits_->setScintHits(*g4tracking_->getDetectorConstruction()->getScintHitCollection(i));
      }
    }
    */
    //clean up the hits after
    g4tracking_->getDetectorConstruction()->clearHitPtrs();
  }
  else {
    std::cout << "\n-----------------------------------------------------"
	      << "\nNo sensitive detector set! No SD Hits to be recorded."
	      << "\n-----------------------------------------------------" << std::endl;
  }
}

void Geant4TrackingProcessor::setTracking(){
  
  //if(g4tracking_->getSteppingAction()->getPrimariesVtrPtr()){
  trackingRecord_->setPrimaries(g4tracking_->getSteppingAction()->getPrimariesVtrPtr());
  //}
  

  /*
  if(g4tracking_.getSteppingAction()->getInParticlesVtrPtr()){
    trackingRecord_->setInParticles(*g4tracking_.getSteppingAction()->getInParticlesVtrPtr());
  }
  if(g4tracking_.getSteppingAction()->getOutParticlesVtrPtr()){
    trackingRecord_->setOutParticles(*g4tracking_.getSteppingAction()->getOutParticlesVtrPtr());
  }
  */
  
  //if(g4tracking_->getSteppingAction()->getAllParticlesVtrPtr()){
  trackingRecord_->setAllParticles(g4tracking_->getSteppingAction()->getAllParticlesVtrPtr());
  //}
  
  g4tracking_->getSteppingAction()->ClearTrackVtrs();
}


