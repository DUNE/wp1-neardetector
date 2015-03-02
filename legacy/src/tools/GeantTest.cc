//____________________________________________________________________________
/*!

\class    GeantTest

\brief    Program to test geant simulation

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
#include "Geant4TrackingProcessor.hh"

int main(int argc, char ** argv) {
  try {
    // Create Processor Manage.
    LbnoProcessorManager manager;

    //Initiate geometry loader
    GeometryLoader * geomLoader = new GeometryLoader();

    // Initialize your processors here. The processors will define
    // default values for all runtime parameters.
    Geant4TrackingProcessor   *g4tracking = new Geant4TrackingProcessor(geomLoader);
    g4tracking->setGunOption(1);

    // Get command line arguments if any. 
    // This will modify the default values for the runtime parameters.
    manager.getRunTimeArguments(argc, argv);

    //reinitiate the geometry loader
    geomLoader->initialize();

    // Now add your processors to the manages. 
    manager.addProcessor(g4tracking);
    //make sure gun is on, otherwise it will cause seg fault as no vertices generated
    g4tracking->setGunOption(1);

    manager.printToStream(cout);

    // Run your processors.
    manager.go();

    // Store the result.
    //manager.write();

   //close the geometry
   //geomLoader->close();

  } catch (LbnoException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }
  return 0;
}


