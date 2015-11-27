//____________________________________________________________________________
/*!

\class    NuEventFileLoadAlgorithm

\brief    Class for algorithm to read in neutrino vertices into GENIE

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Oct 2015
*/
//____________________________________________________________________________
#ifndef EVGEN_NUEVENTFILELOADALGORITHM_H
#define EVGEN_NUEVENTFILELOADALGORITHM_H 1

#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include <Ntuple/NtpMCEventRecord.h>
#include <Ntuple/NtpMCTreeHeader.h>
#include <EVGCore/EventRecord.h>
#include <GHEP/GHepParticle.h>
#include <GHEP/GHepUtils.h>
#include <PDG/PDGCodes.h>
#include <PDG/PDGUtils.h>
#include <PDG/PDGLibrary.h>
#include <Conventions/Constants.h>

#include "GasTPCDataLib.hh"

using namespace genie;
using namespace std;

class NuEventFileLoadAlgorithm {
 public:
  NuEventFileLoadAlgorithm();
  virtual ~NuEventFileLoadAlgorithm();

  NeutrinoEvent * getEvent(){return nuEvent_;};
  NeutrinoEvent * getEvent(int eventNumber);
  NeutrinoEvent * getGstEvent(int event);
  NeutrinoEvent * getGHepEvent(int eventid);
  NeutrinoEvent * getPGEvent(int eventid);

  TTree * getInputTree(){return inputTree_;};
  EventRecord * getNtpMCEventRecord(int eventid); 

  bool getNextEvent();

  void initialize(std::string inputFileName, std::string inputTreeName, std::string inputGeom);
  void initialize(std::string inputFileName, TTree *inputTree, std::string inputGeom);
  void close();

  std::string getGeometryFileName() {return inputGeom_;};
  std::string getTreeName() {return inputTreeName_;};
  int getEventNumber() { return entryCount_;};
  int getNumberOfEvents() { return nuEvents_nEntries;};

  void setVerbose(int verb) { verbose_ = verb;};
  void setTreeName(std::string treename) { inputTreeName_ = treename;};

  template <class dataType>
  void setDataAddress(dataType* &dataObject, TTree* dataTree) {
    dataObject = new dataType();
    dataTree->GetBranch( dataObject->getRecordName().c_str() )->SetAddress(&dataObject);
    dataTree->GetBranch("backTracer")->SetAddress(0);
    dataTree->GetBranch("gmcrec")->SetAddress(0);
  }

  template <class dataType>
  void deleteDataAddress(dataType* &dataObject){
    delete dataObject;
  }

  //void setBackTracer(TTree* dataTree){dataTree->GetBranch("backTracer")->SetAddress(&backTracerBuff_);};

 private:

  //NeutrinoHit    	*nuHit_;
  NeutrinoEvent  	*nuEvent_;

  int nuHits_nEntries,nuEvents_nEntries;

  TFile* inputFile_;
  TTree* inputTree_;

  std::string inputFileName_,inputGeom_;
  std::string inputTreeName_;

  int entryCount_;

  //double nuPx_, nuPy_, nuPz_, nuEn_;
  //double parentVx_, parentVy_, parentVz_;
  //int nuPID_, nuPDG_;

  int verbose_;

  // Gst tree variables
  int neu, tgt, fspl, nf, ni, resid, pdgf[500], pdgi[500], neut_code;
  double Ev, El, pl, pxl, pyl, pzl, cthl, En, pxn, pyn, pzn, wght, x, y, Q2, W, vtxx, vtxy, vtxz, vtxt, pxf[500], pyf[500], pzf[500], Ef[500], pf[500], cthf[500], pxi[500], pyi[500], pzi[500], Ei[500];
  bool cc, sea, qel, mec, res, dis, coh, dfr, imd, nuel, charm;

  // GENIE gtree variables
  NtpMCEventRecord * mcrec;
  NtpMCTreeHeader * thdr;

  // particle gun tree variables
  double Px,Py,Pz,E,X,Y,Z,M;
  int pdg;
  
};

#endif
