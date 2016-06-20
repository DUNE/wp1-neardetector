//____________________________________________________________________________
/*!

\class    GasTPCRecon

\brief    

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  June 2016
\last update June 2016
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

#include "GasTPCDataLib.hh"
#include "TXMLEngine.h"
#include "Messenger.hh"
#include "ReconUtils.hh"

#include "GasTPCRecon.hh"

using namespace std;
using namespace util;
using namespace ReconUtils;

int main(int argc, char ** argv) {

  std::string datacard   = "";
  std::string inputfile  = "";
  std::string outputfile = "";

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
  }
  // --------------------------------------------------------------------------
  TFile *FileMC = new TFile(inputfile.c_str(),"READ");
  if(FileMC->GetNkeys()<1){
    std::cout << "ERROR::Bad input file: " << inputfile << std::endl;
    return 1;
  }
  // --------------------------------------------------------------------------
  bool fhc = true;
  // Select anti-neutrino configuration if running on rhc
  if(inputfile.find("antineutrino") != std::string::npos)
    fhc = false;
  // --------------------------------------------------------------------------
  // Load the xml file
  const char* gastpc_dir = getenv("GasTPC");
  if(datacard == "")
    datacard = "GeantSimulation.xml";
  std::string fdatacard = string(gastpc_dir) + "/src/config/" + datacard;

  ReadDatacard(fdatacard);
  // --------------------------------------------------------------------------
  TTree *nuEventTree      = (TTree*)FileMC->Get("NuInteractions");
  nuEventTree->SetBranchAddress("nuInteractions",  &fnuEvent);
  nuEventTree->SetBranchAddress("gmcrec",  &fgrec);
  TTree *detectorHitsTree = (TTree*)FileMC->Get("Tracking");
  detectorHitsTree->SetBranchAddress("SimulData",           &fsimData);
  detectorHitsTree->SetBranchAddress("GeantTrackingTruth",  &ftrackingRecord);

  int nEntries          = nuEventTree->GetEntries();
  int TrnEntries        = detectorHitsTree->GetEntries();

  if(TrnEntries != nEntries){
    std::cout << "ERROR::Truth tree and GEANT tree doesn't have the same number of entries in file: " << inputfile << " , EXIT!!!" << std::endl;
    return 1;
  }
  // --------------------------------------------------------------------------
  // Setup the output tree
  ReadOutputTree();

  // Spill by spill analysis
  cout << "INFO::Numbers of spill to simulate: " << fNSpillsSimulated << endl;

  for(int j=0; j<fNSpillsSimulated; j++){
    // Vector to store products from reco
    std::vector<TrackParticle> DetectorEvent;
    std::vector<EcalTrackParticle> EcalDetectorEvent;
    std::vector<NeutrinoEvent> NeutrinoEventV;
    std::vector<GeantParticle> GeantTrack;
    std::vector<GeantParticle> GeantParentTrack;
    
    // Event loop
    for(int i=0; i<nEntries; i++){
      nuEventTree->GetEntry(i);
      detectorHitsTree->GetEntry(i);
      
      // True neutrino event
      NeutrinoEvent *nuEvent_           = fnuEvent;
      if( !nuEvent_ ) continue;

      // Temporary
      int spill = nuEvent_->GetSpillNumber();
      if( spill > fNSpillsSimulated )
	spill = spill - fNSpillsSimulated;

      // Only events in this spill
      if( j != spill ) continue;

      spill = 1000000*nuEvent_->getRunID() + spill;

      GeantTrackingTruth* trackingtruth = ftrackingRecord;
      SimulData*          simuldata     = fsimData;
      if( !trackingtruth || !simuldata ) continue;

      // Information about all geant particles
      std::vector<GeantParticle> prims        = trackingtruth->getPrimariesPtr();
      std::vector<GeantParticle> allparticles = trackingtruth->getAllParticlesPtr();
      // TPC Hits
      HitCollection tpcSdHits = simuldata->getTpcFidHits();
      // Ecal hits
      ScintHitCollection scintSdHits = simuldata->getScintHits();
      
      // ----------------------------------------------------------------------
      // TPC hits
      // ----------------------------------------------------------------------
      // Take the unique track id for each tpc track
      std::set<int> myset; // set takes only unique elements
      for(int k=0; k<tpcSdHits.size(); k++){
	SDHit tmpHit = tpcSdHits.at(k);
	//if(tmpHit.getPDG() > 99999){continue;}
	myset.insert(tmpHit.getTrackID());
	if( myset.size() > kMaxTrack )
	  break;
      }
      
      // Get the number of TPC tracks - this also includes tracks with just one geant hit
      int NGeantTracks     = myset.size();

      // Loop start
      Int_t counter = 0;
      Int_t inecal = 0;

      for(std::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it){
	int value = *it;
	
	// Initialize
	Int_t rseed = 9999, npdg = 0; Int_t nhits = 0; Int_t idparent = 0;  Int_t pdgparent = -1; Int_t charge = 0; Int_t exitdetector = -1;
	Double_t edep = 0.0, length = 0.0, ftimepos = 0.0, ltimepos = 0.0;
	TLorentzVector tmom, backmom;
	TVector3 firstpos, lastpos;
	TVector2 xzfirstpos, xzlastpos;
	GeantParticle part;
	GeantParticle parentpart;
	
	for(int j=0; j<tpcSdHits.size(); j++){
	  SDHit tmpHit = tpcSdHits.at(j);
	  
	  // Only those the track id matched
	  if( value == tmpHit.getTrackID() ){
	    nhits++;
	    npdg         = tmpHit.getPDG();
	    edep         += tmpHit.getEdep()*1000;
	    idparent     = tmpHit.getParentID();
	    charge       = tmpHit.getCharge();
	    exitdetector = tmpHit.getTrackLeftVolume();
	    rseed        = (int)tmpHit.getP4().E() + j + i + rseed;
	    
	    if( nhits == 1 ){
	      firstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	      xzfirstpos.Set(tmpHit.getPosition().X(),tmpHit.getPosition().Z());
	      tmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	      ftimepos = tmpHit.getPosition().T();
	      // Find true track and parent - need to loop twice to access both primaries and secondaries
	      for(int k=0;k<prims.size();k++){
		GeantParticle geantparticle = prims.at(k);

		if( value == geantparticle.getTrackID() )
		  part = geantparticle;
		
		if( idparent == geantparticle.getTrackID() )
		  parentpart = geantparticle;

	      }

	      for(int k=0;k<allparticles.size();k++){
		GeantParticle geantparticle = allparticles.at(k);

		if( value == geantparticle.getTrackID() )
		  part = geantparticle;
		
		if( idparent == geantparticle.getTrackID() )
		  parentpart = geantparticle;

	      }
	    }
	    else{	    
	      TVector3 temppos(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	      backmom.SetXYZT(tmpHit.getP4().X(),tmpHit.getP4().Y(),tmpHit.getP4().Z(),0);
	      if( lastpos.X() == 0 ){
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
	    
	    //tpcSdHits.erase(tpcSdHits.begin() + j);
	    //j--;
	  }
	}
	
	if( rseed == 9999 )
	  rseed = rseed + value + i;

	// Track found - now check if it has been reconstructed
	if( nhits < 3 ) continue;
	int nreconhits  = (int)length/10; // 1 hit / cm
	if( !ReconUtils::IsReconTrack(nreconhits,rseed) ) continue;

	TVector3 freconpos = ReconUtils::Smearer( firstpos, tpcresolution, rseed );
	TVector3 lreconpos = ReconUtils::Smearer( lastpos, tpcresolution, rseed*rseed );
	bool ReconInFV     = ReconUtils::inVesselFV( freconpos, fiducialZ, fiducialXY );
	bool BackReconInFV = ReconUtils::inVesselFV( lreconpos, fiducialZ, fiducialXY );
	double TrackLength = (lastpos-firstpos).Mag();
	double ReconTrackLength = (lreconpos-freconpos).Mag();

	// dedx
	//double fdedx                   = edep/(xzlastpos-xzfirstpos).Mod();
	double fdedx = edep/length/10.0; // Mev/cm
	double dedxres = ReconUtils::DEDXResolution(gaspressure,length/1000.0); // tracklength has to be in meters
	// dedx resolution is the %
	dedxres = dedxres*fdedx/100.0;
	double dedxrecon = ReconUtils::Smearer(fdedx,dedxres,rseed);

	if( dedxrecon <= 0 ) continue;

	double dedxExpMuon = dedxCorMIP*ReconUtils::DEDXExp(tmom.Vect().Mag(), 105.66);
	double dedxExpPion = dedxCorMIP*ReconUtils::DEDXExp(tmom.Vect().Mag(), 139.57);
	double dedxExpElec = dedxCorElec*ReconUtils::DEDXExp(tmom.Vect().Mag(), 0.511);
	double dedxExpKaon = dedxCorMIP*ReconUtils::DEDXExp(tmom.Vect().Mag(), 493.67);
	double dedxExpProt = dedxCorMIP*ReconUtils::DEDXExp(tmom.Vect().Mag(), 938.27);

	// Additional correction based on the truth dEdx
	if( abs(npdg) == 13 ){
	  double diff = fdedx - dedxExpMuon;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}
	else if( abs(npdg) == 211 ){
	  double diff = fdedx - dedxExpPion;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}
	else if( abs(npdg) == 11 ){
	  double diff = fdedx - dedxExpElec;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}
	else if( abs(npdg) == 321 ){
	  double diff = fdedx - dedxExpKaon;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}
	else if( abs(npdg) == 2212 ){
	  double diff = fdedx - dedxExpProt;
	  dedxExpMuon = dedxExpMuon + diff;
	  dedxExpPion = dedxExpPion + diff;
	  dedxExpElec = dedxExpElec + diff;
	  dedxExpKaon = dedxExpKaon + diff;
	  dedxExpProt = dedxExpProt + diff;
	}

	double pullmuon  = (dedxrecon-dedxExpMuon)/dedxres;
	double pullpion  = (dedxrecon-dedxExpPion)/dedxres;
	double pullelec  = (dedxrecon-dedxExpElec)/dedxres;
	double pullkaon  = (dedxrecon-dedxExpKaon)/dedxres;
	double pullprot  = (dedxrecon-dedxExpProt)/dedxres;

	// Angle smear
	double ang             = ReconUtils::Smearer(acos(tmom.CosTheta()),angresolution,rseed);

	// Momentum smear
	double truthtransmom   = TMath::Sqrt(tmom.Y()*tmom.Y() + tmom.Z()*tmom.Z());
	double tracklength     = TMath::Sqrt( (lastpos.Y()-firstpos.Y())*(lastpos.Y()-firstpos.Y()) + (lastpos.Z()-firstpos.Z())*(lastpos.Z()-firstpos.Z()) );
	double getsagita       = ReconUtils::getTrackSagitta(magfield,tracklength,truthtransmom);
	double smearsagita     = ReconUtils::Smearer(getsagita,deltaSagitta,rseed);
	int reconcharge        = ReconUtils::getReconCharge(smearsagita,charge);
	double transvmom       = ReconUtils::getReconTransMomentum(magfield,tracklength,smearsagita,deltaSagitta); 

	double pxsmear         = ReconUtils::getReconMomentumX(transvmom,truthtransmom,tmom.X(),rseed);

	double ReconMomentum   = TMath::Sqrt(transvmom*transvmom + pxsmear*pxsmear);

	if( ReconMomentum <= 0 ) continue;

	//double pz = ReconMomentum*cos(ang);
	//double py = ReconMomentum*ReconMomentum - pz*pz - pxsmear*pxsmear;
	//if(py > 0)
	//py = TMath::Sqrt(py);
	//else
	//py =0;

	// Back momentum
	double btruthtransmom  = TMath::Sqrt(backmom.Y()*backmom.Y() + backmom.Z()*backmom.Z());
	double bgetsagita      = ReconUtils::getTrackSagitta(magfield,tracklength,btruthtransmom);
	double bsmearsagita    = ReconUtils::Smearer(bgetsagita,deltaSagitta,rseed);
	double btransvmom      = ReconUtils::getReconTransMomentum(magfield,tracklength,bsmearsagita,deltaSagitta);
	double bpxsmear        = ReconUtils::getReconMomentumX(btransvmom,btruthtransmom,backmom.X(),rseed);
	double BackMomentum    = TMath::Sqrt(btransvmom*btransvmom + bpxsmear*bpxsmear);

	bool inecal = false;
	double recontime = -9999.0; double lrecontime = -9999.0; double ecaledep = 0.0;
	int ecal = -1; int necal = 0; int firstecal = -1; int necalhits = 0;
	TVector3 ecalfirstpos, ecallastpos;
	for(int jj=0; jj<scintSdHits.size(); jj++){
	  scintHit tmpHit = scintSdHits.at(jj);
	
	  // Only those the track id matched
	  if( value == tmpHit.getTrackID() ){
	    necalhits++;
	    
	    if(necalhits == 1){
	      ecaledep += tmpHit.getEdep()*1000;
	      inecal = true;
	      double time = tmpHit.getPosition().T();
	      recontime = ReconUtils::Smearer(time,ecalTimeRes,rseed);
	      firstecal = tmpHit.getEcalNumber();
	      ecalfirstpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	    }

	    if( tmpHit.getEcalNumber() == firstecal){
	      ecaledep += tmpHit.getEdep()*1000;
	      ecallastpos.SetXYZ(tmpHit.getPosition().X(),tmpHit.getPosition().Y(),tmpHit.getPosition().Z());
	      lrecontime = tmpHit.getPosition().T();
	    }

	    if( ecal != tmpHit.getEcalNumber() ){
	      ecal = tmpHit.getEcalNumber();
	      necal++;
	    }
	    
	  }
	} // scint hits

	// Track position
	TLorentzVector fpos(freconpos.X(), freconpos.Y(), freconpos.Z(), recontime);
	TLorentzVector lpos(lreconpos.X(), lreconpos.Y(), lreconpos.Z(), lrecontime);
	// For ecal
	TVector3 fecalreconpos = ReconUtils::Smearer( ecalfirstpos, tpcresolution, rseed );
	TVector3 lecalreconpos = ReconUtils::Smearer( ecallastpos, tpcresolution, rseed*rseed );
	TLorentzVector ecalfpos(fecalreconpos.X(), fecalreconpos.Y(), fecalreconpos.Z(), recontime);
	TLorentzVector ecallpos(lecalreconpos.X(), lecalreconpos.Y(), lecalreconpos.Z(), lrecontime);

	// Define a new tpc track
	TrackParticle* track = new TrackParticle();

	// Set the track properties - a better way of doing this?
	track->SetReconMomentum(ReconMomentum);
	track->SetBackMomentum(BackMomentum);
	track->SetReconCostheta(cos(ang));
	track->SetReconPosition(fpos);
	track->SetBackPosition(lpos);
	track->SetReconCharge(reconcharge);
	track->SetRecondEdx(dedxrecon);
	track->SetdEdxSigma(dedxres);
	track->SetMuonPull(pullmuon);
	track->SetPionPull(pullpion);
	track->SetElecPull(pullelec);
	track->SetKaonPull(pullkaon);
	track->SetProtPull(pullprot);
	track->SetReconTrackLength(ReconTrackLength);
	track->SetInEcal(inecal);
	track->SetInFV(ReconInFV);
	track->SetBackInFV(BackReconInFV);
	track->SetNEcals(necal);
	track->SetSpillNumber(spill);
	track->SetFlippedKinematics(false);

	track->SetdEdx(fdedx);
	track->SetdEdxMuon(dedxExpMuon);
	track->SetdEdxPion(dedxExpPion);
	track->SetdEdxElec(dedxExpElec);
	track->SetdEdxKaon(dedxExpKaon);
	track->SetdEdxProt(dedxExpProt);
	track->SetTrackLength(TrackLength);
	track->SetNGeantHits(nhits);
	track->SetTotalTPCEDep(edep);

	// Flip kinematics - only for tracks coming outside the FV and stop in FV
	if( BackReconInFV && !ReconInFV ){
	  track->SetReconMomentum(BackMomentum);
	  track->SetBackMomentum(ReconMomentum);
	  track->SetReconCostheta(-cos(ang));
	  track->SetReconPosition(lpos);
	  track->SetBackPosition(fpos);
	  track->SetReconCharge(-reconcharge);

	  track->SetInFV(BackReconInFV);
	  track->SetBackInFV(ReconInFV);
	  track->SetFlippedKinematics(true);
	}

	// Define a new ecal track
	EcalTrackParticle* ecaltrack = new EcalTrackParticle();

	// Set the ecaltrack properties - this is the first ecal the track enters
	ecaltrack->SetEcal(firstecal);
	ecaltrack->SetReconPosition(ecalfpos);
	ecaltrack->SetBackPosition(ecallpos);
	ecaltrack->SetNGeantHits(necalhits);
	ecaltrack->SetTotalECALEDep(ecaledep);
	ecaltrack->SetSpillNumber(spill);

	if( BackReconInFV && !ReconInFV ){
	  ecaltrack->SetReconPosition(ecallpos);
	  ecaltrack->SetBackPosition(ecalfpos);
	  ecaltrack->SetFlippedKinematics(true);
	}

	DetectorEvent.push_back(*track);
	EcalDetectorEvent.push_back(*ecaltrack);
	NeutrinoEventV.push_back(*nuEvent_);
	GeantParentTrack.push_back(parentpart);
	GeantTrack.push_back(part);
      } // track iterator

    } // event loop

    fevent->setTracksInEvent(DetectorEvent);
    fevent->setEcalTracksInEvent(EcalDetectorEvent);
    fevent->setParticlesInEvent(GeantTrack);
    fevent->setParentParticlesInEvent(GeantParentTrack);
    fevent->setTrueVertexInEvent(NeutrinoEventV);

    fOutData->Fill();

    fevent->clear();

    std::vector<TrackParticle>().swap(DetectorEvent); 
    DetectorEvent.resize(0);

    std::vector<NeutrinoEvent>().swap(NeutrinoEventV); 
    NeutrinoEventV.resize(0);

    std::vector<GeantParticle>().swap(GeantTrack); 
    GeantTrack.resize(0);

    std::vector<GeantParticle>().swap(GeantParentTrack); 
    GeantParentTrack.resize(0);
  } // spill loop

  // Save neutrino interactions tree
  ReadOutputTree(true);
  for(Int_t i=0; i<nEntries; i++){
    nuEventTree->GetEntry(i);
    
    // True neutrino event
    NeutrinoEvent *nuEvent_           = fnuEvent;
    genie::NtpMCEventRecord * grec    = fgrec;
    genie::EventRecord *eventr        = grec->event;
    fAnalNtpMCEventRecord->Fill(i, eventr);
    
    fTrueData->Fill();
  }

  // Write to output file
  TFile *f = new TFile(outputfile.c_str(),"RECREATE");
  fTrueData->Write();
  fOutData->Write();
}

void ReadOutputTree(bool truetree){
  if(!truetree){
    fOutData   = new TTree("TPCRecon", "Reconstucted TPC Info");
    makeOutTreeBranches<SimulDetectorEvent>(fevent);
    // line needed to avoid memory-resident Tree error
    fOutData->SetDirectory(0);
  }
  else{
    fTrueData = new TTree("NuInteractions", "True Tree");
    makeOutTreeBranches2<NeutrinoEvent>(fnuEvent);
    fTrueData->SetDirectory(0);
  }
}

void ReadDatacard(std::string datacard){

  LOG("GasTPCRecon",pNOTICE) << "Reading configuration from XML file [" << datacard << "]";

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
  ecalTimeRes          = util::XML2Dbl(xml,root_node,"Analysis/EcalTimeRes");
  fNSpillsSimulated    = (int)util::XML2Dbl(xml,root_node,"NSpillsSimulated");

  // Release memory
  xml->FreeDoc(xmldoc);
  delete xml;
}
