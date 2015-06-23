//____________________________________________________________________________
/*!

\class    GasTPCAnalysis

\brief    Program to group the hits and do basic recon

\author	  Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Jun 2015
\last update Jun 2015
*/
//____________________________________________________________________________
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <cmath>
#include <algorithm>

#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TVector3.h>
#include <TVector2.h>
#include <TCanvas.h>
#include <TRandom3.h>

#include "GasTPCPlotter.hh"

const double deltaSagitta = 0.05; // mm
const double scintresolution = 10; // mm
const double tpcresolution = 0.6; // mm
const double magfield = 0.4; // tesla
const double angresolution = 0.2; // rad

int main(int argc, char ** argv) {

  std::string inputfile  = "";
  std::string outputfile = "";
  std::string target     = "";
  bool savehisto = false;
  for(int iArg = 1; iArg < argc; iArg++) {
    if((!strcasecmp(argv[iArg],"-i"))||(!strcasecmp(argv[iArg],"-input"))) {
      inputfile = argv[++iArg] ;
    }
    if((!strcasecmp(argv[iArg],"-o"))||(!strcasecmp(argv[iArg],"-output"))) {
      outputfile = argv[++iArg] ;
    }
    if((!strcasecmp(argv[iArg],"-t"))||(!strcasecmp(argv[iArg],"-target"))) {
      target = argv[++iArg] ;
    }
    if(!strcasecmp(argv[iArg],"-s")) {
      savehisto = true;
    }
  }

  TFile FileMC(inputfile.c_str(),"READ");
  if(FileMC.GetNkeys()<1){
    std::cout << "ERROR::Bad input file: " << inputfile << std::endl;
    return 1;
  }

  
  GasTPCPlotter *tpcplotter = new GasTPCPlotter();

  tpcplotter->setupTrees(FileMC);

  TTree *nuEventTree      = tpcplotter->getNuInteractionsTree();
  TTree *detectorHitsTree = tpcplotter->getTrackingTree();

  Int_t nEntries          = nuEventTree->GetEntries();
  Int_t TrnEntries        = detectorHitsTree->GetEntries();

  if(TrnEntries != nEntries){
    std::cout << "ERROR::Truth tree and GEANT tree doesn't have the same number of entries in file: " << inputfile << std::endl;
    return 1;
  }
  
  // The output tree
  TTree *GasTPCTree = new TTree("GasTPCTree","GasTPCTree");
  const Int_t kMaxTrack = 250;
  Int_t EventID, RunID, ISTopology, FSTopology, neutPdg, neutcode, Target, NGeantHits[kMaxTrack], NGeantTracks, NCEcalTracks, TrackID[kMaxTrack], ParentID[kMaxTrack], Pdg[kMaxTrack], ParentPdg[kMaxTrack];
  Double_t FirstGeantHitPosition[kMaxTrack][3], LastGeantHitPosition[kMaxTrack][3], VertexPosition[3], x, y, Q2, W, weight, Enu, dEdx[kMaxTrack], dEdxRecon[kMaxTrack], dEdxTruncated[kMaxTrack], dEdxReconTruncated[kMaxTrack], dEdxSigma[kMaxTrack];
  Double_t Momentum[kMaxTrack], ReconMomentum[kMaxTrack], Pt[kMaxTrack], Costheta[kMaxTrack], ReconCostheta[kMaxTrack], TrackLength[kMaxTrack], TotalEDep[kMaxTrack];
  Int_t NFSParticles, FSPdg[kMaxTrack],  NISParticles, ISPdg[kMaxTrack], ReconCharge[kMaxTrack];
  Double_t FSMomentum[kMaxTrack], FSCostheta[kMaxTrack];

  Int_t NGamma, GammaPdg[kMaxTrack], GammaParentPdg[kMaxTrack], GammaParentID[kMaxTrack], GammaWhichEcal[kMaxTrack], GammaNHits[kMaxTrack];
  Double_t GammaMomentum[kMaxTrack], GammaEDep[kMaxTrack], GammaFirstGeantHitPosition[kMaxTrack][3];

  Int_t NNeutron, NeutronWhichEcal[kMaxTrack], NeutronNHits[kMaxTrack];
  Double_t NeutronMomentum[kMaxTrack], NeutronEDep[kMaxTrack], NeutronFirstGeantHitPosition[kMaxTrack][3];

  Int_t EcalNGeantHits[kMaxTrack], WhichEcal[kMaxTrack], EcalFirstLayer[kMaxTrack], EcalLastLayer[kMaxTrack];
  Double_t EcalMomentum[kMaxTrack], EcalTrackLength[kMaxTrack], EcalTotalEDep[kMaxTrack], EcalFirstGeantHitPosition[kMaxTrack][3], EcalLastGeantHitPosition[kMaxTrack][3], EcalShowerWidth[kMaxTrack][3], EcalShowerEDepSlices[kMaxTrack][5];

  Int_t NCEcalNGeantHits[kMaxTrack], NCWhichEcal[kMaxTrack], NCEcalUnusedHits;
  Double_t NCEcalMomentum[kMaxTrack], NCEcalTrackLength[kMaxTrack], NCEcalTotalEDep[kMaxTrack], NCEcalFirstGeantHitPosition[kMaxTrack][3], NCEcalLastGeantHitPosition[kMaxTrack][3];
 
  GasTPCTree->Branch("EventID",                      &EventID,                      "EventID/I");
  GasTPCTree->Branch("RunID",                        &RunID,                        "RunID/I");
  GasTPCTree->Branch("ISTopology",                   &ISTopology,                   "ISTopology/I");
  GasTPCTree->Branch("FSTopology",                   &FSTopology,                   "FSTopology/I");
  GasTPCTree->Branch("Enu",                          &Enu,                          "Enu/D");
  GasTPCTree->Branch("neutPdg",                      &neutPdg,                      "neutPdg/I");
  GasTPCTree->Branch("neutcode",                     &neutcode,                     "neutcode/I");
  GasTPCTree->Branch("x",                            &x,                            "x/D");
  GasTPCTree->Branch("y",                            &y,                            "y/D");
  GasTPCTree->Branch("Q2",                           &Q2,                           "Q2/D");
  GasTPCTree->Branch("W",                            &W,                            "W/D");
  GasTPCTree->Branch("weight",                       &weight,                       "weight/D");
  GasTPCTree->Branch("Target",                       &Target,                       "Target/I");
  
  GasTPCTree->Branch("NGeantTracks",                 &NGeantTracks,                 "NGeantTracks/I");
  GasTPCTree->Branch("NGeantHits",                   &NGeantHits,                   "NGeantHits[NGeantTracks]/I");  
  GasTPCTree->Branch("FirstGeantHitPosition",        &FirstGeantHitPosition,        "FirstGeantHitPosition[NGeantTracks][3]/D");
  GasTPCTree->Branch("LastGeantHitPosition",         &LastGeantHitPosition,         "LastGeantHitPosition[NGeantTracks][3]/D");
  GasTPCTree->Branch("VertexPosition",               &VertexPosition,               "VertexPosition[3]/D");
  GasTPCTree->Branch("Momentum",                     &Momentum,                     "Momentum[NGeantTracks]/D");
  GasTPCTree->Branch("ReconMomentum",                &ReconMomentum,                "ReconMomentum[NGeantTracks]/D");
  GasTPCTree->Branch("ReconCharge",                  &ReconCharge,                  "ReconCharge[NGeantTracks]/I");
  GasTPCTree->Branch("Pt",                           &Pt,                           "Pt[NGeantTracks]/D");
  GasTPCTree->Branch("Costheta",                     &Costheta,                     "Costheta[NGeantTracks]/D");
  GasTPCTree->Branch("ReconCostheta",                &ReconCostheta,                "ReconCostheta[NGeantTracks]/D");
  GasTPCTree->Branch("TrackLength",                  &TrackLength,                  "TrackLength[NGeantTracks]/D");
  GasTPCTree->Branch("Pdg",                          &Pdg,                          "Pdg[NGeantTracks]/I");
  GasTPCTree->Branch("ParentPdg",                    &ParentPdg,                    "ParentPdg[NGeantTracks]/I");
  GasTPCTree->Branch("TotalEDep",                    &TotalEDep,                    "TotalEDep[NGeantTracks]/D");
  GasTPCTree->Branch("TrackID",                      &TrackID,                      "TrackID[NGeantTracks]/I");
  GasTPCTree->Branch("ParentID",                     &ParentID,                     "ParentID[NGeantTracks]/I");
  GasTPCTree->Branch("dEdx",                         &dEdx,                         "dEdx[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxRecon",                    &dEdxRecon,                    "dEdxRecon[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxTruncated",                &dEdxTruncated,                "dEdxTruncated[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxReconTruncated",           &dEdxReconTruncated,           "dEdxReconTruncated[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxSigma",                    &dEdxSigma,                    "dEdxSigma[NGeantTracks]/D");

  GasTPCTree->Branch("EcalNGeantHits",               &EcalNGeantHits,               "EcalNGeantHits[NGeantTracks]/I");  
  GasTPCTree->Branch("EcalFirstGeantHitPosition",    &EcalFirstGeantHitPosition,    "EcalFirstGeantHitPosition[NGeantTracks][3]/D");
  GasTPCTree->Branch("EcalLastGeantHitPosition",     &EcalLastGeantHitPosition,     "EcalLastGeantHitPosition[NGeantTracks][3]/D");
  GasTPCTree->Branch("EcalMomentum",                 &EcalMomentum,                 "EcalMomentum[NGeantTracks]/D");
  GasTPCTree->Branch("EcalTrackLength",              &EcalTrackLength,              "EcalTrackLength[NGeantTracks]/D");
  GasTPCTree->Branch("EcalTotalEDep",                &EcalTotalEDep,                "EcalTotalEDep[NGeantTracks]/D");
  GasTPCTree->Branch("WhichEcal",                    &WhichEcal,                    "WhichEcal[NGeantTracks]/I");
  GasTPCTree->Branch("EcalFirstLayer",               &EcalFirstLayer,               "EcalFirstLayer[NGeantTracks]/I");
  GasTPCTree->Branch("EcalLastLayer",                &EcalLastLayer,                "EcalLastLayer[NGeantTracks]/I");
  GasTPCTree->Branch("EcalShowerWidth",              &EcalShowerWidth,              "EcalShowerWidth[NGeantTracks][3]/D");
  GasTPCTree->Branch("EcalShowerEDepSlices",         &EcalShowerEDepSlices,         "EcalShowerEDepSlices[NGeantTracks][5]/D");

  GasTPCTree->Branch("NGamma",                       &NGamma,                       "NGamma/I");
  GasTPCTree->Branch("GammaPdg",                     &GammaPdg,                     "GammaPdg[NGamma]/I");
  GasTPCTree->Branch("GammaParentPdg",               &GammaParentPdg,               "GammaParentPdg[NGamma]/I"); 
  GasTPCTree->Branch("GammaParentID",                &GammaParentID,                "GammaParentID[NGamma]/I"); 
  GasTPCTree->Branch("GammaWhichEcal",               &GammaWhichEcal,               "GammaWhichEcal[NGamma]/I");
  GasTPCTree->Branch("GammaNHits",                   &GammaNHits,                   "GammaNHits[NGamma]/I");  
  GasTPCTree->Branch("GammaMomentum",                &GammaMomentum,                "GammaMomentum[NGamma]/D");
  GasTPCTree->Branch("GammaEDep",                    &GammaEDep,                    "GammaEDep[NGamma]/D");
  GasTPCTree->Branch("GammaFirstGeantHitPosition",   &GammaFirstGeantHitPosition,   "GammaFirstGeantHitPosition[NGamma][3]/D");
  
  GasTPCTree->Branch("NNeutron",                     &NNeutron,                     "NNeutron/I");
  GasTPCTree->Branch("NeutronNHits",                 &NeutronNHits,                 "NeutronNHits[NNeutron]/I");
  GasTPCTree->Branch("NeutronWhichEcal",             &NeutronWhichEcal,             "NeutronWhichEcal[NNeutron]/I");
  GasTPCTree->Branch("NeutronMomentum",              &NeutronMomentum,              "NeutronMomentum[NNeutron]/D");
  GasTPCTree->Branch("NeutronEDep",                  &NeutronEDep,                  "NeutronEDep[NNeutron]/D");
  GasTPCTree->Branch("NeutronFirstGeantHitPosition", &NeutronFirstGeantHitPosition, "NeutronFirstGeantHitPosition[NNeutron][3]/D");
  
  GasTPCTree->Branch("NISParticles",                 &NISParticles,                 "NISParticles/I");
  GasTPCTree->Branch("ISPdg",                        &ISPdg,                        "ISPdg[NISParticles]/I");
  GasTPCTree->Branch("NFSParticles",                 &NFSParticles,                 "NFSParticles/I");
  GasTPCTree->Branch("FSPdg",                        &FSPdg,                        "FSPdg[NFSParticles]/I");
  GasTPCTree->Branch("FSMomentum",                   &FSMomentum,                   "FSMomentum[NFSParticles]/D");
  GasTPCTree->Branch("FSCostheta",                   &FSCostheta,                   "FSCostheta[NFSParticles]/D");

  GasTPCTree->Branch("NCEcalTracks",                 &NCEcalTracks,                 "NCEcalTracks/I");
  GasTPCTree->Branch("NCEcalNGeantHits",             &NCEcalNGeantHits,             "NCEcalNGeantHits[NCEcalTracks]/I");  
  GasTPCTree->Branch("NCEcalFirstGeantHitPosition",  &NCEcalFirstGeantHitPosition,  "NCEcalFirstGeantHitPosition[NCEcalTracks][3]/D");
  GasTPCTree->Branch("NCEcalLastGeantHitPosition",   &NCEcalLastGeantHitPosition,   "NCEcalLastGeantHitPosition[NCEcalTracks][3]/D");
  GasTPCTree->Branch("NCEcalMomentum",               &NCEcalMomentum,               "NCEcalMomentum[NCEcalTracks]/D");
  GasTPCTree->Branch("NCEcalTrackLength",            &NCEcalTrackLength,            "NCEcalTrackLength[NCEcalTracks]/D");
  GasTPCTree->Branch("NCEcalTotalEDep",              &NCEcalTotalEDep,              "NCEcalTotalEDep[NCEcalTracks]/D");
  GasTPCTree->Branch("NCWhichEcal",                  &NCWhichEcal,                  "NCWhichEcal[NCEcalTracks]/I");
  GasTPCTree->Branch("NCEcalUnusedHits",             &NCEcalUnusedHits,             "NCEcalUnusedHits/I");
  
  // Event displays
  const int n = nEntries;
  TH2D *HitPositionXY[n]; TH2D *HitPositionXZ[n]; TH2D *HitPositionYZ[n];
  TString histname;
  Int_t bin  = 700; Int_t xmin  = -3500; Int_t xmax  = 3500;
  for(Int_t j = 0; j < n; j++){
    histname.Form("histoxy_%i",j);
    HitPositionXY[j] = new TH2D(histname.Data(),histname.Data(),bin,xmin,xmax,bin,xmin,xmax);
    histname.Form("histoxz_%i",j);
    HitPositionXZ[j] = new TH2D(histname.Data(),histname.Data(),bin,xmin,xmax,bin,xmin,xmax);
    histname.Form("histoyz_%i",j);
    HitPositionYZ[j] = new TH2D(histname.Data(),histname.Data(),bin,xmin,xmax,bin,xmin,xmax);
    if(!savehisto){
      delete HitPositionXY[j];
      delete HitPositionXZ[j];
      delete HitPositionYZ[j];
    }
  }
  
  GasTPCTree->SetDirectory(0);

  //std::vector<ParticleDescrShortRecord> fss;
  //std::vector<ParticleDescrShortRecord> iss;

  //std::vector<GeantDaughterParticle> *allparticles;
  //std::vector<GeantPrimaryParticle>  *prims;  

  //NeutrinoEvent *nuEvent_ = tpcplotter->getNuEvent();
  //GeantTrackingTruth* trackingtruth = tpcplotter->getTrackingTruth();

  TRandom3 * rand = new TRandom3();

  // Event loop
  for(Int_t i=0; i<n; i++){
    nuEventTree->GetEntry(i);
    detectorHitsTree->GetEntry(i);

    NeutrinoEvent *nuEvent_           = tpcplotter->getNuEvent();
    GeantTrackingTruth* trackingtruth = tpcplotter->getTrackingTruth();

    // Initialize
    NCEcalUnusedHits = 0; NCEcalTracks = 0; NGeantTracks = 0; NFSParticles = 0; NISParticles = 0; NNeutron = 0; NGamma = 0;   
    
    // Topology
    ParticleDescrShortRecord lepton           = nuEvent_->getFspl();
    ParticleDescrShortRecord hitnucleon       = nuEvent_->getHitNucleon();
    std::vector<ParticleDescrShortRecord> fss = nuEvent_->getFssVector();
    std::vector<ParticleDescrShortRecord> iss = nuEvent_->getIssVector();

    // Argon interaction
    bool argon = false;
    Target = hitnucleon.getPDG();
    if(Target == 1000180400)
      argon = true;
    
    if(target == "argon"  && !argon) continue;
    if(target == "vessel" && argon) continue;

    TLorentzVector lepmom = lepton.getP4();

    Int_t inter = -1; Int_t inter_fs = -1;
    if(argon){
      NISParticles = iss.size() + 1; // + 1 to count for the lepton
      NFSParticles = fss.size() + 1; // + 1 to count for the lepton
      // Set the lepton
      ISPdg[0] = lepton.getPDG();
      FSPdg[0] = lepton.getPDG();
      FSMomentum[0] = lepmom.Vect().Mag();
      FSCostheta[0] = lepmom.CosTheta();
      // Primary state topologies
      Int_t npic  = 0, npi0 = 0, nother = 0, count = 0;
      for(std::vector< ParticleDescrShortRecord >::iterator k = iss.begin();k != iss.end();++k){
	ParticleDescrShortRecord issparticle = *k;
	count++; 
	ISPdg[count] = issparticle.getPDG();
	if(issparticle.getPDG() == 211 || issparticle.getPDG() == -211)
	  npic++;
	else if(issparticle.getPDG() == 111)
	  npi0++;
	else if(issparticle.getPDG() == 311 || issparticle.getPDG() == 3222 ||  issparticle.getPDG() == 3112 || issparticle.getPDG() == 3122 || issparticle.getPDG() == 130 || issparticle.getPDG() == -311 || issparticle.getPDG() == -3222 ||  issparticle.getPDG() == -3112 || issparticle.getPDG() == -3122 || issparticle.getPDG() == -130)
	  nother++;
      }     
      
      if(nuEvent_->getScatteringType()==1){
	if(npic==0 && npi0 == 0 && nother==0)
	  inter = 1;
	else if(npic == 1 && npi0 == 0 && nother==0)
	  inter = 2;
	else if(npic == 0 && npi0 == 1 && nother==0)
	  inter = 3;
	else if(npic == 1 && npi0 == 1 && nother==0)
	  inter = 4;
	else
	  inter = 5;
      }
      else{
	if(npic == 0 && npi0 == 1 && nother==0)
	  inter = 10;
	else
	  inter = 11;
      }

      // Final state topologies
      count = 0;
      Int_t npic_fs  = 0, npi0_fs = 0, nother_fs = 0;  
      for(std::vector< ParticleDescrShortRecord >::iterator k = fss.begin();k != fss.end();++k){
	ParticleDescrShortRecord fssparticle = *k;
	count++;
	TLorentzVector fssmom = fssparticle.getP4();
	FSPdg[count]      = fssparticle.getPDG();
	FSMomentum[count] = fssmom.Vect().Mag();
	FSCostheta[count] = fssmom.CosTheta();

	if(fssparticle.getPDG() == 211 || fssparticle.getPDG() == -211)
	  npic_fs++;
	else if(fssparticle.getPDG() == 111)
	  npi0_fs++;
	else if(fssparticle.getPDG() == 311 || fssparticle.getPDG() == 3222 || fssparticle.getPDG() == 3112 || fssparticle.getPDG() == 3122 || fssparticle.getPDG() == 130 || fssparticle.getPDG() == -311 || fssparticle.getPDG() == -3222 ||  fssparticle.getPDG() == -3112 || fssparticle.getPDG() == -3122 || fssparticle.getPDG() == -130)
	  nother_fs++;
      }
      
      
      if(nuEvent_->getScatteringType()==1){
	if(npic_fs ==0 && npi0_fs == 0 && nother_fs == 0)
	  inter_fs = 1;
	else if(npic_fs == 1 && npi0_fs == 0 && nother_fs == 0)
	  inter_fs = 2;
	else if(npic_fs == 0 && npi0_fs == 1 && nother_fs == 0)
	  inter_fs = 3;
	else if(npic_fs == 1 && npi0_fs == 1 && nother_fs == 0)
	  inter_fs = 4;
	else
	  inter_fs = 5;
      }
      else{
	if(npic_fs == 0 && npi0_fs == 1 && nother_fs == 0)
	  inter_fs = 10;
	else
	  inter_fs = 11;
      }
    }
    else{
      NISParticles = 0;
      NFSParticles = 0;
      inter_fs = 0;
      inter = 0;
    }

    // Fill the true variables
    EventID           = nuEvent_->getEventID();
    RunID             = nuEvent_->getRunID();
    ISTopology        = inter;
    FSTopology        = inter_fs;
    Enu               = nuEvent_->getNuEnergy();
    neutcode          = nuEvent_->getNEUTCode();
    neutPdg           = nuEvent_->getInteractionType();
    x                 = nuEvent_->getx();
    y                 = nuEvent_->gety();
    Q2                = nuEvent_->getQ2();
    W                 = nuEvent_->getW();
    weight            = nuEvent_->getWeight();
    VertexPosition[0] = nuEvent_->getPosition().X();
    VertexPosition[1] = nuEvent_->getPosition().Y();
    VertexPosition[2] = nuEvent_->getPosition().Z();

    // Finished with these
    std::vector<ParticleDescrShortRecord>().swap(fss);
    fss.resize(0);
    std::vector<ParticleDescrShortRecord>().swap(iss);
    iss.resize(0);
    // --------------------------------------------------------------------------------------
    // Information about all geant particles
    std::vector<GeantPrimaryParticle>  *prims        = trackingtruth->getPrimariesPtr();
    std::vector<GeantDaughterParticle> *allparticles = trackingtruth->getAllParticlesPtr();
    // TPC Hits
    HitCollection tpcSdHits = tpcplotter->getSimData()->getTpcFidHits();
    // Ecal hits
    ScintHitCollection scintSdHits = tpcplotter->getSimData()->getScintHits();

    //cout << i << " , Beginning No of TPC Hits = " << tpcSdHits.size() << " , Number of Ecal Hits = " << scintSdHits.size() << endl;
    // To speed up things a little bit
    if(scintSdHits.size() > 25000)
      scintSdHits.clear();
    // --------------------------------------------------------------------------------------
    // TPC hits
    // --------------------------------------------------------------------------------------
    // Take the unique track id for each tpc track
    std::set<int> myset; // set takes only unique elements
    for(Int_t k=0; k<tpcSdHits.size(); k++){
      SDHit tmpHit = tpcSdHits.at(k);
      if(tmpHit.getPDG() > 99999){continue;}
      myset.insert(tmpHit.getTrackID());
      if(myset.size() > kMaxTrack)
	break;
    }

    // Get the number of TPC tracks - this also includes tracks with just one geant hit
    NGeantTracks     = myset.size();

    // Loop start
    Int_t counter = 0;
    
    for(std::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it){
      int value = *it;

      // Initialize
      Int_t npdg = 0; Int_t nhits = 0; Int_t idparent = 0;  Int_t pdgparent = -1; Int_t  wecal = -1; Int_t charge = 0;
      Double_t edep = 0.0;
      TLorentzVector tmom;
      TVector3 firstpos, lastpos;
      TVector2 xzfirstpos, xzlastpos;
      
      for(Int_t j=0; j<tpcSdHits.size(); j++){
	SDHit tmpHit = tpcSdHits.at(j);
      
	// Only those the track id matched
	if(value == tmpHit.getTrackID()){
	  npdg = tmpHit.getPDG();
	  nhits++;
	  edep += tmpHit.getEdep()*1000;
	  idparent = tmpHit.getParentID();
	  pdgparent = tmpHit.getParent().getPDG();
	  charge = tmpHit.getCharge();

	  if(nhits == 1){
	    firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    xzfirstpos.Set(tmpHit.getPosition().X(),tmpHit.getPosition().Z());
	    tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	  }
	  else{
	    lastpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    xzlastpos.Set(tmpHit.getPosition().X(),tmpHit.getPosition().Z());
	  }
	  
	  if(savehisto){
	    HitPositionXY[i]->Fill(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getEdep()*1000);
	    HitPositionXZ[i]->Fill(tmpHit.getPosition().X(),tmpHit.getPosition().Z(),tmpHit.getEdep()*1000);
	    HitPositionYZ[i]->Fill(tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getEdep()*1000);
	  }
	  
	  //tpcSdHits.erase(tpcSdHits.begin() + j);
	  //j--;
	}
      }

      // Position resolution
      FirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),tpcresolution);
      FirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),tpcresolution);
      FirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),tpcresolution);
      LastGeantHitPosition[counter][0]  = lastpos.X();
      LastGeantHitPosition[counter][1]  = lastpos.Y();
      LastGeantHitPosition[counter][2]  = lastpos.Z();
      
      NGeantHits[counter]               = nhits;
      TotalEDep[counter]                = edep;
      Pdg[counter]                      = npdg;
      ParentPdg[counter]                = pdgparent;
      if(lastpos.Mag() > 0)
	TrackLength[counter]            = (lastpos-firstpos).Mag();
      else
	TrackLength[counter]            = 0.0;
      Momentum[counter]                 = tmom.Vect().Mag();
      Pt[counter]                       = tmom.Perp();
      Costheta[counter]                 = tmom.CosTheta();
      TrackID[counter]                  = value;     
      ParentID[counter]                 = idparent;
      // dE/dx
      if(nhits > 1 && (xzlastpos-xzfirstpos).Mod() > 0){
	dEdx[counter]                   = edep/(xzlastpos-xzfirstpos).Mod()/10;
	dEdxSigma[counter]              = dEdx[counter]*5.4*pow(20*(xzlastpos-xzfirstpos).Mod()/1000,-0.37)/100;
	dEdxRecon[counter]              = rand->Gaus(dEdx[counter],dEdxSigma[counter]);
	double ang                      = rand->Gaus(acos(tmom.CosTheta()),angresolution);
	ReconCostheta[counter]          = cos(ang);
      }
      else{
	dEdx[counter]                   = 0.0;
	dEdxSigma[counter]              = 0.0;
	dEdxRecon[counter]              = 0.0;
	ReconCostheta[counter]          = 0.0;
      }
      // dE/dx truncated
      Double_t edepperhit   = edep/nhits;
      Double_t edepmax      = 2.5*edepperhit;
      Double_t lengthperhit = (xzlastpos-xzfirstpos).Mod()/nhits;
      
      Int_t losthits =0; Double_t edeptrun = 0.0;
      for(Int_t j=0; j<tpcSdHits.size(); j++){
	SDHit tmpHit = tpcSdHits.at(j);
	// Only those the track id matched
	if(value == tmpHit.getTrackID()){
	  if(tmpHit.getEdep()*1000 > edepmax)
	    losthits++;
	  else
	    edeptrun += tmpHit.getEdep()*1000; 
	}
      }
      
      Double_t nlength = (xzlastpos-xzfirstpos).Mod() - losthits*lengthperhit;
      if(nhits > 1 && (xzlastpos-xzfirstpos).Mod() > 0){
	dEdxTruncated[counter]                   = edeptrun/nlength/10;
	dEdxReconTruncated[counter]              = rand->Gaus(dEdxTruncated[counter],dEdxSigma[counter]);
      }
      else{
	dEdxTruncated[counter]                   = 0.0;
	dEdxReconTruncated[counter]              = 0.0;
      }

      // Smeared momentum
      if(nhits > 1 && TrackLength[counter] > 0){
	// Only transverse coordiantes to mag field
	double truthtransmom   = sqrt(tmom.Y()*tmom.Y() + tmom.Z()*tmom.Z());
	double tracklength     = sqrt( (lastpos.Y()-firstpos.Y())*(lastpos.Y()-firstpos.Y()) + (lastpos.Z()-firstpos.Z())*(lastpos.Z()-firstpos.Z()) );
	double getsagita       = magfield*tracklength*tracklength/(26.7*truthtransmom);
	double smearsagita     = rand->Gaus(getsagita,deltaSagitta);
	if(smearsagita < 0)
	  charge = -charge;
	if(smearsagita < deltaSagitta)
	  smearsagita = deltaSagitta;
	
	double transvmom = magfield*tracklength*tracklength/(26.7*smearsagita);
	// Assume a similar resolution for the px component
	double pxres = abs(1-transvmom/truthtransmom);
	double pxsmear = rand->Gaus(tmom.X(),pxres*tmom.X());
	ReconMomentum[counter] = sqrt(transvmom*transvmom + pxsmear*pxsmear);
	ReconCharge[counter]   = charge; 
      }
      else{
	ReconMomentum[counter] = 0.0;
	ReconCharge[counter]   = 0;
      }
  
      // Does the track goes or started in the ecal?
      // Reset
      nhits = 0; edep = 0.0; tmom.SetXYZT(0.0,0.0,0.0,0.0); firstpos.SetXYZ(0.0,0.0,0.0); lastpos.SetXYZ(0.0,0.0,0.0);
      Int_t firstlayr = -1, lastlayr = -1;
      Double_t slices[5] = {0.0,0.0,0.0,0.0,0.0};
      TVector3 firstshower, lastshower;
      std::set<int> ecalparentset;
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	// Only those the track id matched
	if(value == tmpHit.getTrackID()){
	  nhits++;
	  edep += tmpHit.getEdep()*1000;
	  int temp = 0;
	  
	  if(nhits == 1){
	    firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	    wecal = tmpHit.getEcalNumber();
	    firstlayr = tmpHit.getLayerNumber();
	    firstshower.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    lastshower.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	  }
	  else{
	    lastpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    // Ecal pid variables
	    if(tmpHit.getPosition().X() > firstshower.X())
	      firstshower.SetX(tmpHit.getPosition().X());
	    else if(tmpHit.getPosition().X() < lastshower.X())
	      lastshower.SetX(tmpHit.getPosition().X());
	    
	    if(tmpHit.getPosition().Y() > firstshower.Y())
	      firstshower.SetY(tmpHit.getPosition().Y());
	    else if(tmpHit.getPosition().Y() < lastshower.Y())
	      lastshower.SetY(tmpHit.getPosition().Y());

	    if(tmpHit.getPosition().Z() > firstshower.Z())
	      firstshower.SetZ(tmpHit.getPosition().Z());
	    else if(tmpHit.getPosition().Z() < lastshower.Z())
	      lastshower.SetZ(tmpHit.getPosition().Z());
	    
	    if((lastpos-firstpos).Mag() < 10)
	      slices[0] += tmpHit.getEdep()*1000;
	    else if((lastpos-firstpos).Mag() >= 10 && (lastpos-firstpos).Mag() < 20)
	      slices[1] += tmpHit.getEdep()*1000;
	    else if((lastpos-firstpos).Mag() >= 20 && (lastpos-firstpos).Mag() < 30)
	      slices[2] += tmpHit.getEdep()*1000;
	    else if((lastpos-firstpos).Mag() >= 30 && (lastpos-firstpos).Mag() < 40)
	      slices[3] += tmpHit.getEdep()*1000;
	    else
	      slices[4] += tmpHit.getEdep()*1000;


	    // Get layer-ecal
	    if(tmpHit.getLayerNumber() > lastlayr)
	      lastlayr = tmpHit.getLayerNumber();
	    
	    temp = tmpHit.getEcalNumber(); 
	    if(wecal != temp && wecal < 10)
	      wecal = wecal*10 + temp;
	  }
	  
	  // Event display histograms
	  if(savehisto){
	    HitPositionXY[i]->Fill(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getEdep()*1000);
	    HitPositionXZ[i]->Fill(tmpHit.getPosition().X(),tmpHit.getPosition().Z(),tmpHit.getEdep()*1000);
	    HitPositionYZ[i]->Fill(tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getEdep()*1000);
	  }
	  
	  // Remove this hit
	  scintSdHits.erase(scintSdHits.begin() + j);
	  j--;
	}
	if(value == tmpHit.getParentID()){
	  ecalparentset.insert(tmpHit.getTrackID());
	}
      }
      
      // Fill the tree
      EcalFirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),scintresolution);
      EcalFirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),scintresolution);
      EcalFirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),scintresolution);
      EcalLastGeantHitPosition[counter][0]  = lastpos.X();
      EcalLastGeantHitPosition[counter][1]  = lastpos.Y();
      EcalLastGeantHitPosition[counter][2]  = lastpos.Z();

      if(lastpos.Mag() > 0)
	EcalTrackLength[counter]            = (lastpos-firstpos).Mag();
      else
	EcalTrackLength[counter]            = 0.0;
      EcalMomentum[counter]                 = tmom.Vect().Mag();
      WhichEcal[counter]                    = wecal;
      EcalFirstLayer[counter]               = firstlayr;
      
      // Check for showering objects
      for(std::set<int>::iterator ecit=ecalparentset.begin(); ecit!=ecalparentset.end(); ++ecit){
 	int nvalue = *ecit;
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);
	  if(nvalue == tmpHit.getTrackID()){
	    nhits++;
	    edep += tmpHit.getEdep()*1000;

	    // Ecal pid variables
	    if(tmpHit.getPosition().X() > firstshower.X())
	      firstshower.SetX(tmpHit.getPosition().X());
	    else if(tmpHit.getPosition().X() < lastshower.X())
	      lastshower.SetX(tmpHit.getPosition().X());
	    
	    if(tmpHit.getPosition().Y() > firstshower.Y())
	      firstshower.SetY(tmpHit.getPosition().Y());
	    else if(tmpHit.getPosition().Y() < lastshower.Y())
	      lastshower.SetY(tmpHit.getPosition().Y());

	    if(tmpHit.getPosition().Z() > firstshower.Z())
	      firstshower.SetZ(tmpHit.getPosition().Z());
	    else if(tmpHit.getPosition().Z() < lastshower.Z())
	      lastshower.SetZ(tmpHit.getPosition().Z());

	    lastpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    if((lastpos-firstpos).Mag() < 10)
	      slices[0] += tmpHit.getEdep()*1000;
	    else if((lastpos-firstpos).Mag() >= 10 && (lastpos-firstpos).Mag() < 20)
	      slices[1] += tmpHit.getEdep()*1000;
	    else if((lastpos-firstpos).Mag() >= 20 && (lastpos-firstpos).Mag() < 30)
	      slices[2] += tmpHit.getEdep()*1000;
	    else if((lastpos-firstpos).Mag() >= 30 && (lastpos-firstpos).Mag() < 40)
	      slices[3] += tmpHit.getEdep()*1000;
	    else
	      slices[4] += tmpHit.getEdep()*1000;

	    // Get layer-ecal
	    if(tmpHit.getLayerNumber() > lastlayr)
	      lastlayr = tmpHit.getLayerNumber();
	    
	    // Event display histograms
	    if(savehisto){
	      HitPositionXY[i]->Fill(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getEdep()*1000);
	      HitPositionXZ[i]->Fill(tmpHit.getPosition().X(),tmpHit.getPosition().Z(),tmpHit.getEdep()*1000);
	      HitPositionYZ[i]->Fill(tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getEdep()*1000);
	    }
	    
	    // Remove this hit
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	  if(nvalue == tmpHit.getParentID()){
	    ecalparentset.insert(tmpHit.getTrackID());
	  }
	}
      }

      EcalNGeantHits[counter]               = nhits;
      EcalTotalEDep[counter]                = edep;
      EcalLastLayer[counter]                = lastlayr;

      EcalShowerWidth[counter][0]           = rand->Gaus(firstshower.X(),scintresolution)-rand->Gaus(lastshower.X(),scintresolution);
      EcalShowerWidth[counter][1]           = rand->Gaus(firstshower.Y(),scintresolution)-rand->Gaus(lastshower.Y(),scintresolution);
      EcalShowerWidth[counter][2]           = rand->Gaus(firstshower.Z(),scintresolution)-rand->Gaus(lastshower.Z(),scintresolution);

      for(Int_t k=0; k<5; k++)
	EcalShowerEDepSlices[counter][k]    = slices[k];

      ecalparentset.clear();
      counter++;
    }

    //cout << i << " , After tpc tracking No of TPC Hits = " << tpcSdHits.size() << " , Number of Ecal Hits = " << scintSdHits.size() << endl;
    
    // --------------------------------------------------------------------------------------   
    // Gammas
    // --------------------------------------------------------------------------------------
    std::set<int> neutralset;
    
    for(int j =0;j<prims->size();j++){
      GeantPrimaryParticle * prim = &prims->at(j);
      if(prim->getPDG()==111){ // pi0
	Int_t pi0id = prim->getTrackID();
	for(std::vector< GeantDaughterParticle >::iterator k = allparticles->begin();k != allparticles->end();++k){
	  GeantDaughterParticle geantdaughter = *k;
	  if( (geantdaughter.getPDG() == 22 || abs(geantdaughter.getPDG()) == 11) && pi0id == geantdaughter.getParentID()){
	    neutralset.insert(geantdaughter.getTrackID());
	    if(neutralset.size() > kMaxTrack)
	      break;
	  }
	}
      }
      else if(prim->getPDG()==22 || abs(prim->getPDG()) == 11){// primary photon
	neutralset.insert(prim->getTrackID());
	if(neutralset.size() > kMaxTrack)
	  break;
      }
    }
 
    NGamma = neutralset.size();
    
    counter = 0;
    for(std::set<int>::iterator iit=neutralset.begin(); iit!=neutralset.end(); ++iit){
      int value = *iit;

      Double_t mom = 0.0; Int_t pid = 0; Int_t ppdg = 0; Int_t gpdg = 0;
      Double_t edep = 0.0; Int_t ecal = -1; Int_t hits = 0;
      TVector3 firstpos;
      std::set<int> gammaset;
       
      for(int j =0;j<prims->size();j++){
	GeantPrimaryParticle *prim = &prims->at(j);
	if(prim->getTrackID() == value){
	  mom  = prim->getP4().Vect().Mag();
	  pid  = prim->getParentID();
	  gpdg = prim->getPDG();
	  ppdg = 0;
	}
      }
      
      if(mom == 0.0){
	for(int j =0;j<allparticles->size();j++){
	  GeantDaughterParticle *geantdaughter = &allparticles->at(j);
	  if(geantdaughter->getTrackID() == value){
	    mom  = geantdaughter->getP4().Vect().Mag();
	    pid  = geantdaughter->getParentID();
	    ppdg = geantdaughter->getParent().getPDG();
	    gpdg = geantdaughter->getPDG();
	  } 
	}
      }

      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	if(tmpHit.getTrackID() == value){
	  hits++;
	  edep += tmpHit.getEdep()*1000;

	  int temp = 0;
	  
	  if(hits == 1){
	    firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    ecal = tmpHit.getEcalNumber();
	  }
	  else{
	    temp = tmpHit.getEcalNumber(); 
	    if(ecal != temp && ecal < 10)
	     ecal = ecal*10 + temp;
	  }

	  scintSdHits.erase(scintSdHits.begin() + j);
	  j--;
	}
	if(tmpHit.getParentID() == value){
	  gammaset.insert(tmpHit.getTrackID());
	}
      }

      for(std::set<int>::iterator t=gammaset.begin(); t!=gammaset.end(); ++t){
	int nvalue = *t;

	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);
	  if(tmpHit.getTrackID() == nvalue){
	    hits++;
	    edep += tmpHit.getEdep()*1000;

	    // Determine in which ecal
	    if(ecal < 0){
	      int temp = 0;
	  
	      if(hits == 1){
		firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
		ecal = tmpHit.getEcalNumber();
	      }
	      else{
		temp = tmpHit.getEcalNumber(); 
		if(ecal != temp && ecal < 10)
		  ecal = ecal*10 + temp;
	      }
	    }
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	  if(tmpHit.getParentID() == nvalue){
	    gammaset.insert(tmpHit.getTrackID());
	  }
	}
      }

      // Found other gamma hits associated
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	// same ecal as the main clsuster
	if(ecal == tmpHit.getEcalNumber() && tmpHit.getParent().getTrackID() == pid){
	  //TVector3 hitpos(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	  //if( (hitpos-firstpos).Mag() < 700){ // not miles away from the main cluster
	  hits++;
	  edep += tmpHit.getEdep()*1000;
	  scintSdHits.erase(scintSdHits.begin() + j);
	  j--;
	  //}
	}
      }

      // Still nothing found? Group all the hits from this pi0
      if(ecal < 0){
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);

	  if(tmpHit.getParent().getTrackID() == pid){
	    hits++;
	    edep += tmpHit.getEdep()*1000;
	    
	    // Determine in which ecal
	    int temp = 0;
	    
	    if(hits == 1){
	      firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	      ecal = tmpHit.getEcalNumber();
	    }
	    else{
	      temp = tmpHit.getEcalNumber(); 
	      if(ecal != temp && ecal < 10)
		ecal = ecal*10 + temp;
	    }
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	}
      }

      gammaset.clear();

      GammaMomentum[counter]  = mom;
      GammaParentID[counter]  = pid;
      GammaPdg[counter]       = gpdg;
      GammaParentPdg[counter] = ppdg;
      GammaNHits[counter]     = hits;
      GammaEDep[counter]      = edep;
      GammaWhichEcal[counter] = ecal;
      GammaFirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),scintresolution);
      GammaFirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),scintresolution);
      GammaFirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),scintresolution);
      counter++;
    }

    //cout << i << " , After gamma No of TPC Hits = " << tpcSdHits.size() << " , Number of Ecal Hits = " << scintSdHits.size() << endl;
    
    // --------------------------------------------------------------------------------------
    // Neutrons
    // --------------------------------------------------------------------------------------
    // Clear sets
    neutralset.clear();
    counter = 0;
    // Check for neutrons
    for(int j =0;j<prims->size();j++){
      GeantPrimaryParticle * prim = &prims->at(j);
      if(prim->getPDG()==2112){
	neutralset.insert(prim->getTrackID());
	if(neutralset.size() > kMaxTrack)
	  break;
      }
    }

    NNeutron = neutralset.size();
    
    for(std::set<int>::iterator iit=neutralset.begin(); iit!=neutralset.end(); ++iit){
      int value = *iit;

      Int_t ecal = -1, hits = 0, pid = 0;
      Double_t mom = 0.0, edep = 0.0;
      TVector3 firstpos;
      std::set<int> neutronset;
      
      for(std::vector<GeantPrimaryParticle>::iterator t=prims->begin(); t!=prims->end(); ++t){
	GeantPrimaryParticle prim = *t;
	if(prim.getTrackID() == value){
	  mom  = prim.getP4().Vect().Mag();
	  pid  = prim.getTrackID();
	}
      }
      
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	if(tmpHit.getTrackID() == value){
	
	  hits++;
	  edep += tmpHit.getEdep()*1000;
	  
	  int temp = 0;
	  
	  if(hits == 1){
	    firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    ecal = tmpHit.getEcalNumber();
	  }
	  else{
	    temp = tmpHit.getEcalNumber(); 
	    if(ecal != temp && ecal < 10)
	      ecal = ecal*10 + temp;
	  }

	  scintSdHits.erase(scintSdHits.begin() + j);
	  j--;
	}
	else if(tmpHit.getParentID() == value){
	  neutronset.insert(tmpHit.getTrackID());
	}
      }
      
      for(std::set<int>::iterator t=neutronset.begin(); t!=neutronset.end(); ++t){
	int nvalue = *t;
	
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);
	  if(tmpHit.getTrackID() == nvalue){
	    hits++;
	    edep += tmpHit.getEdep()*1000;

	    // Determine in which ecal
	    if(ecal < 0){
	      int temp = 0;
	  
	      if(hits == 1){
		firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
		ecal = tmpHit.getEcalNumber();
	      }
	      else{
		temp = tmpHit.getEcalNumber(); 
		if(ecal != temp && ecal < 10)
		  ecal = ecal*10 + temp;
	      }
	    }

	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	  else if(tmpHit.getParentID() == nvalue){
	    neutronset.insert(tmpHit.getTrackID());
	  }
	}
      }

      // other neutron hits associated
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	// same ecal as the main clsuster
	if(ecal == tmpHit.getEcalNumber() && tmpHit.getParent().getTrackID() == pid){
	  //TVector3 hitpos(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	  //if( (hitpos-firstpos).Mag() < 700){ // not miles away from the main cluster
	    hits++;
	    edep += tmpHit.getEdep()*1000;
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	    //}
	}
      }   

      // Still nothing found? Group all the hits from this pi0
      if(ecal < 0){
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);

	  if(tmpHit.getParent().getTrackID() == pid){
	    hits++;
	    edep += tmpHit.getEdep()*1000;
	    
	    // Determine in which ecal
	    if(ecal < 0){
	      int temp = 0;
	  
	      if(hits == 1){
		firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
		ecal = tmpHit.getEcalNumber();
	      }
	      else{
		temp = tmpHit.getEcalNumber(); 
		if(ecal != temp && ecal < 10)
		  ecal = ecal*10 + temp;
	      }
	    }
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	}
      }
      
      NeutronMomentum[counter]  = mom;
      NeutronNHits[counter]     = hits;
      NeutronEDep[counter]      = edep;
      NeutronWhichEcal[counter] = ecal;
      NeutronFirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),scintresolution);
      NeutronFirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),scintresolution);
      NeutronFirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),scintresolution);
      
      neutronset.clear();
      counter++;
    }
    
    //cout << i << ", After neutron No of TPC Hits = " << tpcSdHits.size() << " , Number of Ecal Hits = " << scintSdHits.size() << endl;
    
    // --------------------------------------------------------------------------------------
    // Other ecal neutral clusters
    // --------------------------------------------------------------------------------------
    // Take the unique track id for each ecal primary track
    neutralset.clear();
    for(Int_t k=0; k<scintSdHits.size(); k++){
      scintHit tmpHit = scintSdHits.at(k);
      if(tmpHit.getPDG() > 99999 || tmpHit.getParentID() > 0 ){continue;}
      neutralset.insert(tmpHit.getTrackID());
      if(neutralset.size() > kMaxTrack)
	break;
    }

    for(Int_t k=0; k<scintSdHits.size(); k++){
      scintHit tmpHit = scintSdHits.at(k);
      Int_t pid = tmpHit.getParent().getPDG();
      if(abs(pid) == 311 || abs(pid) == 3222 ||  abs(pid) == 3112 || abs(pid) == 3122 || abs(pid) == 130){
	if(tmpHit.getPDG() != 22 && abs(tmpHit.getPDG()) != 11){
	  neutralset.insert(tmpHit.getTrackID());
	  if(neutralset.size() > kMaxTrack)
	    break;
	}
      }
    }

    NCEcalTracks = neutralset.size();
    
    counter = 0;
    for(std::set<int>::iterator iit=neutralset.begin(); iit!=neutralset.end(); ++iit){
      int value = *iit;

      // Initialize
      Int_t npdg = 0; Int_t nhits = 0; Int_t idparent = 0;  Int_t pdgparent = -1; Int_t  wecal = -1; 
      Double_t edep = 0.0; 
      TLorentzVector tmom;
      TVector3 firstpos, lastpos; 
      
      std::set<int> ecalparent;
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	// Only those the track id matched
	if(value == tmpHit.getTrackID()){
	  npdg = tmpHit.getPDG();
	  
	  nhits++;
	  edep += tmpHit.getEdep()*1000;
	  idparent = tmpHit.getParentID();
	  pdgparent = tmpHit.getParent().getPDG();
	  
	  if(nhits == 1){
	    firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	    wecal = tmpHit.getEcalNumber();
	  }
	  else{
	    lastpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	  }
	  
	  scintSdHits.erase(scintSdHits.begin() + j);
	  j--;
	  
	}
	else if(value == tmpHit.getParentID()){
	  ecalparent.insert(tmpHit.getTrackID());
	}
      }     
      
      for(std::set<int>::iterator ecit=ecalparent.begin(); ecit!=ecalparent.end(); ++ecit){
 	int nvalue = *ecit;
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);
	  if(nvalue == tmpHit.getTrackID()){
	    nhits++;
	    edep += tmpHit.getEdep()*1000;
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	  else if(nvalue == tmpHit.getParentID()){
	    ecalparent.insert(tmpHit.getTrackID());
	  }
	}
      }

      
      //for(Int_t j=0; j<scintSdHits.size(); j++){
      //scintHit tmpHit = scintSdHits.at(j);
      //if(value == tmpHit.getParent().getTrackID()){
	  //cout << tmpHit.getPDG() << " , " << tmpHit.getP4().Vect().Mag() << " , " << tmpHit.getTrackID() << " , " << tmpHit.getParentID() << " , " << tmpHit.getParent().getTrackID() << " , " << tmpHit.getParent().getPDG() << endl;
      //  scintSdHits.erase(scintSdHits.begin() + j);
      //  j--;
      //}
      //}
      
      
      NCEcalFirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),scintresolution);
      NCEcalFirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),scintresolution);
      NCEcalFirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),scintresolution);
      NCEcalLastGeantHitPosition[counter][0]  = lastpos.X();
      NCEcalLastGeantHitPosition[counter][1]  = lastpos.Y();
      NCEcalLastGeantHitPosition[counter][2]  = lastpos.Z();

      NCEcalNGeantHits[counter]               = nhits;
      NCEcalTotalEDep[counter]                = edep;
      if(lastpos.Mag() > 0)
	NCEcalTrackLength[counter]            = (lastpos-firstpos).Mag();
      NCEcalMomentum[counter]                 = tmom.Vect().Mag();
      NCWhichEcal[counter]                    = wecal;
 
      ecalparent.clear();
      counter++;
    }
    
    NCEcalUnusedHits = scintSdHits.size();

    //cout << i << " , No of TPC Hits = " << tpcSdHits.size() << " , Number of Ecal Hits = " << scintSdHits.size() << endl;

    // Clear the vectors
    scintSdHits.clear();
    tpcSdHits.clear();
    neutralset.clear();
  
    tpcplotter->getSimData()->clearSDHits();
    
    std::set<int>().swap(neutralset);
    neutralset.empty();
    std::set<int>().swap(myset);
    myset.empty();
    
    std::vector<GeantDaughterParticle>().swap(*allparticles);
    allparticles->resize(0);
    std::vector<GeantPrimaryParticle>().swap(*prims);
    prims->resize(0);

    //delete nuEvent_;
    //delete trackingtruth;
    
    // Fill the output tree
    GasTPCTree->Fill(); 
  } // end of event loop
  // --------------------------------------------------------------------------------------
  delete rand;
  tpcplotter->deleteTrees();
  FileMC.Close();
  
  // OutputFile
  TFile *f = new TFile(outputfile.c_str(),"RECREATE");
  GasTPCTree->Write();
  
  if(savehisto){
    for(Int_t j = 0; j < n; j++){
      TString dhsn = HitPositionXY[j]->GetName(); dhsn += "_2Dpar_can";
      TString dhst = HitPositionXY[j]->GetTitle(); dhst += "_2Dpar_can";
      TCanvas *c1 = new TCanvas(dhsn.Data(),dhst.Data());
      c1->Divide(1,3);
      c1->cd(1);
      HitPositionXY[j]->Draw("colz");
      HitPositionXY[j]->GetXaxis()->SetTitle("X [mm]");
      HitPositionXY[j]->GetYaxis()->SetTitle("Y [mm]");
      c1->cd(2);
      HitPositionXZ[j]->Draw("colz");
      HitPositionXZ[j]->GetXaxis()->SetTitle("X [mm]");
      HitPositionXZ[j]->GetYaxis()->SetTitle("Z [mm]");
      c1->cd(3);
      HitPositionYZ[j]->Draw("colz");
      HitPositionYZ[j]->GetXaxis()->SetTitle("Y [mm]");
      HitPositionYZ[j]->GetYaxis()->SetTitle("Z [mm]");
      c1->Write();
    }
  }
  
  return 0;
}
