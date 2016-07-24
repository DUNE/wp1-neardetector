// -------------------------------------------------------------------
/// \file   VertexStats.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 24 July 2016
// -------------------------------------------------------------------

#include <TChain.h>
#include <TGeoManager.h>

#include <Ntuple/NtpMCTreeHeader.h>



TChain* OpenGenieFiles(const std::string& filename)
{
  TChain* chain = new TChain("gtree");
  chain->Add(filename);
  return chain;
}



int main(int argc, char const *argv[])
{
  std::string geom_file = "geometry.gdml";
  std::string genie_files = "";

  TChain* chain = OpenGenieFiles(genie_files);

  genie::NtpMCEventRecord* mcrec = 0;
  chain->SetBranchAddress("gmrec", &mcrec);

  TGeoManager* geomgr = new TGeoManager();
  geomgr->Import(geom_file);

  TFile* current_file = 0;
  double total_pot = 0.;

  for (unsigned int i=0; i<chain->GetEntries(); i++) {

    chain->GetEntry(i);

    if (current_file != chain->GetFile()) {
      total_pot += chain.GetWeight();
      current_file = chain->GetFile();
    }

    EventRecord* record = mcrec->event;

    TLorentzVector* vertex_position = record->Vertex();
    std::cout << vertex_position << std::endl;

    //Interaction* inter = event->Summary();




  }

  return 0;
}