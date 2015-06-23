//____________________________________________________________________________
/*!

\class    GasTPCPlotter

\brief    Base class for plotting

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2015
*/
//____________________________________________________________________________
#ifndef UTILS_PLOTTER_H
#define UTILS_PLOTTER_H 1

#include <vector>
#include <TTree.h>
#include <TFile.h>

#include "GeometryLoader.hh"
#include "GasTPCDataLib.hh"
#include "RunStats.hh"

class GasTPCPlotter {
 public:
  GasTPCPlotter();
  virtual ~GasTPCPlotter();

  bool inFiducialVolume(NeutrinoEvent* nu_event,double fidCut);
  bool setupTrees(TFile &inputfile);
  bool deleteTrees();
  bool copyTree(TFile * oldfile,std::string newfileName,std::string treeName);
  bool copyTrees(int startrun,int runs,std::string oldfileBaseName,std::string newfileName,std::string treeName);
  bool loadGeom(std::string filename = "",std::string tpcname = "");

  MuonDecayEvent* 	getMuDecay()       const {return muDecay_;}
  PionDecayEvent* 	getPiDecay()       const {return piDecay_;}
  NeutrinoHit*    	getNuHit()         const {return nuHit_;}
  NeutrinoEvent*  	getNuEvent()       const {return nuEvent_;}
  SimulData*      	getSimData()       const {return simData_;}
  GeantTrackingTruth*	getTrackingTruth() const {return trackingRecord_;}
  MuonRange*      	getMuonRange()     const {return range_;}
  RunStats*       	getRunStats()      const {return stats_;}

  TTree*          getNuInteractionsTree()  const {return nuEventTree;}
  TTree*          getTrackingTree()        const {return detectorHitsTree;}

  std::string     getInputFileName() const  {return inputFileName_;}
  std::string     getOutputFileName() const {return outputFileName_;}
  int getNumberOfRuns()		      const {return runs_;};

  int getVerbose() {return verbose_;};

  void push_backTree(TTree* t);
  virtual void plot() {}

  template <class dataType>
  void setDataAddress(dataType* &dataObject, TTree* dataTree) {
    dataObject = new dataType();
    dataTree->GetBranch( dataObject->getRecordName().c_str() )->SetAddress(&dataObject);
    this->setBackTracer(dataTree);
    //dataTree->GetBranch("backTracer")->SetAddress(0);
    //dataTree->GetBranch("backTracer")->SetAddress(&backTracerBuff_);
  }

  template <class dataType>
  void deleteDataAddress(dataType* &dataObject){
    delete dataObject;
    dataObject = NULL;
  }

  void setBackTracer(TTree* dataTree);

 protected:
  PionDecayEvent 	*piDecay_;
  MuonDecayEvent 	*muDecay_;
  NeutrinoHit    	*nuHit_;
  NeutrinoEvent  	*nuEvent_;
  SimulData      	*simData_;
  GeantTrackingTruth	*trackingRecord_;
  RunStats 	 	*stats_;
  MuonRange      	*range_;

  std::string inputFileName_;
  std::string outputFileName_;
  std::string selectedVolume_;
  std::string geomFileName_;
  int runs_,events_;
  int verbose_;
  double maxEnergy_,magField_;
  double fiducialCut_;

  bool useMagFieldFromCard;

  int nuHits_nEntries;
  int nuEvents_nEntries;
  int detHits_nEntries;

  std::vector<TTree*> data_;

  TTree* nuHitTree;
  TTree* nuEventTree;
  TTree* detectorHitsTree;

  std::vector<int>* backTracerBuff_;
  std::vector<int> backTracer_;

//geometry
  GeometryLoader * geomLoader;
  TGeoVolume * world;
  TGeoVolume * tpcVolume;

};

#endif

