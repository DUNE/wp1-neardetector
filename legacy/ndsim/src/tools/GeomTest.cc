//____________________________________________________________________________
/*!

\class    GeomTest

\brief    Program to test geometry loader

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  June 2014
\last update June 2014

*/
//____________________________________________________________________________
#include <iostream>

#include <TTree.h>
#include <TFile.h>

#include "DataCards.hh"
#include "GasTPCProcessorManager.hh"
#include "Geant4TrackingAlgorithm.hh"
#include "GenieNuEventAlgorithm.hh"

int main(int argc, char ** argv) {

  try {
    Geant4TrackingAlgorithm simul;
    GenieNuEventAlgorithm evgen;
    GasTPCProcessorManager manager;
    manager.getRunTimeArguments(argc, argv);

    simul.loadDataCards();
    simul.initialize();

    evgen.initGenieHstFluxDriver("../data/nufl_-002300_GLOBES_100Km.txt");

  } catch (GasTPCException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }

  return 0;
}

