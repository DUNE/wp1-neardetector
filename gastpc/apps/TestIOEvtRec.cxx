// -------------------------------------------------------------------
/// \file   TestIOEvtRec.cxx
/// \brief  Test of the gastpc ROOT file reader/writer.
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 31 July 2016
// -------------------------------------------------------------------

#include "RootFileWriter.h"
#include "RootFileReader.h"
#include "EventRecord.h"
#include "MCParticle.h"

#include <iostream>


void CreateFile(gastpc::RootFileWriter& w, 
                const std::string& filename, int n=10)
{
  std::cout << "CreateFile(): " << filename << std::endl;

  // Create an event record and add it 10 particles
  gastpc::EventRecord evtrec;

  for (int i=0; i<n; i++) {
    gastpc::MCParticle* mcp = new gastpc::MCParticle();
    mcp->SetMCID(i);
    evtrec.Add(mcp);
  }

  std::cout << " - The event record contains " << evtrec.GetMCParticles().size()
            << " MC particles." << std::endl;

  bool ok = w.OpenFile(filename);
  
  if (!ok) {
    std::cerr << "ERROR: Could not open file!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  w.Write(evtrec);
  w.CloseFile();
}


void ReadFile(gastpc::RootFileReader& r, const std::string& filename)
{
  std::cout << "ReadFile(): " << filename << std::endl;

  // Open file, print number of entries and read first one
  bool ok = r.OpenFile(filename);

  if (!ok) {
    std::cerr << "ERROR: Could not open file!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::cout << " - File contains " << r.GetNumberOfEntries() 
            << " entries." << std::endl; 

  gastpc::EventRecord evtrec = r.Read(0);
  std::cout << " - The event record contains " << evtrec.GetMCParticles().size()
            << " MC particles." << std::endl;

  r.CloseFile();
}


int main()
{
  std::string filename_1 = "test_io_evtrec.1.root";
  std::string filename_2 = "test_io_evtrec.2.root";

  ////////////////////////////

  gastpc::RootFileWriter writer;
  CreateFile(writer, filename_1, 10);
  CreateFile(writer, filename_2, 20);

  ////////////////////////////

  gastpc::RootFileReader reader;
  ReadFile(reader, filename_1);
  ReadFile(reader, filename_2);  

  return 0;
}
