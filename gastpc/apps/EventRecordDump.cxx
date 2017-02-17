// -------------------------------------------------------------------
/// \file   EventRecordDump.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 17 Feb 2017
// -------------------------------------------------------------------

#include "RootFileReader.h"
#include "EventRecord.h"
#include "MCGenInfo.h"
#include "MCParticle.h"

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <getopt.h>


namespace {
  int event_number_ = 0;
  std::string input_filename_("");
}



void PrintUsage()
{
  std::cerr << "\nUsage: EventRecordDump -i <filename> [-n <number>]\n"
            << std::endl;
  std::cerr << "   -i, --input-file    : Input file\n"
            << "   -n, --event-number  : Event to print out (all by default)"
            << std::endl;
  std::exit(EXIT_FAILURE);
}


void ParseCmdLineOptions(int argc, char** argv)
{
  int c;

  static struct option long_options[] =
  {
    {"input-file",   required_argument, 0, 'i'},
    {"event-number", required_argument, 0, 'n'},
    {0, 0, 0, 0}
  };

  while (true) {
    int option_index = 0;
    c = getopt_long(argc, argv, "i:n:", long_options, &option_index);
    if (c == -1) break;
    switch (c) {
      case 'i':
        input_filename_ = std::string(optarg);
        break;
      case 'n':
        event_number_ = std::atoi(optarg);
      case '?':
        PrintUsage();
      default:
        PrintUsage();
    }
  }

  if (input_filename_ == "") PrintUsage();
}


bool ReadGenieRecord(gastpc::MCGenInfo* mcgi)
{
  genie::NtpMCEventRecord* gmcrec = mcgi->GetGeneratorRecord();
  std::cout << *gmcrec << std::endl;

  //genie::Interaction* interaction = (gmcrec->event)->Summary();
  //const genie::Target& tgt = interaction->InitState().Tgt();
  //std::cout << "Interaction in Z = " <<

  //if (tgt.Z() != 18) continue;

  return true;
}


int main(int argc, char* argv[])
{
  ParseCmdLineOptions(argc, argv);

  gastpc::RootFileReader r;
  r.OpenFile(input_filename_);

  int min_range, max_range;
  min_range = event_number_;
  if (min_range > 0) max_range = min_range;
  else max_range = r.GetNumberOfEntries();

  for (int i=min_range; i<max_range; ++i) {
    gastpc::EventRecord& evtrec = r.Read(i);
    for (gastpc::MCGenInfo* mcgi: evtrec.GetMCGenInfo()) {
      ReadGenieRecord(mcgi);
   }
  }

  return EXIT_SUCCESS;
}
