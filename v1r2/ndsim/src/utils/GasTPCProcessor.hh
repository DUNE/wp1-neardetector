//____________________________________________________________________________
/*!

\class    GasTPCProcessor

\brief    Base class for processor

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
#ifndef UTILS_GasTPCPROCESSOR_H
#define UTILS_GasTPCPROCESSOR_H 1

#include <string>
#include <vector>

#include <TTree.h>

#include "GasTPCDataLib.hh"

#include <Ntuple/NtpMCEventRecord.h>

class GasTPCProcessor {

 public:
  GasTPCProcessor();
  virtual ~GasTPCProcessor() {};

  virtual bool process() =0;
  virtual void initOutDataTree() =0;
  virtual void loadInDataTree() =0;

  int getVerbose() { return verbose_;};

  void setInDataTreePtr(TTree* t)      {inData_ = t;}
  void setOutDataTreePtr(TTree* t)     {outData_ = t;}
  TTree* getOutDataTree() const        {return outData_;}
  void setName(std::string n)          {name_ = n;}
  std::string getName() const          {return name_;}
  std::vector<int> getBackTracer()     {return backTracer_;}

  void SetFilePtr(TFile * file){file_ = file;};

 protected:
  template <class dataType>
  void setInTreeBranches(dataType* &dataObject) {
    dataObject = new dataType();
    inData_->GetBranch("gmcrec")->SetAddress(&fNtpMCEventRecord);
    inData_->GetBranch("backTracer")->SetAddress(&backTracerBuff_);
    inData_->GetBranch(dataObject->getRecordName().c_str() )->SetAddress(&dataObject);
  }

  template <class dataType>
  void makeOutTreeBranches(dataType* &dataObject) {
    dataObject = new dataType();
    outData_->Branch("gmcrec","genie::NtpMCEventRecord", &fNtpMCEventRecord);
    outData_->Branch("backTracer", &backTracer_);
    outBranch_ = outData_->Branch(dataObject->getRecordName().c_str(), &dataObject);
  }

  template <class dataType>
  void setOutTreeBasketSize(int basketSize, dataType* dataObject) {
    outData_->SetBasketSize("gmcrec", basketSize);
    outData_->SetBasketSize("backTracer", basketSize);
    outData_->SetBasketSize(dataObject->getRecordName().c_str(), basketSize);
  }

  template <class dataType>
  void deleteDataAddress(dataType* &dataObject){
    delete dataObject;
  }

  std::string name_;
  TFile * file_;
  TTree* inData_;
  TTree* outData_;
  TBranch* outBranch_;
  std::vector<int> backTracer_;
  std::vector<int>* backTracerBuff_;
  int precessCount_;
  int recordCount_;
  int outTreeBasketSize_;

  int verbose_;

  // genie
  genie::NtpMCEventRecord * fNtpMCEventRecord;
};

#endif

