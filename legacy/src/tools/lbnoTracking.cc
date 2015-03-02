//____________________________________________________________________________
/*!

\class    lbnoTracking

\brief    Program to load primaries from neutrino verticies and track them
	  Only tracking, no flux or event generation

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  June 2013
\last update June 2013

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
#include "LoadNuEventProcessor.hh"
#include "Geant4TrackingProcessor.hh"

int main(int argc, char ** argv) {
  try {
    // Create Processor Manage.
    LbnoProcessorManager manager;

    //Initiate geometry loader
    //GeometryLoader * geomLoader = new GeometryLoader();

    // Initialize your processors here. The processors will define
    // default values for all runtime parameters.
    LoadNuEventProcessor     *eventLoader  = new LoadNuEventProcessor();
    Geant4TrackingProcessor   *g4tracking = new Geant4TrackingProcessor();//geomLoader);

    // Get command line arguments if any. 
    // This will modify the default values for the runtime parameters.
    manager.getRunTimeArguments(argc, argv);
    //manager.loadDataCards();

    // Now add your processors to the manages. 
    manager.addProcessor(eventLoader); 
    manager.addProcessor(g4tracking);

    //reinitiate the geometry loader
    //geomLoader->initialize(eventLoader->get);

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


