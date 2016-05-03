//____________________________________________________________________________
/*!

\class    GasTPCAnalysis: This is in a development mode at the moment

\brief    Program to group the hits and do basic recon

\author	  Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Jun 2015
\last update Nov 2015
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
#include <TMath.h>

#include "GasTPCPlotter.hh"
#include "TXMLEngine.h"
#include "Messenger.hh"

#include "GasTPCAnalysis.hh"

#include "ReconUtils.hh"

using namespace std;
using namespace util;
using namespace ReconUtils;

int main(int argc, char ** argv) {

  std::string datacard   = "";
  std::string inputfile  = "";
  std::string outputfile = "";
  std::string target     = "";
  int nentries = -1;
  bool savehisto = false;
  bool doecal = true;
  bool pg = false;
  bool fhc = true;

  for(int iArg = 1; iArg < argc; iArg++) {
    if((!strcasecmp(argv[iArg],"-d"))||(!strcasecmp(argv[iArg],"-datacard"))) {
      datacard = argv[++iArg] ;
    }
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
    if(!strcasecmp(argv[iArg],"-e")) {
     doecal = false;
    }
    if((!strcasecmp(argv[iArg],"-n"))) {
      nentries = atoi(argv[++iArg]);
      std::cout << "INFO::Generating " << nentries << " events." << std::endl;
    }
    if(!strcasecmp(argv[iArg],"-p")) {
      pg = true;
    }
    if(!strcasecmp(argv[iArg],"-rhc")) {
      fhc = false;
    }
  }

  TFile FileMC(inputfile.c_str(),"READ");
  if(FileMC.GetNkeys()<1){
    std::cout << "ERROR::Bad input file: " << inputfile << std::endl;
    return 1;
  }

  // Select anti-neutrino configuration if running on rhc
  if(inputfile.find("antineutrino") != std::string::npos)
    fhc = false;

  // Load the xml file
  const char* gastpc_dir = getenv("GasTPC");
  if(datacard == "")
    datacard = "GeantSimulation.xml";
  std::string fdatacard = string(gastpc_dir) + "/src/config/" + datacard;

  ReadDatacard(fdatacard);
  
  // Setup the output tree
  TTree *GasTPCTree = new TTree("GasTPCTree","GasTPCTree");

  ReadOutputTree(GasTPCTree);

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
   
  // Event histo-displays
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

  // Number of spills simulated
  const double pot_per_spill = 7.5e13;
  int nspills = (int)(fPOTSimulated/pot_per_spill);

  std::cout << "INFO::Number of spills simulated: " << nspills << std::endl;
  
  GasTPCTree->SetDirectory(0);

  // Random generator
  TRandom3 * rand = new TRandom3();

  // Event loop
  for(Int_t i=0; i<n; i++){
    if(i == nentries) break;
    nuEventTree->GetEntry(i);
    detectorHitsTree->GetEntry(i);
    
    NeutrinoEvent *nuEvent_           = tpcplotter->getNuEvent();
    GeantTrackingTruth* trackingtruth = tpcplotter->getTrackingTruth();
 
    if(!pg){
      genie::NtpMCEventRecord * grec    = tpcplotter->getGenieMCEventRecord();
      genie::EventRecord *eventr = grec->event;
      fAnalNtpMCEventRecord->Fill(i, eventr);
    }

    // Initialize
    NCEcalUnusedHits = 0; NCEcalTracks = 0; NGeantTracks = 0; NFSParticles = 0; NISParticles = 0; NNeutron = 0; NGamma = 0;   
    
    // Topology
    std::vector<ParticleDescrShortRecord> fss = nuEvent_->getFssVector();
    std::vector<ParticleDescrShortRecord> iss = nuEvent_->getIssVector();

    // Argon interaction
    bool argon = false;
    Target = nuEvent_->getNucleonPdg();
    if(Target == 1000180400)
      argon = true;

    TVector3 vpos(nuEvent_->getPosition().X(), nuEvent_->getPosition().Y(), nuEvent_->getPosition().Z());
 
    if(target == "argon"  && !argon) continue;
    if(target == "vessel" && argon) continue;

    // In FV
    bool VertexinVessel = false;
    if(ReconUtils::inVesselFV(vpos, 2500, 2500)){
      VertexinVessel = true;
    }
      
    //cout << "----- Event --------- " << i << endl;
    Int_t inter_fs = -1;
    Int_t npic  = 0, npi0 = 0;
    Double_t Eneutral = 0, plepton = 0;
    //if(argon){
    if(VertexinVessel && Target != 1000130270){
      NISParticles = iss.size();
      NFSParticles = fss.size();
      // Set the lepton
      // Primary state topologies
      Int_t nother = 0, count = 0;
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

      // Final state topologies
      count = 0;
      Int_t npic_fs  = 0, npi0_fs = 0, nother_fs = 0, nprot = 0;  
      for(std::vector< ParticleDescrShortRecord >::iterator k = fss.begin();k != fss.end();++k){
	ParticleDescrShortRecord fssparticle = *k;
	count++;
	TLorentzVector fssmom = fssparticle.getP4();
	FSPdg[count]      = fssparticle.getPDG();
	FSMomentum[count] = fssmom.Vect().Mag();
	FSCostheta[count] = fssmom.CosTheta();

	if(fssparticle.getPDG() == 211 || fssparticle.getPDG() == -211){
	  npic_fs++;
	}
  else if (fssparticle.getPDG() == 2212) {
    nprot++;
  }
	else if(fssparticle.getPDG() == 111){
	  npi0_fs++;
	  Eneutral += fssmom.E();
	}
	else if(fssparticle.getPDG() == 311 || fssparticle.getPDG() == 3222 || fssparticle.getPDG() == 3112 || fssparticle.getPDG() == 3122 || fssparticle.getPDG() == 130 || fssparticle.getPDG() == -311 || fssparticle.getPDG() == -3222 ||  fssparticle.getPDG() == -3112 || fssparticle.getPDG() == -3122 || fssparticle.getPDG() == -130)
	  nother_fs++;
      
	else if(fssparticle.getPDG() == 2112)
	  Eneutral += fssmom.E();
	else if( TMath::Abs(fssparticle.getPDG()) == 13)
	  plepton = TMath::Sqrt(fssmom.X()*fssmom.X() + fssmom.Y()*fssmom.Y() + fssmom.Z()*fssmom.Z());
	else if( TMath::Abs(fssparticle.getPDG()) == 11)
	  plepton = TMath::Sqrt(fssmom.X()*fssmom.X() + fssmom.Y()*fssmom.Y() + fssmom.Z()*fssmom.Z());
      }

      if(nuEvent_->getScatteringType()==1){
	if(npic_fs ==0 && npi0_fs == 0 && nother_fs == 0 && nprot == 0)
	  inter_fs = 1;
  else if (npic_fs ==0 && npi0_fs == 0 && nother_fs == 0 && nprot == 1)
    inter_fs = 2;
	else if(npic_fs == 1 && npi0_fs == 0 && nother_fs == 0)
	  inter_fs = 3;
	else if(npic_fs == 0 && npi0_fs == 1 && nother_fs == 0)
	  inter_fs = 5;
	else if(npic_fs == 1 && npi0_fs == 1 && nother_fs == 0)
	  inter_fs = 6;
	else
	  inter_fs = 7;
      }
      else{
	if(npic_fs == 0 && npi0_fs == 1 && nother_fs == 0)
	  inter_fs = 10;
	else if(npic_fs == 1 && npi0_fs == 0 && nother_fs == 0)
	  inter_fs = 11;
	else
	  inter_fs = 12;
      }
    }
    else{
      NISParticles = 0;
      NFSParticles = 0;
      inter_fs = 0;
    }

    // Fill the true variables
    bool isCC = false;
    if(nuEvent_->getScatteringType()==1)
      isCC = true;

    bool isNC = false;
    if(nuEvent_->getScatteringType()==0)
      isNC = true;

    bool isQEL = false;
    if(abs(nuEvent_->getNEUTCode()) == 1)
      isQEL = true;

    bool isMEC = false;
    if(abs(nuEvent_->getNEUTCode()) == 2)
      isMEC = true;

    bool isCoh = false;
    if(abs(nuEvent_->getNEUTCode()) == 16 || abs(nuEvent_->getNEUTCode()) == 36)
      isCoh = true;
   
    EventID           = nuEvent_->getEventID();
    RunID             = nuEvent_->getRunID();
    SpillNumber       = nuEvent_->GetSpillNumber();
    FSTopology        = inter_fs;
    Ev_All            = nuEvent_->getNuEnergy();
    neutcode          = nuEvent_->getNEUTCode();
    neutPdg           = nuEvent_->getNeutrinoType();
    x                 = nuEvent_->getx();
    y_All             = nuEvent_->gety();
    Q2                = nuEvent_->getQ2();
    W                 = nuEvent_->getW();
    POTWeight         = 1.0;//nuEvent_->getWeight();
    VertexPosition[0] = nuEvent_->getPosition().X();
    VertexPosition[1] = nuEvent_->getPosition().Y();
    VertexPosition[2] = nuEvent_->getPosition().Z();
    VertexPosition[3] = nuEvent_->getPosition().T();

    // Sample based on truth
    // Ev_reco and y_reco Temporary for the first pass. This will be done properly in the next release 
    if(VertexinVessel && Target != 1000130270){
      sample = ReconUtils::getSampleTruth(neutPdg,FSTopology,fhc);
      reactionmode      = ReconUtils::getInteractionMode(nuEvent_->getNeutrinoType(), npic, npi0, isCC, isNC, isQEL, isMEC, isCoh);
      Ev                = nuEvent_->getNuEnergy();
      y                 = nuEvent_->gety();
      Ev_reco = Ev - Eneutral;
      Ev_reco = ReconUtils::Smearer(Ev_reco, 0.1*Ev_reco, EventID+RunID+SpillNumber);
      if(Ev_reco < 0.0)
	Ev_reco = 0;
      
      if(Ev_reco < 100.0)
	sample = -1;
      
      if(Ev_reco > 0 && sample < 10 && sample >= 0 && plepton > 0)
	y_reco = 1.0 - plepton/Ev_reco;
      else
	y_reco = 1.0;
    }
    else{
      sample = -1;
      Ev     = -10.0;
      y_reco = -10.0;
      reactionmode      = -1;
      y = -10.0;
    }

    // Finished with these
    std::vector<ParticleDescrShortRecord>().swap(fss);
    fss.resize(0);
    std::vector<ParticleDescrShortRecord>().swap(iss);
    iss.resize(0);
    // --------------------------------------------------------------------------------------
    // Information about all geant particles
    std::vector<GeantParticle>  prims        = trackingtruth->getPrimariesPtr();
    std::vector<GeantParticle> allparticles = trackingtruth->getAllParticlesPtr();
    // TPC Hits
    HitCollection tpcSdHits = tpcplotter->getSimData()->getTpcFidHits();
    // Ecal hits
    ScintHitCollection scintSdHits = tpcplotter->getSimData()->getScintHits();
    // --------------------------------------------------------------------------------------
    // TPC hits
    // --------------------------------------------------------------------------------------
    // Take the unique track id for each tpc track
    std::set<int> myset; // set takes only unique elements
    for(Int_t k=0; k<tpcSdHits.size(); k++){
      SDHit tmpHit = tpcSdHits.at(k);
      //if(tmpHit.getPDG() > 99999){continue;}
      myset.insert(tmpHit.getTrackID());
      if(myset.size() > kMaxTrack)
	break;
    }

    // Get the number of TPC tracks - this also includes tracks with just one geant hit
    NGeantTracks     = myset.size();

    // Loop start
    Int_t counter = 0;
    Int_t inecal = 0;
    
    for(std::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it){
      int value = *it;

      // Initialize
      Int_t rseed = 9999, npdg = 0; Int_t nhits = 0; Int_t idparent = 0;  Int_t pdgparent = -1; Int_t  wecal = -1; Int_t  lecal = -1; Int_t charge = 0; Int_t primpdg = 0; Int_t primid = -1; Int_t exitdetector = -1;
      Double_t edep = 0.0, length = 0.0, ftimepos = 0.0, ltimepos = 0.0;
      TLorentzVector tmom;
      TVector3 firstpos, lastpos;
      TVector2 xzfirstpos, xzlastpos;

      for(Int_t j=0; j<tpcSdHits.size(); j++){
	SDHit tmpHit = tpcSdHits.at(j);

	// Only those the track id matched
	if(value == tmpHit.getTrackID()){
	  nhits++;
	  npdg         = tmpHit.getPDG();
	  edep         += tmpHit.getEdep()*1000;
	  idparent     = tmpHit.getParentID();
	  charge       = tmpHit.getCharge();
	  exitdetector = tmpHit.getTrackLeftVolume();
	  rseed        = (int)tmpHit.getP4().E() + j + i + rseed;

	  if(nhits == 1){
	    firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    xzfirstpos.Set(tmpHit.getPosition().X(),tmpHit.getPosition().Z());
	    tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	    ftimepos = tmpHit.getPosition().T();
	    // Find parent
	    for(int k=0;k<allparticles.size();k++){
	      GeantParticle geantdaughter = allparticles.at(k);
	      if(idparent == geantdaughter.getTrackID()){
		pdgparent  = geantdaughter.getPDG();
		break;
	      }
	    }
	  }
	  else{	    
	    TVector3 temppos(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    if(lastpos.X() == 0){
	      Double_t hitdist = (temppos-firstpos).Mag();
	      length += hitdist;
	    }
	    else{
	      Double_t hitdist = (lastpos-temppos).Mag();
	      length += hitdist;
	    }
	    
	    lastpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    xzlastpos.Set(tmpHit.getPosition().X(),tmpHit.getPosition().Z());
	    ltimepos = tmpHit.getPosition().T();
	  }
	  
	  if(savehisto){
	    HitPositionXY[i]->Fill(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getEdep());
	    HitPositionXZ[i]->Fill(tmpHit.getPosition().X(),tmpHit.getPosition().Z(),tmpHit.getEdep());
	    HitPositionYZ[i]->Fill(tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getEdep());
	  }
	  
	  //tpcSdHits.erase(tpcSdHits.begin() + j);
	  //j--;
	}
      }

      if(rseed == 999)
	rseed = rseed+ value + i;

      // Position truth and recon
      FirstGeantHitPosition[counter][0] = firstpos.X();
      FirstGeantHitPosition[counter][1] = firstpos.Y();
      FirstGeantHitPosition[counter][2] = firstpos.Z();
      FirstGeantHitPosition[counter][3] = ftimepos;
      TVector3 freconpos =  ReconUtils::Smearer(firstpos, tpcresolution, rseed);
 
      if(ReconUtils::inVesselFV(freconpos, fiducialZ, fiducialXY))
	ReconInFV[counter] = 1;
      else
	ReconInFV[counter] = 0;

      ReconPosition[counter][0] = freconpos.X();
      ReconPosition[counter][1] = freconpos.Y();
      ReconPosition[counter][2] = freconpos.Z();
      ReconPosition[counter][3] = ftimepos;

      LastGeantHitPosition[counter][0]  = lastpos.X();
      LastGeantHitPosition[counter][1]  = lastpos.Y();
      LastGeantHitPosition[counter][2]  = lastpos.Z();
      LastGeantHitPosition[counter][3]  = ltimepos;
      
      TVector3 lreconpos =  ReconUtils::Smearer(lastpos, tpcresolution, rseed*rseed);
      if(lastpos.Mag() > 0){
	ReconLastPosition[counter][0]  = lreconpos.X();
	ReconLastPosition[counter][1]  = lreconpos.Y();
	ReconLastPosition[counter][2]  = lreconpos.Z();
	ReconLastPosition[counter][3]  = ltimepos;

	TrackLength[counter]           = (lastpos-firstpos).Mag();
	ReconTrackLength[counter]      = (lreconpos-freconpos).Mag();

	// dedx
	//double fdedx                   = edep/(xzlastpos-xzfirstpos).Mod();
	double fdedx = edep/length/10.0; // Mev/cm
	double dedxres = ReconUtils::DEDXResolution(gaspressure,length/1000.0); // tracklength has to be in meters
	// dedx resolution is the %
	dedxres = dedxres*fdedx/100.0;
	double dedxrecon = ReconUtils::Smearer(fdedx,dedxres,rseed);
	
	dEdx[counter]                   = fdedx;
	dEdxSigma[counter]              = dedxres;
	RecondEdx[counter]              = dedxrecon;
	if(RecondEdx[counter] < 0)
	  RecondEdx[counter] = 0.0;

	double dedxExpMuon = dedxCorMIP*ReconUtils::DEDXExp(tmom.Vect().Mag(), 105.66);
	double dedxExpPion = dedxCorMIP*ReconUtils::DEDXExp(tmom.Vect().Mag(), 139.57);
	double dedxExpElec = dedxCorElec*ReconUtils::DEDXExp(tmom.Vect().Mag(), 0.511);
	double dedxExpKaon = dedxCorMIP*ReconUtils::DEDXExp(tmom.Vect().Mag(), 493.67);
	double dedxExpProt = dedxCorMIP*ReconUtils::DEDXExp(tmom.Vect().Mag(), 938.27);

	// Additional correction based on the truth dEdx
	if(abs(npdg) == 13){
	  double diff = fdedx - dedxExpMuon;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}
	else if(abs(npdg) == 211){
	  double diff = fdedx - dedxExpPion;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}
	else if(abs(npdg) == 11){
	  double diff = fdedx - dedxExpElec;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}
	else if(abs(npdg) == 321){
	  double diff = fdedx - dedxExpKaon;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}
	else if(abs(npdg) == 2212){
	  double diff = fdedx - dedxExpProt;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}

	dEdxMuon[counter]               = dedxExpMuon;
	dEdxPion[counter]               = dedxExpPion;
	dEdxElec[counter]               = dedxExpElec;
	dEdxKaon[counter]               = dedxExpKaon;
	dEdxProt[counter]               = dedxExpProt;

	pullmuon[counter]               = (dedxrecon-dedxExpMuon)/dedxres;
	pullpion[counter]               = (dedxrecon-dedxExpPion)/dedxres;
	pullelec[counter]               = (dedxrecon-dedxExpElec)/dedxres;
	pullkaon[counter]               = (dedxrecon-dedxExpKaon)/dedxres;
	pullprot[counter]               = (dedxrecon-dedxExpProt)/dedxres;

	// Angle smear
	double ang = ReconUtils::Smearer(acos(tmom.CosTheta()),angresolution,rseed);
	ReconCostheta[counter]          = cos(ang);

	// Momentum smear
	double truthtransmom   = TMath::Sqrt(tmom.Y()*tmom.Y() + tmom.Z()*tmom.Z());
	double tracklength     = TMath::Sqrt( (lastpos.Y()-firstpos.Y())*(lastpos.Y()-firstpos.Y()) + (lastpos.Z()-firstpos.Z())*(lastpos.Z()-firstpos.Z()) );
	double getsagita       = ReconUtils::getTrackSagitta(magfield,tracklength,truthtransmom);
	double smearsagita     = ReconUtils::Smearer(getsagita,deltaSagitta,rseed);
	int reconcharge        = ReconUtils::getReconCharge(smearsagita,charge);
	double transvmom       = ReconUtils::getReconTransMomentum(magfield,tracklength,smearsagita,deltaSagitta); 

	double pxsmear         = ReconUtils::getReconMomentumX(transvmom,truthtransmom,tmom.X(),9999*value);

	ReconMomentum[counter] = TMath::Sqrt(transvmom*transvmom + pxsmear*pxsmear);
	ReconCharge[counter]   = reconcharge;

	double pz = ReconMomentum[counter]*ReconCostheta[counter];
	double py = ReconMomentum[counter]*ReconMomentum[counter] - pz*pz - pxsmear*pxsmear;
	if(py > 0)
	  py = TMath::Sqrt(py);
	else
	  py =0;
	
	ReconMomentumXYZ[counter][0] = pxsmear;
	ReconMomentumXYZ[counter][1] = py;
	ReconMomentumXYZ[counter][2] = pz;

	if(ReconMomentum[counter] <= 0){ // consider this as efficiency loss
	  ReconMomentum[counter] = 0.0;
	  ReconCharge[counter]   = 0;
	  ReconMomentumXYZ[counter][0] = 0.0;
	  ReconMomentumXYZ[counter][1] = 0.0;
	  ReconMomentumXYZ[counter][2] = 0.0;
	}
      }
      else{
	ReconLastPosition[counter][0]  = 0.0;
	ReconLastPosition[counter][1]  = 0.0;
	ReconLastPosition[counter][2]  = 0.0;

	TrackLength[counter]            = 0.0;
	ReconTrackLength[counter]       = 0.0;

	dEdx[counter]                   = 0.0;
	dEdxSigma[counter]              = 0.0;
	dEdxMuon[counter]               = 0.0;
	dEdxPion[counter]               = 0.0;
	dEdxElec[counter]               = 0.0;
	dEdxKaon[counter]               = 0.0;
	dEdxProt[counter]               = 0.0;
	RecondEdx[counter]              = 0.0;
	ReconCostheta[counter]          = 0.0;

	ReconMomentum[counter]          = 0.0;
	ReconCharge[counter]            = 0;
	ReconMomentumXYZ[counter][0]    = 0.0;
	ReconMomentumXYZ[counter][1]    = 0.0;
	ReconMomentumXYZ[counter][2]    = 0.0;
      }
  
      LeftVolume[counter]               = exitdetector;
      NGeantHits[counter]               = nhits;
      TotalEDep[counter]                = edep;
      Pdg[counter]                      = npdg;
      ParentPdg[counter]                = pdgparent;
      HitLength[counter]                = length;
      Momentum[counter]                 = tmom.Vect().Mag();
      Pt[counter]                       = tmom.Perp();
      Costheta[counter]                 = tmom.CosTheta();
      TrackID[counter]                  = value;     
      ParentID[counter]                 = idparent;
      PrimaryPdg[counter]               = primpdg;
      Charge[counter]                   = charge;
      int nreconhits                    = (int)length/10; // 1 hit / cm
      IsTPCRecon[counter]               = (int)ReconUtils::IsReconTrack(nreconhits,rseed);

      // Does the track go in the ecal?
      // Reset
      nhits = 0; edep = 0.0; tmom.SetXYZT(0.0,0.0,0.0,0.0); firstpos.SetXYZ(0.0,0.0,0.0); lastpos.SetXYZ(0.0,0.0,0.0), ftimepos = 0.0, ltimepos = 0.0;
      Int_t firstlayr = -1, lastlayr = -1, necal = 0;
      Double_t slices[5] = {0.0,0.0,0.0,0.0,0.0};
      TVector3 firstshower, lastshower;
      std::set<int> ecalparentset;
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	// Only those the track id matched
	if(value == tmpHit.getTrackID()){
	  nhits++;
	  edep += tmpHit.getEdep()*1000;
	  if(nhits == 1){
	    necal++;
	    firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),tmpHit.getP4().T());
	    wecal = tmpHit.getEcalNumber();
	    firstlayr = tmpHit.getLayerNumber();
	    firstshower.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    lastshower.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    ftimepos = tmpHit.getPosition().T();
	  }
	  else{
	    lastpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    if(tmpHit.getPosition().T() > ltimepos)
	      ltimepos = tmpHit.getPosition().T();
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

	    int temp = tmpHit.getEcalNumber();
	    if(wecal !=  temp && necal < 3){     
	      wecal = wecal*10 + temp;
	      necal++;
	    }
	    
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
	/*
	if(value == tmpHit.getParent().getTrackID() || primid == tmpHit.getParent().getTrackID()){
	  Int_t pid = tmpHit.getParent().getPDG();
	  if(abs(pid) == 11 || abs(pid) == 13 || abs(pid) == 211 || abs(pid) == 321 || abs(pid) == 2212 || abs(pid) == 22 || abs(pid) == 2112)
	    ecalparentset.insert(tmpHit.getTrackID());
	}
	*/
      }
	
      // Check for showering objects
      for(std::set<int>::iterator ecit=ecalparentset.begin(); ecit!=ecalparentset.end(); ++ecit){
 	int nvalue = *ecit;
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);
	  if(nvalue == tmpHit.getTrackID()){
	    nhits++;
	    edep += tmpHit.getEdep()*1000;
	    if(nhits == 1){
	      necal++;
	      firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	      ftimepos = tmpHit.getPosition().T();
	      tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	      wecal = tmpHit.getEcalNumber();
	      firstlayr = tmpHit.getLayerNumber();
	      firstshower.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	      lastshower.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    }
	    else{
	      lastpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	      if(tmpHit.getPosition().T() > ltimepos)
		ltimepos = tmpHit.getPosition().T();
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

	      int temp = tmpHit.getEcalNumber();
	      if(wecal != temp && necal < 3){
		wecal = wecal*10 + temp;
		necal++;
	      }
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
	  if(nvalue == tmpHit.getParentID()){
	    ecalparentset.insert(tmpHit.getTrackID());
	  }
	  /*
	  if(value == tmpHit.getParent().getTrackID() || primid == tmpHit.getParent().getTrackID()){
	    Int_t pid = tmpHit.getParent().getPDG();
	    if(abs(pid) == 11 || abs(pid) == 13 || abs(pid) == 211 || abs(pid) == 321 || abs(pid) == 2212 || abs(pid) == 22 || abs(pid) == 2112)
	      ecalparentset.insert(tmpHit.getTrackID());
	  }
	  */
	}
      }

      // Fill the tree
      EcalFirstGeantHitPosition[counter][0] = firstpos.X();
      EcalFirstGeantHitPosition[counter][1] = firstpos.Y();
      EcalFirstGeantHitPosition[counter][2] = firstpos.Z();
      EcalFirstGeantHitPosition[counter][3] = ftimepos;

      EcalLastGeantHitPosition[counter][0]  = lastpos.X();
      EcalLastGeantHitPosition[counter][1]  = lastpos.Y();
      EcalLastGeantHitPosition[counter][2]  = lastpos.Z();
      EcalLastGeantHitPosition[counter][3]  = ltimepos;

      if(ftimepos !=0){
	TVector3 ecalreconpos =  ReconUtils::Smearer(firstpos, scintresolution, rseed);
	double ecalrecontime  =  ReconUtils::Smearer(ftimepos, scintTimeRes, rseed);
	EcalReconHitPosition[counter][0] = ecalreconpos.X();
	EcalReconHitPosition[counter][1] = ecalreconpos.Y();
	EcalReconHitPosition[counter][2] = ecalreconpos.Z();
	EcalReconHitPosition[counter][3] = ecalrecontime;
      }
      else{
	EcalReconHitPosition[counter][0] = 0.0;
	EcalReconHitPosition[counter][1] = 0.0;
	EcalReconHitPosition[counter][2] = 0.0;
	EcalReconHitPosition[counter][3] = 0.0;
      }

      if(lastpos.Mag() > 0){
	EcalTrackLength[counter][0]         = fabs(lastpos.X()-firstpos.X());
	EcalTrackLength[counter][1]         = fabs(lastpos.Y()-firstpos.Y());
	EcalTrackLength[counter][2]         = fabs(lastpos.Z()-firstpos.Z());
      }
      else{
	EcalTrackLength[counter][0]         = 0.0;
	EcalTrackLength[counter][1]         = 0.0;
	EcalTrackLength[counter][2]         = 0.0;
      }
      EcalMomentum[counter]                 = tmom.Vect().Mag();
      EcalFirstLayer[counter]               = firstlayr;

      EcalNGeantHits[counter]               = nhits;
      EcalTotalEDep[counter]                = edep;
      EcalLastLayer[counter]                = lastlayr;
      //if(lecal > 0)
      //wecal = wecal*100 + lecal;
      WhichEcal[counter]                    = wecal;
      NEcals[counter]                       = necal; 

      EcalShowerWidth[counter][0]           = rand->Gaus(firstshower.X(),scintresolution)-rand->Gaus(lastshower.X(),scintresolution);
      EcalShowerWidth[counter][1]           = rand->Gaus(firstshower.Y(),scintresolution)-rand->Gaus(lastshower.Y(),scintresolution);
      EcalShowerWidth[counter][2]           = rand->Gaus(firstshower.Z(),scintresolution)-rand->Gaus(lastshower.Z(),scintresolution);

      for(Int_t k=0; k<5; k++)
	EcalShowerEDepSlices[counter][k]    = slices[k];

      if(nhits > 3)
	inecal++;

      ecalparentset.clear();
      counter++;
    }

    NTPCTracksInEcal = inecal;

    // Temporary
    if(NGeantTracks == 0 || NTPCTracksInEcal == 0){
      sample = -1;
      Ev     = -10.0;
      y_reco = -10.0;
      reactionmode      = -1;
      y = -10.0;
    }
    // --------------------------------------------------------------------------------------
    //cout << i << " , After tpc tracking No of TPC Hits = " << tpcSdHits.size() << " , Number of Ecal Hits = " << scintSdHits.size() << endl;
    // BELOW IS REALLY WORK IN PROGRESS
    // --------------------------------------------------------------------------------------   
    // Gammas
    // --------------------------------------------------------------------------------------
    // short lived products
    /*
    std::set<int> slivedset;
    for(int j =0;j<prims->size();j++){
      GeantPrimaryParticle * prim = &prims->at(j);
      Int_t pid = prim->getPDG();
      if(abs(pid) == 311 || abs(pid) == 3222 ||  abs(pid) == 3112 || abs(pid) == 3122 || abs(pid) == 130)
	slivedset.insert(prim->getTrackID());
    }

    // secondary pi0s
    std::set<int> secpi0set;
    for(std::vector< GeantDaughterParticle >::iterator k = allparticles->begin();k != allparticles->end();++k){
      GeantDaughterParticle geantdaughter = *k;
      if(geantdaughter.getPDG() == 111) 
	secpi0set.insert(geantdaughter.getParentID());
    }
    */
    std::set<int> neutralset;

    //for(std::vector< GeantDaughterParticle >::iterator k = allparticles->begin();k != allparticles->end();++k){
    //GeantDaughterParticle geantdaughter = *k;
    //if(geantdaughter.getPDG() == 111)
	//cout << "Secondary Pi0: geantdaughter trackid: " <<  geantdaughter.getTrackID() << " , geantdaughter pid = " << geantdaughter.getParentID() << endl;
      //else if(abs(geantdaughter.getPDG()) == 211 || abs(geantdaughter.getPDG()) == 2112 ||  abs(geantdaughter.getPDG()) == 2212)
      //cout << "Secondary Other Pdg:" << geantdaughter.getPDG() << " geantdaughter trackid: " <<  geantdaughter.getTrackID() << " , geantdaughter pid = " << geantdaughter.getParentID() << endl;
      //for(std::set<int>::iterator ecit=slivedset.begin(); ecit!=slivedset.end(); ++ecit){
 	//int nvalue = *ecit;
	//if(geantdaughter.getParentID() == nvalue)
	//cout << "Short lived daughter trackid: " <<  geantdaughter.getTrackID() << " , geantdaughter pid = " << geantdaughter.getParentID() << " , pdg = " << geantdaughter.getPDG() << endl;
	//}
    //}
    
    for(int j =0;j<prims.size();j++){
      //GeantParticle * prim = &prims->at(j);
      GeantParticle prim = prims.at(j);
      //cout << "Primary pdg = " << prim->getPDG() << " , track id = " << prim->getTrackID() << endl;
      // primary photon-electron
      if(prim.getPDG()==22 || abs(prim.getPDG()) == 11){// primary photon-electron
	neutralset.insert(prim.getTrackID());
	if(neutralset.size() > kMaxTrack)
	  break;
      }
      // pi0
      if(prim.getPDG()!=111) {continue;}
      Int_t pi0id = prim.getTrackID();
      //for(std::vector< GeantParticle >::iterator k = allparticles.begin();k != allparticles.end();++k){
      for(int k=0;k<allparticles.size();k++){
	//GeantParticle geantdaughter = *k;
	GeantParticle geantdaughter = allparticles.at(k);
	if(/* (geantdaughter.getPDG() == 22 || abs(geantdaughter.getPDG()) == 11) && */pi0id == geantdaughter.getParentID()){
	  //cout << "Pi0: geantdaughter pdg: " <<  geantdaughter.getPDG() << " , geantdaughter pid = " << geantdaughter.getParentID() << " , pi0id = " << pi0id << endl;
	  neutralset.insert(geantdaughter.getTrackID());
	  if(neutralset.size() > kMaxTrack)
	    break;
	}
      }
    }
    // Find the first parent id
    int firstpid = 100000000;
    for(Int_t j=0; j<scintSdHits.size(); j++){
      scintHit tmpHit = scintSdHits.at(j);
      //int pi0id = 0;
      
      if(/*tmpHit.getParent().getPDG() == 111 && */tmpHit.getParentID() < firstpid){ //&& tmpHit.getParent().getTrackID() != pi0id){
	firstpid  = tmpHit.getParentID();
	//pi0id = tmpHit.getParent().getTrackID();
      }	
      
    }

    for(Int_t j=0; j<scintSdHits.size(); j++){
      scintHit tmpHit = scintSdHits.at(j);
      if(tmpHit.getParentID() == firstpid)
	neutralset.insert(tmpHit.getTrackID());
    }

    NGamma = neutralset.size();
 
    counter = 0;
    for(std::set<int>::iterator iit=neutralset.begin(); iit!=neutralset.end(); ++iit){
      int value = *iit;

      Double_t mom = 0.0; Int_t pid = 0; Int_t ppdg = 0; Int_t gpdg = 0; Int_t primid = -1;
      Double_t edep = 0.0; Int_t wecal = -1; Int_t lecal = -1; Int_t hits = 0;
      TLorentzVector firstpos;
      std::set<int> gammaset;
       
      for(int j =0;j<prims.size();j++){
	//GeantParticle *prim = &prims->at(j);
	GeantParticle prim = prims.at(j);
	if(prim.getTrackID() == value){
	  mom  = prim.getP4().Vect().Mag();
	  pid  = prim.getParentID();
	  gpdg = prim.getPDG();
	  ppdg = 0;
	}
      }
      
      if(mom == 0.0){
	for(int j =0;j<allparticles.size();j++){
	  //GeantParticle *geantdaughter = &allparticles->at(j);
	  GeantParticle geantdaughter = allparticles.at(j);
	  if(geantdaughter.getTrackID() == value){
	    mom  = geantdaughter.getP4().Vect().Mag();
	    pid  = geantdaughter.getParentID();
	    //ppdg = geantdaughter->getParent().getPDG();
	    gpdg = geantdaughter.getPDG();
	  } 
	}
      }

      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	if(tmpHit.getTrackID() == value){
	  hits++;
	  edep += tmpHit.getEdep()*1000;
	  primid    = -999;//tmpHit.getParent().getTrackID();
	  //cout << "TPC hit id : " << value << " , tpc parent = " << ppdg << " , pdg = " << tmpHit.getPDG() << " , " << tmpHit.getP4().Vect().Mag() << " , " << tmpHit.getTrackID() << " , " << tmpHit.getParentID() << " , " << tmpHit.getParent().getTrackID() << " , " << tmpHit.getParent().getPDG() << endl;
	  if(hits == 1){
	    firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	    wecal = tmpHit.getEcalNumber();
	  }
	  else{
	    lecal = tmpHit.getEcalNumber(); 
	  }

	  scintSdHits.erase(scintSdHits.begin() + j);
	  j--;
	}
	if(tmpHit.getParentID() == value){
	  gammaset.insert(tmpHit.getTrackID());
	}
	/*
	if(value == tmpHit.getParent().getTrackID() || primid == tmpHit.getParent().getTrackID()){
	  Int_t pid = tmpHit.getParent().getPDG();
	  if(abs(pid) == 11 || abs(pid) == 22)
	    gammaset.insert(tmpHit.getTrackID());
	}
	*/
      }

      for(std::set<int>::iterator t=gammaset.begin(); t!=gammaset.end(); ++t){
	int nvalue = *t;

	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);
	  if(tmpHit.getTrackID() == nvalue){
	    hits++;
	    edep += tmpHit.getEdep()*1000;
	    primid    = -999;//tmpHit.getParent().getTrackID();
	    //cout << "S TPC hit id : " << value << " , tpc parent = " << ppdg << " , pdg = " << tmpHit.getPDG() << " , " << tmpHit.getP4().Vect().Mag() << " , " << tmpHit.getTrackID() << " , " << tmpHit.getParentID() << " , " << tmpHit.getParent().getTrackID() << " , " << tmpHit.getParent().getPDG() << endl;
	    // Determine in which ecal
	    if(wecal < 0){
	      if(hits == 1){
		firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
		wecal = tmpHit.getEcalNumber();
	      }
	      else{
		lecal = tmpHit.getEcalNumber();
	      }
	    }
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	  if(tmpHit.getParentID() == nvalue){
	    gammaset.insert(tmpHit.getTrackID());
	  }
	  /*
	  if(value == tmpHit.getParent().getTrackID() || primid == tmpHit.getParent().getTrackID()){
	    Int_t pid = tmpHit.getParent().getPDG();
	    if(abs(pid) == 11 || abs(pid) == 22)
	      gammaset.insert(tmpHit.getTrackID());
	  }
	  */
	}
      }

      /*
      // Found other gamma hits associated
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	// same ecal as the main clsuster
	//if(wecal == tmpHit.getDetector() && tmpHit.getParent().getTrackID() == pid){
	if(tmpHit.getParent().getTrackID() == pid){
	  //TVector3 hitpos(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	  //if( (hitpos-firstpos).Mag() < 700){ // not miles away from the main cluster
	  hits++;
	  edep += tmpHit.getEdep()*1000;
	  scintSdHits.erase(scintSdHits.begin() + j);
	  j--;
	  //}
	}
      }
      */
      // Still nothing found? Group all the hits from this pi0
      /*
      if(wecal < 0){
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);

	  if(tmpHit.getParent().getTrackID() == pid){
	    hits++;
	    edep += tmpHit.getEdep()*1000;
	    
	    // Determine in which ecal
	    int temp = 0;
	    
	    if(hits == 1){
	      firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	      wecal = tmpHit.getEcalNumber();
	    }
	    else{
	      lecal = tmpHit.getEcalNumber();
	    }
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	}
      }
      */
      gammaset.clear();

      GammaMomentum[counter]  = mom;
      GammaParentID[counter]  = pid;
      GammaPdg[counter]       = gpdg;
      GammaParentPdg[counter] = ppdg;
      GammaNHits[counter]     = hits;
      GammaEDep[counter]      = edep;
      if(lecal > 0)
	wecal = wecal*10 + lecal;
      GammaWhichEcal[counter] = wecal;
      GammaFirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),scintresolution);
      GammaFirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),scintresolution);
      GammaFirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),scintresolution);
      GammaFirstGeantHitPosition[counter][3] = rand->Gaus(firstpos.T());
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
    for(int j =0;j<prims.size();j++){
      //GeantParticle * prim = &prims->at(j);
      GeantParticle prim = prims.at(j);
      if(prim.getPDG()==2112){
	neutralset.insert(prim.getTrackID());
	if(neutralset.size() > kMaxTrack)
	break;
      }
    }

    for(Int_t k=0; k<scintSdHits.size(); k++){
      scintHit tmpHit = scintSdHits.at(k);
      //if(tmpHit.getParent().getPDG() != 2112 ){continue;}
      if(tmpHit.getPDG() != 2112 ){continue;}
      neutralset.insert(tmpHit.getTrackID());
      if(neutralset.size() > kMaxTrack)
      break;
    }

    NNeutron = neutralset.size();
    
    for(std::set<int>::iterator iit=neutralset.begin(); iit!=neutralset.end(); ++iit){
      int value = *iit;

      Int_t wecal = -1, lecal = 1, hits = 0, pid = 0;
      Double_t mom = 0.0, edep = 0.0;
      TLorentzVector firstpos;
      std::set<int> neutronset;
      
      //for(std::vector<GeantParticle>::iterator t=prims.begin(); t!=prims.end(); ++t){
      for(int j =0;j<prims.size();j++){
	//GeantParticle prim = *t;
	GeantParticle prim = prims.at(j);
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
	  
	  if(hits == 1){
	    firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	    wecal = tmpHit.getEcalNumber();
	  }
	  else{
	    lecal = tmpHit.getEcalNumber();
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
	    if(wecal < 0){
	      if(hits == 1){
		firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
		wecal = tmpHit.getEcalNumber();
	      }
	      else{
		lecal = tmpHit.getEcalNumber();
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
      /*
      // other neutron hits associated
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	// same ecal as the main clsuster
	//if(wecal == tmpHit.getDetector() && tmpHit.getParent().getTrackID() == pid){
	if(tmpHit.getParent().getTrackID() == pid){
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
      if(wecal < 0){
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);

	  if(tmpHit.getParent().getTrackID() == pid){
	    hits++;
	    edep += tmpHit.getEdep()*1000;
	    
	    // Determine in which ecal
	    if(wecal < 0){
	      int temp = 0;
	  
	      if(hits == 1){
		firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
		wecal = tmpHit.getEcalNumber();
	      }
	      else{
		lecal = tmpHit.getEcalNumber();
	      }
	    }
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	  }
	}
      }
      */
      NeutronMomentum[counter]  = mom;
      NeutronNHits[counter]     = hits;
      NeutronEDep[counter]      = edep;
      if(lecal > 0)
	wecal = wecal*100 + lecal;
      NeutronWhichEcal[counter] = wecal;
      NeutronFirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),scintresolution);
      NeutronFirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),scintresolution);
      NeutronFirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),scintresolution);
      NeutronFirstGeantHitPosition[counter][3] = rand->Gaus(firstpos.T());
      
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
      //if(tmpHit.getPDG() > 99999 || tmpHit.getParentID() > 0 ){continue;}
      if(tmpHit.getParentID() > 0 ){continue;}
      neutralset.insert(tmpHit.getTrackID());
      if(neutralset.size() > kMaxTrack)
	break;
    }

    for(Int_t k=0; k<scintSdHits.size(); k++){
      scintHit tmpHit = scintSdHits.at(k);
      //Int_t pid = tmpHit.getParent().getPDG();
      //if(abs(pid) == 311 || abs(pid) == 3222 ||  abs(pid) == 3112 || abs(pid) == 3122 || abs(pid) == 130){
	if(tmpHit.getPDG() != 22 && abs(tmpHit.getPDG()) != 11){
	  neutralset.insert(tmpHit.getTrackID());
	  if(neutralset.size() > kMaxTrack)
	    break;
	}
	//}
    }

    std::set<int> fset;
    for(Int_t k=0; k<scintSdHits.size(); k++){
      scintHit tmpHit = scintSdHits.at(k);
      //fset.insert(tmpHit.getParent().getTrackID());
      fset.insert(tmpHit.getTrackID());
    }
    /*
    //NCEcalTracks = neutralset.size();
    
    counter = 0;
    for(std::set<int>::iterator iit=neutralset.begin(); iit!=neutralset.end(); ++iit){
      int value = *iit;
      //cout << "NC Track id = " << value << endl;
      // Initialize
      Int_t npdg = 0; Int_t nhits = 0; Int_t idparent = 0;  Int_t pdgparent = -1; Int_t  wecal = -1; Int_t  lecal = -1;
      Double_t edep = 0.0, ftimepos = 0.0, ltimepos = 0.0; 
      TLorentzVector tmom;
      TLorentzVector firstpos, lastpos; 
      
      std::set<int> ecalparent;
      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	
	// Only those the track id matched
	if(value == tmpHit.getTrackID()){
	  npdg = tmpHit.getPDG();
	  nhits++;
	  edep += tmpHit.getEdep()*1000;
	  idparent = tmpHit.getParentID();
	  pdgparent = 999;//tmpHit.getParent().getPDG();
	  
	  if(nhits == 1){
	    firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	    ftimepos = tmpHit.getPosition().T();
	    tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	    wecal = tmpHit.getEcalNumber();
	  }
	  else{
	    lastpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	    ltimepos = tmpHit.getPosition().T();
	    lecal = tmpHit.getEcalNumber();
	  }
	  
	  scintSdHits.erase(scintSdHits.begin() + j);
	  j--;
	  
	}
	else if(value == tmpHit.getParentID()){ //|| value == tmpHit.getParent().getTrackID()){
	  ecalparent.insert(tmpHit.getTrackID());
	}
      }     
      //cout << "NC set size = " << ecalparent.size() << endl;
      for(std::set<int>::iterator ecit=ecalparent.begin(); ecit!=ecalparent.end(); ++ecit){
 	int nvalue = *ecit;
	for(Int_t j=0; j<scintSdHits.size(); j++){
	  scintHit tmpHit = scintSdHits.at(j);
	  if(nvalue == tmpHit.getTrackID()){
	    nhits++;
	    edep += tmpHit.getEdep()*1000;
	    pdgparent = 999;//tmpHit.getParent().getPDG();
	    
	    if(nhits == 1){
	      firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	      ftimepos = tmpHit.getPosition().T();
	      tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	      wecal = tmpHit.getEcalNumber();
	    }
	    else{
	      lastpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	      ltimepos = tmpHit.getPosition().T();
	      lecal = tmpHit.getEcalNumber();
	    }
	    
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	    
	  }
	  else if(nvalue == tmpHit.getParentID()){
	    ecalparent.insert(tmpHit.getTrackID());
	  }
	}
      }

      NCEcalFirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),scintresolution);
      NCEcalFirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),scintresolution);
      NCEcalFirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),scintresolution);
      NCEcalFirstGeantHitPosition[counter][3] = rand->Gaus(firstpos.T());
      NCEcalLastGeantHitPosition[counter][0]  = lastpos.X();
      NCEcalLastGeantHitPosition[counter][1]  = lastpos.Y();
      NCEcalLastGeantHitPosition[counter][2]  = lastpos.Z();
      NCEcalLastGeantHitPosition[counter][3]  = lastpos.T();

      NCEcalNGeantHits[counter]               = nhits;
      NCEcalTotalEDep[counter]                = edep;
      if(lastpos.Mag() > 0)
	NCEcalTrackLength[counter]            = (lastpos-firstpos).Vect().Mag();
      NCEcalMomentum[counter]                 = tmom.Vect().Mag();
      if(lecal > 0)
	wecal = wecal*100 + lecal;
      NCWhichEcal[counter]                    = wecal;
      NCPdg[counter]                          = pdgparent;
 
      ecalparent.clear();
      counter++;
    }
    
    // Other Neutral Clusters
    std::set<int> finalset;
    for(Int_t k=0; k<scintSdHits.size(); k++){
      scintHit tmpHit = scintSdHits.at(k);
      //finalset.insert(tmpHit.getParent().getTrackID());
      finalset.insert(tmpHit.getTrackID());
    }
    
    NCEcalTracks = neutralset.size() + finalset.size();
        
    for(std::set<int>::iterator ecit=finalset.begin(); ecit!=finalset.end(); ++ecit){
      int nvalue = *ecit;

      Int_t npdg = 0; Int_t nhits = 0; Int_t idparent = 0;  Int_t pdgparent = -1; Int_t  wecal = -1; Int_t  lecal = -1;
      Double_t edep = 0.0; 
      TLorentzVector tmom;
      TLorentzVector firstpos, lastpos; 

      for(Int_t j=0; j<scintSdHits.size(); j++){
	scintHit tmpHit = scintSdHits.at(j);
	//if(nvalue == tmpHit.getParent().getTrackID()){
	if(nvalue == tmpHit.getTrackID()){
	  nhits++;
	    edep += tmpHit.getEdep()*1000;
	    pdgparent = -999;//tmpHit.getParent().getPDG();
	    
	    if(nhits == 1){
	      firstpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	      tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	      wecal = tmpHit.getEcalNumber();
	    }
	    else{
	      lastpos.SetXYZT(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z(),tmpHit.getPosition().T());
	      lecal = tmpHit.getEcalNumber();
	    }
	    
	    scintSdHits.erase(scintSdHits.begin() + j);
	    j--;
	    
	}
      }

      NCEcalFirstGeantHitPosition[counter][0] = rand->Gaus(firstpos.X(),scintresolution);
      NCEcalFirstGeantHitPosition[counter][1] = rand->Gaus(firstpos.Y(),scintresolution);
      NCEcalFirstGeantHitPosition[counter][2] = rand->Gaus(firstpos.Z(),scintresolution);
      NCEcalFirstGeantHitPosition[counter][3] = firstpos.T();
      NCEcalLastGeantHitPosition[counter][0]  = lastpos.X();
      NCEcalLastGeantHitPosition[counter][1]  = lastpos.Y();
      NCEcalLastGeantHitPosition[counter][2]  = lastpos.Z();
      NCEcalLastGeantHitPosition[counter][3]  = lastpos.T();

      NCEcalNGeantHits[counter]               = nhits;
      NCEcalTotalEDep[counter]                = edep;
      if(lastpos.Mag() > 0)
	NCEcalTrackLength[counter]            = (lastpos-firstpos).Vect().Mag();
      NCEcalMomentum[counter]                 = tmom.Vect().Mag();
      if(lecal > 0)
	wecal = wecal*100 + lecal;
      NCWhichEcal[counter]                    = wecal;
      NCPdg[counter]                          = pdgparent;
 
      counter++;
    }
    */
    /*
    for(Int_t j=0; j<scintSdHits.size(); j++){
      scintHit tmpHit = scintSdHits.at(j);
      cout << tmpHit.getPDG() << " , " << tmpHit.getP4().Vect().Mag() << " , " << tmpHit.getTrackID() << " , " << tmpHit.getParentID() << endl;
    }
    */
    NCEcalUnusedHits = scintSdHits.size();

    //cout << i << " , No of TPC Hits = " << tpcSdHits.size() << " , Number of Ecal Hits = " << scintSdHits.size() << endl;

    // Clear the vectors
    scintSdHits.clear();
    tpcSdHits.clear();
    neutralset.clear();
    //finalset.clear();
  
    tpcplotter->getSimData()->clearSDHits();
    
    std::set<int>().swap(neutralset);
    neutralset.empty();
    std::set<int>().swap(myset);
    myset.empty();
    
    //std::vector<GeantParticle>().swap(allparticles);
    allparticles.resize(0);
    //std::vector<GeantParticle>().swap(prims);
    prims.resize(0);

    //delete nuEvent_;
    //delete trackingtruth;
  
    // Fill the output tree
    GasTPCTree->Fill(); 
  } // end of event loop
  // --------------------------------------------------------------------------------------
  delete rand;
  //delete possmear;
  //delete dedxsmear;
  //delete momsmearer;
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

  //delete fNtpMCEventRecord;
  return 0;
}

void ReadOutputTree(TTree *GasTPCTree){
  GasTPCTree->Branch("gmcrec","genie::NtpMCEventRecord", &fAnalNtpMCEventRecord);
  GasTPCTree->Branch("EventID",                      &EventID,                      "EventID/I");
  GasTPCTree->Branch("RunID",                        &RunID,                        "RunID/I");
  GasTPCTree->Branch("SpillNumber",                  &SpillNumber,                  "SpillNumber/I");
  GasTPCTree->Branch("Ev",                           &Ev,                           "Ev/D");
  GasTPCTree->Branch("Ev_All",                       &Ev_All,                       "Ev_All/D");
  GasTPCTree->Branch("Ev_reco",                      &Ev_reco,                      "Ev_reco/D");
  GasTPCTree->Branch("neutPdg",                      &neutPdg,                      "neutPdg/I");
  GasTPCTree->Branch("FSTopology",                   &FSTopology,                   "FSTopology/I");
  GasTPCTree->Branch("neutcode",                     &neutcode,                     "neutcode/I");
  GasTPCTree->Branch("x",                            &x,                            "x/D");
  GasTPCTree->Branch("y",                            &y,                            "y/D");
  GasTPCTree->Branch("y_All",                        &y_All,                        "y_All/D");
  GasTPCTree->Branch("y_reco",                       &y_reco,                       "y_reco/D");
  GasTPCTree->Branch("Q2",                           &Q2,                           "Q2/D");
  GasTPCTree->Branch("W",                            &W,                            "W/D");
  GasTPCTree->Branch("POTWeight",                    &POTWeight,                    "POTWeight/D");
  GasTPCTree->Branch("Target",                       &Target,                       "Target/I");
  GasTPCTree->Branch("sample",                       &sample,                       "sample/I");
  GasTPCTree->Branch("reactionmode",                 &reactionmode,                 "reactionmode/I");
  
  GasTPCTree->Branch("NGeantTracks",                 &NGeantTracks,                 "NGeantTracks/I");
  GasTPCTree->Branch("LeftVolume",                   &LeftVolume,                   "LeftVolume[NGeantTracks]/I");
  GasTPCTree->Branch("NGeantHits",                   &NGeantHits,                   "NGeantHits[NGeantTracks]/I");  
  GasTPCTree->Branch("FirstGeantHitPosition",        &FirstGeantHitPosition,        "FirstGeantHitPosition[NGeantTracks][4]/D");
  GasTPCTree->Branch("LastGeantHitPosition",         &LastGeantHitPosition,         "LastGeantHitPosition[NGeantTracks][4]/D");
  GasTPCTree->Branch("VertexPosition",               &VertexPosition,               "VertexPosition[4]/D");
  GasTPCTree->Branch("Momentum",                     &Momentum,                     "Momentum[NGeantTracks]/D");
  GasTPCTree->Branch("Pt",                           &Pt,                           "Pt[NGeantTracks]/D");
  GasTPCTree->Branch("Costheta",                     &Costheta,                     "Costheta[NGeantTracks]/D");
  GasTPCTree->Branch("TrackLength",                  &TrackLength,                  "TrackLength[NGeantTracks]/D");
  GasTPCTree->Branch("HitLength",                    &HitLength,                    "HitLength[NGeantTracks]/D");
  GasTPCTree->Branch("Pdg",                          &Pdg,                          "Pdg[NGeantTracks]/I");
  GasTPCTree->Branch("ParentPdg",                    &ParentPdg,                    "ParentPdg[NGeantTracks]/I");
  GasTPCTree->Branch("TotalEDep",                    &TotalEDep,                    "TotalEDep[NGeantTracks]/D");
  GasTPCTree->Branch("TrackID",                      &TrackID,                      "TrackID[NGeantTracks]/I");
  GasTPCTree->Branch("ParentID",                     &ParentID,                     "ParentID[NGeantTracks]/I");
  GasTPCTree->Branch("dEdx",                         &dEdx,                         "dEdx[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxSigma",                    &dEdxSigma,                    "dEdxSigma[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxMuon",                     &dEdxMuon,                     "dEdxMuon[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxPion",                     &dEdxPion,                     "dEdxPion[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxElec",                     &dEdxElec,                     "dEdxElec[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxKaon",                     &dEdxKaon,                     "dEdxKaon[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxProt",                     &dEdxProt,                     "dEdxProt[NGeantTracks]/D");
  GasTPCTree->Branch("dEdxTruncated",                &dEdxTruncated,                "dEdxTruncated[NGeantTracks]/D");
  GasTPCTree->Branch("PrimaryPdg",                   &PrimaryPdg,                   "PrimaryPdg[NGeantTracks]/I");
  GasTPCTree->Branch("Charge",                       &Charge,                       "Charge[NGeantTracks]/I");

  GasTPCTree->Branch("IsTPCRecon",                   &IsTPCRecon,                   "IsTPCRecon[NGeantTracks]/I");
  GasTPCTree->Branch("ReconInFV",                    &ReconInFV,                    "ReconInFV[NGeantTracks]/I");
  GasTPCTree->Branch("NTPCTracksInEcal",             &NTPCTracksInEcal,             "NTPCTracksInEcal/I");
  GasTPCTree->Branch("ReconPosition",                &ReconPosition,                "ReconPosition[NGeantTracks][4]/D");
  GasTPCTree->Branch("ReconLastPosition",            &ReconLastPosition,            "ReconLastPosition[NGeantTracks][4]/D");
  GasTPCTree->Branch("ReconTrackLength",             &ReconTrackLength,             "ReconTrackLength[NGeantTracks]/D");
  GasTPCTree->Branch("ReconCostheta",                &ReconCostheta,                "ReconCostheta[NGeantTracks]/D");
  GasTPCTree->Branch("ReconMomentum",                &ReconMomentum,                "ReconMomentum[NGeantTracks]/D");
  GasTPCTree->Branch("ReconMomentumXYZ",             &ReconMomentumXYZ,             "ReconMomentumXYZ[NGeantTracks][3]/D");
  GasTPCTree->Branch("ReconCharge",                  &ReconCharge,                  "ReconCharge[NGeantTracks]/I");
  GasTPCTree->Branch("RecondEdx",                    &RecondEdx,                    "RecondEdx[NGeantTracks]/D");
  GasTPCTree->Branch("RecondEdxTruncated",           &RecondEdxTruncated,           "RecondEdxTruncated[NGeantTracks]/D");
  GasTPCTree->Branch("pullmuon",                     &pullmuon,                     "pullmuon[NGeantTracks]/D");
  GasTPCTree->Branch("pullpion",                     &pullpion,                     "pullpion[NGeantTracks]/D");
  GasTPCTree->Branch("pullelec",                     &pullelec,                     "pullelec[NGeantTracks]/D");
  GasTPCTree->Branch("pullkaon",                     &pullkaon,                     "pullkaon[NGeantTracks]/D");
  GasTPCTree->Branch("pullprot",                     &pullprot,                     "pullprot[NGeantTracks]/D");
  GasTPCTree->Branch("EcalReconHitPosition",         &EcalReconHitPosition,         "EcalReconHitPosition[NGeantTracks][4]/D");
  
  GasTPCTree->Branch("EcalNGeantHits",               &EcalNGeantHits,               "EcalNGeantHits[NGeantTracks]/I");
  GasTPCTree->Branch("NEcals",                       &NEcals,                       "NEcals[NGeantTracks]/I");
  GasTPCTree->Branch("EcalFirstGeantHitPosition",    &EcalFirstGeantHitPosition,    "EcalFirstGeantHitPosition[NGeantTracks][4]/D");
  GasTPCTree->Branch("EcalLastGeantHitPosition",     &EcalLastGeantHitPosition,     "EcalLastGeantHitPosition[NGeantTracks][4]/D");
  GasTPCTree->Branch("EcalMomentum",                 &EcalMomentum,                 "EcalMomentum[NGeantTracks]/D");
  GasTPCTree->Branch("EcalTrackLength",              &EcalTrackLength,              "EcalTrackLength[NGeantTracks][3]/D");
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
  GasTPCTree->Branch("GammaFirstGeantHitPosition",   &GammaFirstGeantHitPosition,   "GammaFirstGeantHitPosition[NGamma][4]/D");
  
  GasTPCTree->Branch("NNeutron",                     &NNeutron,                     "NNeutron/I");
  GasTPCTree->Branch("NeutronNHits",                 &NeutronNHits,                 "NeutronNHits[NNeutron]/I");
  GasTPCTree->Branch("NeutronWhichEcal",             &NeutronWhichEcal,             "NeutronWhichEcal[NNeutron]/I");
  GasTPCTree->Branch("NeutronMomentum",              &NeutronMomentum,              "NeutronMomentum[NNeutron]/D");
  GasTPCTree->Branch("NeutronEDep",                  &NeutronEDep,                  "NeutronEDep[NNeutron]/D");
  GasTPCTree->Branch("NeutronFirstGeantHitPosition", &NeutronFirstGeantHitPosition, "NeutronFirstGeantHitPosition[NNeutron][4]/D");
  
  GasTPCTree->Branch("NISParticles",                 &NISParticles,                 "NISParticles/I");
  GasTPCTree->Branch("ISPdg",                        &ISPdg,                        "ISPdg[NISParticles]/I");
  GasTPCTree->Branch("NFSParticles",                 &NFSParticles,                 "NFSParticles/I");
  GasTPCTree->Branch("FSPdg",                        &FSPdg,                        "FSPdg[NFSParticles]/I");
  GasTPCTree->Branch("FSMomentum",                   &FSMomentum,                   "FSMomentum[NFSParticles]/D");
  GasTPCTree->Branch("FSCostheta",                   &FSCostheta,                   "FSCostheta[NFSParticles]/D");
  
  GasTPCTree->Branch("NCEcalTracks",                 &NCEcalTracks,                 "NCEcalTracks/I");
  GasTPCTree->Branch("NCEcalNGeantHits",             &NCEcalNGeantHits,             "NCEcalNGeantHits[NCEcalTracks]/I");  
  GasTPCTree->Branch("NCEcalFirstGeantHitPosition",  &NCEcalFirstGeantHitPosition,  "NCEcalFirstGeantHitPosition[NCEcalTracks][4]/D");
  GasTPCTree->Branch("NCEcalLastGeantHitPosition",   &NCEcalLastGeantHitPosition,   "NCEcalLastGeantHitPosition[NCEcalTracks][4]/D");
  GasTPCTree->Branch("NCEcalMomentum",               &NCEcalMomentum,               "NCEcalMomentum[NCEcalTracks]/D");
  GasTPCTree->Branch("NCEcalTrackLength",            &NCEcalTrackLength,            "NCEcalTrackLength[NCEcalTracks]/D");
  GasTPCTree->Branch("NCEcalTotalEDep",              &NCEcalTotalEDep,              "NCEcalTotalEDep[NCEcalTracks]/D");
  GasTPCTree->Branch("NCWhichEcal",                  &NCWhichEcal,                  "NCWhichEcal[NCEcalTracks]/I");
  GasTPCTree->Branch("NCPdg",                        &NCPdg,                        "NCPdg[NCEcalTracks]/I");
  GasTPCTree->Branch("NCEcalUnusedHits",             &NCEcalUnusedHits,             "NCEcalUnusedHits/I");
}

void ReadDatacard(std::string datacard){

  LOG("GeometryConstruction",pNOTICE) << "Reading configuration from XML file [" << datacard << "]";

  TXMLEngine* xml = new TXMLEngine(); 
  XMLDocPointer_t xmldoc = xml->ParseFile(datacard.c_str());
  if (xmldoc==0) {
    std::cerr << "Exp >> " << "Can not parse input: " << datacard << std::endl;
    delete xml;
    return;  
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  deltaSagitta         = util::XML2Dbl(xml,root_node,"Analysis/deltaSagitta");
  tpcresolution        = util::XML2Dbl(xml,root_node,"Analysis/TPCPositRes");
  angresolution        = util::XML2Dbl(xml,root_node,"Analysis/TPCAngulRes");
  magfield             = util::XML2Dbl(xml,root_node,"MagneticFieldStrength");
  scintresolution      = util::XML2Dbl(xml,root_node,"Analysis/EcalPositRes");
  scintTimeRes         = util::XML2Dbl(xml,root_node,"Analysis/EcalTimeRes");
  dedxCorMIP           = util::XML2Dbl(xml,root_node,"Analysis/DEDXScaleMIP");
  dedxCorElec          = util::XML2Dbl(xml,root_node,"Analysis/DEDXScaleEle");
  gaspressure          = util::XML2Dbl(xml,root_node,"Analysis/GasPressure");
  fiducialXY           = util::XML2Dbl(xml,root_node,"Analysis/FVXY");
  fiducialZ            = util::XML2Dbl(xml,root_node,"Analysis/FVZ");
  fPOTSimulated        = util::XML2Dbl(xml,root_node,"POTSimulated");

  // Release memory
  xml->FreeDoc(xmldoc);
  delete xml;
}
