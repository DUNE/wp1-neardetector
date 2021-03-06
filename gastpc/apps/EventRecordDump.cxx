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
  int event_number_ = -1;
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
        break;
      case '?':
        PrintUsage();
      default:
        PrintUsage();
    }
  }

  if (input_filename_ == "") PrintUsage();
}


int main(int argc, char* argv[])
{
  ParseCmdLineOptions(argc, argv);

  gastpc::RootFileReader r;
  r.OpenFile(input_filename_);

  int min_range, max_range;

  if (event_number_ >= 0) {
    min_range = event_number_;
    max_range = event_number_ + 1;
  }
  else {
    min_range = 0;
    max_range = r.GetNumberOfEntries();
  }

  for (int i=min_range; i<max_range; ++i) {
    gastpc::EventRecord& evtrec = r.Read(i);
    std::cout << evtrec << std::endl;
  }

  r.CloseFile();

  return EXIT_SUCCESS;
}
