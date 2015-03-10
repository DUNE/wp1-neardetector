//____________________________________________________________________________
/*!

\class    GasTPCPlot

\brief    Program to make basic plots

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  June 2013
\last update June 2014

*/
//____________________________________________________________________________
#include <iostream>
#include <vector>
#include <map>

#include <TTree.h>
#include<TH1.h>
#include<TH2.h>
#include<TH3F.h>
#include <TFile.h>
#include <TChain.h>

#include "GasTPCPlotter.hh"

class myPlotter : public GasTPCPlotter {
 public:
  myPlotter() {}
  virtual ~myPlotter() {}

  void plot();
  void initHistos();
  void write();

 private:
  TFile file_;

  TH1F fluxE_h1d_;
  TH1F nuEventE_h1d_;
  TH1F nuMuEventE_h1d_,nuAntiMuEventE_h1d_;
  TH1F nuEEventE_h1d_,nuAntiEEventE_h1d_;

  TH1F nuEventChargedE_CC_h1d_;
  TH1F nuEventNeutronE_CC_h1d_;
  TH1F nuEventPhotonE_CC_h1d_;
  TH1F nuEventPiZeroE_CC_h1d_;
  TH1F nuEventChargedE_NC_h1d_;
  TH1F nuEventNeutronE_NC_h1d_;
  TH1F nuEventPhotonE_NC_h1d_;
  TH1F nuEventPiZeroE_NC_h1d_;
  TH1F nuEventNeutrinoE_NC_h1d_;

  TH1I nuEventPiZero_CC_h1_;
  TH1I nuEventPiZero_NC_h1_;

  //particle kinetic energies
  TH1F allKinE_h1d_;
  TH1F protonKinE_h1d_,neutronKinE_h1d_;
  TH1F pipKinE_h1d_,pimKinE_h1d_,pi0KinE_h1d_;
  TH1F electronKinE_h1d_,positronKinE_h1d_;
  TH1F gammaKinE_h1d_;
  TH1F muonKinE_h1d_,aMuonKinE_h1d_;  

  TH1F proton_range_TPC_h1d_;
  TH1F pion_range_TPC_h1d_;
  TH1F electron_range_TPC_h1d_;

  TH1F range_h1d_;
  TH2F nuHitXY_h2d_;
  TH1F nuEventZ_h1d_;
  TH2F nuEventXY_h2d_;
  TH2F nuEventXZ_h2d_;
  TH2F rangeVE_h2d_;
  TH3F nuHits_h3d_;
  TH3F nuEvents_h3d_;
  TH3F detHits_h3d_;
  TH2F detHits_h2d_Hor;
  TH2F detHits_h2d_Ver;

  TH2F range_h2d_Hor;
  TH2F range_h2d_Ver;

  //counts
  int allParticleCount;
  int protonCount,neutronCount;
  int pipCount,pimCount,pi0Count;
  int electronCount,positronCount;
  int gammaCount;
  int muonCount, aMuonCount;

};

void myPlotter::initHistos(){

  int binning = maxEnergy_*10;
  //declare histograms
  fluxE_h1d_.SetNameTitle("nuFluxE", "nu flux energy");
  fluxE_h1d_.SetBins(binning, 0., maxEnergy_);

  nuEventE_h1d_.SetNameTitle("nuEventE", "nu event energy");
  nuEventE_h1d_.SetBins(binning, 0., maxEnergy_);

  nuMuEventE_h1d_.SetNameTitle("nuMuEventE", "nu_mu event energy");
  nuMuEventE_h1d_.SetBins(binning, 0., maxEnergy_);
  nuAntiMuEventE_h1d_.SetNameTitle("nuntiMuEventE", "anu_mu event energy");
  nuAntiMuEventE_h1d_.SetBins(binning, 0., maxEnergy_);
  nuEEventE_h1d_.SetNameTitle("nuEEventE", "nu_e event energy");
  nuEEventE_h1d_.SetBins(binning, 0., maxEnergy_);
  nuAntiEEventE_h1d_.SetNameTitle("nuAntiEEventE", "anu_e event energy");
  nuAntiEEventE_h1d_.SetBins(binning, 0., maxEnergy_);

  nuEventPiZero_CC_h1_.SetNameTitle("nuEventPiZero_CC_h1_","nuEventPiZero_CC_h1_");
  nuEventPiZero_CC_h1_.SetBins(20, 0,20);
  nuEventPiZero_NC_h1_.SetNameTitle("nuEventPiZero_NC_h1_","nuEventPiZero_NC_h1_");
  nuEventPiZero_NC_h1_.SetBins(20, 0,20);

  allKinE_h1d_.SetNameTitle("allKinEnergy", "all particles kinetic energy");
  allKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  protonKinE_h1d_.SetNameTitle("protonKinEnergy", "proton kinetic energy");
  protonKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  neutronKinE_h1d_.SetNameTitle("neutronKinEnergy", "neutron kinetic energy");
  neutronKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  pipKinE_h1d_.SetNameTitle("pipKinEnergy", "pi+ kinetic energy");
  pipKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  pimKinE_h1d_.SetNameTitle("pimKinEnergy", "pi- kinetic energy");
  pimKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  pi0KinE_h1d_.SetNameTitle("pi0KinEnergy", "pi0 kinetic energy");
  pi0KinE_h1d_.SetBins(binning, 0., maxEnergy_);
  electronKinE_h1d_.SetNameTitle("electronKinEnergy", "electron kinetic energy");
  electronKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  positronKinE_h1d_.SetNameTitle("positronKinEnergy", "positron kinetic energy");
  positronKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  muonKinE_h1d_.SetNameTitle("muonKinEnergy", "muon kinetic energy");
  muonKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  aMuonKinE_h1d_.SetNameTitle("aMuonKinEnergy", "anti muon kinetic energy");
  aMuonKinE_h1d_.SetBins(binning, 0., maxEnergy_);
  gammaKinE_h1d_.SetNameTitle("gammaKinEnergy", "gamma kinetic energy");
  gammaKinE_h1d_.SetBins(binning, 0., maxEnergy_);

  nuEventChargedE_CC_h1d_.SetNameTitle("nuEventChargedE_CC", "nu event charged particle energy");
  nuEventChargedE_CC_h1d_.SetBins(100, 0., 1.);

  nuEventNeutronE_CC_h1d_.SetNameTitle("nuEventNeutronE_CC", "nu event neutron energy");
  nuEventNeutronE_CC_h1d_.SetBins(100, 0., 1.);

  nuEventPhotonE_CC_h1d_.SetNameTitle("nuEventPhotonE_CC", "nu event photon energy");
  nuEventPhotonE_CC_h1d_.SetBins(100, 0., 1.);

  nuEventChargedE_NC_h1d_.SetNameTitle("nuEventChargedE_NC", "nu event charged particle energy");
  nuEventChargedE_NC_h1d_.SetBins(100, 0., 1.);

  nuEventNeutronE_NC_h1d_.SetNameTitle("nuEventNeutronE_NC", "nu event neutron energy");
  nuEventNeutronE_NC_h1d_.SetBins(100, 0., 1.);

  nuEventPhotonE_NC_h1d_.SetNameTitle("nuEventPhotonE_NC", "nu event photon energy");
  nuEventPhotonE_NC_h1d_.SetBins(100, 0.,1.);

  nuEventNeutrinoE_NC_h1d_.SetNameTitle("nuEventNeutrinoE_NC", "nu event neutrino energy");
  nuEventNeutrinoE_NC_h1d_.SetBins(100, 0.,1.);

  range_h1d_.SetNameTitle("muRange", "mu range");
  range_h1d_.SetBins(300, 0., 10.);

  nuHitXY_h2d_.SetNameTitle("nuHitXY", "nu Hit XY pos");
  nuHitXY_h2d_.SetBins(400, -200., 200.,400, -200., 200.);

  nuEventZ_h1d_.SetNameTitle("nuEventZ", "nu Event Z pos");
  nuEventZ_h1d_.SetBins(400, -200., 200.);

  nuEventXY_h2d_.SetNameTitle("nuEventXY", "nu Event XY pos");
  nuEventXY_h2d_.SetBins(400, -200., 200.,400, -200., 200.);

  nuEventXZ_h2d_.SetNameTitle("nuEventXZ", "nu Event XZ pos");
  nuEventXZ_h2d_.SetBins(400, -200., 200.,600, -300., 300.);

  rangeVE_h2d_.SetNameTitle("muRangeE", "mu range vs. mu energy");
  rangeVE_h2d_.SetBins(400, 0., 20., 400, 0., 20.);

  nuHits_h3d_.SetNameTitle("nuHits", "nu hits");
  nuHits_h3d_.SetBins(400, -1., 39., 400, -20., 20., 400, -20., 20.);

  //nuEvents_h1d_.SetNameTitle("nuEvents", "nu events");

  nuEvents_h3d_.SetNameTitle("nuEvents", "nu events");
  nuEvents_h3d_.SetBins(400, -1., 39., 400, -20., 20., 400, -20., 20.);

  detHits_h3d_.SetNameTitle("detHits", "detector hits");
  detHits_h3d_.SetBins(600, -3., 3., 600, -3., 3., 600, -3., 3.);

  detHits_h2d_Hor.SetNameTitle("detHits_h", "detector hits");
  detHits_h2d_Hor.SetBins(600, -3., 3., 600, -3., 3.);

  detHits_h2d_Ver.SetNameTitle("detHits_v", "detector hits");
  detHits_h2d_Ver.SetBins(600, -3., 3., 600, -3., 3.);
 
  range_h2d_Hor.SetNameTitle("range_h", "range");
  range_h2d_Hor.SetBins(700, -25., 45., 600, -30., 30.);

  range_h2d_Ver.SetNameTitle("range_v", "range");
  range_h2d_Ver.SetBins(800, -30., 50., 600, -30., 30.);


}

void myPlotter::plot() {

  //nodes
  int tpcFidCount = 0;
  int notActiveCount = 0;
  int vesselCount = 0;
  int innerVesselCount = 0;
  int scintCount = 0;
  int magnetCount = 0;
  int cavityCount = 0;
  int mindCount = 0;
  int rockCount = 0;
  int otherCount = 0;

  electronCount = 0;
  positronCount = 0;
  muonCount = 0;
  aMuonCount = 0;
  gammaCount = 0;
  pipCount = 0;
  pimCount = 0;
  pi0Count = 0;
  protonCount = 0;
  neutronCount = 0;
 
  //exposure
  double exposure = 0;
  
  //***loop over runs***
  //define number of runs
  int runs = runs_;

  std::string filename = inputFileName_;
  std::stringstream ss;

  int totalNuEvents = 0;

  for(int j = 0;j<runs;j++){

    ss << j;

    std::string filename_run = filename + ss.str() + ".root";
    std::cout << "\n************************************************************************************************"
		<<"\nFilename being read: " << filename_run
		<< "\n************************************************************************************************";
    TFile datafile(filename_run.c_str());

    //clear stringstream 
    ss.str("");

    //set data addresses
    if(!setupTrees(datafile)) return;
    
    double phy = TMath::Pi()/180.;
    
    int i=0;
    /*    
    std::cout << "\n----------------------------------"
	      << "\n    Looping over entries...   "
	      << "\n----------------------------------\n";
    */
    if(nuHitTree){      
      //loop over nuHit tree
      while(i<nuHits_nEntries) {
    	nuHitTree->GetEntry(i);
	
    	double nuE = nuHit_->getP4().E() * (1./CLHEP::GeV);
    	fluxE_h1d_.Fill(nuE);
	
	TLorentzVector nuHitPos = nuHit_->getPosition() * (1./ CLHEP::cm);
	nuHitXY_h2d_.Fill(nuHitPos.X(),nuHitPos.Y());
	//if(nuHitPos.X() ==0 && nuHitPos.Y() ==0 && nuHitPos.Z()==0)count++;
	//std::cout << "\nEnergy " << nuE << " GeV "<<std::endl;
    	i++;
      }
      
    }

    i=0;
    
    if(nuEventTree){
      this->setBackTracer(nuEventTree);

      bool cc;
      
      //loop over nuEvent tree
      while(i<nuEvents_nEntries) {
	
    	nuEventTree->GetEntry(i);
	if(nuHitTree)nuHitTree->GetEntry(backTracer_[0]);
	
    	i++;

	//node names
	std::string nodeName = nuEvent_->getNodeName();
	//volume counts
	if(nodeName.compare( 0,6, "tpcFid" ) ==0)tpcFidCount++;
  	else if(nodeName.compare( 0,7,"cathode") ==0)notActiveCount++;
  	else if(nodeName.compare( 0,5,"anode") ==0)notActiveCount++;
	else if(nodeName.compare( 0,11, "innerVessel" ) ==0)innerVesselCount++;
	else if(nodeName.compare( 0,6, "vessel" ) ==0)vesselCount++;
	else if(nodeName.compare( 0,5, "scint" ) ==0)scintCount++;
	else if(nodeName.compare( 0,6, "mother" ) ==0)innerVesselCount++;
	else if(nodeName.compare( 0,6, "magnet" ) ==0)magnetCount++;
	else if(nodeName.compare( 0,6, "cavity" ) ==0)cavityCount++;
	else if(nodeName.compare( 0,4, "mind" ) ==0)mindCount++;
	else if(nodeName.compare( 0,4, "rock" ) ==0)rockCount++;
  	else otherCount++;

	//consider events only in specifed volume
	if(nodeName.compare( 0,selectedVolume_.size(), selectedVolume_ ) !=0 && selectedVolume_ != "all")continue;	

	//---------------------Neutrino---------------------------//
        double nuE = nuEvent_->getNuEnergy() * (1./CLHEP::GeV);
    	int nuPDG;
	if(nuHit_)nuPDG = nuHit_->getPDG();
	else nuPDG =0;

    	nuEventE_h1d_.Fill(nuE);
	if(nuPDG==14)nuMuEventE_h1d_.Fill(nuE);
	if(nuPDG==-14)nuAntiMuEventE_h1d_.Fill(nuE);
	if(nuPDG==12)nuEEventE_h1d_.Fill(nuE);
	if(nuPDG==-12)nuAntiEEventE_h1d_.Fill(nuE);
	
	//-----------Final State Primary Lepton-------------------//
   	int nuEvFsplPDG = nuEvent_->getFspl().getPDG();
    	double nuEvFsplE = nuEvent_->getFspl().getP4().E() * (1./CLHEP::GeV);
	double nuEvFsplMass = nuEvent_->getFspl().getMass() * (1./CLHEP::GeV);
    	double nuEvFsplT = nuEvFsplE - nuEvFsplMass;
    	double nuEvFsplMomX = nuEvent_->getFspl().getP4().X() * (1./CLHEP::GeV);
    	double nuEvFsplMomY = nuEvent_->getFspl().getP4().Y() * (1./CLHEP::GeV);
    	double nuEvFsplMomZ = nuEvent_->getFspl().getP4().Z() * (1./CLHEP::GeV);
	double nuEvFsplMom = TMath::Power( (nuEvFsplMomX*nuEvFsplMomX 
					    + nuEvFsplMomY*nuEvFsplMomY + nuEvFsplMomZ*nuEvFsplMomZ) ,0.5);
	
    	int nuEvNuclPDG = nuEvent_->getHitNucleon().getPDG();
    	double nuEvNuclE = nuEvent_->getHitNucleon().getP4().E() * (1./CLHEP::GeV);
	
	if (nuEvFsplPDG==13)muonKinE_h1d_.Fill(nuEvFsplT);
	else if (nuEvFsplPDG==-13)aMuonKinE_h1d_.Fill(nuEvFsplT);
	else if (nuEvFsplPDG==11)electronKinE_h1d_.Fill(nuEvFsplT);
	else if (nuEvFsplPDG==-11)positronKinE_h1d_.Fill(nuEvFsplT);
	
	if (nuEvFsplPDG==13 || nuEvFsplPDG==-13 
	    || nuEvFsplPDG==11 || nuEvFsplPDG==-11 
	    || nuEvFsplPDG==15 || nuEvFsplPDG==-15){
		cc = true;
		allKinE_h1d_.Fill(nuEvFsplT);
	}
	else cc = false;
		
	//-----------Final State Secondaries-------------------//
    	double nuEvFssChargedE_CC = 0.0;
    	double nuEvFssChargedE_NC = 0.0;
    	double nuEvFssNeutronE_CC = 0.0;
    	double nuEvFssNeutronE_NC = 0.0;
    	double nuEvFssGammaE_CC = 0.0;
    	double nuEvFssGammaE_NC = 0.0;
    	double nuEvNeutrinoE_NC = 0.0;
	
    	double nuEvFinalNuE = 0.0;
	
	double totKinEnergy = nuEvFsplT;
	
	std::vector<ParticleDescrShortRecord> vector = nuEvent_->getFssVector();
	
	int PiZeroCount = 0;

	//loop over secondaries
    	for(int index=0;index<vector.size(); index++){
	  
      	  int nuEvFssPDG = vector.at(index).getPDG();
	  double nuEvFssE = vector.at(index).getP4().E() * (1./CLHEP::GeV);
	  double nuEvFssMass = vector.at(index).getMass() * (1./CLHEP::GeV);
	  double nuEvFssT = nuEvFssE - nuEvFssMass;
    	  double nuEvFssMomX = vector.at(index).getP4().X() * (1./CLHEP::GeV);
    	  double nuEvFssMomY = vector.at(index).getP4().Y() * (1./CLHEP::GeV);
    	  double nuEvFssMomZ = vector.at(index).getP4().Z() * (1./CLHEP::GeV);
	  double nuEvFssMom = TMath::Power( ( (nuEvFssMomX*nuEvFssMomX) 
					      + (nuEvFssMomY*nuEvFssMomY) + (nuEvFssMomZ*nuEvFssMomZ) ) ,0.5);	  
	  //put cut of 1 MeV on secondaries
	  //if(nuEvFssT<0.01)continue;

	  allKinE_h1d_.Fill(nuEvFssT);

	  if(nuEvFssPDG == 11)electronKinE_h1d_.Fill(nuEvFssT);
	  else if(nuEvFssPDG == -11)positronKinE_h1d_.Fill(nuEvFssT);
	  else if(nuEvFssPDG == 13)muonKinE_h1d_.Fill(nuEvFssT);
	  else if(nuEvFssPDG == -13)aMuonKinE_h1d_.Fill(nuEvFssT);
	  else if(nuEvFssPDG == 2212)protonKinE_h1d_.Fill(nuEvFssT);
	  else if(nuEvFssPDG == 2112)neutronKinE_h1d_.Fill(nuEvFssT);
	  else if(nuEvFssPDG == 211)pipKinE_h1d_.Fill(nuEvFssT);
	  else if(nuEvFssPDG == -211)pimKinE_h1d_.Fill(nuEvFssT);
	  else if(nuEvFssPDG == 111){
		pi0KinE_h1d_.Fill(nuEvFssT);
		PiZeroCount++;
	  }
	  else if(nuEvFssPDG == 22)gammaKinE_h1d_.Fill(nuEvFssT);
	  
          //CC events
          if (cc){	    
	    //add the muon energy as it is charged
	    if(index==0) nuEvFssChargedE_CC +=nuEvFsplT;
	    
	    if( (nuEvFssPDG == 13) || (nuEvFssPDG == -13) 
		|| (nuEvFssPDG == 11) || (nuEvFssPDG == -11)
		|| (nuEvFssPDG == 15) || (nuEvFssPDG == -15) 
		|| (nuEvFssPDG == 2212) 
		|| (nuEvFssPDG == 211) || (nuEvFssPDG == -211) ){
	      
	      nuEvFssChargedE_CC += nuEvFssT;
	      totKinEnergy += nuEvFssT;
	    }
	    
	    if( nuEvFssPDG == 2112 ){
	      
	      nuEvFssNeutronE_CC += nuEvFssT;
	      totKinEnergy += nuEvFssT;
	    }
	    
	    if( (nuEvFssPDG == 22) || (nuEvFssPDG == 111) ){
	      
	      nuEvFssGammaE_CC += nuEvFssT;
	      totKinEnergy += nuEvFssT;
	    }	    
          }
	  
          //NC events
          if (!cc){
	    
	    //add the muon energy as it is charged
	    if(index==0) nuEvFssChargedE_CC +=nuEvFsplT;
	    
	    if( (nuEvFssPDG == 13) || (nuEvFssPDG == -13) 
		|| (nuEvFssPDG == 11) || (nuEvFssPDG == -11)
		|| (nuEvFssPDG == 15) || (nuEvFssPDG == -15) 
		|| (nuEvFssPDG == 2212) 
		|| (nuEvFssPDG == 211) || (nuEvFssPDG == -211) ){
	      
	      nuEvFssChargedE_NC += nuEvFssT;
	      totKinEnergy += nuEvFssT;
	    }
	    
	    if( nuEvFssPDG == 2112 ){
	      nuEvFssNeutronE_NC += nuEvFssT;
	      totKinEnergy += nuEvFssT;
	    }
	    
	    if( (nuEvFssPDG == 22) || (nuEvFssPDG == 111) ){
	      nuEvFssGammaE_NC += nuEvFssT;
	      totKinEnergy += nuEvFssT;
	    }	    
          }

	  if(verbose_>1){
	    std::cout << "\nParticle: " <<nuEvFssPDG 
		      << ", Mass: "<< nuEvFssMass << " GeV/c2"
		      << ", Energy: "<< nuEvFssE << " GeV"
		      << ", KineticEnergy: "<< nuEvFssT << " GeV"
		      << ", In Node: " << nodeName.c_str();
	  }
	}
	
	//double totEnergy = nuEvFssChargedE_CC + nuEvFssNeutronE_CC + nuEvFssGammaE_CC 
	//			+ nuEvFssChargedE_NC + nuEvFssNeutronE_NC + nuEvFssGammaE_NC + nuEvFinalNuE;
	
	//add the hit nucleon energy to give the total primary energy at interaction vertex
	
	  if(verbose_>0){
	  	std::cout << "\n-----------------------------------"
		    << "\nCharged Current Interaction? = " << cc 
		    << "\nNeutrino Energy = " << nuE << " GeV"
		    << "\nNucleon Energy = " << nuEvNuclE << " GeV"
		    << "\nFspl Kinetic Energy = " << nuEvFsplT << " GeV"
		    << "\nFspl Mass = " << nuEvFsplMass << " GeV/c2";

	  	if(cc)std::cout << "\nCharged Energy = " << nuEvFssChargedE_CC << " GeV" <<std::endl;
	  	if(!cc)std::cout << "\nCharged Energy = " << nuEvFssChargedE_NC << " GeV" <<std::endl;
	  	if(cc)std::cout << "\nNeutron Energy = " << nuEvFssNeutronE_CC << " GeV" <<std::endl;
	  	if(!cc)std::cout << "\nNeutron Energy = " << nuEvFssNeutronE_NC << " GeV" <<std::endl;
	  	if(cc)std::cout << "\nGamma Energy = " << nuEvFssGammaE_CC << " GeV" <<std::endl;
	  	if(!cc)std::cout << "\nGamma Energy = " << nuEvFssGammaE_NC << " GeV" <<std::endl;
	  	
	  
	    	//std::cout << "\nTotal Energy from sum of GENIE primaries = " << nuE << " GeV" <<std::endl;
	    	std::cout << "\nTotal Kinetic Energy = " << totKinEnergy << " GeV" <<std::endl;
	  }
    	//nuE += nuEvNuclE;
	if (cc)nuEventPiZero_CC_h1_.Fill(PiZeroCount);
	if (!cc)nuEventPiZero_NC_h1_.Fill(PiZeroCount);

    	double chargedE_CC = nuEvFssChargedE_CC/totKinEnergy;
    	double neutronE_CC = nuEvFssNeutronE_CC/totKinEnergy;
    	double gammaE_CC = nuEvFssGammaE_CC/totKinEnergy;
    	double chargedE_NC = nuEvFssChargedE_NC/totKinEnergy;
    	double neutronE_NC = nuEvFssNeutronE_NC/totKinEnergy;
    	double gammaE_NC = nuEvFssGammaE_NC/totKinEnergy;
    	double neutrinoE_NC = nuEvNeutrinoE_NC/totKinEnergy;

	if(verbose_>0){
	  if (cc)std::cout << "\nCharged CC Energy ratio = " << chargedE_CC;
	  if (cc)std::cout << "\nNeutron CC Energy ratio = " << neutronE_CC;
	  if (cc)std::cout << "\nGamma CC Energy ratio = " << gammaE_CC;
	  if (!cc)std::cout << "\nCharged NC Energy ratio = " << chargedE_NC;
	  if (!cc)std::cout << "\nNeutron NC Energy ratio = " << neutronE_NC;
	  if (!cc)std::cout << "\nGamma NC Energy ratio = " << gammaE_NC;
	  if (!cc)std::cout << "\nNeutrino NC Energy ratio = " << neutrinoE_NC;
	  
	  std::cout << "\nRatio Sum = "
		    << chargedE_CC+neutronE_CC+gammaE_CC+chargedE_NC+neutronE_NC+gammaE_NC+neutrinoE_NC
		    << " \n----------------" <<std::endl;
	}
    	
	//fill hists
	if (chargedE_CC>0)nuEventChargedE_CC_h1d_.Fill(chargedE_CC);
    	if (chargedE_NC>0)nuEventChargedE_NC_h1d_.Fill(chargedE_NC);
    	if (neutronE_CC>0)nuEventNeutronE_CC_h1d_.Fill(neutronE_CC);
    	if (neutronE_NC>0)nuEventNeutronE_NC_h1d_.Fill(neutronE_NC);
    	if (gammaE_CC>0)nuEventPhotonE_CC_h1d_.Fill(gammaE_CC);
    	if (gammaE_NC>0)nuEventPhotonE_NC_h1d_.Fill(gammaE_NC);
	if (neutrinoE_NC>0)nuEventNeutrinoE_NC_h1d_.Fill(neutrinoE_NC);
	
    	//TLorentzVector nuHitPos = nuHit_->getPosition() * (1./ CLHEP::cm);
    	TLorentzVector nuEvPos = nuEvent_->getPosition() * (1./ CLHEP::cm);
		
    	nuEventZ_h1d_.Fill(nuEvPos.Z());
    	nuEventXY_h2d_.Fill(nuEvPos.X(), nuEvPos.Y());
    	nuEventXZ_h2d_.Fill(nuEvPos.X(), nuEvPos.Z());
	
      }
      //particle counts
      allParticleCount = allKinE_h1d_.GetEntries();
      electronCount = electronKinE_h1d_.GetEntries();
      positronCount = positronKinE_h1d_.GetEntries();
      protonCount = protonKinE_h1d_.GetEntries();
      neutronCount = neutronKinE_h1d_.GetEntries();
      muonCount = muonKinE_h1d_.GetEntries();
      aMuonCount = aMuonKinE_h1d_.GetEntries();
      pipCount = pipKinE_h1d_.GetEntries();
      pimCount = pimKinE_h1d_.GetEntries();
      pi0Count = pi0KinE_h1d_.GetEntries();
      gammaCount = gammaKinE_h1d_.GetEntries();

    }	
    
    i=0;
    
    datafile.Close();

    if(verbose_>=0){
	//volumes
    	std::cout << "\n**************************************"
                  <<"\nVolume Counts:"
                  << "\n\tTpcFidVolume      = " << tpcFidCount 
		  << "\n\tNotActiveVolume   = " << notActiveCount
                  << "\n\tVesselVolume      = " << vesselCount
                  << "\n\tInnerVesselVolume = " << innerVesselCount
                  << "\n\tScintVolume       = " << scintCount
                  << "\n\tMagnetVolume      = " << magnetCount
                  << "\n\tCavityVolume      = " << cavityCount
                  << "\n\tMindVolume        = " << mindCount
        //        << "\n\tMotherVolume      = " << motherCount
                  << "\n\tRockVolume        = " << rockCount
                  << "\n\tNo Volume found   = " << otherCount
                  << "\n**************************************\n";
	//particle numbers at vertex
    	std::cout << "\n**************************************"
                  <<"\nParticle Counts (primary):"
                  << "\n\tProtons      = " << protonCount 
                  << "\n\tNeutrons     = " << neutronCount 
                  << "\n\tElectrons    = " << electronCount 
                  << "\n\tPositrons    = " << positronCount 
                  << "\n\tPi+          = " << pipCount 
                  << "\n\tPi-          = " << pimCount 
                  << "\n\tPi0          = " << pi0Count 
                  << "\n\tGamma        = " << gammaCount 
                  << "\n\tMuons        = " << muonCount
                  << "\n\tAnti muons   = " << aMuonCount
                  << "\n\t----------------------------"
                  << "\n\tAll  	      = " << allParticleCount
                  << "\n**************************************\n";
    }

    //get and print statistics
    if(stats_){
	double tmp_exposure;
	stats_->printToStream(std::cout);
  	map<string,string,ci_less> statsInfo = stats_->getInfoMap();
  	map<string,string>::const_iterator it;
	for (it = statsInfo.begin(); it != statsInfo.end(); it++) {
	  std::string statName = it->first;
	  std::size_t found = statName.find("Real Exposure");
	  if(found!=std::string::npos){
		std::stringstream ss_scientific(it->second);
  		ss_scientific.precision(2);
  		ss_scientific>> scientific >> tmp_exposure;
		exposure += tmp_exposure;
		//exposure += atoi(it->second.c_str());
	  }
	}
    }

    //clear stringstream 
    ss.str("");

  } //end loop over files

  std::cout << "\n------------------------------------------------------------------------"
		<< "\nTotal number of neutrino interactions in geometry: "
		<< totalNuEvents
		<< "\nTotal Exposure [p.o.t]:   "
		<< exposure
	 	<< "\nTPC Interactions:         " 
		<< tpcFidCount
		<< "\nAverage Exposure [p.o.t]: "
		<< exposure/runs_
		<< "\n------------------------------------------------------------------------\n";

  std::cout << "\n-----------------------------------------------------------------------------------------"
	    << "\n  Writing to file: " << outputFileName_.c_str()
	    << "\n-----------------------------------------------------------------------------------------\n";

  TFile histofile(outputFileName_.c_str(),"recreate");
  this->write();

  std::cout << "\n----------------------------------"
	    << "\n             Closing...           "
	    << "\n----------------------------------\n";

  histofile.Close();
}

void myPlotter::write(){

  fluxE_h1d_.SetLineColor(kBlack);
  fluxE_h1d_.Write();
  nuHitXY_h2d_.SetDrawOption("COLZ");
  nuHitXY_h2d_.Write();

  nuEventE_h1d_.SetLineColor(kBlack);
  nuEventE_h1d_.Write();
  nuAntiMuEventE_h1d_.SetLineColor(kBlue);
  nuMuEventE_h1d_.Write();
  nuAntiMuEventE_h1d_.SetLineColor(kRed);
  nuAntiMuEventE_h1d_.Write();
  nuEEventE_h1d_.SetLineColor(kGreen);
  nuEEventE_h1d_.Write();
  nuAntiEEventE_h1d_.SetLineColor(kOrange);
  nuAntiEEventE_h1d_.Write();
  
  nuEventPiZero_CC_h1_.Write();
  nuEventPiZero_CC_h1_.SetLineColor(kBlack);
  nuEventPiZero_NC_h1_.Write();
  nuEventPiZero_NC_h1_.SetLineColor(kRed);

  allKinE_h1d_.SetLineColor(kBlack);
  allKinE_h1d_.SetLineWidth(2);
  allKinE_h1d_.Write();
  protonKinE_h1d_.SetLineColor(kBlack);
  protonKinE_h1d_.Write();
  neutronKinE_h1d_.SetLineColor(kRed);
  neutronKinE_h1d_.Write();
  pipKinE_h1d_.SetLineColor(kBlack);
  pipKinE_h1d_.Write();
  pimKinE_h1d_.SetLineColor(kRed);
  pimKinE_h1d_.Write();
  pi0KinE_h1d_.SetLineColor(kBlack);
  pi0KinE_h1d_.Write();
  gammaKinE_h1d_.SetLineColor(kRed);
  gammaKinE_h1d_.Write();
  electronKinE_h1d_.SetLineColor(kBlack);
  electronKinE_h1d_.Write();
  positronKinE_h1d_.SetLineColor(kRed);
  positronKinE_h1d_.Write();
  muonKinE_h1d_.SetLineColor(kBlack);
  muonKinE_h1d_.Write();
  aMuonKinE_h1d_.SetLineColor(kRed);
  aMuonKinE_h1d_.Write();

  nuEventZ_h1d_.Write();
  nuEventXY_h2d_.SetDrawOption("COLZ");
  nuEventXY_h2d_.Write();
  nuEventXZ_h2d_.SetDrawOption("COLZ");
  nuEventXZ_h2d_.Write();

  nuEventChargedE_CC_h1d_.Write();
  nuEventChargedE_NC_h1d_.Write();
  nuEventNeutronE_CC_h1d_.Write();
  nuEventNeutronE_NC_h1d_.Write();
  nuEventPhotonE_CC_h1d_.Write();
  nuEventPhotonE_NC_h1d_.Write();
  nuEventNeutrinoE_NC_h1d_.Write();

  //nuHits_h3d_.SetMarkerColor(kGreen);
  //nuHits_h3d_.Write();

  //nuEvents_h3d_.SetMarkerColor(kBlue);
  //nuEvents_h3d_.Write();

  //detHits_h3d_.SetMarkerColor(kRed);
  //detHits_h3d_.Write();

  detHits_h2d_Ver.SetMarkerColor(kRed);
  detHits_h2d_Ver.Write();
  detHits_h2d_Hor.Write();

  range_h2d_Ver.Write();
  range_h2d_Hor.Write();

  range_h1d_.Write();
  rangeVE_h2d_.Write();
}

int main(int argc, char ** argv) {
  try {
    myPlotter plotter;
    plotter.getRunTimeArguments(argc, argv);
    plotter.loadDataCards();
    plotter.initHistos();
    plotter.plot();

  } catch (GasTPCException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }

  return 0;
}


