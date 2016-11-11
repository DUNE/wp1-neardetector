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


DstWriter::DstWriter(): gmcrec(0), file_(0), tree_(0)
{
}


DstWriter::~DstWriter()
{
  CloseFile();
  delete file_;
}


bool DstWriter::OpenFile(const std::string& filename,
                         const std::string& option)
{
  // Close any previously opened file
  CloseFile();

  // Create a new ROOT file returning false if something goes wrong
  file_ = new TFile(filename.c_str(), option.c_str());
  if (!file_ || file_->IsZombie()) return false;

  // Create a tree and all its branches
  tree_ = new TTree("NdtfDst", "DUNE ND-TF GArTPC DST");
  tree_->Branch("gmcrec", "genie::NtpMCEventRecord", &gmcrec);
  tree_->Branch("RunID",          &RunID, "RunID/I");
  tree_->Branch("EventID",        &EventID, "EventID/I");
  tree_->Branch("Sample",         &Sample, "Sample/I");
  tree_->Branch("Ev_reco",        &Ev_reco, "Ev_reco/D");
  tree_->Branch("Ev",             &Ev, "Ev/D");
  tree_->Branch("Y",              &Y, "Y/D");
  tree_->Branch("Y_reco",         &Y_reco, "Y_reco/D");
  tree_->Branch("VertexPosition", VertexPosition, "VertexPosition[4]/D");
  tree_->Branch("NGeantTracks",   &NGeantTracks, "NGeantTracks/I");
  tree_->Branch("TrackID",        TrackID, "TrackID[NGeantTracks]/I");
  tree_->Branch("Momentum",       Momentum, "Momentum[NGeantTracks]/D");
  tree_->Branch("TotalEDep",      TotalEDep, "TotalEDep[NGeantTracks]/D");
  tree_->Branch("Pdg",            Pdg, "Pdg[NGeantTracks]/I");
  tree_->Branch("dEdx",           dEdx, "dEdx[NGeantTracks]/D");
  tree_->Branch("NGeantHits",     NGeantHits, "NGeantHits[NGeantTracks]/I");

  tree_->SetWeight(3.75E15);

  return true;
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