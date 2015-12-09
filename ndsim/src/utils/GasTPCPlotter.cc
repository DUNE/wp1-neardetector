//____________________________________________________________________________
/*!

\class    GasTPCPlotter

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Oct 2015
*/
//____________________________________________________________________________
#include "GasTPCPlotter.hh"

GasTPCPlotter::GasTPCPlotter() {
  //backTracerBuff_ = &backTracer_;
}

GasTPCPlotter::~GasTPCPlotter() {
  //this->deleteTrees();
}

void GasTPCPlotter::push_backTree(TTree* t) {
  data_.push_back(t);
}

bool GasTPCPlotter::setupTrees(TFile &datafile){

  //if file is return 0
  if(datafile.GetNkeys()<1){
    std::cout << "\n\n!!Problem reading event from file, exiting...." <<std::endl;
    return false;
  }

  //set data addresses
  if(datafile.Get("NuHits")){
    nuHitTree = (TTree*)datafile.Get("NuHits");
    this->setDataAddress<NeutrinoHit>(nuHit_, nuHitTree);
    nuHits_nEntries = nuHitTree->GetEntries();
    //push_backTree(nuHitTree);
  }
  else {
    nuHitTree = NULL;
    nuHit_ = NULL;
    nuHits_nEntries = 0;
  }

  if(datafile.Get("NuInteractions")){
    nuEventTree = (TTree*)datafile.Get("NuInteractions");
    this->setDataAddress<NeutrinoEvent>(nuEvent_, nuEventTree);
    nuEvents_nEntries = nuEventTree->GetEntries();
    //push_backTree(nuEventTree);
  }
  else{
    nuEventTree = NULL;
    nuEvent_ = NULL;
    nuEvents_nEntries = 0;
  }
  
  if(datafile.Get("Tracking")){
    detectorHitsTree = (TTree*)datafile.Get("Tracking");
    this->setDataAddress<SimulData>(simData_, detectorHitsTree);
    this->setDataAddress<GeantTrackingTruth>(trackingRecord_, detectorHitsTree);
    detHits_nEntries = detectorHitsTree->GetEntries();
    //push_backTree(detectorHitsTree);
  }
  else{
    detectorHitsTree = NULL;
    trackingRecord_ = NULL;
    simData_ = NULL;
    detHits_nEntries = 0;
  }
  //get run statistics
  if(datafile.Get("runStats")){
    stats_ = dynamic_cast<RunStats*>(datafile.Get("runStats"));
  }
  else stats_ = NULL;

  return true;
}

bool GasTPCPlotter::deleteTrees(){
  
  if(nuHit_){
    this->deleteDataAddress<NeutrinoHit>(nuHit_);
    nuHit_ = NULL;
  }
  if(nuEvent_){
    this->deleteDataAddress<NeutrinoEvent>(nuEvent_);
    nuEvent_ = NULL;
  }
  if(simData_){
    this->deleteDataAddress<SimulData>(simData_);
    simData_ = NULL;
  }
  if(trackingRecord_){
    this->deleteDataAddress<GeantTrackingTruth>(trackingRecord_);
    trackingRecord_ = NULL;
  }

  return true;
}

bool GasTPCPlotter::copyTree(TFile * oldfile,std::string newfileName,std::string treeName){

  //Get old tree and set top branch address
  TTree *oldtree = (TTree*)oldfile->Get(treeName.c_str());
  this->setupTrees(*oldfile);
  
  //Create a new file + a clone of old tree in new file
  TFile *newfile = new TFile(newfileName.c_str(),"recreate");
  TTree *newtree = oldtree->CloneTree();
  
  //newtree->Print();
  newfile->Write();
  
  //delete from memory
  this->deleteTrees();
  delete newfile;
  
  return true;
}

bool GasTPCPlotter::copyTrees(int startrun,int runs,std::string oldfileBaseName,std::string newfileName,std::string treeName){

  //Create a new file + a clone of old tree in new file
  TFile *newfile = new TFile(newfileName.c_str(),"recreate");
  
  //loop over runs
  for(int i=startrun;i<(startrun+runs);i++){
    std::stringstream ss;
    ss << i;
    std::string filename = oldfileBaseName + ss.str() + ".root";
    
    TFile datafile(filename.c_str());
    TTree *oldtree = (TTree*)datafile.Get(treeName.c_str());
    //set data addresses
    if(!setupTrees(datafile)) continue;
    
    TTree *newtree = oldtree->CloneTree();
    std::string treename = newtree->GetName();
    treename += "_run" + ss.str();
    newfile->cd();
    newtree->Write(treename.c_str(), TObject::kOverwrite);
    
    //delete from memory
    this->deleteTrees();
  }
  
  //newtree->Print();
  newfile->Write();
  delete newfile;
  
  return true;
}
