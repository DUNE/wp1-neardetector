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

  TRandom3* random = new TRandom3(rnd_);
  MomentumSmearer momentum_smearer(random);

  gastpc::RootFileReader r;
  r.OpenFile(input_file_);

  for (int i=0; i<r.GetNumberOfEntries(); ++i) {

    gastpc::EventRecord& evtrec = r.Read(i);

    // Select neutrino interactions in active volume
    gastpc::MCGenInfo* mcgi =
      InteractionFinder::ProcessEvent(evtrec.GetMCGenInfo());

    // Process mc particles
    for (gastpc::MCParticle* mcp: mcgi->GetMCParticles()) {
      std::cout << *mcp << std::endl;
      momentum_smearer.ProcessParticle(mcp);
    }
  }

  return EXIT_SUCCESS;
}
