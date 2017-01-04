// -------------------------------------------------------------------
/// \file   DstWriter.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 11 Aug 2016
// -------------------------------------------------------------------

#include "DstWriter.h"

#include <Ntuple/NtpMCEventRecord.h>

#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>


DstWriter::DstWriter(): gmcrec(0), file_(0), tree_(0)
{
}


DstWriter::~DstWriter()
{
  CloseFile();
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

  tree_->Branch("gmcrec", "genie::NtpMCEventRecord", &(this->gmcrec));

  tree_->Branch("RunID",   &(this->RunID),   "RunID/I");
  tree_->Branch("EventID", &(this->EventID), "EventID/I");
  tree_->Branch("Sample",  &(this->Sample),  "Sample/I");

  tree_->Branch("Ev",      &(this->Ev),      "Ev/D");
  tree_->Branch("Ev_reco", &(this->Ev_reco), "Ev_reco/D");
  tree_->Branch("Y",       &(this->Y),       "Y/D");
  tree_->Branch("Y_reco",  &(this->Y_reco),  "Y_reco/D");

  tree_->Branch("VertexPosition", this->VertexPosition, "VertexPosition[4]/D");

  tree_->Branch("NTracks", &(this->NTracks), "NTracks/I");

  tree_->Branch("TrackID", this->TrackID, "TrackID[NTracks]/I");
  tree_->Branch("FamilyTreeLevel", this->FamilyTreeLevel, "FamilyTreeLevel[NTracks]/I");
  tree_->Branch("Pdg",      this->Pdg,      "Pdg[NTracks]/I");
  tree_->Branch("Pdg_reco", this->Pdg_reco, "Pdg_reco[NTracks]/I");

  tree_->Branch("Momentum",      this->Momentum,      "Momentum[NTracks]/D");
  tree_->Branch("Momentum_reco", this->Momentum_reco, "Momentum_reco[NTracks]/D");

  //tree_->SetWeight(3.75E15);
}


void DstWriter::CloseFile()
{
  if (this->IsFileOpen()) {
    file_->Write();
    file_->Close();
  }
}


void DstWriter::Write()
{ 
  tree_->Fill();
}


bool DstWriter::IsFileOpen() const
{
  if (!file_) return false;
  else return file_->IsOpen();
}
