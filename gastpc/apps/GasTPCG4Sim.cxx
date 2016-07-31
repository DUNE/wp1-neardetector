// -------------------------------------------------------------------
/// \file   GasTPCG4Sim.cxx
/// \brief  
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
  bool batch_mode_ = true;
  int num_events_ = 0; 
  std::string cfgmacro_filename_(""); ///< Filename of configuration macro
}


void ParseCmdLineOptions(int argc, char** argv)
{
  int c;

  static struct option long_options[] =
  {
    {"interactive", no_argument,       0, 'i'},
    {"batch",       no_argument,       0, 'b'},
    {"nevents",     required_argument, 0, 'n'},
    {0, 0, 0, 0}
  };

  while (true) {

    int option_index = 0;
    c = getopt_long(argc, argv, "ibn:", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'b':
        batch_mode_ = true;
        break;
      case 'i':
        batch_mode_ = false;
        break;
      case 'n':
        num_events_ = std::atoi(optarg);
        break;
      case '?':
        std::exit(EXIT_FAILURE);
      default:
        std::exit(EXIT_FAILURE);
    }
  }
}



void PrintUsage()
{
  std::cerr << "\nUsage: " << std::endl;
  exit(EXIT_FAILURE);
}


int main(int argc, char** argv)
{
  // Parse command-line options
  ParseCmdLineOptions(argc, argv);

  if (argc < 2) PrintUsage();

  // If there is no other command-line argument to be processed, abort
  // because the user has not provided a configuration macro.
  // (The variable optind is set by getopt_long to the index of the next
  // element of the argv array to be processed. Once getopt has found all 
  // of the option arguments, this variable can be used to determine where 
  // the remaining non-option arguments begin.)
  //if (optind == argc) PrintUsage();

  // Assume that the name of the configuration macro is the first
  // command-line parameters that is not a GNU option.
  //G4String macro_filename = argv[optind];

  //if (macro_filename == "") PrintUsage(); 

  //////////////////////////////////////////////////////////

  RunManager* runmgr = new RunManager();
  //runmgr->ExecuteMacroFile(macro_filename);
  runmgr->Initialize();

  if (!batch_mode_) {
    G4VisManager* vismgr = new G4VisExecutive();
    vismgr->Initialize();
    G4UIsession* session = new G4UIterminal(new G4UItcsh);
    G4UImanager::GetUIpointer()->
      ApplyCommand("/control/execute vis.mac");
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