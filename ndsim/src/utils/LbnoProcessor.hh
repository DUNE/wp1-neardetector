//____________________________________________________________________________
/*!

\class    LbnoProcessor

\brief    Base class for processor

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#ifndef UTILS_LBNOPROCESSOR_H
#define UTILS_LBNOPROCESSOR_H 1

#include <string>
#include <vector>

#include <TTree.h>

#include "DataCards.hh"
#include "LbnoDataLib.hh"

class LbnoProcessor {

 public:
  LbnoProcessor();
  virtual ~LbnoProcessor() {};

  virtual bool process() =0;
  virtual void initOutDataTree() =0;
  virtual void loadInDataTree() =0;
  virtual void loadDataCards() =0;

  int getVerbose() { return verbose_;};

  virtual bool getGunOption() {return gunOn_;};

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
    //std::cout << "setTreeBranches " << dataObject->getRecordName().c_str() << std::endl;
    inData_->GetBranch("backTracer")->SetAddress(&backTracerBuff_);
    inData_->GetBranch(dataObject->getRecordName().c_str() )->SetAddress(&dataObject);
  }

  template <class dataType>
  void makeOutTreeBranches(dataType* &dataObject) {
    dataObject = new dataType();
    //std::cout << "setTreeBranches " << dataObject->getRecordName().c_str() << std::endl;
    outData_->Branch("backTracer", &backTracer_);
    outBranch_ = outData_->Branch(dataObject->getRecordName().c_str(), &dataObject);
//    outData_->SetBranchStatus("*",1);
  }

  template <class dataType>
  void setOutTreeBasketSize(int basketSize, dataType* dataObject) {
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
  DataCards *cards_;
  std::vector<int> backTracer_;
  std::vector<int>* backTracerBuff_;
  int precessCount_;
  int recordCount_;
  int outTreeBasketSize_;

  int verbose_;

  bool gunOn_;
};

#endif

