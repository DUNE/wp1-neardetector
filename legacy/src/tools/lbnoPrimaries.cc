//____________________________________________________________________________
/*!

\class    lbnoPrimaries

\brief    Program to generate neutrino vertices from the input beam file.
	  No geant tracking, only primaries

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  June 2013
\last update June 2013

*/
//____________________________________________________________________________
#include <iostream>

#include <TTree.h>
#include <TFile.h>

#include "DataCards.hh"
#include "GeometryLoader.hh"
#include "LbnoProcessorManager.hh"
#include "LoadFluxProcessor.hh"
#include "GenieNuEventProcessor.hh"

int main(int argc, char ** argv) {
  
  try {
    // Create Processor Manager
    LbnoProcessorManager manager;
  
    //Initiate geometry loader
    GeometryLoader * geomLoader = new GeometryLoader();

    //load the processors
    LoadFluxProcessor         *fluxFromFile    = new LoadFluxProcessor(geomLoader);
    GenieNuEventProcessor     *eventGenerator  = new GenieNuEventProcessor(geomLoader);

    // Get command line arguments if any. 
    // This will modify the default values for the runtime parameters.
    manager.getRunTimeArguments(argc, argv);
    manager.loadDataCards();

    //reinitiate the geometry loader
    geomLoader->initialize();

    // Now add your processors to the manager
    manager.addProcessor(fluxFromFile);
    manager.addProcessor(eventGenerator);

    manager.printToStream(cout);

    // Run your processors.
    manager.go();

    // Store the result.
    manager.write();
    
  } catch (LbnoException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }
  return 0;
}
