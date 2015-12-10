//____________________________________________________________________________
/*!

\class    LoadNuEventProcessor

\brief    Processor class to read in generated neutrino events 
	  from previous neutrino run

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Jan 2014
\last update Oct 2015
*/
//____________________________________________________________________________
#ifndef PROCESSORS_LOADNUEVENT
#define PROCESSORS_LOADNUEVENT   1

#include <string>
#include <iostream>

#include <TSystem.h>
#include <TLorentzVector.h>
#include <TTree.h>

#include "GasTPCProcessor.hh"
#include "NuEventFileLoadAlgorithm.hh"

#include <Ntuple/NtpMCEventRecord.h>

class LoadNuEventProcessor : public GasTPCProcessor {

 public:
  LoadNuEventProcessor();
  virtual ~LoadNuEventProcessor();

  bool process();
  void initOutDataTree();
  void loadInDataTree() {};
  void initialize(std::string inputFileName, TTree *inputTree, std::string inputGeom, int nspills);
  void initialize(std::string inputFileName, std::string inputTreeName, std::string inputGeom, int nspills);
  void setVerbose(int verbose) {loader_.setVerbose(verbose);};
  void setNEvents(int nevents) {maxevents_ = nevents;};
  void setMinEvents(int nevents) {minevents_ = nevents;};

  int getFileEntries() 			{return loader_.getNumberOfEvents();};
  std::string getGeomFile() 		{return loader_.getGeometryFileName();};
  int getNEvents()                      {return maxevents_;};
  int getMinEvents()                    {return minevents_;};
  int getNSpills()                      {return nspills_;};

  NuEventFileLoadAlgorithm * getFluxFileAlgo()   {return & loader_;};

 private:
  NeutrinoEvent* event_; 
  NuEventFileLoadAlgorithm loader_;

  genie::NtpMCEventRecord * mcrec_;

  int maxevents_, minevents_;
  int nspills_;
};

#endif

