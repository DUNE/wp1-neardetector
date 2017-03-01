// -------------------------------------------------------------------
/// \file   GasTPCPseudoReco.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#include "RootFileReader.h"
#include "EventRecord.h"
#include "MCGenInfo.h"
#include "MCParticle.h"
#include "RecoParticle.h"
#include "MCTrack.h"
#include "MCHit.h"
#include "InteractionFinder.h"
#include "MomentumSmearer.h"
#include "ParticleIdentification.h"
#include "NeutrinoEnergy.h"
#include "Units.h"

#include <Ntuple/NtpMCEventRecord.h>

#include <TRandom3.h>
#include <TFile.h>
#include <TTree.h>

#include <getopt.h>
#include <string>
#include <iostream>


namespace {
  int rnd_=123456;
  std::string mode_("");
  std::string input_file_("");
  std::string output_file_("");
  std::string geometry_file_("");
  std::string ecal_data_("");
}

namespace dst {
  genie::NtpMCEventRecord* gmcrec = 0; ///< Pointer to Genie's event record

  int RunID;
  int EventID;
  int Sample;  ///< Sample ID (as defined by VALOR)
  int NTracks; ///< Number of tracks in neutrino interaction

  double Ev;      ///< Neutrino energy
  double Ev_reco; ///< Reconstructed neutrino energy
  double Y;       ///< Inelasticity
  double Y_reco;  ///< Reconstructed inelasticity

  double VertexPosition[4]; ///< Initial vertex (position and time)

  int TrackID[500];         ///< MC track ID number
  int RecoTrack[500];       ///< Reconstructed: 1; Not reco: 0
  int FamilyTreeLevel[500]; ///< Primary: 1; Secondary: 2 ...
  int Pdg[500];             ///< PDG code of each track
  int Pdg_reco[500];        ///< Reconstructed PDG code

  double Momentum[500][3];      ///< Initial momentum
  double Momentum_reco[500][3]; ///< Reconstructed momentum
}


void PrintUsage()
{
  std::cerr << "\nUsage: GasTPCReco [-r <number>] -m <string>\n"
            << "                   -i <path> -o <path> -d <path>"
            << "\n" << std::endl;
  std::cerr << "   -r, --random-seed    : Random seed\n"
            << "   -m, --mode           : neutrino or antineutrino\n"
            << "   -i, --input-file     : Input file\n"
            << "   -o, --output-file    : Output file\n"
            << "   -d, --ecal-data      : ROOT file with ECAL data\n"
            << std::endl;

  exit(EXIT_FAILURE);
}


void ParseCmdLineOptions(int argc, char** argv)
{
  int c;

  while (true) {

    static struct option long_options[] =
    {
      {"random-seed", required_argument, 0, 'r'},
      {"mode",        required_argument, 0, 'm'},
      {"input-file",  required_argument, 0, 'i'},
      {"output-file", required_argument, 0, 'o'},
      {"ecal-data",   required_argument, 0, 'd'},
      {0, 0, 0, 0}
    };

    int option_index = 0;
    c = getopt_long(argc, argv, "r:m:i:o:d:", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'r':
        rnd_ = std::atoi(optarg);
        break;
      case 'm':
        mode_ = std::string(optarg);
        break;
      case 'i':
        input_file_ = std::string(optarg);
        break;
      case 'o':
        output_file_ = std::string(optarg);
        break;
      case 'd':
        ecal_data_ = std::string(optarg);
        break;
      case '?':
        PrintUsage();
        break;
      default:
        PrintUsage();
        break;
    }
  }

  if (input_file_ == "" || output_file_ == "" || mode_ == "") PrintUsage();
}


TTree* DefineOutputTree()
{
  TTree* tree = new TTree("NdtfDst", "DUNE ND-TF GArTPC DST");

  tree->Branch("gmcrec", "genie::NtpMCEventRecord", &(dst::gmcrec));

  tree->Branch("RunID",   &(dst::RunID),   "RunID/I");
  tree->Branch("EventID", &(dst::EventID), "EventID/I");
  tree->Branch("Sample",  &(dst::Sample),  "Sample/I");

  tree->Branch("Ev",      &(dst::Ev),      "Ev/D");
  tree->Branch("Ev_reco", &(dst::Ev_reco), "Ev_reco/D");
  tree->Branch("Y",       &(dst::Y),       "Y/D");
  tree->Branch("Y_reco",  &(dst::Y_reco),  "Y_reco/D");

  tree->Branch("VertexPosition", dst::VertexPosition, "VertexPosition[4]/D");

  tree->Branch("NTracks",  &(dst::NTracks), "NTracks/I");

  tree->Branch("TrackID",   dst::TrackID,   "TrackID[NTracks]/I");
  tree->Branch("RecoTrack", dst::RecoTrack, "RecoTrack[NTracks]/I");
  tree->Branch("Pdg",       dst::Pdg,       "Pdg[NTracks]/I");
  tree->Branch("Pdg_reco",  dst::Pdg_reco,  "Pdg_reco[NTracks]/I");

  tree->Branch("Momentum",      dst::Momentum,      "Momentum[NTracks][3]/D");
  tree->Branch("Momentum_reco", dst::Momentum_reco, "Momentum_reco[NTracks][3]/D");

  return tree;
}



int main(int argc, char** argv)
{
  ParseCmdLineOptions(argc, argv);

  bool FHC;
  if (mode_=="neutrino") FHC = true;
  else FHC = false;

  TRandom3* random = new TRandom3(rnd_);
  MomentumSmearer momentum_smearer(random);
  ParticleIdentification pid(random);

  TFile ofile(output_file_.c_str(), "RECREATE");
  TTree* tree = DefineOutputTree();

  gastpc::RootFileReader r;
  r.OpenFile(input_file_);

  for (int i=0; i<r.GetNumberOfEntries(); ++i) {

    gastpc::EventRecord& evtrec = r.Read(i);

    ////////////////////////////////////////////////////////

    // VERTEX FINDING

    gastpc::MCGenInfo* mcgi =
      InteractionFinder::ProcessEvent(evtrec.GetMCGenInfo());

    if (!mcgi) continue; // No argon interaction in this spill

    ////////////////////////////////////////////////////////

    // MOMENTUM PSEUDO-RECONSTRUCTION

    std::pair<gastpc::MCParticle*, gastpc::RecoParticle*> electron(0,0);
    std::pair<gastpc::MCParticle*, gastpc::RecoParticle*> muon(0,0);
    std::vector<std::pair<gastpc::MCParticle*, gastpc::RecoParticle*>> protons;
    std::vector<std::pair<gastpc::MCParticle*, gastpc::RecoParticle*>> pions;
    std::vector<std::pair<gastpc::MCParticle*, gastpc::RecoParticle*>> other;

    std::vector<std::pair<gastpc::MCParticle*, gastpc::RecoParticle*>> all;

    for (gastpc::MCParticle* mcp: mcgi->GetMCParticles()) {

      gastpc::RecoParticle* recop = new gastpc::RecoParticle();
      auto p = std::make_pair(mcp, recop);
      all.push_back(p);

      // Smear momentum of mc particles
      momentum_smearer.ProcessParticle(p);

      // We classify the particles according to their pdg code
      // for simpler classification in next step
      int pdg = std::abs(mcp->GetPDGCode());

      if (pdg == 11) {
        electron.first  = mcp;
        electron.second = recop;
      }
      else if (pdg == 13) {
        muon.first  = mcp;
        muon.second = recop;
      }
      else if (pdg == 211) {
        pions.push_back(p);
      }
      else if (pdg == 2212) {
        protons.push_back(p);
      }
      else {
        other.push_back(p);
      }
    }

    ////////////////////////////////////////////////////////

    // PARTICLE IDENTIFICATION

    bool found_lepton = false;

    // Try to identify the electron candidate
    int reco_pdg = pid.Electron(electron.first, electron.second);
    if (std::abs(reco_pdg) == 11) found_lepton = true;

    // Try to identify the muon candidate if no lepton has been found
    if (found_lepton == false) {
      int reco_pdg = pid.Muon(muon.first, muon.second);
      if (std::abs(reco_pdg) == 13) found_lepton = true;
    }

    // Process now the charged pions
    for (auto p: pions) {
      int reco_pdg = pid.Pion(p.first, p.second, found_lepton, FHC);
      if (std::abs(reco_pdg) == 13) found_lepton = true;
    }

    // Process the protons
    for (auto p: protons) {
      pid.Proton(p.first, p.second);
    }

    for (auto p: other) {
      if (p.first->GetPDGCode() == 111) {

        int mcid = p.first->GetMCID();
        double energy_ecal = 0.;
        for (gastpc::MCTrack* mct: evtrec.GetMCTracks()) {
          std::string label = mct->GetLabel();
          int ancestor = mct->GetMCParticle()->GetAncestor()->GetMCID();
          if ((label != "TPC") && (ancestor == mcid)) {
            for (gastpc::MCHit* mch: mct->GetMCHits())
              energy_ecal += mch->GetAmplitude();
          }
        }

        double ecal_threshold = 100. * gastpc::MeV;
        if (energy_ecal > ecal_threshold) {
          p.second->SetInitialMomentum(energy_ecal, 0., 0.);
          p.second->SetPDGCode(111);
        }
      }
    }

    ////////////////////////////////////////////////////////

    // NEUTRINO ENERGY RECONSTRUCTION

    NeutrinoEnergy nuE(all);

    ////////////////////////////////////////////////////////

    // OUTPUT DST

    genie::NtpMCEventRecord* gmcrec = mcgi->GetGeneratorRecord();
    genie::NtpMCEventRecord* gmcrec_copy = new genie::NtpMCEventRecord();
    gmcrec_copy->Copy(*gmcrec);
    dst::gmcrec = gmcrec_copy;

    dst::RunID   = evtrec.GetRunID();
    dst::EventID = evtrec.GetEventID();
    dst::Sample  = 0; // We don't classify the events this time

    dst::Ev      = nuE.TrueEnergy();
    dst::Ev_reco = nuE.RecoEnergy();
    dst::Y       = nuE.TrueY();
    dst::Y_reco  = nuE.RecoY();

    TLorentzVector* vertex = gmcrec->event->Vertex();
    dst::VertexPosition[0] = vertex->X() * gastpc::meter;
    dst::VertexPosition[1] = vertex->Y() * gastpc::meter;
    dst::VertexPosition[2] = vertex->Z() * gastpc::meter;
    dst::VertexPosition[3] = vertex->T() * gastpc::meter;

    dst::NTracks = all.size();

    unsigned int track_num = 0;

    for (auto p: all) {

      dst::TrackID[track_num] = p.first->GetMCID();
      dst::Pdg[track_num] = p.first->GetPDGCode();
      dst::Momentum[track_num][0] = p.first->GetInitialMomentum().GetX();
      dst::Momentum[track_num][1] = p.first->GetInitialMomentum().GetY();
      dst::Momentum[track_num][2] = p.first->GetInitialMomentum().GetZ();

      if (p.second->GetPDGCode() != 0) { // Reconstructed particle
        dst::RecoTrack[track_num] = 1;
        dst::Pdg_reco[track_num] = p.second->GetPDGCode();
        dst::Momentum_reco[track_num][0] = p.second->GetInitialMomentum().GetX();
        dst::Momentum_reco[track_num][1] = p.second->GetInitialMomentum().GetY();
        dst::Momentum_reco[track_num][2] = p.second->GetInitialMomentum().GetZ();
      }
      else {
        dst::RecoTrack[track_num] = 0;
      }

      ++track_num;
    }

    tree->Fill();

    for (auto p: all) { delete p.second; }
  }

  ofile.Write();
  ofile.Close();
  delete random;
  return EXIT_SUCCESS;
}
