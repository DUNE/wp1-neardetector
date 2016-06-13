#include "GasTPCPlotter.hh"
#include <THnSparse.h>
#include <iostream>

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
  TTree* VoxelsTree = new TTree("VoxelsTree", "VoxelsTree");

  const Int_t dim = 3;
  Int_t bins[dim] = {700, 700, 700};
  Double_t maxs[dim] = { 3500.,  3500., 3558.2+3500.};
  Double_t mins[dim] = {-3500., -3500., 3558.2-3500.};
  THnSparseF* voxels = new THnSparseF("Voxels", "", dim, bins, mins, maxs);
  VoxelsTree->Branch("voxels", "THnSparseF", &voxels);

  for(Int_t i=0; i<detectorHitsTree->GetEntries(); i++){

    detectorHitsTree->GetEntry(i);

    voxels->Reset();

    // TPC Hits
    HitCollection tpcSdHits = tpcplotter->getSimData()->getTpcFidHits();

    for (Int_t j=0; j<tpcSdHits.size(); j++) {
      SDHit tmpHit = tpcSdHits.at(j);
      Double_t xyz[3] = {tmpHit.getPosition().X(), tmpHit.getPosition().Y(), tmpHit.getPosition().Z()};
      voxels->Fill(xyz, tmpHit.getEdep()*1000.);
    }

    VoxelsTree->Fill();
  }

  VoxelsTree->Print();
  FileMC.Write();

  tpcplotter->deleteTrees();
  FileMC.Close();


    //f.Close();
}
