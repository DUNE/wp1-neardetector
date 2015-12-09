//____________________________________________________________________________
/*!

\class    LoadNuEventProcessor

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Jan 2014
\last update Oct 2015

*/
//____________________________________________________________________________
#include "LoadNuEventProcessor.hh"

#include <Ntuple/NtpMCEventRecord.h>
#include <Ntuple/NtpMCTreeHeader.h>
#include <EVGCore/EventRecord.h>

LoadNuEventProcessor::LoadNuEventProcessor(){
  name_ = "LoadNuEventProcessor";
}

LoadNuEventProcessor::~LoadNuEventProcessor(){
  delete mcrec_;
  delete event_;
  loader_.close();
}

void LoadNuEventProcessor::initOutDataTree() {
  outData_ = new TTree("NuInteractions", "Neutrino Events");

  this->makeOutTreeBranches<NeutrinoEvent>(event_);
  if (outTreeBasketSize_)this->setOutTreeBasketSize<NeutrinoEvent>(outTreeBasketSize_, event_);

  // line needed to avoid memory-resident Tree error
  outData_->SetDirectory(0);
}

void LoadNuEventProcessor::initialize(std::string inputFileName, TTree *inputTree, std::string inputGeom){
  verbose_   = 1;
  maxevents_ = -1;
  minevents_ = -1;
  loader_.initialize(inputFileName, inputTree, inputGeom);
  maxevents_ =  loader_.getNumberOfEvents();
  loader_.setVerbose(verbose_);
}

void LoadNuEventProcessor::initialize(std::string inputFileName, std::string inputTreeName, std::string inputGeom){
  verbose_   = 1;
  maxevents_ = -1;
  minevents_ = -1;
  loader_.initialize(inputFileName, inputTreeName, inputGeom);
  maxevents_ =  loader_.getNumberOfEvents();
  loader_.setVerbose(verbose_);
}

bool LoadNuEventProcessor::process() {

  //backTracer_.resize(0);

  if ( loader_.getNextEvent()){ 
    backTracer_++;
    //backTracer_.push_back(recordCount_);
    //set the event to the one read from the file
    //event_ = loader_.getEvent();
    if(loader_.getTreeName().find("gst") != std::string::npos)
      event_ = loader_.getGstEvent(recordCount_);
    else if(loader_.getTreeName().find("gtree") != std::string::npos){
      event_ = loader_.getGHepEvent(recordCount_);
      EventRecord *record = loader_.getNtpMCEventRecord(recordCount_);
      fNtpMCEventRecord->Fill(recordCount_, record);
    }
    else if(loader_.getTreeName().find("pg") != std::string::npos)
      event_ = loader_.getPGEvent(recordCount_);

    if(verbose_>2)event_->printToStream(std::cout);
    outData_->Fill();
    recordCount_++;
    
    return true;
  }

  loader_.close();
  return false;


}




