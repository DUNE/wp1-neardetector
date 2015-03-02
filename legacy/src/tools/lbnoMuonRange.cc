#include <iostream>

#include <TTree.h>
#include <TFile.h>

#include "LbnoDataLib.hh"
#include "DataCards.hh"
#include "LbnoProcessorManager.hh"
#include "RangeEstimationProcessor.hh"

int main(int argc, char ** argv) {

  TFile datafile("nuData.root","UPDATE");
  TTree* DetectorHitsTree = (TTree*)datafile.Get("DetectorHits");

  try {
    // Create Processor Manage.
    LbnoProcessorManager manager;

    // Create Processor
    RangeEstimationProcessor* g4ranger = new RangeEstimationProcessor();
    g4ranger->setInDataTreePtr(DetectorHitsTree);
    //DetectorHitsTree->Print();
    g4ranger->loadInDataTree();

// Get command line arguments if any. 
    // This will modify the default values for the runtime parameters.
    manager.getRunTimeArguments(argc, argv);

    g4ranger->loadDataCards();
    g4ranger->initOutDataTree();

    int nEntries = DetectorHitsTree->GetEntries();
    int i=0;
    while(i<nEntries) {
      g4ranger->process();
      i++;
    }

    g4ranger->getOutDataTree()->Write();
  } catch (LbnoException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }

  return 0;
}




