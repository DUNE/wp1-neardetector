// -------------------------------------------------------------------
/// \file   TestIOGenieRec.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 2 Aug 2016
// -------------------------------------------------------------------

#include "EventRecord.h"
#include "MCGenInfo.h"
#include "RootFileWriter.h"
#include "RootFileReader.h"

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

#include <TFile.h>
#include <TTree.h>

#include <iostream>



void PrintUsage()
{
  std::cerr << "Usage: ./TestIOGenieRec <input_file>" << std::endl;
  std::exit(EXIT_FAILURE);
}


void PrintVertexInfo(genie::NtpMCEventRecord* gmcrec) 
{
  TLorentzVector* vtx = gmcrec->event->Vertex();
  std::cout << " - Vertex position: "
            << " (" << vtx->X() 
            << ", " << vtx->Y()
            << ", " << vtx->Z()
            << ") m" << std::endl;
}


void CreateFile(const std::string& genie_filename,
                const std::string& output_filename)
{
  std::cout << "CreateFile()" << std::endl;

  // Open GENIE ghep file and read the first entry of the event tree
  TFile genie_file(genie_filename.c_str());
  TTree* tree_genie = dynamic_cast<TTree*>(genie_file.Get("gtree"));
  genie::NtpMCEventRecord* gmcrec = 0;
  tree_genie->SetBranchAddress("gmcrec", &gmcrec);
  tree_genie->GetEntry(0);

  PrintVertexInfo(gmcrec);

  // Create a GasTPC EventRecord that includes a neutrino interaction
  // with the above GENIE record
  gastpc::MCGenInfo* mcgi = new gastpc::MCGenInfo();
  mcgi->SetGeneratorRecord(gmcrec);
  gastpc::EventRecord evtrec;
  evtrec.Add(mcgi);

  // Write the event record in an output file
  gastpc::RootFileWriter wr;
  wr.OpenFile(output_filename);
  wr.Write(evtrec);
  wr.CloseFile();
}


void ReadFile(const std::string& filename)
{
  std::cout << "ReadFile()" << std::endl;

  gastpc::RootFileReader r;
  r.OpenFile(filename);

  gastpc::EventRecord evtrec = r.Read(0);
  genie::NtpMCEventRecord* gmcrec = 
    (evtrec.GetMCGenInfo())[0]->GetGeneratorRecord();

  PrintVertexInfo(gmcrec);
}


int main(int argc, char* argv[])
{
  if (argc < 2) PrintUsage();

  // Let's assume that the second parameter is indeed the name
  // of an input GENIE file
  std::string genie_filename(argv[1]);
  std::string output_filename("test_output.root");

  CreateFile(genie_filename, output_filename);
  ReadFile(output_filename);

  return EXIT_SUCCESS;
}
