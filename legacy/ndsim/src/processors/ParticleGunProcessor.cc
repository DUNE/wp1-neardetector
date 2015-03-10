//____________________________________________________________________________
/*!

\class    PartileGunProcessor

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Oct 2014
\last update Oct 2014

*/
//____________________________________________________________________________
#include "ParticleGunProcessor.hh"

ParticleGunProcessor::ParticleGunProcessor() {
  name_ = "ParticleGunProcessor";
  geomLoader_ = NULL;
  this->initDataCards();
}

ParticleGunProcessor::ParticleGunProcessor(GeometryLoader * geomLoader) {
  name_ = "ParticleGunProcessor";
  geomLoader_ = geomLoader;
  this->initDataCards();
}

ParticleGunProcessor::~ParticleGunProcessor(){
  geomLoader_ = NULL;
  if(event_){
	delete event_;
  	event_ = NULL;
  }
}

void ParticleGunProcessor::initDataCards() {
  cards_->AddDataCardInt("Geant4TrackingBasketSize", 64000);
  cards_->AddDataCardDouble("PositionX",0.);
  cards_->AddDataCardDouble("PositionY",0.);
  cards_->AddDataCardDouble("PositionZ",0.);
  cards_->AddDataCardDouble("MomentumX",0.);
  cards_->AddDataCardDouble("MomentumY",0.);
  cards_->AddDataCardDouble("MomentumZ",0.);
  cards_->AddDataCardDouble("MaxEnergy",0.1);
  cards_->AddDataCardDouble("MinEnergy",10.);
  cards_->AddDataCardInt("PDG",22);
  cards_->AddDataCardDouble("Mass",0.);

  //create neutrino event
  event_ = new NeutrinoEvent();
}

void ParticleGunProcessor::loadDataCards() {
  outTreeBasketSize_ = cards_->fetchValueInt("Geant4TrackingBasketSize");
  Px_ = cards_->fetchValueDouble("MomentumX");
  Py_ = cards_->fetchValueDouble("MomentumY");
  Pz_ = cards_->fetchValueDouble("MomentumZ");  
  Emax_ = cards_->fetchValueDouble("MaxEnergy");
  Emin_ = cards_->fetchValueDouble("MinEnergy");
  X_ = cards_->fetchValueDouble("PositionX");
  Y_ = cards_->fetchValueDouble("PositionY");
  Z_ = cards_->fetchValueDouble("PositionZ");
  pdg_ = cards_->fetchValueInt("PDG");
  mass_ = cards_->fetchValueDouble("Mass");

  g4tracking_.loadDataCards();
  verbose_ = g4tracking_.getVerbose();
  if(geomLoader_)g4tracking_.setManager(geomLoader_->getManager());
  g4tracking_.initialize();
}

void ParticleGunProcessor::initOutDataTree() {
  file_->cd();
  outData_   = new TTree("Tracking", "MC simulation tracking and hits");
  
  this->makeOutTreeBranches<NeutrinoEvent>(event_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<NeutrinoEvent>(outTreeBasketSize_, event_);
  this->makeOutTreeBranches<SimulData>(MChits_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<SimulData>(outTreeBasketSize_, MChits_);
  this->makeOutTreeBranches<GeantTrackingTruth>(trackingRecord_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<GeantTrackingTruth>(outTreeBasketSize_, trackingRecord_);

}

void ParticleGunProcessor::loadInDataTree() {
}

bool ParticleGunProcessor::process() {

  // set particle to fspl of neutrino event
  SetupEvent();

  if (g4tracking_.Process(event_) ) {
      	this->setHitCollections();
  	this->setTracking();

      	if(g4tracking_.getVerbose()>0)MChits_->printToStream(std::cout);
      	backTracer_.push_back(recordCount_);

	file_->cd();
     	outData_->Fill();
	if(recordCount_ % 100 ==0){
		int bytesWritten2 = outBranch_->FlushBaskets();
		if(g4tracking_.getVerbose()>0)std::cout << "\nBytes written to file: " << bytesWritten2 <<std::endl;
	}
    	recordCount_++;
	MChits_->clearSDHits();
 	trackingRecord_->clearTracks();
  }

  return true;
}

void ParticleGunProcessor::setHitCollections(){

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
  else {
	std::cout << "\n-----------------------------------------------------"
		  << "\nNo sensitive detector set! No SD Hits to be recorded."
		  << "\n-----------------------------------------------------" << std::endl;
  }

}

void ParticleGunProcessor::setTracking(){

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

void ParticleGunProcessor::SetupEvent(){

  ParticleDescrShortRecord particle;

  //determine monoenergetic or energy spectrum 
  if(Emin_ == Emax_){
	E_ = Emax_;
  }
  else{
	TRandom rand(0);
	rand.SetSeed(0);
	E_ = rand.Uniform(Emin_,Emax_);
  }

  //if all 3 momentum are 0 then indicates to use Gaussian
  while( (Px_ == 0) && (Py_ == 0 ) && ( Pz_ == 0) ){
	TRandom3 rand;
	rand.SetSeed(0);
	Px_ = rand.Gaus();
	Py_ = rand.Gaus();
	Pz_ = rand.Gaus();

  }

  //normalise momentum to unit vectors and multiply by energy
  double Mom = TMath::Sqrt( Px_*Px_ + Py_*Py_ + Pz_*Pz_);

  //turn them into unit vectors and multiply by true momentum
  Px_ *= (TMath::Sqrt( E_*E_ - mass_*mass_ ))/Mom;
  Py_ *= (TMath::Sqrt( E_*E_ - mass_*mass_ ))/Mom;
  Pz_ *= (TMath::Sqrt( E_*E_ - mass_*mass_ ))/Mom;
 
  particle.setP4( TLorentzVector(Px_,Py_,Pz_,E_) );
  particle.setPDG(pdg_);
  particle.setMass(mass_);

  event_->setPosition( TLorentzVector(X_,Y_,Z_,0) );
  event_->setFspl(particle);

}
