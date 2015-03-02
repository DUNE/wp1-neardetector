//____________________________________________________________________________
/*!

\class    MemTest

\brief    Program to test memory usage

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2013
\last update Sep 2013

*/
//____________________________________________________________________________
#include <iostream>

#include <TTree.h>
#include <TFile.h>
#include <TRandom.h>
#include <TRandom3.h>

#include "LbnoProcessorManager.hh"
#include "GeometryLoader.hh"
#include "DataCards.hh"
#include "PiDecayProcessor.hh"
#include "PiFluxProjectionProcessor.hh"
#include "PiFluxFastProcessor.hh"
#include "LoadFluxProcessor.hh"
#include "GenieNuEventProcessor.hh"
#include "Geant4TrackingProcessor.hh"
#include "RangeEstimationProcessor.hh"
#include "MemTestProcessor.hh"

int main(int argc, char ** argv) {
  try {
    // Create Processor Manage.
    LbnoProcessorManager manager;

    //Initiate geometry loader
    GeometryLoader * geomLoader = new GeometryLoader();

    // Initialize your processors here. The processors will define
    // default values for all runtime parameters.
    MemTestProcessor          *memTest         = new MemTestProcessor();
//     PiDecayProcessor          *piDecay         = new PiDecayProcessor();
//     PiFluxProjectionProcessor *fluxProjector   = new PiFluxProjectionProcessor();
//     PiFluxFastProcessor       *fastFlux        = new PiFluxFastProcessor();
//     LoadFluxProcessor         *fluxFromFile    = new LoadFluxProcessor(geomLoader);
//     GenieNuEventProcessor     *eventGenerator  = new GenieNuEventProcessor(geomLoader);
//     Geant4TrackingProcessor   *g4tracking      = new Geant4TrackingProcessor(geomLoader);
//     RangeEstimationProcessor  *g4ranger        = new RangeEstimationProcessor();

    // Get command line arguments if any.
    // This will modify the default values for the runtime parameters.
    manager.getRunTimeArguments(argc, argv);

    //reinitiate the geometry loader
    geomLoader->initialize();

    // Now add your processors to the manages.
     manager.addProcessor(memTest);
//     manager.addProcessor(piDecay);
//     manager.addProcessor(fluxProjector);
//     manager.addProcessor(fastFlux);
//     manager.addProcessor(fluxFromFile);
//     manager.addProcessor(eventGenerator);
//     manager.addProcessor(g4tracking);
//     manager.addProcessor(g4ranger);

    manager.printToStream(cout);

    // Run your processors.
    manager.go();

    // Store the result.
    manager.write();

   //close the geometry
   //geomLoader->close();

  } catch (LbnoException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }
  return 0;
}


