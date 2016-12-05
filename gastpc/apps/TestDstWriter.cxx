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
#include "MCParticle.h"
#include "DstEntry.h"
#include "DstWriter.h"

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

  DstWriter odst;
  odst.OpenFile(output_filename);

  for (int i=0; i<r.GetNumberOfEntries(); ++i) {

    gastpc::EventRecord& evtrec = r.Read(i);

    for (gastpc::MCGenInfo* mcgi: evtrec.GetMCGenInfo()) {

      genie::NtpMCEventRecord* gmcrec = mcgi->GetGeneratorRecord();
      genie::Interaction* interaction = (gmcrec->event)->Summary();
      const genie::Target& tgt = interaction->InitState().Tgt();
      if (tgt.Z() != 18) continue;

      DstEntry dst_entry;

      dst_entry.RunID   = evtrec.GetRunID();
      dst_entry.EventID = mcgi->GetMCID();
      dst_entry.Sample  = -1;

      TLorentzVector* vertex = (gmcrec->event)->Vertex();
      dst_entry.VertexPosition[0] = vertex->X();
      dst_entry.VertexPosition[1] = vertex->Y();
      dst_entry.VertexPosition[2] = vertex->Z();
      dst_entry.VertexPosition[3] = vertex->T();

      dst_entry.NTracks = mcgi->GetParticles().size();

      int num_mcp = 0;

      for (gastpc::MCParticle* mcp: mcgi->GetParticles()) {

        dst_entry.TrackID[num_mcp] = mcp->GetMCID();
        dst_entry.FamilyTreeLevel[num_mcp] = mcp->GetFamilyTreeLevel();
        dst_entry.Pdg[num_mcp] = mcp->GetPDGCode();
        dst_entry.Pdg_reco[num_mcp] = mcp->GetPDGCode();

        dst_entry.InitialMomentum[num_mcp][0] = mcp->GetInitialMomentum()[0];
        dst_entry.InitialMomentum[num_mcp][1] = mcp->GetInitialMomentum()[1];
        dst_entry.InitialMomentum[num_mcp][2] = mcp->GetInitialMomentum()[2];

        dst_entry.Momentum_reco[num_mcp] = 0.;

        ++num_mcp;
      }

      odst.Write(dst_entry);
    }
  }

  odst.CloseFile();

  return EXIT_SUCCESS;
}
