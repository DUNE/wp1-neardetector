//____________________________________________________________________________
/*!

\class    LoadNuEventProcessor

\brief    Processor class to read in generated neutrino events 
	  from previous neutrino run

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Jan 2014
\last update June 2014

*/
//____________________________________________________________________________
#ifndef PROCESSORS_LOADNUEVENT
#define PROCESSORS_LOADNUEVENT   1

#include <string>
#include <iostream>

#include <TSystem.h>
#include <TLorentzVector.h>

#include "GeometryLoader.hh"
#include "LbnoProcessor.hh"
#include "NuEventFileLoadAlgorithm.hh"
#include "EventFilterAlgorithm.hh"

class LoadNuEventProcessor : public LbnoProcessor {

 public:
  LoadNuEventProcessor();
  virtual ~LoadNuEventProcessor() {}

  bool process();
  void initOutDataTree();
  void loadInDataTree() {};
  void loadDataCards();

  int getFileEntries() 			{return loader_.getNumberOfEvents();};
  std::string getGeomFile() 		{return loader_.getGeometryFileName();};

  double getFileExposure() 		{return loader_.getExposure();};
  double getRealExposure() 		{return loader_.getRealExposure();};
  double getProbScale()			{return loader_.getProbScale();};

  NuEventFileLoadAlgorithm * getFluxFileAlgo()   {return & loader_;};

 private:
  NeutrinoEvent* event_; 

  void initDataCards();

  NuEventFileLoadAlgorithm loader_;
  EventFilterAlgorithm filter_;

  int nIterations_;
  bool qeccOnly_;

};

#endif

