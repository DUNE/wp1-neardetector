// -------------------------------------------------------------------
/// \file   GasTPCG4Sim.cxx
/// \brief  Main application of the GasTPC Geant4 simulation.
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 1 June 2016
// -------------------------------------------------------------------

#include "RunManager.h"

#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UIsession.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4UImanager.hh>

#include <getopt.h>


namespace {
  bool batch_mode_ = true;       ///< Batch mode flag
  int num_events_ = 0;           ///< Number of events
  std::string cfg_filename_(""); ///< Filename of configuration macro
  std::string detector_("");     ///< Detector construction name
  std::string generator_("");    ///< Primary generator name
}


void PrintUsage()
{
  std::cerr << "Usage: GasTPCG4Sim [-b|i] [-c <file>] [-n <number>]\n"
            << "                   -d <name> -g <name>"
            << "\n" << std::endl;
  std::cerr << "   -b, --batch          : Run in batch mode (default)\n"
            << "   -i, --interactive    : Run in interactive/graphic mode\n"
            << "   -c, --config-file    : Configuration macro file\n"
            << "   -n, --nevents        : Number of events to simulate\n"
            << "   -d, --detector       : Detector construction tag\n"
            << "   -g, --generator      : Primary generator tag"
            << std::endl; 

  exit(EXIT_FAILURE);
}


void ParseCmdLineOptions(int argc, char** argv)
{
  int c;

  static struct option long_options[] =
  {
    {"interactive", no_argument,       0, 'i'},
    {"batch",       no_argument,       0, 'b'},
    {"config-file", required_argument, 0, 'c'},
    {"nevents",     required_argument, 0, 'n'},
    {"detector",    required_argument, 0, 'd'},
    {"generator",   required_argument, 0, 'g'},
    {0, 0, 0, 0}
  };

  while (true) {

    int option_index = 0;
    c = getopt_long(argc, argv, "ibc:n:d:g:", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'b':
        batch_mode_ = true;
        break;
      case 'i':
        batch_mode_ = false;
        break;
      case 'c':
        cfg_filename_ = std::string(optarg);
        break;
      case 'n':
        num_events_ = std::atoi(optarg);
        break;
      case 'd':
        detector_ = std::string(optarg);
        break;
      case 'g':
        generator_ = std::string(optarg);
        break; 
      case '?':
        std::exit(EXIT_FAILURE);
      default:
        std::exit(EXIT_FAILURE);
    }
  }

  // Make sure the user has provided all mandatory arguments
  if (generator_ == "" || detector_ == "")
    PrintUsage();
}


int main(int argc, char** argv)
{
  ParseCmdLineOptions(argc, argv);

  RunManager* runmgr = new RunManager(detector_, generator_);
  runmgr->ExecuteMacroFile(cfg_filename_);
  runmgr->Initialize();

  if (!batch_mode_) {
    G4VisManager* vismgr = new G4VisExecutive();
    vismgr->Initialize();
    G4UIsession* session = new G4UIterminal(new G4UItcsh);
    G4UImanager::GetUIpointer()->
      ApplyCommand("/control/execute config/g4vis.mac");
    session->SessionStart();

    delete session;
    delete vismgr;
  }
  else {
    runmgr->BeamOn(num_events_);
  }

  delete runmgr;
  return EXIT_SUCCESS;
}