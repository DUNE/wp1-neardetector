#include <iostream>
#include "GasTPCPlotter.hh"


int main(int argc, char ** argv) 
{
  std::string inputfile  = "";

  for(int iArg = 1; iArg < argc; iArg++) {
    if((!strcasecmp(argv[iArg],"-i"))||(!strcasecmp(argv[iArg],"-input"))) {inputfile = argv[++iArg] ;
    }
  }

  TFile FileMC(inputfile.c_str(),"UPDATE");
    if(FileMC.GetNkeys()<1){
      std::cout << "ERROR::Bad input file: " << inputfile << std::endl;
      return 1;
    }

  GasTPCPlotter *tpcplotter = new GasTPCPlotter();
  tpcplotter->setupTrees(FileMC);

  TTree *detectorHitsTree = tpcplotter->getTrackingTree();
  Int_t TrnEntries        = detectorHitsTree->GetEntries();

  TTree* VoxelsTree = new TTree("VoxelsTree", "VoxelsTree");

  THnSparseF* hn = 0;
  VoxelsTree->Branch("Voxels", &hn);

  for(Int_t i=0; i<detectorHitsTree->GetEntries(); i++){

    Int_t bins[3] = {700, 700, 700};
    Double_t maxs[3] = { 3500.,  3500., 3558.2+3500.};
    Double_t mins[3] = {-3500., -3500., 3558.2-3500.};

    THnSparseF voxels("Voxels", "", 3, bins, mins, maxs);

    // TPC Hits
    HitCollection tpcSdHits = tpcplotter->getSimData()->getTpcFidHits();

    for (Int_t j=0; j<tpcSdHits.size(); j++) {
      SDHit tmpHit = tpcSdHits.at(j);
      Double_t xyz[3] = {tmpHit.getPosition().X(), tmpHit.getPosition().Y(), tmpHit.getPosition().Z()};
      voxels.Fill(xyz, tmpHit.getEdep()*1000);
    }

    hn = &voxels;
    VoxelsTree->Fill();
  }

  VoxelsTree->Write();
  FileMC.Close();
}