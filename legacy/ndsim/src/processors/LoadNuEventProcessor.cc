//____________________________________________________________________________
/*!

\class    LoadNuEventProcessor

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Jan 2014
\last update June 2014

*/
//____________________________________________________________________________
#include "LoadNuEventProcessor.hh"

LoadNuEventProcessor::LoadNuEventProcessor() : nIterations_(0){
  name_ = "LoadNuEventProcessor";
  initDataCards();
}

void LoadNuEventProcessor::initOutDataTree() {
  // Create the data tree.
  outData_ = new TTree("NuInteractions", "Neutrino Events");
  //line needed to avoid memory-resident Tree error
  outData_->SetDirectory(0);
  this->makeOutTreeBranches<NeutrinoEvent>(event_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<NeutrinoEvent>(outTreeBasketSize_, event_);

}

void LoadNuEventProcessor::initDataCards() {

  cards_->AddDataCardInt("EventVerbose", 1);
  cards_->AddDataCardBool("qeccOnly", false);
}

void LoadNuEventProcessor::loadDataCards() {

  verbose_           = cards_->fetchValueInt("EventVerbose");
  qeccOnly_	     = cards_->fetchValueBool("qeccOnly");

  loader_.loadDataCards();
  loader_.initialize();
  loader_.setVerbose(verbose_);
}

bool LoadNuEventProcessor::process() {

  backTracer_.resize(0);
  bool done = false;
  bool result = false;

  if ( loader_.getNextEvent() ){ 
        //std::cout << "entry " << outData_->GetEntries() << std::endl;
    	backTracer_.push_back(recordCount_);

	//set the event to the one read from the file
	event_ = loader_.getEvent();
	if(verbose_>0)event_->printToStream(std::cout);
	//ccqe filter for events
	if( !filter_.Process(event_,qeccOnly_) ) return false;
        outData_->Fill();
	//std::cout << "backTracer : " << backTracer_[0] << std::endl;
    	recordCount_++;
    
   	result = true;
  }
  else result = false;

  return result;

}




