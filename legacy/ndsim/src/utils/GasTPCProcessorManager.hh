//____________________________________________________________________________
/*!

\class    GasTPCProcessorManager

\brief    The process manager

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef UTILS_GasTPCPROCESSORMANAGER_H
#define UTILS_GasTPCPROCESSORMANAGER_H 1

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "GasTPCProcessor.hh"
#include "LoadFluxProcessor.hh"
#include "LoadNuEventProcessor.hh"
#include "GenieNuEventProcessor.hh"
#include "Geant4TrackingProcessor.hh"
#include "DataCardsRecord.hh"
#include "RunStats.hh"

typedef vector<GasTPCProcessor*> processorList;

class GasTPCProcessorManager {

 public:
  GasTPCProcessorManager();
  virtual ~GasTPCProcessorManager();

  void startup();
  void initDataCards();
  void loadDataCards();
  void setFile(std::string filename);

  inline void progressBar(int x, int n, int r, int w);

  void addProcessor(GasTPCProcessor* a);
  processorList getProcessors()  const {return processors_;}
  void getRunTimeArguments(int argc, char ** argv);
  void printToStream(ostream& stream);

  void go();
  void writePartial(int processorNumber);
  void write();
  //void plot();

  TFile* GetFilePtr() const	{return datafile_;}
  TTree* getData(unsigned int i) const  {return data_[i];}
  vector <TTree*> getData() const      {return data_;}

 private:
  processorList processors_;

  TFile* datafile_;
  bool fileSet;

  vector<TTree*> data_;
  DataCards *cards_;
  std::string outputFileName_;
  double exposure_;
  double probScale;
  int nIterations_;
  int compression_;
  int verbose_;
};

#endif

