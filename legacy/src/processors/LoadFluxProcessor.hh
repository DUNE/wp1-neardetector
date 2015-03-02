//____________________________________________________________________________
/*!

\class    LoadFluxProcessor

\brief    Processor class to implement reading and projecting the flux 
	  to the specified volume

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#ifndef PROCESSORS_LOADFLUX
#define PROCESSORS_LOADFLUX   1

#include <string>
#include <iostream>

#include <TSystem.h>
#include <TLorentzVector.h>

#include "GeometryLoader.hh"
#include "LbnoProcessor.hh"
#include "PiFluxProjectionAlgorithm.hh"
#include "FluxFileLoadAlgorithm.hh"

class LoadFluxProcessor : public LbnoProcessor {

 public:
  LoadFluxProcessor(GeometryLoader * geomLoader);
  virtual ~LoadFluxProcessor();

  bool process();
  void initOutDataTree();
  void loadInDataTree() {};
  void loadDataCards();

  int getFileEntries() {return nEntries;};
  int getNumberOfIterations() {return iterationCount;};
  double getFileExposure() {return fileExposure_;};

  FluxFileLoadAlgorithm * getFluxFileAlgo()   {return & loader_;};

 private:
  void initDataCards();

  PiFluxProjectionAlgorithm projector_;
  FluxFileLoadAlgorithm loader_;
  GeometryLoader * geomLoader_;

  NeutrinoHit* neutrino_;
  PionDecayEvent *piDecay_;
  int nDecay_;

  int nEntries,iterationCount;
  double fileExposure_;

  //for memory checks
  ProcInfo_t info;
  Long64_t totbytes;
};

#endif

