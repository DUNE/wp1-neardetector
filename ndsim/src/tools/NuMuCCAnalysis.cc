//____________________________________________________________________________
/*!

\class    NuMuCCAnalysis

\brief    Selection of numu CC candidates

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
#include "AnalysisUtils.hh"

#include "NuMuCCAnalysis.hh"

using namespace std;

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
    datacard = "SelectionParameters.xml";
  std::string fdatacard = string(gastpc_dir) + "/src/config/" + datacard;

  ReadDatacard(fdatacard);
  // --------------------------------------------------------------------------
  TTree *TPCRecon      = (TTree*)FileMC->Get("TPCRecon");
  TPCRecon->SetBranchAddress("SimulDetectorEvent",  &fevent);

  // Event loop - each entry is a spill
  for(int i=0; i<TPCRecon->GetEntries(); i++){
    TPCRecon->GetEntry(i);

    std::vector<TrackParticle> DetectorEvent          = fevent->getTracksInEvent();
    std::vector<EcalTrackParticle> EcalDetectorEvent  = fevent->getEcalTracksInEvent();
    std::vector<NeutrinoEvent> NeutrinoEventV         = fevent->getTrueVertexInEvent();
    std::vector<GeantParticle> GeantTrack             = fevent->getParticlesInEvent();
    std::vector<GeantParticle> GeantParentTrack       = fevent->getParentParticlesInEvent();
    /*
    for(int k=0; k<NeutrinoEventV.size(); k++){
      NeutrinoEvent sel_true_vertex = NeutrinoEventV.at(k);

      TVector3 vpos(sel_true_vertex.getPosition().X(), sel_true_vertex.getPosition().Y(), sel_true_vertex.getPosition().Z());
      if( ReconUtils::inVesselFV(vpos, 2000, 2000) )
	cout << "---------- Vertex in FV ---------- " << sel_true_vertex.getNucleonPdg() << endl;
    }
    */
 
    std::vector<TVector3> Vertices = AnalysisUtils::FindTPCVertices(DetectorEvent);
    //if(Vertices.size() == 0) continue;
    cout << "INFO::NVertices Found = " << Vertices.size() << endl;

    std::vector<TVector3> SingleVertices = AnalysisUtils::FindIsoTPCTracks(DetectorEvent);
    //cout << "INFO::Iso Vertices Found = " << SingleVertices.size() << endl;

    if(Vertices.size() == 0 && SingleVertices.size() == 0) continue;
    
    int detid = -1;
    int ntracks = 0;
    double tempmom = 0.0;
    int nbackecal = -1;
    for(int kk=0; kk<Vertices.size(); kk++){
      TVector3 temp = Vertices.at(kk);

      // Check for other vertices in the Ecal
      bool fromecal = false;
      for(int k=0; k<DetectorEvent.size(); k++){
	TrackParticle track = DetectorEvent.at(k);
	EcalTrackParticle ecaltrack = EcalDetectorEvent.at(k);

	double track_time = track.GetReconPosition().T();
	TVector3 track_pos(track.GetReconPosition().X(),
			   track.GetReconPosition().Y(),
			   track.GetReconPosition().Z() );

	if( (track_pos - temp).Mag() > 50.0) continue;

	double ecal_track_time = ecaltrack.GetReconPosition().T();
	TVector3 ecal_track_pos(ecaltrack.GetReconPosition().X(),
				ecaltrack.GetReconPosition().Y(),
				ecaltrack.GetReconPosition().Z() );

	int necal = AnalysisUtils::OtherTracksFromEcal(EcalDetectorEvent, ecal_track_pos, ecal_track_time);

	double ecal_backtrack_time = ecaltrack.GetBackPosition().T();
	TVector3 ecal_backtrack_pos(ecaltrack.GetBackPosition().X(),
				    ecaltrack.GetBackPosition().Y(),
				    ecaltrack.GetBackPosition().Z() );

	nbackecal = AnalysisUtils::OtherTracksFromEcal(EcalDetectorEvent, ecal_backtrack_pos, ecal_backtrack_time);

	if( necal > 1 ) {
	  fromecal = true;
	  break;
	}
      }

      if( fromecal ) continue;

      // Check for stopping products in TPC
      std::vector<TrackParticle> TempEvent;
      for(int k=0; k<DetectorEvent.size(); k++){
	TrackParticle track = DetectorEvent.at(k);

	TVector3 track_pos(track.GetReconPosition().X(),
			   track.GetReconPosition().Y(),
			   track.GetReconPosition().Z() );

	if( (track_pos - temp).Mag() > 50.0) continue;
	TempEvent.push_back(track);
      }

      //cout << "Number of tracks associated with this vertex = " << TempEvent.size() << endl;

      bool bkg = false;
      if( TempEvent.size() == 2 ){
	TrackParticle track1 = TempEvent.at(0);
	TrackParticle track2 = TempEvent.at(1);
	
	TrackParticle tracktemp;
	if( track1.GetReconMomentum() < track2.GetReconMomentum() ){
	  tracktemp = track1;
	  track1 = track2;
	  track2 = tracktemp;
	}
	
	if( track1.GetReconPosition().T() > 0 && track2.GetReconPosition().T() > 0 ){
	  if( track1.GetReconPosition().T() < track2.GetReconPosition().T() && track1.GetReconCostheta()*track2.GetReconCostheta() < 0 && fabs(track1.GetReconPosition().T() - track2.GetReconPosition().T() ) > 5.0 )
	    bkg = true;
	}
	else if( track1.GetReconPosition().T() < 0 && track2.GetReconPosition().T() > 0 ){
	  if( track1.GetReconCostheta()*track2.GetReconCostheta() < 0 )
	    bkg = true;
	}
      }

      if( bkg ) continue;

      int nothertracks = 0;
      for(int k=0; k<TempEvent.size(); k++){
	TrackParticle temp_track = TempEvent.at(k);
	TVector3 temp_track_backpos(temp_track.GetBackPosition().X(),
				    temp_track.GetBackPosition().Y(),
				    temp_track.GetBackPosition().Z() );

	for(int k=0; k<DetectorEvent.size(); k++){
	  TrackParticle track = DetectorEvent.at(k);

	  TVector3 track_pos(track.GetReconPosition().X(),
			     track.GetReconPosition().Y(),
			     track.GetReconPosition().Z() );

	  //if( (track_pos - temp_track_backpos).Mag() == 0.0) continue;

	  if( (track_pos - temp_track_backpos).Mag() < 50.0)
	    nothertracks++;
	}
      }

      if( nothertracks > 0 ) continue;

      //double tempmom = 0.0;
      ntracks = 0;
      for(int k=0; k<DetectorEvent.size(); k++){
	TrackParticle track = DetectorEvent.at(k);
	EcalTrackParticle ecaltrack = EcalDetectorEvent.at(k);
	
	double track_time = track.GetReconPosition().T();
	TVector3 track_pos(track.GetReconPosition().X(),
			   track.GetReconPosition().Y(),
			   track.GetReconPosition().Z() );

	if( (track_pos - temp).Mag() > 50.0) continue;
	ntracks++;
	TrackParticle sel_track       = DetectorEvent.at(k);
	NeutrinoEvent sel_true_vertex = NeutrinoEventV.at(k);
	GeantParticle sel_true_track  = GeantTrack.at(k);
	GeantParticle sel_true_parent = GeantParentTrack.at(k);
	/*
	if(abs(sel_true_track.getPDG()) != 11){
	  TVector3 vpos(sel_true_vertex.getPosition().X(), sel_true_vertex.getPosition().Y(), sel_true_vertex.getPosition().Z());
	  cout << ntracks << " Track: Pdg = " << sel_true_track.getPDG() << " , parent = " << sel_true_parent.getPDG() << " Vertex in FV = " << ReconUtils::inVesselFV(vpos, 2000, 2000)
	       << " , Mom = " << sel_track.GetReconMomentum() << " , angle = " << sel_track.GetReconCostheta() << " , Charge = " << track.GetReconCharge() << " , Muon pull = " << track.GetMuonPull()
	    //  << " , inFV = " << sel_track.GetInFV() << " , back in FV = " << sel_track.GetBackInFV() << " , other tracks = " << nothertracks << endl;
	    //<< " , Time = " << track.GetReconPosition().T() << " , " <<  track.GetBackPosition().T() << endl;
	  //<< " , Position X-Y-Z = " << track.GetReconPosition().X() << " , " << track.GetReconPosition().Y() << " , " << track.GetReconPosition().Z() << " , Time = " << track.GetReconPosition().T()
	    
	  //   << " , Target = " << sel_true_vertex.getNucleonPdg() << endl;
	}
	*/
	// NuMu Selection so looking for negative tracks
	if( track.GetReconCharge() > 0 ) continue;

	// Primary track must be muon-like
	if( fabs(track.GetMuonPull()) > MuonPull ) continue;
	
	// Most energetic track
	if( track.GetReconMomentum() > tempmom ){
	  tempmom = track.GetReconMomentum();
	  detid = k;
	}
      }
    }

    if( detid < 0 ) continue;
    
    
    /*
    // Look for single track vertices
    
    for(int kk=0; kk<SingleVertices.size(); kk++){
      TVector3 temp = SingleVertices.at(kk);
      TrackParticle sel_track;
      int id = -1;
      cout << "Vertex X-Y-Z = " << temp.X() << " , " << temp.Y() << " , " << temp.Z() << endl;
      for(int k=0; k<DetectorEvent.size(); k++){
	TrackParticle track = DetectorEvent.at(k);

	double track_time = track.GetReconPosition().T();
	TVector3 track_pos(track.GetReconPosition().X(),
			   track.GetReconPosition().Y(),
			   track.GetReconPosition().Z() );

	cout << "Track X-Y-Z = " << track_pos.X() << " , " << track_pos.Y() << " , " << track_pos.Z() << endl;

	TVector3 track_bpos(track.GetBackPosition().X(),
			    track.GetBackPosition().Y(),
			    track.GetBackPosition().Z() );

	cout << "Track Back X-Y-Z = " << track_bpos.X() << " , " << track_bpos.Y() << " , " << track_bpos.Z() << endl;

	EcalTrackParticle ecaltrack = EcalDetectorEvent.at(k);
	double ecal_track_time = ecaltrack.GetReconPosition().T();
	TVector3 ecal_track_pos(ecaltrack.GetReconPosition().X(),
				ecaltrack.GetReconPosition().Y(),
				ecaltrack.GetReconPosition().Z() );

	int necal = AnalysisUtils::OtherTracksFromEcal(EcalDetectorEvent, ecal_track_pos, ecal_track_time);
	cout << "NEcals = " << AnalysisUtils::OtherTracksFromEcal(EcalDetectorEvent, ecal_track_pos, ecal_track_time) << endl;

	if( (track_pos - temp).Mag() == 0.0){
	  sel_track = track;
	  id = k;
	  break;
	}
      }
    }
    */
    
      /*
      NeutrinoEvent sel_true_vertex = NeutrinoEventV.at(id);
      GeantParticle sel_true_track  = GeantTrack.at(id);
      GeantParticle sel_true_parent = GeantParentTrack.at(id);
    
      TrackParticle partrack;
      int ids = -1;
      bool found = false;
      TVector3 tpos(sel_track.GetReconPosition().X(),
		    sel_track.GetReconPosition().Y(),
		    sel_track.GetReconPosition().Z() );

      for(int k=0; k<DetectorEvent.size(); k++){
	TrackParticle track = DetectorEvent.at(k);
	TVector3 track_pos(track.GetReconPosition().X(),
			   track.GetReconPosition().Y(),
			   track.GetReconPosition().Z() );

	if( (track_pos - temp).Mag() == 0.0) continue;

	if( (track_pos - temp).Mag() < 100.0)
	  found = true;

	TVector3 track_bpos(track.GetBackPosition().X(),
			    track.GetBackPosition().Y(),
			    track.GetBackPosition().Z() );

	if( (track_bpos - temp).Mag() < 100.0)
	  found = true;
	
	GeantParticle temp_true_track  = GeantTrack.at(k);
	GeantParticle temp_true_parent = GeantParentTrack.at(k);
	
	if( sel_true_parent.getTrackID() == temp_true_track.getTrackID() ){
	  partrack = track;
	  ids = k;
	  break;
	}
      }

      if( found ) continue;
       
      TVector3 ppos(partrack.GetBackPosition().X(),
		    partrack.GetBackPosition().Y(),
		    partrack.GetBackPosition().Z() );

      if(ids < 0)
	ppos.SetXYZ(0.0,0.0,0.0);

      double dist = (ppos - tpos).Mag();
      
      if( abs(sel_true_track.getPDG()) != 11 ){
      TVector3 vpos(sel_true_vertex.getPosition().X(), sel_true_vertex.getPosition().Y(), sel_true_vertex.getPosition().Z());
      cout << i << " Argon Track: Pdg = " << sel_true_track.getPDG() << " , parent = " << sel_true_parent.getPDG() << " Vertex in FV = " << ReconUtils::inVesselFV(vpos, 2000, 2000)
	   << " , Mom = " << sel_track.GetReconMomentum() //<< " , Muon Pull = " <<  sel_track.GetMuonPull()
	   << " , Position X-Y-Z = " << sel_track.GetBackPosition().X() << " , " << sel_track.GetBackPosition().Y() << " , " << sel_track.GetBackPosition().Z() << " , T = " << sel_track.GetBackPosition().T()
	//<< " , Position X-Y-Z = " << sel_track.GetReconPosition().X() << " , " << sel_track.GetReconPosition().Y() << " , " << sel_track.GetReconPosition().Z() << " , T = " << sel_track.GetReconPosition().T()
	   << endl;
      //cout << "Parent X-Y-Z = " << partrack.GetBackPosition().X() << " , " << partrack.GetBackPosition().Y() << " , " << partrack.GetBackPosition().Z() << " , T = " << partrack.GetBackPosition().T() << " , dist = " << dist << " , Mom = " << partrack.GetBackMomentum() << endl;
      cout << "Parent X-Y-Z = " << partrack.GetReconPosition().X() << " , " << partrack.GetReconPosition().Y() << " , " << partrack.GetReconPosition().Z() << " , T = " << partrack.GetReconPosition().T() << " , dist = " << dist << " , Mom = " << partrack.GetReconMomentum() << endl;
	// << " , Target = " << sel_true_vertex.getNucleonPdg() << endl;
      }
    }
      */

    
    TrackParticle sel_track       = DetectorEvent.at(detid);
    NeutrinoEvent sel_true_vertex = NeutrinoEventV.at(detid);
    GeantParticle sel_true_track  = GeantTrack.at(detid);
    GeantParticle sel_true_parent = GeantParentTrack.at(detid);
    
    TVector3 vpos(sel_true_vertex.getPosition().X(), sel_true_vertex.getPosition().Y(), sel_true_vertex.getPosition().Z());
    cout << ntracks << " Argon Track: Pdg = " << sel_true_track.getPDG() << " Vertex in FV = " << ReconUtils::inVesselFV(vpos, 2000, 2000)
	 << " , Mom = " << sel_track.GetReconMomentum() << " , Muon Pull = " <<  sel_track.GetMuonPull() << " , angle = " << sel_track.GetReconCostheta()
      //     << " , Position X-Y-Z = " << track.GetReconPosition().X() << " , " << track.GetReconPosition().Y() << " , " << track.GetReconPosition().Z()
	 << " , Target = " << sel_true_vertex.getNucleonPdg() << endl;
    

  } // event loop

}

void ReadDatacard(std::string datacard){

  LOG("NuMuCCAnalysis",pNOTICE) << "Reading configuration from XML file [" << datacard << "]";

  TXMLEngine* xml = new TXMLEngine(); 
  XMLDocPointer_t xmldoc = xml->ParseFile(datacard.c_str());
  if (xmldoc==0) {
    std::cerr << "Exp >> " << "Can not parse input: " << datacard << std::endl;
    delete xml;
    return;  
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  MinTrackLength       = util::XML2Dbl(xml,root_node,"NuMuAnalysis/MinTrackLength");
  MuonPull             = util::XML2Dbl(xml,root_node,"NuMuAnalysis/MuonPull");
  ElecPull             = util::XML2Dbl(xml,root_node,"NuMuAnalysis/ElecPull");
  

  // Release memory
  xml->FreeDoc(xmldoc);
  delete xml;
}
