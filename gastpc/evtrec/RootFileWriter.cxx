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

#include <iostream>


namespace gastpc {

  RootFileWriter::RootFileWriter(): file_(0), tree_(0), evtrec_(0)
  {
  }


  RootFileWriter::~RootFileWriter()
  {
  }


  void RootFileWriter::OpenFile(const std::string& filename)
  {
    std::string option = "RECREATE";

    file_ = new TFile(filename.c_str(), option.c_str());

    if (!file_ || file_->IsZombie()) {
      std::cerr << "RootFileWriter::Initialize()"
                << " -- Error opening ROOT file." << std::endl;
    }

    tree_ = new TTree("GasTPC", "GasTPC event tree.");
    tree_->Branch("EventRecord", "gastpc::EventRecord", &evtrec_, 32000, 0);
  }


  void RootFileWriter::CloseFile()
  {
    if (!file_ || !file_->IsOpen()) return;

    file_->Write();
    file_->Close();
  }


  void RootFileWriter::Write(EventRecord& er)
  {
    evtrec_ = &er;
  }

} // namespace gastpc
