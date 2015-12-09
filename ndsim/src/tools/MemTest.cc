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

#include "GasTPCProcessorManager.hh"
#include "GeometryLoader.hh"
#include "MemTestProcessor.hh"

int main(int argc, char ** argv) {

  std::string inputfile   = "";
  std::string outputfile = "MemoryTest.root";
  int nentries = 100;
  for(int iArg = 1; iArg < argc; iArg++) {
    if((!strcasecmp(argv[iArg],"-i"))||(!strcasecmp(argv[iArg],"-input"))) {
      inputfile = argv[++iArg] ;
    }
    if((!strcasecmp(argv[iArg],"-o"))||(!strcasecmp(argv[iArg],"-output"))) {
      outputfile = argv[++iArg] ;
    }
  }

  // Create Processor Manage.
  GasTPCProcessorManager manager;
  manager.initialize(outputfile,1);
  
  //Initiate geometry loader
  GeometryLoader * geomLoader = new GeometryLoader(inputfile);
  
  // Initialize your processors here. The processors will define
  // default values for all runtime parameters.
  MemTestProcessor          *memTest         = new MemTestProcessor();
  memTest->SetNEntries(nentries);
  
  // Now add your processors to the manages.
  manager.addProcessor(memTest);
  
  manager.printToStream(cout);
  
  // Run your processors.
  manager.go();
  
  // Store the result.
  manager.write();
  
  //close the geometry
  //geomLoader->close();
  
  return 0;
}


