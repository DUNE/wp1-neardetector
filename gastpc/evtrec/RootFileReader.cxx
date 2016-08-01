// -------------------------------------------------------------------
/// \file   RootFileReader.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 31 July 2016
// -------------------------------------------------------------------

#include "RootFileReader.h"

#include "EventRecord.h"

#include <TFile.h>
#include <TTree.h>

#include <iostream>



namespace gastpc {

  RootFileReader::RootFileReader(): evtrec_(0), file_(0), tree_(0) 
  {
  }


  RootFileReader::~RootFileReader()
  {
  }


  void RootFileReader::OpenFile(const std::string& filename)
  {
    if (file_) delete file_;

    file_ = new TFile(filename.c_str());
    tree_ = (TTree*) file_->Get("EventRecord");
    tree_->SetBranchAddress("EventRecord", &evtrec_);
    std::cout << "RootFileReader::OpenFile" << std::endl;
    std::cout << tree_->GetEntries() << std::endl;
  }


  void RootFileReader::CloseFile()
  {
    file_->Close();
  }


  gastpc::EventRecord& RootFileReader::Read(int i)
  {
    tree_->GetEntry(i);
    return *evtrec_;
  }

} // namespace gastpc