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


namespace gastpc {

  RootFileReader::RootFileReader(): evtrec_(0), file_(0), tree_(0) 
  {
  }


  RootFileReader::~RootFileReader()
  {
    CloseFile();
  }


  bool RootFileReader::OpenFile(const std::string& filename)
  {
    // Close any previously opened file
    CloseFile();

    // Create a new ROOT file. Return false if something goes wrong.
    file_ = new TFile(filename.c_str());
    if (!file_ || file_->IsZombie()) return false;

    // Fetch the EventRecord tree from the file and sync the branch 
    // address to the transient record
    tree_ = dynamic_cast<TTree*>(file_->Get("EventRecord"));
    evtrec_ = 0;
    tree_->SetBranchAddress("EventRecord", &evtrec_);

    return true;
  }


  void RootFileReader::CloseFile()
  {
    if (IsFileOpen()) file_->Close();
  }


  gastpc::EventRecord& RootFileReader::Read(int i)
  {
    evtrec_->Clear();
    tree_->GetEntry(i);
    return *evtrec_;
  }


  int RootFileReader::GetNumberOfEntries() const
  {
    if (tree_) return tree_->GetEntries();
    else return 0;
  }


  bool RootFileReader::IsFileOpen() const
  {
    if (!file_) return false;
    else return file_->IsOpen();
  }

} // namespace gastpc
