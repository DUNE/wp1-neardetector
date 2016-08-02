// -------------------------------------------------------------------
/// \file   TestIOGenieRec.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 2 Aug 2016
// -------------------------------------------------------------------

#include "EventRecord.h"
#include "NuInteraction.h"
#include "RootFileWriter.h"
#include "RootFileReader.h"

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

#include <TFile.h>
#include <TTree.h>


int main() 
{
  TFile fgenie("whatever.ghep.root");
  TTree* tgenie = (TTree*) fgenie.Get("gtree");
  genie::NtpMCEventRecord* gmcrec = 0;
  tgenie->SetBranchAddress("gmcrec", &gmcrec);

  tgenie->GetEntry(0);

  gastpc::EventRecord evtrec;
  gastpc::NuInteraction* nuint = new gastpc::NuInteraction();
  nuint->SetGenieRecord(gmcrec);
  evtrec.Add(nuint);
  gastpc::RootFileWriter wr;

  std::string filename = "test_output.root";

  wr.OpenFile(filename);
  wr.Write(evtrec);
  wr.CloseFile();

  gastpc::RootFileReader rd;
  rd.OpenFile(filename);

  gastpc::EventRecord evtrec2 = rd.Read(0);
  std::vector<gastpc::NuInteraction*> nuints = evtrec2.GetNuInteractions();
  genie::NtpMCEventRecord* gmcrec2 = nuints[0]->GetGenieRecord();
  genie::EventRecord* record = gmcrec2->event;
  TLorentzVector* vtx = record->Vertex();

  std::cout << "Vertex: " << vtx->Y() << std::endl;

  return EXIT_SUCCESS;
}