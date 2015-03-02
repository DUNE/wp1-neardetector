//____________________________________________________________________________
/*!

\class    NuEventFileLoadAlgorithm

\brief    Class for algorithm to read in neutrino vertices into GENIE

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#ifndef EVGEN_NUEVENTFILELOADALGORITHM_H
#define EVGEN_NUEVENTFILELOADALGORITHM_H 1

#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TRandom3.h>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "LbnoDataLib.hh"
#include "DataCardsRecord.hh"
#include "DataCards.hh"
#include "RunStats.hh"

class NuEventFileLoadAlgorithm {
 public:
  NuEventFileLoadAlgorithm();
  virtual ~NuEventFileLoadAlgorithm();

  NeutrinoEvent * getEvent(){return nuEvent_;};
  NeutrinoEvent * getEvent(int eventNumber);

  bool getNextEvent();

  void initDataCards();
  void loadDataCards();
  void initialize();
  void close();

  std::string getGeometryFileName() {return inputGeom_;};
  double getExposure() {return exposure;};
  int getEventNumber() { return entryCount_;};
  int getNumberOfEvents() { return nuEvents_nEntries;};

  double getRealExposure() {return realExposure_;};
  double getProbScale()		{return probScale_;};

  void setVerbose(int verb) { verbose_ = verb;};

  template <class dataType>
  void setDataAddress(dataType* &dataObject, TTree* dataTree) {
    dataObject = new dataType();
    dataTree->GetBranch( dataObject->getRecordName().c_str() )->SetAddress(&dataObject);
    dataTree->GetBranch("backTracer")->SetAddress(0);
  }

  template <class dataType>
  void deleteDataAddress(dataType* &dataObject){
	delete dataObject;
  }

  void setBackTracer(TTree* dataTree){dataTree->GetBranch("backTracer")->SetAddress(&backTracerBuff_);};

 private:

  NeutrinoHit    	*nuHit_;
  NeutrinoEvent  	*nuEvent_;

  int nuHits_nEntries,nuEvents_nEntries;

  DataCardsRecord 	*dataCard_;
  RunStats 	 	*stats_;

  DataCards* cards_;
  bool cardsLoaded_;

  TRandom3 * randGen;

  TFile* inputFile_;
  TTree* inputTree_;

  std::string inputFileName_,inputGeom_;
  std::string inputTreeName_;

  int entryCount_;

  double nuPx_, nuPy_, nuPz_, nuEn_;
  double maxNuE_;
  double parentVx_, parentVy_, parentVz_;
  int nuPID_, nuPDG_;

  double exposure,realExposure_,probScale_;

  int verbose_,runNumber_;

  std::vector<int>* backTracerBuff_;
  std::vector<int> backTracer_;
};

#endif
