// -------------------------------------------------------------------
/// \file   DstWriter.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 11 Aug 2016
// -------------------------------------------------------------------

#include "DstWriter.h"

#include "DstEntry.h"

#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>


DstWriter::DstWriter(): file_(0), tree_(0), entry_(0)
{
}


DstWriter::~DstWriter()
{
  CloseFile();
  delete tree_;
  delete file_;
}


void DstWriter::OpenFile(const std::string& filename,
                         const std::string& option)
{
  // Close any previously opened file
  CloseFile();

  // Create a new ROOT file returning false if something goes wrong
  file_ = new TFile(filename.c_str(), option.c_str());

  // Create a tree and all its branches
  tree_ = new TTree("NdtfDst", "DUNE ND-TF GArTPC DST");
  tree_->Branch("DstEntry", &entry_, 32000, 1);
  //tree_->SetWeight(3.75E15);
}


void DstWriter::CloseFile()
{
  if (this->IsFileOpen()) {
    file_->Write();
    file_->Close();
  }
}


void DstWriter::Write(DstEntry& entry)
{ 
  entry_ = &entry; 
  tree_->Fill();
}


bool DstWriter::IsFileOpen() const
{
  if (!file_) return false;
  else return file_->IsOpen();
}
