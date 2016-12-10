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


DstWriter::DstWriter(): file_(0), tree_(0)
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

  tree_->Branch("gmcrec", "genie::NtpMCEventRecord", &(entry_.gmcrec));

  tree_->Branch("RunID",   &(entry_.RunID),   "RunID/I");
  tree_->Branch("EventID", &(entry_.EventID), "EventID/I");
  tree_->Branch("Sample",  &(entry_.Sample),  "Sample/I");

  tree_->Branch("Ev",      &(entry_.Ev),      "Ev/D");
  tree_->Branch("Ev_reco", &(entry_.Ev_reco), "Ev_reco/D");
  tree_->Branch("Y",       &(entry_.Y),       "Y/D");
  tree_->Branch("Y_reco",  &(entry_.Y_reco),  "Y_reco/D");

  tree_->Branch("VertexPosition", entry_.VertexPosition, "VertexPosition[4]/D");

  tree_->Branch("NTracks",  &(entry_.NTracks), "NTracks/I");

  tree_->Branch("TrackID",  (entry_.TrackID),  "TrackID[NTracks]/I");
  tree_->Branch("FamilyTreeLevel", (entry_.FamilyTreeLevel), "FamilyTreeLevel[NTracks]/I");
  tree_->Branch("Pdg",      (entry_.Pdg),      "Pdg[NTracks]/I");
  tree_->Branch("Pdg_reco", (entry_.Pdg),      "Pdg_reco[NTracks]/I");

  tree_->Branch("InitialMomentum", (entry_.InitialMomentum), "InitialMomentum[NTracks][3]/D");
  tree_->Branch("Momentum_reco", (entry_.Momentum_reco), "Momentum_reco[NTracks]/D");

  // tree_->Branch("TrackID",        TrackID, "TrackID[NGeantTracks]/I");
  // tree_->Branch("Momentum",       Momentum, "Momentum[NGeantTracks]/D");
  // tree_->Branch("TotalEDep",      TotalEDep, "TotalEDep[NGeantTracks]/D");
  // tree_->Branch("Pdg",            Pdg, "Pdg[NGeantTracks]/I");
  // tree_->Branch("dEdx",           dEdx, "dEdx[NGeantTracks]/D");
  // tree_->Branch("NGeantHits",     NGeantHits, "NGeantHits[NGeantTracks]/I");

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
  std::cout << entry.gmcrec << std::endl;

  entry_ = entry; 
  tree_->Fill();
}


bool DstWriter::IsFileOpen() const
{
  if (!file_) return false;
  else return file_->IsOpen();
}
