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
\last update June 2015
*/
//____________________________________________________________________________
#include "GasTPCPlotter.hh"

GasTPCPlotter::GasTPCPlotter() : useMagFieldFromCard(false) {
  backTracerBuff_ = &backTracer_;
}

GasTPCPlotter::~GasTPCPlotter() {
  //this->deleteTrees();
}

void GasTPCPlotter::push_backTree(TTree* t) {
  data_.push_back(t);
  //backTracerBuffers_
}

void GasTPCPlotter::setBackTracer(TTree* dataTree) {
  dataTree->GetBranch("backTracer")->SetAddress(&backTracerBuff_);
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

bool GasTPCPlotter::inFiducialVolume(NeutrinoEvent* nu_event,double fidCut){
  
  bool insideFid = false;
  //check if tpcVolume pointer is set and valid
  if(!tpcVolume) return false;
  
  //get the position of the hit and convert to cm as root usees this as the default dimensions
  TLorentzVector nuPos = nu_event->getPosition() * (1./ CLHEP::cm);
  
  double min=0, max=0;
  TGeoShape * bounding_box = tpcVolume->GetShape();
  
  //size of the tpc box - in cm!
  double xrange = bounding_box->GetAxisRange(1, min, max);
  double yrange = bounding_box->GetAxisRange(2, min, max);
  double zrange = bounding_box->GetAxisRange(3, min, max);
  
  //currently the tpc is positioned at the origin in the global coordinates
  // this could potentially not be the general case when more functionallity is added to software
  // then would need to find the position in local volumes and then convert to global coordinates.
  
  //if at the origin then the x,y and z coordiantes of the tpc edges are just the range/2
  double xEdge = xrange/2.0;
  double yEdge = yrange/2.0;
  double zEdge = zrange/2.0;
  
  //units must be in cm
  double xLimit = xEdge - fidCut*(1/CLHEP::cm);
  double yLimit = yEdge - fidCut*(1/CLHEP::cm);
  double zLimit = zEdge - fidCut*(1/CLHEP::cm);
  
  //condition to check it is in fiducial volume
  if( nuPos.X() <= xLimit && nuPos.X() >= -xLimit &&
      nuPos.Y() <= yLimit && nuPos.Y() >= -yLimit &&
      nuPos.Z() <= zLimit && nuPos.Z() >= -zLimit){
    
    insideFid = true;
  }
  else insideFid = false;
  
  return insideFid;
}

bool GasTPCPlotter::loadGeom(std::string filename, std::string tpcname){
  
  std::string tempFileName = filename;

  if(tempFileName == "")tempFileName = geomFileName_;

  //load the geometry file to check nodes
  TFile geomFile(tempFileName.c_str());

  //std::cout << "\n Geometry file is: " << tempFileName.c_str();

  if(!geomFile.IsZombie()){
    geomLoader = new GeometryLoader(tempFileName);
    world = geomLoader->getWorldVolume();
    tpcVolume = geomLoader->getTPCVolume(tpcname);
    if(!tpcVolume){
      std::cout << "\n\n !!!!! ERROR: Cannot find TPC Volume, Exiting... !!!!!" << std::endl;
      return false;
    }
    return true;
  }
  else{
    world = NULL;
    tpcVolume = NULL; 
    std::cout << "\n\n !!!!! ERROR: No geometry file is found, Exiting... !!!!!" << std::endl;
    return false;
  }
}


