// -------------------------------------------------------------------
/// \file   RootFileWriter.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#include "RootFileWriter.h"

#include "EventRecord.h"

#include <TFile.h>
#include <TTree.h>


namespace gastpc {

  RootFileWriter::RootFileWriter(): evtrec_(0), file_(0), tree_(0)
  {
  }


  RootFileWriter::~RootFileWriter()
  {
    if (IsFileOpen()) {
      CloseFile();
      delete file_;
    }
  }


  bool RootFileWriter::OpenFile(const std::string& filename,
                                const std::string& option)
  {
    // Close any previously opened file
    CloseFile();

    // Create new ROOT file. Return false if something did not work.
    file_ = new TFile(filename.c_str(), option.c_str());
    if (!file_ || file_->IsZombie()) return false;

    // Create EventRecord tree
    evtrec_ = 0;
    tree_ = new TTree("EventRecord", "gastpc::EventRecord tree.");
    tree_->Branch("EventRecord", "gastpc::EventRecord", &evtrec_, 32000, 0);

    return true;
  }


  void RootFileWriter::CloseFile()
  {
    if (IsFileOpen()) {
      file_->Write();
      file_->Close();
    }
  }


  void RootFileWriter::Write(EventRecord& er)
  {
    evtrec_ = &er;
    tree_->Fill();
  }


  bool RootFileWriter::IsFileOpen() const
  {
    if (!file_) return false;
    else return file_->IsOpen();
  }

} // namespace gastpc
