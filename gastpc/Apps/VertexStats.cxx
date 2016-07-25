// -------------------------------------------------------------------
/// \file   VertexStats.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 24 July 2016
// -------------------------------------------------------------------

#include <TChain.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>
#include <TLorentzVector.h>

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

#include <iostream>


TChain* OpenGenieFiles(const std::string& filename)
{
  TChain* chain = new TChain("gtree");
  int num_files;
  num_files = chain->Add(filename.c_str());
  std::cout << num_files << std::endl;
  return chain;
}



int main(int argc, char const *argv[])
{
  std::string geom_file = "geometry.gdml";
  std::string genie_files = "neutrino.862806.*";

  TChain* chain = new TChain("gtree");
  chain->Add(genie_files.c_str());

  genie::NtpMCEventRecord* mcrec = 0;
  chain->SetBranchAddress("gmcrec", &mcrec);

  TGeoManager* geomgr = new TGeoManager();
  geomgr->Import(geom_file.c_str());

  std::cout << "Loaded geometry" << std::endl;

  TFile* current_file = 0;
  double total_pot = 0.;

  std::cout << "Number of entries in TChain: " << chain->GetEntries() << std::endl;

  for (int i=0; i<chain->GetEntries(); i++) {

    std::cout << "entry no. " << i << std::endl;

    chain->GetEntry(i);

    std::cout << "POT: " << chain->GetWeight() << std::endl;

    genie::EventRecord* record = mcrec->event;

    TLorentzVector* vertex_position = record->Vertex();

    geomgr->SetCurrentPoint(vertex_position->X() * 100.,
			    vertex_position->Y() * 100.,
			    vertex_position->Z() * 100.);

    TGeoNode* node = geomgr->FindNode();
    std::cout << "Name: " << node->GetName() << std::endl;
    TGeoMatrix* matrix = node->GetMatrix();
    const Double_t* tr = matrix->GetTranslation();

    std::cout << "X = " << tr[0] << " Y = " << tr[1] << " Z = " << tr[2] << std::endl;

  }

  std::cout << total_pot << std::endl;

  return 0;
}
