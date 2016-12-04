// -------------------------------------------------------------------
/// \file   TestDstWriter.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 3 Dic 2016
// -------------------------------------------------------------------

#include "RootFileReader.h"
#include "EventRecord.h"
#include "MCGenInfo.h"

#include <Ntuple/NtpMCEventRecord.h>

#include <iostream>


void PrintUsage()
{
  std::cerr << "Usage: TestDstWriter <input_file>" << std::endl;
  std::exit(EXIT_FAILURE);
}


int main(int argc, char const *argv[])
{
  if (argc < 2) PrintUsage();

  std::string input_filename(argv[1]);
  std::string output_filename("output.dst.root");

  gastpc::RootFileReader r;
  r.OpenFile(input_filename);

  for (int i=0; i<r.GetNumberOfEntries(); ++i) {

    gastpc::EventRecord& evtrec = r.Read(i);

    for (gastpc::MCGenInfo* mcgi: evtrec.GetMCGenInfo()) {

      genie::NtpMCEventRecord* gmcrec = mcgi->GetGeneratorRecord();
      genie::Interaction* interaction = (gmcrec->event)->Summary();
      const genie::Target& tgt = interaction->InitState().Tgt();
      if (tgt.Z() != 18) continue;

      std::cout << "Argon interaction" << std::endl;
    }
  }

  return EXIT_SUCCESS;
}
