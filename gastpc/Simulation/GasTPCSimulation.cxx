// -------------------------------------------------------------------
/// \file   GasTPCSimulation.cxx
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



void PrintUsage()
{
  std::cerr << "\nUsage: " << std::endl;
  exit(EXIT_FAILURE);
}


int main(int argc, char** argv)
{
  // Parse command-line options

  if (argc < 2) PrintUsage();

  G4bool batch = true;
  G4int num_events = 0;

  static struct option long_options[] =
  {
    {"batch", no_argument, 0, 'b'},
    {"interactive", no_argument, 0, 'i'},
    {"num_events", required_argument, 0, 'n'},
    {0, 0, 0, 0}
  };

  int c;

  while (true) {

    opterr = 0;
    c = getopt_long(argc, argv, "bin:", long_options, 0);

    if (c == -1) break; // Exit if we are done reading options

    switch (c) {

      case 'b':
        batch = true;
        break;

      case 'i':
        batch = false;
        break;

      case 'n':
        num_events = atoi(optarg);
        break;

      case '?':
        break;

      default:
        abort();
    }
  }

  // If there is no other command-line argument to be processed, abort
  // because the user has not provided a configuration macro.
  // (The variable optind is set by getopt_long to the index of the next
  // element of the argv array to be processed. Once getopt has found all 
  // of the option arguments, this variable can be used to determine where 
  // the remaining non-option arguments begin.)
  if (optind == argc) PrintUsage();

  // Assume that the name of the configuration macro is the first
  // command-line parameters that is not a GNU option.
  G4String macro_filename = argv[optind];

  if (macro_filename == "") PrintUsage(); 

  //////////////////////////////////////////////////////////

  RunManager* runmgr = new RunManager();
  runmgr->ExecuteMacroFile(macro_filename);
  runmgr->Initialize();

  if (!batch) {
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
    runmgr->BeamOn(num_events);
  }

  delete runmgr;

  return EXIT_SUCCESS;
}