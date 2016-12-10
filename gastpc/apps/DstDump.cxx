// -------------------------------------------------------------------
/// \file   TestIOGenieRec.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 2 Aug 2016
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



void PrintUsage()
{
  std::cerr << "Usage: TestIOGenieRec <input_file>" << std::endl;
  std::exit(EXIT_FAILURE);
}


bool ReadGenieRecord(genie::NtpMCEventRecord* gmcrec)
{
  std::cout << *gmcrec << std::endl;

  //genie::Interaction* interaction = (gmcrec->event)->Summary();
  //const genie::Target& tgt = interaction->InitState().Tgt();
  //std::cout << "Interaction in Z = " << 

  //if (tgt.Z() != 18) continue;

  return true;
}


int main(int argc, char* argv[])
{
  if (argc < 2) PrintUsage();

  TFile dstfile(argv[1]);
  TTree* tree = dynamic_cast<TTree*>(dstfile.Get("NdtfDst"));
  genie::NtpMCEventRecord* gmcrec = 0;
  tree->GetBranch("gmcrec")->SetAddress(&gmcrec);

  for (Long64_t i=0; i<tree->GetEntries(); ++i) {

    tree->GetEntry(i);
    ReadGenieRecord(gmcrec);

  }  

  return EXIT_SUCCESS;
}
