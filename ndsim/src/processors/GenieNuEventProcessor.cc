//____________________________________________________________________________
/*!

\class    GenieNuEventProcessor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#include "GenieNuEventProcessor.hh"

GenieNuEventProcessor::GenieNuEventProcessor(GeometryLoader * geomLoader) {
  name_ = "GenieNuEventProcessor";
  nInteractions_ = 0;
  nInteractionsCC_ = 0;
  nInteractionsNC_ = 0;
  nInteractionsMix_ = 0;
  geomLoader_ = geomLoader;
  this->initDataCards();
}

void GenieNuEventProcessor::initOutDataTree() {
  file_->cd();
  outData_ = new TTree("NuInteractions", "Neutrino Interactions");
  this->makeOutTreeBranches<NeutrinoEvent>(event_);
  if (outTreeBasketSize_)
   this->setOutTreeBasketSize<NeutrinoEvent>(outTreeBasketSize_, event_);
}

void GenieNuEventProcessor::initDataCards() {
  cards_->AddDataCardInt("GenieNuEventBasketSize", 640000);
}

void GenieNuEventProcessor::loadDataCards() {
  outTreeBasketSize_ = cards_->fetchValueInt("GenieNuEventBasketSize");

  eventGenerator_.loadDataCards();
  verbose_ = eventGenerator_.getVerbose();
  eventGenerator_.setManager(geomLoader_->getManager());
  eventGenerator_.initialize();
}

void GenieNuEventProcessor::loadInDataTree() {
  this->setInTreeBranches<NeutrinoHit>(hit_);
}

bool GenieNuEventProcessor::process() {
  inData_->GetEntry(precessCount_);
  precessCount_++;

  // interaction prob. scale - is constant for all events
  genieProbScale = eventGenerator_.getMCjobDriver()->GlobProbScale(); 

  //hit_->printToStream(std::cout);
  if (eventGenerator_.Process(hit_, event_) ) {
    backTracer_.push_back(recordCount_);

    //std::cout << "backTracer : " << backTracer_[0] << ", " << backTracer_[1] << std::endl;
    outData_->Fill();
    recordCount_++;
    
    return true;
  }

  return true;
}


