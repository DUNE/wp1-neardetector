//____________________________________________________________________________
/*!

\class    LoadNuEventProcessor

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Jan 2014
\last update Apr 2015

*/
//____________________________________________________________________________
#include "LoadNuEventProcessor.hh"

LoadNuEventProcessor::LoadNuEventProcessor(){
  name_ = "LoadNuEventProcessor";
}

void LoadNuEventProcessor::initOutDataTree() {
  // Create the data tree.
  outData_ = new TTree("NuInteractions", "Neutrino Events");
  //line needed to avoid memory-resident Tree error
  outData_->SetDirectory(0);
  this->makeOutTreeBranches<NeutrinoEvent>(event_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<NeutrinoEvent>(outTreeBasketSize_, event_);

}

void LoadNuEventProcessor::initialize(std::string inputFileName, TTree *inputTree, std::string inputGeom){
  verbose_   = 1;
  maxevents_ = -1;
  loader_.initialize(inputFileName, inputTree, inputGeom);
  maxevents_ =  loader_.getNumberOfEvents();
  loader_.setVerbose(verbose_);
}

void LoadNuEventProcessor::initialize(std::string inputFileName, std::string inputTreeName, std::string inputGeom){
  verbose_   = 1;
  maxevents_ = -1;
  loader_.initialize(inputFileName, inputTreeName, inputGeom);
  maxevents_ =  loader_.getNumberOfEvents();
  loader_.setVerbose(verbose_);
}

bool LoadNuEventProcessor::process() {

  backTracer_.resize(0);
  bool done = false;
  bool result = false;

  if ( loader_.getNextEvent()){ 
    backTracer_.push_back(recordCount_);
    //set the event to the one read from the file
    //event_ = loader_.getEvent();
    if(loader_.getTreeName().find("gst") != std::string::npos)
      event_ = loader_.getGstEvent(recordCount_);// George
    else if(loader_.getTreeName().find("gtree") != std::string::npos)
      event_ = loader_.getGHepEvent(recordCount_);// George
    else if(loader_.getTreeName().find("pg") != std::string::npos)
      event_ = loader_.getPGEvent(recordCount_);// George

    if(verbose_>2)event_->printToStream(std::cout);
    outData_->Fill();
    recordCount_++;
    
    result = true;
  }
  else result = false;

  return result;

}




