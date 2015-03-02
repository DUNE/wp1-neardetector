//____________________________________________________________________________
/*!

\class    showTracks

\brief    Program to draw hits from a specified event from the output file

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  May 2014
\last update June 2014

*/
//____________________________________________________________________________
#include <iostream>
#include <map>

#include <TApplication.h>
#include <TCanvas.h>
#include "TString.h"
#include "TSystem.h"
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TLine.h>
#include <TH3F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TRandom3.h>
#include <TGeoVolume.h>

#include "PiZeroFinder.hh"
#include "LbnoPlotter.hh"
#include "GeometryLoader.hh"
#include "ProgressBar.hh"
#include "Tracker.hh"

class showTracks : public LbnoPlotter {
  public:
	showTracks();
	~showTracks();

	void initHistos();
	int show();
	void drawLines(TCanvas *canvas,int view);
	void plot(bool writeToPng=false);
	void write(TCanvas *canvas);
	
	void initDataCards();
	void loadDataCards();

  private:
	TFile file_;

	TCanvas* scintCanvas;
	TCanvas* piZeroScintCanvas;
	TCanvas* tpcCanvas;
	TCanvas* allCanvas;

	TH2F scint_tracks_XY_h2d;
	TH2F scint_tracks_YZ_h2d;
	TH2F scint_tracks_XZ_h2d;

	TH2F piZero_scint_tracks_XY_h2d;
	TH2F piZero_scint_tracks_YZ_h2d;
	TH2F piZero_scint_tracks_XZ_h2d;

	TH2F tpc_tracks_XY_h2d;
	TH2F tpc_tracks_YZ_h2d;
	TH2F tpc_tracks_XZ_h2d;

	TH2F all_tracks_XY_h2d;
	TH2F all_tracks_YZ_h2d;
	TH2F all_tracks_XZ_h2d;

	TLine *line1;
	TLine *line2;
	TLine *line3;
	TLine *line4;

	std::string dataFilename,outputFilename,saveDir;
	int eventNumber,binning;

  	std::string geomFileName_;
  	double magField_;
	double scintResolution_;

	int scintTrackCountLimit;
};

showTracks::showTracks() : LbnoPlotter(){
}

showTracks::~showTracks(){
}

void showTracks::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardString("dataFileName", "nuData.root");
  cards_->AddDataCardString("plotFileName", "tracksHistos.root");
  cards_->AddDataCardString("saveDir", "./");
  cards_->AddDataCardDouble("scintResolution", 10.*CLHEP::mm);
  cards_->AddDataCardInt("event", 1);
  cards_->AddDataCardInt("binning", 100);
  cards_->AddDataCardInt("scintTrackCountLimit",-1);//-1 shows all tracks
}

void showTracks::loadDataCards(){
  dataFilename = cards_->fetchValueString("dataFileName");
  outputFilename = cards_->fetchValueString("plotFileName");
  saveDir = cards_->fetchValueString("saveDir");
  scintResolution_ = cards_->fetchValueDouble("scintResolution");
  eventNumber = cards_->fetchValueInt("event");
  binning = cards_->fetchValueInt("binning");
  scintTrackCountLimit = cards_->fetchValueInt("scintTrackCountLimit");
}

void showTracks::initHistos(){

  scintCanvas = new TCanvas("scintCanvas","scintCanvas",1600,500);
  piZeroScintCanvas = new TCanvas("piZeroScintCanvas","piZeroScintCanvas",1600,500);
  tpcCanvas = new TCanvas("tpcCanvas","tpcCanvas",1600,500);
  allCanvas = new TCanvas("allCanvas","allCanvas",1600,500);

  double tpcXDim = 1000.;
  double tpcYDim = 1000.;
  double tpcZDim = 1000.;

  line1 = new TLine(-tpcXDim,-tpcYDim,tpcXDim,-tpcYDim);
  line1->SetLineWidth(2);
  line1->SetLineStyle(2);
  line1->SetLineColor(kBlack);
  line2 = new TLine(-tpcXDim,tpcYDim,tpcXDim,tpcYDim);
  line2->SetLineWidth(2);
  line2->SetLineStyle(2);
  line2->SetLineColor(kBlack);
  line3 = new TLine(-tpcXDim,-tpcYDim,-tpcXDim,tpcYDim);
  line3->SetLineWidth(2);
  line3->SetLineStyle(2);
  line3->SetLineColor(kBlack);
  line4 = new TLine(tpcXDim,-tpcYDim,tpcXDim,tpcYDim);
  line4->SetLineWidth(2);
  line4->SetLineStyle(2);
  line4->SetLineColor(kBlack);

  scintCanvas->Divide(3,1);
  piZeroScintCanvas->Divide(3,1);
  tpcCanvas->Divide(3,1);
  allCanvas->Divide(3,1);

  piZero_scint_tracks_XY_h2d.SetNameTitle("piZero_scint_tracks_XY_h2d","piZero_scint_tracks_XY_h2d");
  piZero_scint_tracks_XY_h2d.GetXaxis()->SetTitle("X [mm]");
  piZero_scint_tracks_XY_h2d.GetXaxis()->SetTitleSize(0.05);
  piZero_scint_tracks_XY_h2d.GetYaxis()->SetTitle("Y [mm]");
  piZero_scint_tracks_XY_h2d.GetYaxis()->SetTitleSize(0.05);
  piZero_scint_tracks_YZ_h2d.SetNameTitle("piZero_scint_tracks_YZ_h2d","piZero_scint_tracks_YZ_h2d");
  piZero_scint_tracks_YZ_h2d.GetXaxis()->SetTitle("Z [mm]");
  piZero_scint_tracks_YZ_h2d.GetXaxis()->SetTitleSize(0.05);
  piZero_scint_tracks_YZ_h2d.GetYaxis()->SetTitle("Y [mm]");
  piZero_scint_tracks_YZ_h2d.GetYaxis()->SetTitleSize(0.05);
  piZero_scint_tracks_XZ_h2d.SetNameTitle("piZero_scint_tracks_XZ_h2d","piZero_scint_tracks_XZ_h2d");
  piZero_scint_tracks_XZ_h2d.GetXaxis()->SetTitle("Z [mm]");
  piZero_scint_tracks_XZ_h2d.GetXaxis()->SetTitleSize(0.05);
  piZero_scint_tracks_XZ_h2d.GetYaxis()->SetTitle("X [mm]");
  piZero_scint_tracks_XZ_h2d.GetYaxis()->SetTitleSize(0.05);

  scint_tracks_XY_h2d.SetNameTitle("scint_tracks_XY_h2d","scint_tracks_XY_h2d");
  scint_tracks_XY_h2d.GetXaxis()->SetTitle("X [mm]");
  scint_tracks_XY_h2d.GetXaxis()->SetTitleSize(0.05);
  scint_tracks_XY_h2d.GetYaxis()->SetTitle("Y [mm]");
  scint_tracks_XY_h2d.GetYaxis()->SetTitleSize(0.05);
  scint_tracks_YZ_h2d.SetNameTitle("scint_tracks_YZ_h2d","scint_tracks_YZ_h2d");
  scint_tracks_YZ_h2d.GetXaxis()->SetTitle("Z [mm]");
  scint_tracks_YZ_h2d.GetXaxis()->SetTitleSize(0.05);
  scint_tracks_YZ_h2d.GetYaxis()->SetTitle("Y [mm]");
  scint_tracks_YZ_h2d.GetYaxis()->SetTitleSize(0.05);
  scint_tracks_XZ_h2d.SetNameTitle("scint_tracks_XZ_h2d","scint_tracks_XZ_h2d");
  scint_tracks_XZ_h2d.GetXaxis()->SetTitle("Z [mm]");
  scint_tracks_XZ_h2d.GetXaxis()->SetTitleSize(0.05);
  scint_tracks_XZ_h2d.GetYaxis()->SetTitle("X [mm]");
  scint_tracks_XZ_h2d.GetYaxis()->SetTitleSize(0.05);

  tpc_tracks_XY_h2d.SetNameTitle("tpc_tracks_XY_h2d","tpc_tracks_XY_h2d");
  tpc_tracks_XY_h2d.GetXaxis()->SetTitle("X [mm]");
  tpc_tracks_XY_h2d.GetXaxis()->SetTitleSize(0.05);
  tpc_tracks_XY_h2d.GetYaxis()->SetTitle("Y [mm]");
  tpc_tracks_XY_h2d.GetYaxis()->SetTitleSize(0.05);
  tpc_tracks_YZ_h2d.SetNameTitle("tpc_tracks_YZ_h2d","tpc_tracks_YZ_h2d");
  tpc_tracks_YZ_h2d.GetXaxis()->SetTitle("Z [mm]");
  tpc_tracks_YZ_h2d.GetXaxis()->SetTitleSize(0.05);
  tpc_tracks_YZ_h2d.GetYaxis()->SetTitle("Y [mm]");
  tpc_tracks_YZ_h2d.GetYaxis()->SetTitleSize(0.05);
  tpc_tracks_XZ_h2d.SetNameTitle("tpc_tracks_XZ_h2d","tpc_tracks_XZ_h2d");
  tpc_tracks_XZ_h2d.GetXaxis()->SetTitle("Z [mm]");
  tpc_tracks_XZ_h2d.GetXaxis()->SetTitleSize(0.05);
  tpc_tracks_XZ_h2d.GetYaxis()->SetTitle("X [mm]");
  tpc_tracks_XZ_h2d.GetYaxis()->SetTitleSize(0.05);

  all_tracks_XY_h2d.SetNameTitle("all_tracks_XY_h2d","all_tracks_XY_h2d");
  all_tracks_XY_h2d.GetXaxis()->SetTitle("X [mm]");
  all_tracks_XY_h2d.GetXaxis()->SetTitleSize(0.05);
  all_tracks_XY_h2d.GetYaxis()->SetTitle("Y [mm]");
  all_tracks_XY_h2d.GetYaxis()->SetTitleSize(0.05);
  all_tracks_YZ_h2d.SetNameTitle("all_tracks_YZ_h2d","all_tracks_YZ_h2d");
  all_tracks_YZ_h2d.GetXaxis()->SetTitle("Z [mm]");
  all_tracks_YZ_h2d.GetXaxis()->SetTitleSize(0.05);
  all_tracks_YZ_h2d.GetYaxis()->SetTitle("Y [mm]");
  all_tracks_YZ_h2d.GetYaxis()->SetTitleSize(0.05);
  all_tracks_XZ_h2d.SetNameTitle("all_tracks_XZ_h2d","all_tracks_XZ_h2d");
  all_tracks_XZ_h2d.GetXaxis()->SetTitle("Z [mm]");
  all_tracks_XZ_h2d.GetXaxis()->SetTitleSize(0.05);
  all_tracks_XZ_h2d.GetYaxis()->SetTitle("X [mm]");
  all_tracks_XZ_h2d.GetYaxis()->SetTitleSize(0.05);

  double length = 2500.0; // in mm - the vessel is maximum 5 X 5 m in length centered on origin
  piZero_scint_tracks_XY_h2d.SetBins(binning,-length,length,binning,-length,length);
  piZero_scint_tracks_YZ_h2d.SetBins(binning,-length,length,binning,-length,length);
  piZero_scint_tracks_XZ_h2d.SetBins(binning,-length,length,binning,-length,length);

  scint_tracks_XY_h2d.SetBins(binning,-length,length,binning,-length,length);
  scint_tracks_YZ_h2d.SetBins(binning,-length,length,binning,-length,length);
  scint_tracks_XZ_h2d.SetBins(binning,-length,length,binning,-length,length);

  tpc_tracks_XY_h2d.SetBins(binning,-length,length,binning,-length,length);
  tpc_tracks_YZ_h2d.SetBins(binning,-length,length,binning,-length,length);
  tpc_tracks_XZ_h2d.SetBins(binning,-length,length,binning,-length,length);

  all_tracks_XY_h2d.SetBins(binning,-length,length,binning,-length,length);
  all_tracks_YZ_h2d.SetBins(binning,-length,length,binning,-length,length);
  all_tracks_XZ_h2d.SetBins(binning,-length,length,binning,-length,length);

}

int showTracks::show(){

    //load the data and stepping files
   TFile datafile(dataFilename.c_str());
    
   if(!setupTrees(datafile))return 0;

   if(detectorHitsTree){
        this->setBackTracer(detectorHitsTree);
        
	if(eventNumber>=detHits_nEntries){
		std::cout << "\nNeutrino event does not exist, exiting..." << std::endl;
		return 0;
	}

	detectorHitsTree->GetEntry(eventNumber);
	if(nuEventTree){
		nuEventTree->GetEntry(backTracer_[1]);

		//node name
		std::string nodeName = nuEvent_->getNodeName();
		//only consider events in the TPC
		if(nodeName.compare( 0,6, "tpcFid" ) !=0){
		  std::cout << "\nNeutrino event not in the TPC, exiting..." << std::endl;
		  return 0;
		}
	}
	else{
		  std::cout << "\nCannot find neutrino event tree, exiting..." << std::endl;
		  return 0;
	}

	//loop over hits
	HitCollection tpcSdHits = simData_->getTpcFidHits();
	ScintHitCollection scintSdHits = simData_->getScintHits();
	//size of the hit vectors per event
        int tpcHitCount = tpcSdHits.size();
        int scintHitCount = scintSdHits.size();

	//primaries from neutrino interaction
	ParticleDescrShortRecord fspl = nuEvent_->getFspl();
	std::vector<ParticleDescrShortRecord> fssVct = nuEvent_->getFssVector();
	TLorentzVector eventVertex = nuEvent_->getPosition();

	///output event details
	nuEvent_->printToStream(std::cout);
	simData_->printToStream(std::cout);

	//make a tracker object to handle tracks instead of hits
	Tracker tracker(scintResolution_);

	/// --- TPC HITS ---- ///
	//create a vector of vectors for track information for each track
	std::vector<HitCollection> tpcTrackVector;
	tpcTrackVector.resize(0);				//clear the track info from previous event
	int tpcTrackHitCount = 0;

	//convert hits into vector of tracks to make it easier to reconstruct each one
	tpcTrackVector = tracker.MakeFromHits(tpcSdHits,true);

	int totalTpcHitCount = 0;
	if(tpcHitCount>0){

	  //read from the scint track vector - loop over each track
	  for(tpcTracks::iterator it = tpcTrackVector.begin(); it != tpcTrackVector.end();++it){

		HitCollection tempVtr= *it;
		totalTpcHitCount += tempVtr.size();

		//loop over hits on each track
		for(int k=0;k<tempVtr.size();k++){
	
		  //get the energy deposition
		  double tpcHitEDep = tempVtr.at(k).getEdep()/CLHEP::keV;
		  int tpcHitPDG = tempVtr.at(k).getPDG();
		  int tpcHitParentID = tempVtr.at(k).getParentID();
	
		  //get the position of the hit
		  TLorentzVector tpcHitPosition = tempVtr.at(k).getPosition();
		  //std::cout << "\n tpcHitPos (" << tpcHitPosition.X() << "," << tpcHitPosition.Y() << "," << tpcHitPosition.Z() << ") mm";

		  tpc_tracks_XY_h2d.Fill(tpcHitPosition.X(),tpcHitPosition.Y(),tpcHitEDep);
		  tpc_tracks_YZ_h2d.Fill(tpcHitPosition.Z(),tpcHitPosition.Y(),tpcHitEDep);
		  tpc_tracks_XZ_h2d.Fill(tpcHitPosition.Z(),tpcHitPosition.X(),tpcHitEDep);

		  all_tracks_XY_h2d.Fill(tpcHitPosition.X(),tpcHitPosition.Y(),tpcHitEDep);
		  all_tracks_YZ_h2d.Fill(tpcHitPosition.Z(),tpcHitPosition.Y(),tpcHitEDep);
		  all_tracks_XZ_h2d.Fill(tpcHitPosition.Z(),tpcHitPosition.X(),tpcHitEDep);
              
		}//end loop over hits
	  }//end loop over tracks

	}

	std::cout << "\nNumber of tracks in tpc: " << tpcTrackVector.size() << ", total hits are then: " << totalTpcHitCount;

	/// --- Scint HITS ---- ///
        //create a vector of vectors for track information for each track
        std::vector<ScintHitCollection> scintTrackVector;
        scintTrackVector.resize(0);                               //clear the track info from previous event
        int scintTrackHitCount = 0;

	//convert hits into vector of tracks to make it easier to reconstruct each one
	scintTrackVector = tracker.MakeFromHits(scintSdHits);
	
  	ScintHitCollection * unmatchedScintHits = new ScintHitCollection();
	PiZeroFinder * piZeroMgr = new PiZeroFinder(unmatchedScintHits);

	int totalScintHitCount = 0;
	int trackCounter = 0;

        if(scintHitCount>0){

	  //read from the scint track vector - loop over each track
	  for(std::vector< ScintHitCollection >::iterator it = scintTrackVector.begin();
			it != scintTrackVector.end();++it){

		//how many tracks to show?
		if(trackCounter>=scintTrackCountLimit && scintTrackCountLimit!=-1)break;
		trackCounter++;

		ScintHitCollection tempVtr= *it;
		totalScintHitCount += tempVtr.size();

		//check if the track exists in the TPC
		int tpcTrackVtrIndex = tracker.MatchScintToTpcTracks(tempVtr,tpcTrackVector);

		//loop over hits on each track
		for(int k=0;k<tempVtr.size();k++){
	
		  //get the energy deposition
		  double scintHitEDep = tempVtr.at(k).getEdep();
		  int scintHitPDG = tempVtr.at(k).getPDG();
		  int scintHitParentID = tempVtr.at(k).getParentID();
		  int scintHitTrackID = tempVtr.at(k).getTrackID();
		  
		  //std::cout << "\nscintTrack: " <<trackCounter<< ", scintHitPDG: " << scintHitPDG << ", scintHitTrackID: " << scintHitTrackID << ", scintHitParentID: " << scintHitParentID;

                  //get the position of the hit
                  TLorentzVector scintHitPosition = tempVtr.at(k).getPosition();
		  //std::cout << "\n scintHitPos (" << scintHitPosition.X() << "," << scintHitPosition.Y() << "," << scintHitPosition.Z() << ") mm"
		//		<<", eDep: " << scintHitEDep;

                  scint_tracks_XY_h2d.Fill(scintHitPosition.X(),scintHitPosition.Y(),scintHitEDep);
                  scint_tracks_YZ_h2d.Fill(scintHitPosition.Z(),scintHitPosition.Y(),scintHitEDep);
                  scint_tracks_XZ_h2d.Fill(scintHitPosition.Z(),scintHitPosition.X(),scintHitEDep);
           
                  all_tracks_XY_h2d.Fill(scintHitPosition.X(),scintHitPosition.Y(),scintHitEDep);
                  all_tracks_YZ_h2d.Fill(scintHitPosition.Z(),scintHitPosition.Y(),scintHitEDep);
                  all_tracks_XZ_h2d.Fill(scintHitPosition.Z(),scintHitPosition.X(),scintHitEDep);

		}//end loop over hits
	  }//end loop over tracks

	  /// --- PiZero HITS ---- ///
          std::vector<ScintHitCollection> piZeroTrackVector;
          piZeroTrackVector.resize(0);                               //clear the track info from previous event

	  std::vector<GeantPrimaryParticle> * prims = trackingRecord_->getPrimariesPtr();

	  //convert hits into vector of tracks to make it easier to reconstruct each one
	  piZeroTrackVector = tracker.PiZeroPhotonTracks(scintSdHits,prims); //tracker.PiZeroTracks(scintSdHits,prims);

	  trackCounter = 0;
	  int totalPiZeroHitCount = 0;

	  double photonEnergyOne = 0.;
	  double photonEnergyTwo = 0.;

	  double totalPiZeroEDep = 0.;
	
	  //read from the scint track vector - loop over each track
	  for(std::vector< ScintHitCollection >::iterator it = piZeroTrackVector.begin();
			it != piZeroTrackVector.end();++it){

		//how many tracks to show?
		//if(trackCounter>=scintTrackCountLimit && scintTrackCountLimit!=-1)break;
		trackCounter++;

		ScintHitCollection tempVtr= *it;
		totalPiZeroHitCount += tempVtr.size();

		//loop over hits on each track
		for(int k=0;k<tempVtr.size();k++){
	
		  //get the energy deposition
		  double scintHitEDep = tempVtr.at(k).getEdep();
		  int scintHitPDG = tempVtr.at(k).getPDG();
		  int scintHitParentID = tempVtr.at(k).getParentID();
		  int scintHitTrackID = tempVtr.at(k).getTrackID();
		  
		  //std::cout << "\nscintTrack: " <<trackCounter<< ", scintHitPDG: " << scintHitPDG << ", scintHitTrackID: " << scintHitTrackID << ", scintHitParentID: " << scintHitParentID;

                  //get the position of the hit
                  TLorentzVector scintHitPosition = tempVtr.at(k).getPosition();

		  totalPiZeroEDep +=scintHitEDep;

                  piZero_scint_tracks_XY_h2d.Fill(scintHitPosition.X(),scintHitPosition.Y(),scintHitEDep);
                  piZero_scint_tracks_YZ_h2d.Fill(scintHitPosition.Z(),scintHitPosition.Y(),scintHitEDep);
                  piZero_scint_tracks_XZ_h2d.Fill(scintHitPosition.Z(),scintHitPosition.X(),scintHitEDep);

		}//end loop over hits
	  }//end loop over tracks

	  for(int k =0;k<piZeroMgr->GetNumberOfPiZerosInEvent(prims);k++){
	  	//check the invariant mass
		std::pair<TLorentzVector,TLorentzVector> photonPairKinemat = piZeroMgr->GetPhotonPairKinematics(k+1,prims,piZeroTrackVector,eventVertex);
		double mass = piZeroMgr->ReconstructPiZeroInvariantMass(photonPairKinemat);
		double piZeroEnergy = piZeroMgr->ReconstructPiZeroEnergy(photonPairKinemat);
		std::cout << "\n------------------------------------------------------------------------------------------------"
			  << "\nFound Pi0 with Invariant Mass [GeV]: " << mass << ", pi0 energy [GeV]: " << piZeroEnergy
			  << "\n------------------------------------------------------------------------------------------------";
	  }

	  std::cout << "\nNumber of pi0 tracks in scintillator: " << piZeroTrackVector.size() << ", showing: " 
			<< scintTrackCountLimit << " track(s) with total hits: " << totalScintHitCount << " , piZero hits: " << totalPiZeroHitCount 
			<< ", TotEDep = " << totalPiZeroEDep << " GeV";
	}
	
	delete unmatchedScintHits;
	delete piZeroMgr;

    }

    this->plot(true);

    std::cout << "\n==================================="
		<< "\n Press Ctrl-C to quit" 
		 << "\n===================================" << std::endl;

    return 1;
}

void showTracks::plot(bool writeToPng){

    piZeroScintCanvas->cd(1);
    piZero_scint_tracks_XY_h2d.Draw("colz");
    piZero_scint_tracks_XY_h2d.SetStats(kFALSE);
    piZero_scint_tracks_XY_h2d.SetContour(99);
    drawLines(piZeroScintCanvas,1);
    piZeroScintCanvas->cd(2);
    piZero_scint_tracks_YZ_h2d.Draw("colz");
    piZero_scint_tracks_YZ_h2d.SetStats(kFALSE);
    piZero_scint_tracks_YZ_h2d.SetContour(99);
    drawLines(piZeroScintCanvas,2);
    piZeroScintCanvas->cd(3);
    piZero_scint_tracks_XZ_h2d.Draw("colz");
    piZero_scint_tracks_XZ_h2d.SetStats(kFALSE);
    piZero_scint_tracks_XZ_h2d.SetContour(99);
    drawLines(piZeroScintCanvas,3);
    if(writeToPng)write(piZeroScintCanvas);

    scintCanvas->cd(1);
    scint_tracks_XY_h2d.Draw("colz");
    scint_tracks_XY_h2d.SetStats(kFALSE);
    scint_tracks_XY_h2d.SetContour(99);
    drawLines(scintCanvas,1);
    scintCanvas->cd(2);
    scint_tracks_YZ_h2d.Draw("colz");
    scint_tracks_YZ_h2d.SetStats(kFALSE);
    scint_tracks_YZ_h2d.SetContour(99);
    drawLines(scintCanvas,2);
    scintCanvas->cd(3);
    scint_tracks_XZ_h2d.Draw("colz");
    scint_tracks_XZ_h2d.SetStats(kFALSE);
    scint_tracks_XZ_h2d.SetContour(99);
    drawLines(scintCanvas,3);
    if(writeToPng)write(scintCanvas);

    tpcCanvas->cd(1);
    tpc_tracks_XY_h2d.Draw("colz");
    tpc_tracks_XY_h2d.SetStats(kFALSE);
    tpc_tracks_XY_h2d.SetContour(99);
    drawLines(tpcCanvas,1);
    tpcCanvas->cd(2);
    tpc_tracks_YZ_h2d.Draw("colz");
    tpc_tracks_YZ_h2d.SetStats(kFALSE);
    tpc_tracks_YZ_h2d.SetContour(99);
    drawLines(tpcCanvas,2);
    tpcCanvas->cd(3);
    tpc_tracks_XZ_h2d.Draw("colz");
    tpc_tracks_XZ_h2d.SetStats(kFALSE);
    tpc_tracks_XZ_h2d.SetContour(99);
    drawLines(tpcCanvas,3);
    if(writeToPng)write(tpcCanvas);

    allCanvas->cd(1);
    all_tracks_XY_h2d.Draw("colz");
    all_tracks_XY_h2d.SetStats(kFALSE);
    all_tracks_XY_h2d.SetContour(99);
    drawLines(allCanvas,1);
    allCanvas->cd(2);
    all_tracks_YZ_h2d.Draw("colz");
    all_tracks_YZ_h2d.SetStats(kFALSE);
    all_tracks_YZ_h2d.SetContour(99);
    drawLines(allCanvas,2);
    allCanvas->cd(3);
    all_tracks_XZ_h2d.Draw("colz");
    all_tracks_XZ_h2d.SetStats(kFALSE);
    all_tracks_XZ_h2d.SetContour(99);
    drawLines(allCanvas,3);
    if(writeToPng)write(allCanvas);


}

void showTracks::drawLines(TCanvas *canvas,int view){
  canvas->cd(view);
  line1->Draw();
  line2->Draw();
  line3->Draw();
  line4->Draw();
}

void showTracks::write(TCanvas *canvas){

   std::stringstream ss;
   ss << eventNumber;
   TString eventAsString = ss.str();
   canvas->SaveAs(saveDir + "showtracks_" + canvas->GetName() + "_event" + eventAsString + ".png");
}

int main(int argc, char ** argv) {

  //create a stand alone appliction using root libraries - by pass the root arguments
  TApplication* rootapp = new TApplication("application",&argc, argv,0,-1);

  try {
	showTracks trackDisplay;
	trackDisplay.initDataCards();
	trackDisplay.getRunTimeArguments(argc, argv);
	trackDisplay.loadDataCards();
	trackDisplay.initHistos();
	if(trackDisplay.show() ==0)return 0;
  	rootapp->Run();
 	std::cin.get();
  	return 1;
   }
   catch (LbnoException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
   }
}
