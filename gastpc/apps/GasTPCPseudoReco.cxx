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
#include "InteractionFinder.h"
#include "MomentumSmearer.h"
#include "ParticleIdentification.h"

#include <TRandom3.h>

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


int main(int argc, char** argv)
{
  ParseCmdLineOptions(argc, argv);

  bool FHC;
  if (mode_=="neutrino") FHC = true;
  else FHC = false;

  TRandom3* random = new TRandom3(rnd_);
  MomentumSmearer momentum_smearer(random);
  ParticleIdentification pid(random);

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

    for (gastpc::MCParticle* mcp: mcgi->GetMCParticles()) {

      // Smear momentum of mc particles
      gastpc::RecoParticle* recop = momentum_smearer.ProcessParticle(mcp);

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
        auto p = std::make_pair(mcp, recop);
        pions.push_back(p);
      }
      else if (pdg == 2212) {
        auto p = std::make_pair(mcp, recop);
        protons.push_back(p);
      }
      else {
        auto p = std::make_pair(mcp, recop);
        other.push_back(p);
      }
    }

    ////////////////////////////////////////////////////////

    // PARTICLE IDENTIFICATION

    bool found_lepton = false;

    // Try to identify the electron candidate
    if (electron.second != 0) {
      int reco_pdg = pid.Electron(electron.first, electron.second);
      if (std::abs(reco_pdg) == 11) found_lepton = true;
    }

    // Try to identify the muon candidate if no lepton has been found
    if (!found_lepton) {
      int reco_pdg = pid.Muon(muon.first, muon.second);
      if (std::abs(reco_pdg) == 13) found_lepton = true;
    }

    // Process now the charged pions
    for (auto p: pions) {
      int reco_pdg = pid.Pion(p.first, p.second, found_lepton, FHC);
      if (std::abs(reco_pdg) == 13) found_lepton = true;
    }

    for (auto p: protons) {
      pid.Proton(p.first, p.second);
    }

  }

  delete random;
  return EXIT_SUCCESS;
}
