//____________________________________________________________________________
/*!

\class    lbnoReconstruct

\brief    Program to perform reconstruction techniques

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  June 2013
\last update June 2014

*/
//____________________________________________________________________________
#include <iostream>
#include <map>

#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TGeoVolume.h>

#include "LbnoPlotter.hh"
#include "GeometryLoader.hh"
#include "ProgressBar.hh"

#include "PiZeroFinder.hh"
#include "TpcMomSmear.hh"
#include "Tracker.hh"

class myReconstructor : public LbnoPlotter {
 public:
  myReconstructor() {}
  virtual ~myReconstructor() {}

  void initHistos();
  void write();

  void reconstruct();
  void makeGraphs();
  //double ccqeReconstruct(HitCollection &lepton,NeutrinoEvent* nu_event);
  //double particleMomentum(HitCollection &track);
  bool inFiducialVolume(NeutrinoEvent* nu_event,double fidCut);
  bool loadGeom(std::string);
  bool isCCQE(NeutrinoEvent* );
  bool isBadEvent(double reconRatio,double cutOff);

  void showBadEvents();
  void insertAscending(ReconData neutrino,std::vector<ReconData> &neutrinoData);
  void calculateMeanAndErrors(std::vector<ReconData> &neutrinoData,std::vector<double> &meanVector,std::vector<double> &errorVector);

  void initDataCards();
  void loadDataCards();

 private:
  TFile file_;

  int badFileCount;

  int pdg_,leptonPdg_;
  int hitCountCondition_;
  double range;
  double resolution_,scintResolution_;
  double fiducialCut_;
  double reconstructionCut_;
  double momentumMinSelect_, momentumMaxSelect_;

  double factor;

  double sagitta,deltaSagitta;
  double piZeroMassCutValue_;

  TH1F nuEvent_truth_energy_h1d;

  TH1F sagitta_reconDiff_h1d;

  TH1F tracking_track_length_fspl_h1d;
  TH1F tracking_track_length_proton_h1d;
  TH1F tracking_track_length_pion_h1d;
  TH1F tracking_recon_mom_h1d;
  TH1F tracking_recon_mom_CCQE_h1d;
  TH1F tracking_recon_mom_CCQE_calc_h1d;
  TH1F tracking_reconDiff_mom_h1d;
  TH1F tracking_reconDiff_mom_lepton_h1d;
  TH1F tracking_reconDiff_mom_proton_h1d;
  TH1F tracking_reconDiff_mom_CCQE_h1d;
  TH1F tracking_reconDiff_mom_CCQE_lepton_h1d;
  TH1F tracking_reconDiff_mom_CCQE_proton_h1d;
  TH1F tracking_reconDiff_mom_CCQE_calc_h1d;
  TH1F tracking_reconDiff_mom_CCQE_muAndProt_h1d;
  TH1F tracking_reconDiff_mom_CCQE_muOnly_h1d;
  TH1F tracking_reconDiff_mom_CCQE_protonOnly_h1d;

  TH1F tracking_piZero_reconEnergy_1pi0_h1d;
  TH1F tracking_piZero_reconEnergy_2pi0_h1d;
  TH1F tracking_piZero_reconEnergy_3pi0_h1d;
  TH1F tracking_piZero_reconEnergy_all_h1d;
  TH1F tracking_piZero_invariantMass_1pi0_h1d;
  TH1F tracking_piZero_invariantMass_2pi0_h1d;
  TH1F tracking_piZero_invariantMass_3pi0_h1d;
  TH1F tracking_piZero_invariantMass_all_h1d;
  TH1F tracking_piZero_photonAngle_all_h1d;
  TH1F tracking_piZero_reconCount_all_h1d;
  TH1F tracking_piZero_truthCount_all_h1d;

  TGraphErrors tracking_recon_mom_grph;
  TGraphErrors tracking_recon_mom_CCQE_grph;

  TH2F nuEvent_XY_position_h2d;
  TH2F nuEvent_XZ_position_h2d;

  TH2F sagitta_truthVsRecon_h2d;

  TH2F tracking_truthVsRecon_mom_h2d;
  TH2F tracking_truthVsRecon_mom_lepton_h2d;
  TH2F tracking_truthVsRecon_mom_proton_h2d;
  TH2F tracking_truthVsRecon_mom_CCQE_h2d;
  TH2F tracking_truthVsRecon_mom_CCQE_calc_h2d;
  TH2F tracking_truthVsRecon_mom_CCQE_lepton_h2d;
  TH2F tracking_truthVsRecon_mom_CCQE_proton_h2d;
  TH2F tracking_truthVsDiff_mom_h2d;
  TH2F tracking_rangeVsEDep_h2d;
  TH2F tracking_rangeVsMom_h2d;
  TH2F tracking_reconDiffVsRange_lepton_h2d;
  TH2F tracking_reconDiffVsRange_proton_h2d;
  TH2F tracking_reconDiffVsRange_CCQE_lepton_h2d;
  TH2F tracking_reconDiffVsRange_CCQE_proton_h2d;
  TH2F tracking_EDepVsMom_h2d;
  TH2F tracking_fullyContained_Mom_truthVsRecon_h2d;

  GeometryLoader * geomLoader;
  TGeoVolume * world;
  TGeoVolume * tpcVolume;

  ParticleDescrShortRecord * leptonTruthInfo;
  ParticleDescrShortRecord * protonTruthInfo;

  std::vector<ReconData> reconNeutrinos,reconCCQENeutrinos;
  std::vector< std::vector<ReconData> > badFileVector;
  std::vector<double> neutrinoReconMeanValues,neutrinoCCQEReconMeanValues;
  std::vector<double> neutrinoReconErrorValues,neutrinoCCQEReconErrorValues;

};

void myReconstructor::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardString("dataFileName", "nuData.root");
  cards_->AddDataCardString("plotFileName", "nuHistos.root");
  cards_->AddDataCardString("geomFileName", "");
  cards_->AddDataCardInt("numberOfRuns", 100);
  cards_->AddDataCardInt("numberOfEvents", -1);
  cards_->AddDataCardInt("plotterVerbose", 0);
  cards_->AddDataCardInt("pdg", 0);
  cards_->AddDataCardInt("leptonFlavour", 13);
  cards_->AddDataCardDouble("resolution", 0.05*CLHEP::mm);
  cards_->AddDataCardDouble("scintResolution", 10.*CLHEP::mm);
  cards_->AddDataCardDouble("piZeroMassCut", 0.1*PIZEROMASS); //in MeV
  cards_->AddDataCardDouble("maxEnergy", 30.0*CLHEP::GeV);	//in GeV
  cards_->AddDataCardDouble("momentumMinSelect", 0.0*CLHEP::GeV);//in GeV
  cards_->AddDataCardDouble("momentumMaxSelect", 30.0*CLHEP::GeV);//in GeV
  cards_->AddDataCardDouble("fiducialCut",10.0*CLHEP::cm);
  cards_->AddDataCardInt("requiredHitsPerTrack",2);
  cards_->AddDataCardDouble("reconstructionCut",0.4);	// reconstruction outside of (t-r)/t < 0.6 && > 1.4
}

void myReconstructor::loadDataCards() {
  inputFileName_  	= cards_->fetchValueString("dataFileName");
  outputFileName_  	= cards_->fetchValueString("plotFileName");
  geomFileName_  	= cards_->fetchValueString("geomFileName");
  runs_		  	= cards_->fetchValueInt("numberOfRuns");
  events_		= cards_->fetchValueInt("numberOfEvents");
  verbose_ 		= cards_->fetchValueInt("plotterVerbose");
  pdg_		  	= cards_->fetchValueInt("pdg");
  leptonPdg_		= cards_->fetchValueInt("leptonFlavour");
  resolution_ 		= cards_->fetchValueDouble("resolution");
  scintResolution_ 	= cards_->fetchValueDouble("scintResolution");
  maxEnergy_ 		= cards_->fetchValueDouble("maxEnergy");
  momentumMinSelect_ 	= cards_->fetchValueDouble("momentumMinSelect");
  momentumMaxSelect_ 	= cards_->fetchValueDouble("momentumMaxSelect");
  fiducialCut_ 		= cards_->fetchValueDouble("fiducialCut");
  hitCountCondition_  	= cards_->fetchValueInt("requiredHitsPerTrack");
  reconstructionCut_	= cards_->fetchValueDouble("reconstructionCut");
  piZeroMassCutValue_	= cards_->fetchValueDouble("piZeroMassCut");
}

bool myReconstructor::loadGeom(std::string filename){
  
  //load the geometry file to check nodes
  TFile geomFile(filename.c_str());

  if(!geomFile.IsZombie()){
	geomLoader = new GeometryLoader(filename);
     	world = geomLoader->getWorldVolume();
     	tpcVolume = geomLoader->getTPCVolume();
     	return true;
  }
  else{
  	world = NULL;
	tpcVolume = NULL; 
  	return false;
  }
}

void myReconstructor::initHistos(){

  //declare histograms
  int binning = maxEnergy_*10;
  nuEvent_truth_energy_h1d.SetNameTitle("nuEvent_truth_energy_h1d", "nu flux truth energy");
  nuEvent_truth_energy_h1d.SetBins(binning, 0., maxEnergy_);
  
  //double mean_sagitta = 10.0;	//mean sagitta value is around 10 mm
  sagitta_reconDiff_h1d.SetNameTitle("sagitta_reconDiff_h1d","(truth - recon)/truth sagitta value");
  sagitta_reconDiff_h1d.SetBins(2000,-10.,10.);	// in mm
  //sagitta_reconDiff_h1d.SetBins(400,mean_sagitta - 3*resolution_,mean_sagitta + 3*resolution_);

  tracking_track_length_fspl_h1d.SetNameTitle("tracking_track_length_fspl_h1d","tracking fspl track length");
  tracking_track_length_fspl_h1d.SetBins(500,0.,5.);
  tracking_track_length_proton_h1d.SetNameTitle("tracking_track_length_proton_h1d","tracking proton track length");
  tracking_track_length_proton_h1d.SetBins(500,0.,5.);
  tracking_track_length_pion_h1d.SetNameTitle("tracking_track_length_pion_h1d","tracking pion track length");
  tracking_track_length_pion_h1d.SetBins(500,0.,5.);
  tracking_recon_mom_h1d.SetNameTitle("tracking_recon_mom_h1d", "tracking reconstructed momentum");
  tracking_recon_mom_h1d.SetBins(binning,0., maxEnergy_);
  tracking_recon_mom_CCQE_h1d.SetNameTitle("tracking_recon_mom_CCQE_h1d", "tracking reconstructed momentum for CCQE events only");
  tracking_recon_mom_CCQE_h1d.SetBins(binning,0., maxEnergy_);
  tracking_recon_mom_CCQE_calc_h1d.SetNameTitle("tracking_recon_mom_CCQE_calc_h1d", "tracking reconstructed momentum for CCQE events only using calculation");
  tracking_recon_mom_CCQE_calc_h1d.SetBins(binning,0., maxEnergy_);
  tracking_reconDiff_mom_h1d.SetNameTitle("tracking_reconDiff_mom_h1d","tracking (truth - reconstructed / truth) momentum");
  tracking_reconDiff_mom_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_lepton_h1d.SetNameTitle("tracking_reconDiff_mom_lepton_h1d","tracking (truth - reconstructed / truth) momentum - lepton only");
  tracking_reconDiff_mom_lepton_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_proton_h1d.SetNameTitle("tracking_reconDiff_mom_proton_h1d","tracking (truth - reconstructed / truth) momentum - proton only");
  tracking_reconDiff_mom_proton_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_CCQE_h1d.SetNameTitle("tracking_reconDiff_mom_CCQE_h1d","tracking (truth - reconstructed / truth) momentum for CCQE events");
  tracking_reconDiff_mom_CCQE_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_CCQE_lepton_h1d.SetNameTitle("tracking_reconDiff_mom_CCQE_lepton_h1d","tracking (truth - reconstructed / truth) momentum for CCQE events- lepton only");
  tracking_reconDiff_mom_CCQE_lepton_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_CCQE_proton_h1d.SetNameTitle("tracking_reconDiff_mom_CCQE_proton_h1d","tracking (truth - reconstructed / truth) momentum for CCQE events- proton only");
  tracking_reconDiff_mom_CCQE_proton_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_CCQE_calc_h1d.SetNameTitle("tracking_reconDiff_mom_CCQE_calc_h1d","tracking (truth - reconstructed / truth) momentum for CCQE events using calculation");
  tracking_reconDiff_mom_CCQE_calc_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_CCQE_muAndProt_h1d.SetNameTitle("tracking_reconDiff_mom_CCQE_muAndProt_h1d","tracking (truth - reconstructed / truth) momentum for CCQE events with both lepton and proton showing as tracks");
  tracking_reconDiff_mom_CCQE_muAndProt_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_CCQE_muOnly_h1d.SetNameTitle("tracking_reconDiff_mom_CCQE_muOnly_h1d","tracking (truth - reconstructed / truth) momentum for CCQE events with only the lepton showing as track");
  tracking_reconDiff_mom_CCQE_muOnly_h1d.SetBins(400,-2.,2.);
  tracking_reconDiff_mom_CCQE_protonOnly_h1d.SetNameTitle("tracking_reconDiff_mom_CCQE_protonOnly_h1d","tracking (truth - reconstructed / truth) momentum for CCQE events with only the proton showing as track");
  tracking_reconDiff_mom_CCQE_protonOnly_h1d.SetBins(400,-2.,2.);

  int piZeroInvariantMassBinning = 600;
  tracking_piZero_invariantMass_1pi0_h1d.SetNameTitle("tracking_piZero_invariantMass_1pi0_h1d","tracking_piZero_invariantMass_1pi0_h1d");
  tracking_piZero_invariantMass_1pi0_h1d.SetBins(piZeroInvariantMassBinning,0.,300.);
  tracking_piZero_invariantMass_2pi0_h1d.SetNameTitle("tracking_piZero_invariantMass_2pi0_h1d","tracking_piZero_invariantMass_2pi0_h1d");
  tracking_piZero_invariantMass_2pi0_h1d.SetBins(piZeroInvariantMassBinning,0.,300.);
  tracking_piZero_invariantMass_3pi0_h1d.SetNameTitle("tracking_piZero_invariantMass_3pi0_h1d","tracking_piZero_invariantMass_3pi0_h1d");
  tracking_piZero_invariantMass_3pi0_h1d.SetBins(piZeroInvariantMassBinning,0.,300.);
  tracking_piZero_invariantMass_all_h1d.SetNameTitle("tracking_piZero_invariantMass_all_h1d","tracking_piZero_invariantMass_all_h1d");
  tracking_piZero_invariantMass_all_h1d.SetBins(piZeroInvariantMassBinning,0.,300.);
 
  tracking_piZero_reconEnergy_1pi0_h1d.SetNameTitle("tracking_piZero_reconEnergy_1pi0_h1d","tracking_piZero_reconEnergy_1pi0_h1d");
  tracking_piZero_reconEnergy_1pi0_h1d.SetBins(binning, 0., maxEnergy_);
  tracking_piZero_reconEnergy_2pi0_h1d.SetNameTitle("tracking_piZero_reconEnergy_2pi0_h1d","tracking_piZero_reconEnergy_2pi0_h1d");
  tracking_piZero_reconEnergy_2pi0_h1d.SetBins(binning, 0., maxEnergy_);
  tracking_piZero_reconEnergy_3pi0_h1d.SetNameTitle("tracking_piZero_reconEnergy_3pi0_h1d","tracking_piZero_reconEnergy_3pi0_h1d");
  tracking_piZero_reconEnergy_3pi0_h1d.SetBins(binning, 0., maxEnergy_);
  tracking_piZero_reconEnergy_all_h1d.SetNameTitle("tracking_piZero_reconEnergy_all_h1d","tracking_piZero_reconEnergyall_h1d");
  tracking_piZero_reconEnergy_all_h1d.SetBins(binning, 0., maxEnergy_);
  
  tracking_piZero_photonAngle_all_h1d.SetNameTitle("tracking_piZero_photonAngle_all_h1d","tracking_piZero_photonAngle_all_h1d");
  tracking_piZero_photonAngle_all_h1d.SetBins(400,-1.,1.);

  tracking_piZero_reconCount_all_h1d.SetNameTitle("tracking_piZero_reconCount_all_h1d","tracking_piZero_reconCount_all_h1d");
  tracking_piZero_reconCount_all_h1d.SetBins(binning, 0., maxEnergy_);
  tracking_piZero_truthCount_all_h1d.SetNameTitle("tracking_piZero_truthCount_all_h1d","tracking_piZero_truthCount_all_h1d");
  tracking_piZero_truthCount_all_h1d.SetBins(binning, 0., maxEnergy_);

  nuEvent_XY_position_h2d.SetNameTitle("nuEvent_XY_position_h2d","XY position of neutrino event in the tpc");
  nuEvent_XY_position_h2d.SetBins(240,-120.,120.,240,-120.,120.);
  nuEvent_XZ_position_h2d.SetNameTitle("nuEvent_XZ_position_h2d","XZ position of neutrino event in the tpc");
  nuEvent_XZ_position_h2d.SetBins(240,-120.,120.,300,-150.,150.);

  sagitta_truthVsRecon_h2d.SetNameTitle("sagitta_truthVsRecon_h2d","truth vs recon values for the sagitta");
  sagitta_truthVsRecon_h2d.SetBins(1000,0.,1000.,1000,0.,1000.);
  //sagitta_truthVsRecon_h2d.SetBins(400,mean_sagitta - 3*resolution_,mean_sagitta + 3*resolution_,400,mean_sagitta - 3*resolution_,mean_sagitta + 3*resolution_);

  tracking_truthVsRecon_mom_h2d.SetNameTitle("tracking_truthVsRecon_mom_h2d", "tracking truth vs recon mom");
  tracking_truthVsRecon_mom_h2d.SetBins(binning*2, 0.,maxEnergy_, binning*6,0.,maxEnergy_*3);
  tracking_truthVsRecon_mom_lepton_h2d.SetNameTitle("tracking_truthVsRecon_mom_lepton_h2d", "tracking truth vs recon mom for leptons");
  tracking_truthVsRecon_mom_lepton_h2d.SetBins(binning*2, 0.,maxEnergy_, binning*6,0.,maxEnergy_*3);
  tracking_truthVsRecon_mom_proton_h2d.SetNameTitle("tracking_truthVsRecon_mom_proton_h2d", "tracking truth vs recon mom for protons");
  tracking_truthVsRecon_mom_proton_h2d.SetBins(binning*2, 0.,maxEnergy_, binning*6,0.,maxEnergy_*3);
  tracking_truthVsRecon_mom_CCQE_h2d.SetNameTitle("tracking_truthVsRecon_mom_CCQE_h2d", "tracking truth vs recon mom for CCQE events only");
  tracking_truthVsRecon_mom_CCQE_h2d.SetBins(binning*2, 0.,maxEnergy_, binning*6,0.,maxEnergy_*3);
  tracking_truthVsRecon_mom_CCQE_calc_h2d.SetNameTitle("tracking_truthVsRecon_mom_CCQE_calc_h2d", "tracking truth vs recon mom for CCQE events only using calculation");
  tracking_truthVsRecon_mom_CCQE_calc_h2d.SetBins(binning*2, 0.,maxEnergy_, binning*6,0.,maxEnergy_*3);
  tracking_truthVsRecon_mom_CCQE_lepton_h2d.SetNameTitle("tracking_truthVsRecon_mom_lepton_CCQE_h2d", "tracking truth vs recon mom for leptons in CCQE events");
  tracking_truthVsRecon_mom_CCQE_lepton_h2d.SetBins(binning*2, 0.,maxEnergy_, binning*6,0.,maxEnergy_*3);
  tracking_truthVsRecon_mom_CCQE_proton_h2d.SetNameTitle("tracking_truthVsRecon_mom_proton_CCQE_h2d", "tracking truth vs recon mom for protons in CCQE events");
  tracking_truthVsRecon_mom_CCQE_proton_h2d.SetBins(binning*2, 0.,maxEnergy_, binning*6,0.,maxEnergy_*3);
  tracking_truthVsDiff_mom_h2d.SetNameTitle("tracking_truthVsDiff_mom_h2d", "tracking truth vs diff mom");
  tracking_truthVsDiff_mom_h2d.SetBins(binning, 0.,maxEnergy_,120,-10.,2.);

  tracking_reconDiffVsRange_lepton_h2d.SetNameTitle("tracking_reconDiffVsRange_lepton_h2d","tracking reconDiff vs range for leptons");
  tracking_reconDiffVsRange_lepton_h2d.SetBins(400,-2.,2.,500,0.,5.);
  tracking_reconDiffVsRange_proton_h2d.SetNameTitle("tracking_reconDiffVsRange_proton_h2d","tracking reconDiff vs range for protons");
  tracking_reconDiffVsRange_proton_h2d.SetBins(400,-2.,2.,500,0.,5.);
  tracking_reconDiffVsRange_CCQE_lepton_h2d.SetNameTitle("tracking_reconDiffVsRange_lepton_CCQE_h2d","tracking reconDiff vs range for leptons - CCQE only");
  tracking_reconDiffVsRange_CCQE_lepton_h2d.SetBins(400,-2.,2.,500,0.,5.);
  tracking_reconDiffVsRange_CCQE_proton_h2d.SetNameTitle("tracking_reconDiffVsRange_proton_CCQE_h2d","tracking reconDiff vs range for protons - CCQE only");
  tracking_reconDiffVsRange_CCQE_proton_h2d.SetBins(400,-2.,2.,500,0.,5.);
  tracking_rangeVsEDep_h2d.SetNameTitle("tracking_rangeVsEDep_h2d","tracking range vs eDep histogram");
  tracking_rangeVsEDep_h2d.SetBins(500,0.,5.,500,0.,100.);
  tracking_rangeVsMom_h2d.SetNameTitle("tracking_rangeVsMom_h2d","tracking range vs mom histogram");
  tracking_rangeVsMom_h2d.SetBins(500,0.,5.,500,0.,10.);
  tracking_EDepVsMom_h2d.SetNameTitle("tracking_EDepVsMom_h2d","tracking eDep vs Mom histogram");
  tracking_EDepVsMom_h2d.SetBins(500,0.,100.,500,0.,10.);
  tracking_fullyContained_Mom_truthVsRecon_h2d.SetNameTitle("tracking_fullyContained_Mom_truthVsRecon_h2d","fully contained tracks in tpc truth vs recon mom");
  tracking_fullyContained_Mom_truthVsRecon_h2d.SetBins(binning, 0.,maxEnergy_, binning*3,0.,maxEnergy_*3.);
  
  factor = 10.0;
}

void myReconstructor::reconstruct() {

  leptonTruthInfo = new ParticleDescrShortRecord();
  protonTruthInfo = new ParticleDescrShortRecord();

  deltaSagitta = resolution_;//*CLHEP::mm;

  //nodes
  int tpcFidCount = 0;
  int vesselCount = 0;
  int innerVesselCount = 0;
  int scintCount = 0;
  int magnetCount = 0;
  int cavityCount = 0;
  int rockCount = 0;
  int otherCount = 0;

  //QELCC count
  int qelccCount = 0;
  //exposure
  double exposure = 0;

  //define number of runs
  int runs = runs_;

  std::string filename = inputFileName_;
  std::stringstream ss;

  int totalNuEvents = 0;

  //create a progress bar for file loop and one for entries
  ProgressBar progFiles;
  ProgressBar progEntries;

  badFileCount = 0;

  //***loop over runs***
  for(int j = 0;j<runs;j++){

    //read in the file
    ss << j;

    std::string filename_process = filename + ss.str() + ".root";
    if(verbose_>0){
      std::cout 
	<< "\n************************************************************************************************"
	<< "\nFilename being read: " << filename_process
	<< "\n************************************************************************************************" <<std::endl;
    }

    //load the data and stepping files
    TFile datafile(filename_process.c_str());
    //if file is bad skip it
    if(datafile.GetNkeys()<1){
	badFileCount++;
        //clear stringstream 
        ss.str("");
	continue;	
    }

    //clear stringstream 
    ss.str("");

    //set data addresses
    if(!setupTrees(datafile)) return;

    //keep a count of neutrino events
    totalNuEvents +=nuEvents_nEntries;

    //just load the geometry form the first file and use this for all runs
    if(j==0){	
    	//load the geometry file - exit if no file is found or loaded
    	if( !loadGeom(geomFileName_) ){
		std::cout << "\n\n !!!!! ERROR: No geometry file is found, Exiting... !!!!!" << std::endl;
		return;
    	}
    
    	//also exit if it cannot find the tpc volume
    	if (!tpcVolume){
		std::cout << "\n\n !!!!! ERROR: Cannot find TPC Volume, Exiting... !!!!!" << std::endl;
		return;
	}
    }

    int i=0;
    if(verbose_>2){  
      std::cout << "\n----------------------------------"
	      << "\n    Looping over entries...   "
	      << "\n----------------------------------\n";
    }

    //make a vector to store the badly reconstructed events
    std::vector<ReconData> badEventID;

    // Detector Hits tree used for reconstruction
    if(detectorHitsTree){
      this->setBackTracer(detectorHitsTree);
            
      ///create a tracker object to sort hits into "tracks"///
      Tracker * trackMgr = new Tracker(scintResolution_);

      //loop over detector tree
      for(i=0;i<detHits_nEntries;i++) {
        detectorHitsTree->GetEntry(i);
      
	//set backtracers for neutrino event truth info
        if(nuHitTree)nuHitTree->GetEntry(backTracer_[0]);
        nuEventTree->GetEntry(backTracer_[1]);

	//node name
	std::string nodeName = nuEvent_->getNodeName();
	//only consider events in the TPC - skip to next event
	if(nodeName.compare( 0,6, "tpcFid" ) !=0)continue;

	//std::cout << "\nBacktracer: " << backTracer_[0] << ", " << backTracer_[1] << ", " << backTracer_[2] <<std::endl;
	
	//stop when reach number of required events - use -1 for all events
	if( (events_==tpcFidCount) && (events_>0) )break;

	//get the truth position of the neutrino event
	TLorentzVector nuEvPos = nuEvent_->getPosition() * (1./ CLHEP::cm);
	TLorentzVector eventVertex = nuEvent_->getPosition(); // in mm
        TLorentzVector nuHitPos;
	if(nuHit_)nuHitPos = nuHit_->getPosition() * (1./ CLHEP::cm);
	else nuHitPos.SetXYZT(0.,0.,0.,0.);

	//check if in fiducial cut - skip to next event if does not fit cut
        if( !inFiducialVolume(nuEvent_,fiducialCut_) )continue;

	//truth plot for event position
	nuEvent_XY_position_h2d.Fill( nuEvPos.X(),nuEvPos.Y() );
	nuEvent_XZ_position_h2d.Fill( nuEvPos.X(),nuEvPos.Z() );

	//get the original energy of the neutrino - truth info
        double nuE;
        double nuE2 = nuEvent_->getNuEnergy() * (1./CLHEP::GeV);	//should be the same
	if(nuHit_)nuE = nuHit_->getP4().E() * (1./CLHEP::GeV);
	else nuE = nuE2;

	//std::cout << "\nEnergy from Hit: " << nuE
	//	<< "\nEnergy from Event: " << nuE2;

	nuEvent_truth_energy_h1d.Fill(nuE);			//fill in the truth energy
        double nuMomX;
	if(nuHit_)nuMomX = nuHit_->getP4().X() * (1./CLHEP::GeV);
	else nuMomX = 0.;
        double nuMomY;
	if(nuHit_) nuMomY= nuHit_->getP4().Y() * (1./CLHEP::GeV);
	else nuMomY = 0.;
        double nuMomZ;
	if(nuHit_)nuMomZ = nuHit_->getP4().Z() * (1./CLHEP::GeV);
	else nuMomZ = 0.;
	double nuMom = TMath::Sqrt( nuMomX*nuMomX + nuMomY*nuMomY + nuMomZ*nuMomZ);
	if(nuMom==0.)nuMom=nuE;

	//check if it is CCQE as defined as nu_{l} + N -> l + p + N'
	bool ccqe = isCCQE(nuEvent_);
	
	if(verbose_>0){
		std::cout <<"\n *** Event Number: " << backTracer_[1];
		nuEvent_->printToStream(std::cout);
	}

	if(verbose_>1 && ccqe){
		std::cout << "\n===== CCQE ======";
		std::cout << "\n--- Lepton --- \n";
		leptonTruthInfo->printToStream(std::cout);
		std::cout << "\n--- Proton --- \n";
		protonTruthInfo->printToStream(std::cout);
	}

	double ccqeNuMomCalcResult = -1.;
	bool ccqe_leptonAndProtonHits = false;
	bool ccqe_leptonOnlyHits = false;
	bool ccqe_protonOnlyHits = false;

	//loop over hits
	HitCollection tpcSdHits = simData_->getTpcFidHits();
	ScintHitCollection scintSdHits = simData_->getScintHits();
	//size of the hit vectors per event
        int tpcHitCount = tpcSdHits.size();
        int scintHitCount = scintSdHits.size();

	bool trackSet = false;	
	bool goodEvent = false;	//use this to check if particles that leave hits in the event all have range>0
	
	//loop over and read tracks 
	bool foundMuon = false;
	bool foundProton = false;

	//the total reconstructed neutrino energy
	double totalReconNuEnergy = 0.;
	//sum the tracks transverse momentum to get original neutrino momentum
	double totalTransMomentum = 0.;
	double totalEDep = 0.;	//also the energy deposition
	double totalEDep2 = 0.;	//also the energy deposition

	/// --- TPC HITS ---- ///
	TpcMomSmear smearer(magField_*(1./CLHEP::tesla),deltaSagitta);

	//create a vector of vectors for track information for each track
	std::vector<HitCollection> tpcTrackVector;
	tpcTrackVector.resize(0);				//clear the track info from previous event
	int tpcTrackHitCount = 0;
	if(tpcHitCount>0){
		//convert hits into vector of tracks to make it easier to reconstruct each one
		tpcTrackVector = trackMgr->MakeFromHits(tpcSdHits,true);

		//read from the TPC tracks
		for(std::vector< HitCollection >::iterator k = tpcTrackVector.begin();k != tpcTrackVector.end();++k){
		  int eventID = backTracer_[1];
		  HitCollection tempVtr= *k;
		  //number of hits per track
		  int numberOfHits = tempVtr.size();
		  //total number of hits from all tracks
		  tpcTrackHitCount += numberOfHits;

		  double initKinEnergy = tempVtr.front().getKinEnergy() * (1./CLHEP::GeV);
		  double lastKinEnergy = tempVtr.back().getKinEnergy() * (1./CLHEP::GeV);
		
		  //momentum reconstruction
		  double trackTruthTransMomentum = smearer.getTruthTransMomentum(tempVtr);
		  trackTruthTransMomentum *=  (1./CLHEP::GeV);

		  //get the smeared momentum
		  double trackTransMomentum = smearer.particleMomentum(tempVtr);
		  trackTransMomentum *=  (1./CLHEP::GeV);

		  //check if it is negative - this will show as charge mis identification
		  if(trackTransMomentum<0.)trackTransMomentum = -trackTransMomentum;
		  
		  //if the track has zero momentum then skip track
		  //if(trackTransMomentum==0)continue;

		  //sum all the momenta - this is the reconstructed neutrino momentum
		  totalTransMomentum += trackTransMomentum;

		  //get range of each particle
		  double range = 0.;
		  //must have at least 2 points to get a range
		  if(numberOfHits>1){
		  	int fssPDG = tempVtr.front().getPDG();
		  	int fssTrackID = tempVtr.front().getTrackID();

			//range
			range = smearer.getTrackLength(tempVtr);
			range *= (1./CLHEP::m);

			//sagitta
			double truthSag = smearer.getTrackSagitta(tempVtr,trackTruthTransMomentum*CLHEP::GeV);
			double smearSag = smearer.getSmearSagitta(truthSag);
			truthSag *= (1./CLHEP::mm);
			smearSag *= (1./CLHEP::mm);

			TLorentzVector initPos = tempVtr.front().getPosition() * (1./CLHEP::m);
			TLorentzVector lastPos = tempVtr.back().getPosition() * (1./CLHEP::m);

			double eDep = tempVtr.front().getKinEnergy() * (1./CLHEP::MeV) - tempVtr.back().getKinEnergy() * (1./CLHEP::MeV);
			totalEDep2 += eDep;
			bool fullyContained = false;
			if( (tempVtr.back().getTrackLeftVolume()) == 0 )fullyContained = true;
			
			if(fssPDG == 13)foundMuon = true;
			if(fssPDG == 2212)foundProton = true;

			if(verbose_>2){
			  std::cout << "\n Event: " << eventID 
			    	  << "\n fss PDG " << fssPDG
				  << "\n fss Track Vector Size " << numberOfHits
				  << "\n fss Initial Pos (" <<  initPos.X() << ", " << initPos.Y() << ", " << initPos.Z() << ") m"
				  << "\n fss Final Pos (" <<  lastPos.X() << ", " << lastPos.Y() << ", " << lastPos.Z() << ") m"
				  << "\n fss truth mom " << trackTruthTransMomentum  << " GeV/c"
				  << "\n fss Trans Mom " << trackTransMomentum<< " GeV/c"
				  << "\n fss truth sagitta " << truthSag << " mm"
				  << "\n fss smear sagitta " << smearSag << " mm"
				  << "\n fss EDep " << eDep<< " MeV"
				  << "\n fss Initial Kinetic Energy " << initKinEnergy<< " GeV"
				  << "\n fss Final Kinetic Energy " << lastKinEnergy<< " GeV"
				  << "\n fss Range " << range  << " m"
				  << "\n first " << tempVtr.front().getTrackLeftVolume()
				  << "\n last " << tempVtr.back().getTrackLeftVolume()
				  << "\n ---Track fully contained in TPC? " <<  fullyContained << std::endl;
			}
	
			///------------------- Fill histograms --------------------///
			//for particles that are fully contained in the TPC only
			if(range>0. && fullyContained == true){
				//only fill histograms for particle selected from pdg_ input
				if(fssPDG==pdg_ || fssPDG==-pdg_){
					tracking_rangeVsEDep_h2d.Fill(range,eDep);
					if(fullyContained == true)tracking_rangeVsMom_h2d.Fill(range,trackTransMomentum);
					if(fullyContained == true)tracking_fullyContained_Mom_truthVsRecon_h2d.Fill(trackTruthTransMomentum,trackTransMomentum);
				}
				//if pdg_ input is set to 0 then fill hists for all particles
				else if(pdg_ == 0){		
					tracking_rangeVsEDep_h2d.Fill(range,eDep);
					tracking_rangeVsMom_h2d.Fill(range,trackTransMomentum);
					if(fullyContained == true)tracking_fullyContained_Mom_truthVsRecon_h2d.Fill(trackTruthTransMomentum,trackTransMomentum);
				}
				if(fssPDG==2212)tracking_track_length_proton_h1d.Fill(range);
				if(fssPDG==13)tracking_track_length_fspl_h1d.Fill(range);
				if(fssPDG==211 || fssPDG==-211)tracking_track_length_pion_h1d.Fill(range);

			}
			//for particles that show in the TPC
			if(range>0. && (numberOfHits > hitCountCondition_) ){
				//only fill histograms for particle selected from pdg_ input
				if(fssPDG==pdg_ || fssPDG==-pdg_){
					sagitta_reconDiff_h1d.Fill( (truthSag-smearSag)/truthSag );
					sagitta_truthVsRecon_h2d.Fill( truthSag,smearSag );
				}
				//if pdg_ input is set to 0 then fill hists for all particles
				else if(pdg_ == 0){	
					sagitta_reconDiff_h1d.Fill( (truthSag-smearSag)/truthSag );
					sagitta_truthVsRecon_h2d.Fill( truthSag,smearSag );
				}
				tracking_EDepVsMom_h2d.Fill(eDep,trackTransMomentum);
				if(fssPDG==13){
					tracking_truthVsRecon_mom_lepton_h2d.Fill(trackTruthTransMomentum,trackTransMomentum);
					tracking_reconDiff_mom_lepton_h1d.Fill( (trackTruthTransMomentum-trackTransMomentum)/trackTruthTransMomentum );
					tracking_reconDiffVsRange_lepton_h2d.Fill( (trackTruthTransMomentum-trackTransMomentum)/trackTruthTransMomentum,range );
				}
				if(fssPDG==2212){
					tracking_truthVsRecon_mom_proton_h2d.Fill(trackTruthTransMomentum,trackTransMomentum);
					tracking_reconDiff_mom_proton_h1d.Fill( (trackTruthTransMomentum-trackTransMomentum)/trackTruthTransMomentum );
					tracking_reconDiffVsRange_proton_h2d.Fill( (trackTruthTransMomentum-trackTransMomentum)/trackTruthTransMomentum,range );
				}
				if(ccqe){
				  if( ((fssPDG==13 || fssPDG==-13 || fssPDG ==11||fssPDG==-11) && fssPDG==leptonPdg_) || fssPDG==leptonPdg_){
					ccqeNuMomCalcResult = smearer.ccqeReconstruct(protonTruthInfo,tempVtr,nuEvent_);
					tracking_truthVsRecon_mom_CCQE_lepton_h2d.Fill(trackTruthTransMomentum,trackTransMomentum);
					tracking_reconDiff_mom_CCQE_lepton_h1d.Fill( (trackTruthTransMomentum-trackTransMomentum)/trackTruthTransMomentum );
					tracking_reconDiffVsRange_CCQE_lepton_h2d.Fill( (trackTruthTransMomentum-trackTransMomentum)/trackTruthTransMomentum,range );
				  }
				  if(fssPDG==2212){
					tracking_truthVsRecon_mom_CCQE_proton_h2d.Fill(trackTruthTransMomentum,trackTransMomentum);
					tracking_reconDiff_mom_CCQE_proton_h1d.Fill( (trackTruthTransMomentum-trackTransMomentum)/trackTruthTransMomentum );
					tracking_reconDiffVsRange_CCQE_proton_h2d.Fill( (trackTruthTransMomentum-trackTransMomentum)/trackTruthTransMomentum,range );
				  }
				}
				goodEvent = true;
			}
			else goodEvent = false;
		  }
		}
	} // end loop over tpc tracks

	/// --- Scint HITS ---- ///
	/// --- PiZero HITS ---- ///
	PiZeroFinder * piZeroMgr = new PiZeroFinder();
        std::vector<ScintHitCollection> piZeroTrackVector;
        piZeroTrackVector.resize(0);                               //clear the track info from previous event

	std::vector<GeantPrimaryParticle> * prims = trackingRecord_->getPrimariesPtr();

	//convert hits into vector of tracks to make it easier to reconstruct each one
	piZeroTrackVector = trackMgr->PiZeroPhotonTracks(scintSdHits,prims); //tracker.PiZeroTracks(scintSdHits,prims);

	if(scintHitCount>0){
	  int totalPiZeroHitCount = 0;

	  double photonEnergyOne = 0.;
	  double photonEnergyTwo = 0.;

	  double totalPiZeroEDep = 0.;

	  int piZerosInEvent = piZeroMgr->GetNumberOfPiZerosInEvent(prims);

	  //loop over pi zeros
	  for(int k =0;k<piZerosInEvent;k++){
	  	//check the invariant mass
		std::pair<TLorentzVector,TLorentzVector> photonPairKinemat = piZeroMgr->GetPhotonPairKinematics(k+1,prims,piZeroTrackVector,eventVertex);
		double mass = piZeroMgr->ReconstructPiZeroInvariantMass(photonPairKinemat);
		mass *= 1000.; //mass from MeV to GeV
		double piZeroEnergy = piZeroMgr->ReconstructPiZeroEnergy(photonPairKinemat);
		double angle = piZeroMgr->CalculateAngleBetweenPhotons(photonPairKinemat);
		double cosAngle = TMath::Cos(angle);
		if(verbose_>0){
		  std::cout << "\n-------------------------------------------------------------------"
		  	    << "\nFound Pi0 with Invariant Mass [MeV]: " << mass << ", pi0 energy [GeV]: " << piZeroEnergy
			    << "\n-------------------------------------------------------------------";
		}
		if(mass>0){
		  tracking_piZero_invariantMass_all_h1d.Fill(mass);
		  tracking_piZero_photonAngle_all_h1d.Fill(cosAngle);
		  if(piZerosInEvent==1)tracking_piZero_invariantMass_1pi0_h1d.Fill(mass);
		  else if(piZerosInEvent==2)tracking_piZero_invariantMass_2pi0_h1d.Fill(mass);
		  else if(piZerosInEvent==3)tracking_piZero_invariantMass_3pi0_h1d.Fill(mass);
		}
	   } //end loop over pi0s


	  int photonTracksInEvent = piZeroTrackVector.size();
	  int reconPiZeroCount = 0;
	  //loop over photon tracks from pi0s
	  int k =0;

	  while( photonTracksInEvent>k ){
		std::pair<TLorentzVector,TLorentzVector> photonPairKinemat =
			 piZeroMgr->MatchPhotonPair(k+1,piZeroTrackVector,eventVertex,piZeroMassCutValue_*(1./CLHEP::GeV));
		k++;

		//if it matches them it removes them from the vector and need to begin loop again
		if( photonPairKinemat.second.E() >0){
		  k=0;
		  photonTracksInEvent = piZeroTrackVector.size();
		}
		else continue;

		//else find reconstructed energy of the photon
		double piZeroEnergy = piZeroMgr->ReconstructPiZeroEnergy(photonPairKinemat);
		if( piZeroEnergy>0. ){
		  reconPiZeroCount++;
		  tracking_piZero_reconEnergy_all_h1d.Fill(piZeroEnergy);
		  if(piZerosInEvent==1)tracking_piZero_reconEnergy_1pi0_h1d.Fill(piZeroEnergy);
		  else if(piZerosInEvent==2)tracking_piZero_reconEnergy_2pi0_h1d.Fill(piZeroEnergy);
		  else if(piZerosInEvent==3)tracking_piZero_reconEnergy_3pi0_h1d.Fill(piZeroEnergy);
		}

	  }//end loop over photons

	  tracking_piZero_truthCount_all_h1d.Fill(nuE,piZerosInEvent);
	  tracking_piZero_reconCount_all_h1d.Fill(nuE,reconPiZeroCount);
	  std::cout << "\nFound " << reconPiZeroCount << " pi Zeros, actually are " << 	piZerosInEvent << ", Nu Energy = " << nuE;

	} // end scintillator hits analysis
	
	delete piZeroMgr;

	if(totalTransMomentum>0. && goodEvent &&
	  momentumMinSelect_ < nuMom && momentumMaxSelect_ > nuMom){

		ReconData reconstructedNeutrinoEvent(backTracer_[1],nuMom,totalTransMomentum);
		reconstructedNeutrinoEvent.setCCQE(ccqe);
		//add to the reconstructed neutrino vector in ascending order
		insertAscending(reconstructedNeutrinoEvent,reconNeutrinos);

		//select only CCQE events with a lepton as fspl
		if(ccqe){
			double ratio = ccqeNuMomCalcResult/nuMom;
			double ratioDiff = (nuMom-ccqeNuMomCalcResult)/nuMom;

			if(foundMuon && foundProton)tracking_reconDiff_mom_CCQE_muAndProt_h1d.Fill( ratioDiff );
			else if(foundMuon)tracking_reconDiff_mom_CCQE_muOnly_h1d.Fill( ratioDiff );
			else if(foundProton)tracking_reconDiff_mom_CCQE_protonOnly_h1d.Fill( ratioDiff );

			tracking_recon_mom_CCQE_h1d.Fill(totalTransMomentum);	//CCQE events only
			tracking_reconDiff_mom_CCQE_h1d.Fill( ratioDiff );
			tracking_truthVsRecon_mom_CCQE_h2d.Fill(nuMom,ccqeNuMomCalcResult);
			
			if(ccqeNuMomCalcResult>0.){
				reconstructedNeutrinoEvent.setRecon(ccqeNuMomCalcResult);
				insertAscending(reconstructedNeutrinoEvent,reconCCQENeutrinos);

				tracking_recon_mom_CCQE_calc_h1d.Fill(ccqeNuMomCalcResult);
				tracking_reconDiff_mom_CCQE_calc_h1d.Fill( (nuMom-ccqeNuMomCalcResult)/nuMom );
				tracking_truthVsRecon_mom_CCQE_calc_h2d.Fill( nuMom,ccqeNuMomCalcResult );
			}

			if( isBadEvent(ratio,reconstructionCut_) ){
				ReconData tempData(i,nuMom,totalTransMomentum);
				badEventID.push_back(tempData);
			}

			qelccCount++;
		}//end ccqe

		tracking_recon_mom_h1d.Fill(totalTransMomentum);
		tracking_reconDiff_mom_h1d.Fill( (nuMom-totalTransMomentum)/nuMom );
		tracking_truthVsRecon_mom_h2d.Fill(nuMom,totalTransMomentum);
		tracking_truthVsDiff_mom_h2d.Fill( nuMom,((nuMom-totalTransMomentum)/nuMom) );
	}//end tpc recon

	//increment tpc count
	tpcFidCount++;

	if(verbose_>1){
        	std::cout << "\n ================================================================ "
			  << "\n Event: " << backTracer_[1] 
			  << "\n Tpc Hits: " << tpcHitCount
			  << "\n Scint Hits: " << scintHitCount
		  	  << "\n Track Hits: " << tpcTrackHitCount
		  	  << "\n Total Energy Deposited [MeV]: " << totalEDep
		  	  << "\n Total Energy Deposited2 [MeV]: " << totalEDep2
		  	  << "\n Calculated Reconstructed Momentum [GeV/c]: " << ccqeNuMomCalcResult
		  	  << "\n Total Reconstructed Momentum [GeV/c]: " << totalTransMomentum
		  	  << "\n Total Neutrino Truth Momentum [GeV/c]: " << nuMom
		  	  << "\n *** Ratio: " << totalTransMomentum/nuMom 
			  << "\n ================================================================ " << std::endl;
	}

      	if(verbose_>2)std::cout << "\n*** ENTRY: " << i << ", tpcFidCount = " <<tpcFidCount << ", qelccCount = " <<qelccCount;
	//if(verbose_<=0)progEntries.showPercent(i,detHits_nEntries,0,0);

      }//end loop over detector tree

      delete trackMgr;
    } //end detector hits

    this->deleteTrees();
    datafile.Close();

    //get and print statistics
    if(stats_){
	double tmp_exposure;
	if(verbose_>0){
		std::cout << "\n";
		stats_->printToStream(std::cout);
	}
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
    badFileVector.push_back(badEventID);
    if(verbose_<=0)progFiles.showPercent(j,runs,runs,0);
  } //end loop over files

  std::cout << "\n------------------------------------------------------------------------"
		<< "\nTotal Exposure [p.o.t]:                            "
		<< exposure
		<< "\nAverage Exposure [p.o.t]:                          "
		<< exposure/(runs_-badFileCount)
		<< "\nTotal number of neutrino interactions in target:   "
		<< totalNuEvents
	 	<< "\nTPC Interactions:                                  " 
		<< tpcFidCount
	 	<< "\nTPC CCQE Interactions:                             " 
		<< qelccCount << " (" << qelccCount*100./tpcFidCount << " %)"
		<< "\n------------------------------------------------------------------------\n"
		<< "\n====================================================="
		<< "\n Files that could not be read: " << badFileCount 
		<< "\n=====================================================";

  if(leptonTruthInfo)delete leptonTruthInfo;
  if(protonTruthInfo)delete protonTruthInfo;
}

void myReconstructor::write(){

  std::cout << "\n-----------------------------------------------------------------------------------------"
	    << "\n  Writing to file: " << outputFileName_.c_str()
	    << "\n-----------------------------------------------------------------------------------------\n";

  TFile histofile(outputFileName_.c_str(),"recreate");

  nuEvent_truth_energy_h1d.SetMarkerColor(kBlue);
  nuEvent_truth_energy_h1d.Write();

  sagitta_reconDiff_h1d.Write();

  tracking_track_length_fspl_h1d.Write();
  tracking_track_length_proton_h1d.Write();
  tracking_track_length_pion_h1d.Write();
  tracking_recon_mom_h1d.Write();
  tracking_recon_mom_CCQE_h1d.Write();
  tracking_recon_mom_CCQE_calc_h1d.Write();
  tracking_reconDiff_mom_h1d.Write();
  tracking_reconDiff_mom_lepton_h1d.Write();
  tracking_reconDiff_mom_proton_h1d.Write();
  tracking_reconDiff_mom_CCQE_h1d.Write();
  tracking_reconDiff_mom_CCQE_lepton_h1d.Write();
  tracking_reconDiff_mom_CCQE_proton_h1d.Write();
  tracking_reconDiff_mom_CCQE_calc_h1d.Write();
  tracking_reconDiff_mom_CCQE_muAndProt_h1d.Write();
  tracking_reconDiff_mom_CCQE_muOnly_h1d.Write();
  tracking_reconDiff_mom_CCQE_protonOnly_h1d.Write();

  tracking_piZero_photonAngle_all_h1d.Write();
  tracking_piZero_invariantMass_1pi0_h1d.Write();
  tracking_piZero_invariantMass_2pi0_h1d.Write();
  tracking_piZero_invariantMass_3pi0_h1d.Write();
  tracking_piZero_invariantMass_all_h1d.Write();
  tracking_piZero_reconEnergy_1pi0_h1d.Write();
  tracking_piZero_reconEnergy_2pi0_h1d.Write();
  tracking_piZero_reconEnergy_3pi0_h1d.Write();
  tracking_piZero_reconEnergy_all_h1d.Write();

  tracking_piZero_truthCount_all_h1d.Write();
  tracking_piZero_reconCount_all_h1d.Write();

  nuEvent_XY_position_h2d.Write();
  nuEvent_XZ_position_h2d.Write();

  sagitta_truthVsRecon_h2d.Write();

  tracking_truthVsRecon_mom_h2d.Write();
  tracking_truthVsRecon_mom_lepton_h2d.Write();
  tracking_truthVsRecon_mom_proton_h2d.Write();
  tracking_truthVsRecon_mom_CCQE_h2d.Write();
  tracking_truthVsRecon_mom_CCQE_calc_h2d.Write();
  tracking_truthVsRecon_mom_CCQE_lepton_h2d.Write();
  tracking_truthVsRecon_mom_CCQE_proton_h2d.Write();
  tracking_truthVsDiff_mom_h2d.Write();
  tracking_reconDiffVsRange_lepton_h2d.Write();
  tracking_reconDiffVsRange_proton_h2d.Write();
  tracking_reconDiffVsRange_CCQE_lepton_h2d.Write();
  tracking_reconDiffVsRange_CCQE_proton_h2d.Write();
  tracking_rangeVsEDep_h2d.Write();
  tracking_rangeVsMom_h2d.Write();
  tracking_EDepVsMom_h2d.Write();
  tracking_fullyContained_Mom_truthVsRecon_h2d.Write();

  tracking_recon_mom_grph.Write();
  tracking_recon_mom_CCQE_grph.Write();

  std::cout << "\n----------------------------------"
	    << "\n             Closing...           "
	    << "\n----------------------------------\n";

  histofile.Close();
}

void myReconstructor::makeGraphs(){

  //number of bins
  int numberOfBins = tracking_truthVsRecon_mom_h2d.GetNbinsX()/factor;
  //bin width
  double binWidth = tracking_truthVsRecon_mom_h2d.GetBinWidth(0)*factor;
  std::vector<double> xBins;
  std::vector<double> xErrorBins;

  //calculate the means and errors for each point on graph -stored in vector
  calculateMeanAndErrors(reconNeutrinos,neutrinoReconMeanValues,neutrinoReconErrorValues);
  calculateMeanAndErrors(reconCCQENeutrinos,neutrinoCCQEReconMeanValues,neutrinoCCQEReconErrorValues);

  //fill the x axis
  for(int xIndex=0;xIndex<numberOfBins-1;xIndex++){
	double xBin = binWidth*(xIndex + 0.5);
	double xErrorBin = binWidth/2.;
	xBins.push_back(xBin);
	xErrorBins.push_back(xErrorBin);

	//some output
	//std::cout << "\n\t(x,y,dx,dy) : (" << xBin << ", " << neutrinoReconMeanValues.at(xIndex) << ", " << xErrorBin << ", "
	//		 << neutrinoReconErrorValues.at(xIndex) << ")" << std::endl;
	//std::cout << "\n\t(x,y,dx,dy) : (" << xBin << ", " << neutrinoCCQEReconMeanValues.at(xIndex) << ", " << xErrorBin << ", " 
	//		 << neutrinoCCQEReconErrorValues.at(xIndex) << ")" << std::endl;
  }

  tracking_recon_mom_grph = TGraphErrors(numberOfBins, &xBins[0], &neutrinoReconMeanValues[0],&xErrorBins[0],&neutrinoReconErrorValues[0]);
  tracking_recon_mom_grph.SetTitle("tracking_recon_mom_grph");
  tracking_recon_mom_grph.SetName("tracking_recon_mom_grph");

  tracking_recon_mom_CCQE_grph = TGraphErrors(numberOfBins, &xBins[0], &neutrinoCCQEReconMeanValues[0],&xErrorBins[0],&neutrinoCCQEReconErrorValues[0]);
  tracking_recon_mom_CCQE_grph.SetTitle("tracking_recon_mom_CCQE_grph");
  tracking_recon_mom_CCQE_grph.SetName("tracking_recon_mom_CCQE_grph");
}

bool myReconstructor::inFiducialVolume(NeutrinoEvent* nu_event,double fidCut){

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

bool myReconstructor::isCCQE(NeutrinoEvent* nu_event){

  bool cc = nu_event->getQELCC();
  ParticleDescrShortRecord fsplParticle = nu_event->getFspl();
  int fsplPDG = fsplParticle.getPDG();
  double nuEnergy = nu_event->getNuEnergy() * (1./CLHEP::GeV);	//should be the same
  std::vector<ParticleDescrShortRecord> fssParticles = nu_event->getFssVector();
  int fssCount = fssParticles.size();

  bool isccqe = false;
  bool protonSet = false;
  bool chargedLepton = false;

  //check it is a charged lepton, -1 selects all flavours together
  if((fsplPDG == 13 || fsplPDG== -13 || fsplPDG==11 ||fsplPDG==-11 || fsplPDG==15 || fsplPDG==-15) && leptonPdg_==-1)chargedLepton=true;
  else if(fsplPDG == leptonPdg_)chargedLepton=true;
  else chargedLepton = false;

  //loop over secondaries to get a rigorous CCQE event definition
  if(cc && fssCount <= 3 && chargedLepton){

	  (*leptonTruthInfo) = fsplParticle;

    	  for(int index=0;index<fssCount; index++){
		int particlePDG = fssParticles.at(index).getPDG();
		
		//ccqe event should have one proton and possibly a recoil nucleus
 		if(particlePDG == 2212 && !protonSet){
			isccqe = true;
			protonSet = true;
			(*protonTruthInfo) = fssParticles.at(index);
		}
	  	else if(protonSet && particlePDG < 200000){
			isccqe = false;	//only allow for the other particle to be a nucleus recoil
			//protonTruthInfo = NULL;
		}
    	  }
    }

  else{
	isccqe = false;
	(*leptonTruthInfo) = ParticleDescrShortRecord();
	(*protonTruthInfo) = ParticleDescrShortRecord();
  }

  return isccqe;
}

bool myReconstructor::isBadEvent(double reconRatio,double cutOff){
	if ( (reconRatio < (1. - cutOff) ) || (reconRatio > (1. + cutOff) ) )return true;
	else return false;
}

void myReconstructor::showBadEvents(){

  int files = badFileVector.size();
  int badCount = 0;

  std::cout << "\n******** Badly reconstructed Geant Event IDs ********";

  for(int k=0;k<files;k++){
	
	int tempSize = badFileVector.at(k).size();
	std::cout << "\nFile Number: " << k;

	for(int l=0;l<tempSize;l++){
		//std::cout << "\n\tEvent Number: " << badFileVector.at(k).at(l);
		badFileVector.at(k).at(l).printToStream(std::cout);
	}
	
	badCount += tempSize;
  }

  std::cout << "\nTotal Number of Bad Events: " << badCount <<std::endl;

}

void myReconstructor::insertAscending(ReconData neutrino,std::vector<ReconData> &neutrinoData){

  int size = neutrinoData.size();
  double truthMom = neutrino.getTruth();
  
  //insert elements 
  for(int pos=0;pos<=size;pos++){
		
  	//first element
  	if(size == 0){
		neutrinoData.push_back(neutrino);
		break;
	}

	double tempTruthMom = neutrinoData.at(pos).getTruth();
	//second element
	if(size == 1){
		if(truthMom > tempTruthMom)neutrinoData.push_back(neutrino);
		else neutrinoData.insert(neutrinoData.begin(),neutrino);
		break;
	}

	//other elements
	//if it reaches the end of the vector then it must be the largest
	if(pos == size-1){
		neutrinoData.push_back(neutrino);
		break;
	}

	double tempNextTruthMom = neutrinoData.at(pos+1).getTruth();

	//for vectors larger than 1 entry
	if(truthMom > tempTruthMom && truthMom < tempNextTruthMom){
		neutrinoData.insert(neutrinoData.begin()+pos+1,neutrino);
		break;
	}
	//insert at the begining of the vector
	else if(truthMom <= tempTruthMom){
		neutrinoData.insert(neutrinoData.begin()+pos,neutrino);
		break;
	}

	//size = neutrinoData.size();
  }

}

void myReconstructor::calculateMeanAndErrors(std::vector<ReconData> &neutrinoData,std::vector<double> &meanVector,std::vector<double> &errorVector){

  //size of data
  int dataSize = neutrinoData.size();

  //use bin width from histograms
  double binWidth = tracking_truthVsRecon_mom_h2d.GetBinWidth(0)*factor;
  //number of bins
  int numberOfBins = tracking_truthVsRecon_mom_h2d.GetNbinsX()/factor;

  //create a vector to store all values that fit in the bin
  std::vector<double> reconValuesInBin;
  //record last position in neutrino vector
  int lastIndex = 0;

  //loop over bins
  for(int bin=0;bin<numberOfBins;bin++){

	double lowerBound = binWidth*bin;
	double upperBound = binWidth*(bin+1);

	//loop over neutrino data
	for(int pos=lastIndex;pos<dataSize;pos++){
	
		double tempTruthMom = neutrinoData.at(pos).getTruth();
		double tempReconMom = neutrinoData.at(pos).getRecon();

		//does it fit in the bin?
		if(tempTruthMom > lowerBound && tempTruthMom <= upperBound)reconValuesInBin.push_back(tempReconMom);
		else{
			//calculate mean and add it to the mean vector
			double sum = 0.0;
			double diffSquared = 0.0;
			for(int i =0;i<reconValuesInBin.size();i++){
				double reconMomInBin = reconValuesInBin.at(i);
				sum += reconMomInBin;
			}
			double mean = 0.0;
			if(reconValuesInBin.size() > 0 )mean = sum / reconValuesInBin.size();
			meanVector.push_back(mean);

			for(int i =0;i<reconValuesInBin.size();i++){
				double reconMomInBin = reconValuesInBin.at(i);
				diffSquared += (reconMomInBin - mean) * (reconMomInBin - mean);
			}
			double sigma = 0.0;
			double error = 0.0;
  			if(mean != 0 && reconValuesInBin.size() > 1)sigma = TMath::Sqrt(diffSquared / (reconValuesInBin.size() -1));
			if(sigma !=0 && reconValuesInBin.size() > 0)error = TMath::Sqrt(1./(reconValuesInBin.size()))*sigma;
			errorVector.push_back(error);
			//record last position in neutrino vector
			lastIndex = pos;

			//clear for the next bin
			reconValuesInBin.clear();
			break;
		}
	}
  }
  
}

int main(int argc, char ** argv) {
  try {
    myReconstructor reconstructor;
    reconstructor.initDataCards();
    reconstructor.getRunTimeArguments(argc, argv);
    reconstructor.loadDataCards();
    reconstructor.initHistos();
    reconstructor.reconstruct();
    reconstructor.makeGraphs();
    reconstructor.write();
    if(reconstructor.getVerbose()>1)reconstructor.showBadEvents();

  } catch (LbnoException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }

  return 0;
}

///--------- These functions moved to classes TpcMomSmear and Tracker in recon folder -----------------///
/*
double myReconstructor::getTrackLength(HitCollection &track){

	//get the range of the track
	TLorentzVector initPos = track.front().getPosition() * (1./CLHEP::m);
	TLorentzVector lastPos = track.back().getPosition() * (1./CLHEP::m);
        double trackLength = TMath::Sqrt( TMath::Power(lastPos.X() - initPos.X(),2) 
				  + TMath::Power(lastPos.Y() - initPos.Y(),2)  
				  + TMath::Power(lastPos.Z() - initPos.Z(),2) );

	trackLength *= CLHEP::m;
	return trackLength;// * (1./CLHEP::m);	//in meters

}

double myReconstructor::getTruthTransMomentum(HitCollection &track){
	
	double momX = track.front().getP4().X() * (1./CLHEP::GeV);
	double momY = track.front().getP4().Y() * (1./CLHEP::GeV);
	double momZ = track.front().getP4().Z() * (1./CLHEP::GeV);
	double momentumTrans = TMath::Sqrt((momY*momY) + (momZ*momZ));

	momentumTrans *= CLHEP::GeV;
	return momentumTrans;	//in GeV/c
}

double myReconstructor::getTruthMomentum(HitCollection &track){
	
	double momX = track.front().getP4().X() * (1./CLHEP::GeV);
	double momY = track.front().getP4().Y() * (1./CLHEP::GeV);
	double momZ = track.front().getP4().Z() * (1./CLHEP::GeV);
	double momentum = TMath::Sqrt((momX*momX) + (momY*momY) + (momZ*momZ));

	momentum *= CLHEP::GeV;
	return momentum;	//in GeV/c
}

double myReconstructor::getReconTransMomentum(double trackLength,double sagitta){

	//if it is less than the resolution then let the sagitta = resolution
	if(sagitta<deltaSagitta)sagitta = deltaSagitta;

	//use 0 as the mark that the sagitta is zero and you cannot make a measurement of the sagitta
	if(sagitta==0)return 0.0;

	double reconTransMomentum = (magField_*(1./CLHEP::tesla) * trackLength*(1./CLHEP::m) * trackLength*(1./CLHEP::m))/(26.7*sagitta*(1./CLHEP::m));
	
	reconTransMomentum *= CLHEP::GeV;
	return reconTransMomentum;	// in GeV/c
}

double myReconstructor::getTrackSagitta(HitCollection &track,double transMom){

	double trackLength = getTrackLength(track);	// in meters
	//trackLength *= CLHEP::m;

	double sagitta = (magField_*(1./CLHEP::tesla)* trackLength*(1./CLHEP::m) * trackLength*(1./CLHEP::m))/(26.7*transMom*(1./CLHEP::GeV) );
	sagitta *= CLHEP::m;
	
	return sagitta;
}

double myReconstructor::getSmearSagitta(double truthSag){

	double sagittaSmear = rand->Gaus(truthSag*(1./CLHEP::m),deltaSagitta*(1./CLHEP::m));
	sagittaSmear *= CLHEP::m;

	return sagittaSmear;
}

double myReconstructor::particleMomentum(HitCollection &track){

	//get the range of the track
        double particleTrackLength = getTrackLength(track);// * (1./CLHEP::m);
	//particleTrackLength *= CLHEP::m;

	//get the truth trans momentum
	double particleMomTrans_truth = getTruthTransMomentum(track);// * (1./CLHEP::GeV);
	//particleMomTrans_truth *= CLHEP::GeV;

	double particleMom_truth = getTruthMomentum(track);// * (1./CLHEP::GeV);
	//particleMom_truth *= CLHEP::GeV;
	
	// calculate the sagitta from truth p
	double particleTrackSagitta_cal = getTrackSagitta(track,particleMomTrans_truth);// *(1./CLHEP::m);	
	//particleTrackSagitta_cal *= CLHEP::m;

	// draw from gaussian of recalculated sagitta a random value
	double particleTrackSagitta_rand = getSmearSagitta(particleTrackSagitta_cal);// *(1./CLHEP::m);
	//particleTrackSagitta_rand *= CLHEP::m;
	
	//if(particleTrackSagitta_rand<0.)particleTrackSagitta_rand = -particleTrackSagitta_rand;//*1.0e-6;	//don't allow negative values for the sagitta

	// reconstructed transverse momentum is then recalculated from gaussian values
	double particleMomTrans_recon = getReconTransMomentum(particleTrackLength,particleTrackSagitta_rand);//* (1./CLHEP::GeV);
	//particleMomTrans_recon *= CLHEP::GeV;

	double deltaP_trans = deltaSagitta*particleMomTrans_recon/(particleTrackSagitta_rand);//* (1./CLHEP::GeV);
	//deltaP_trans *= CLHEP::GeV;

	return particleMomTrans_recon;
}

//ccqe defined as a lepton (e,mu,tau) in the final state with a proton only!
//nu_{l} + n -> l + p
double myReconstructor::ccqeReconstruct(HitCollection &lepton,NeutrinoEvent* nu_event){
	 
	if(!protonTruthInfo)return -1.;
		 			
	TLorentzVector initPos = lepton.at(0).getPosition() * (1./CLHEP::m);
	TLorentzVector nextPos = lepton.at(1).getPosition() * (1./CLHEP::m);
	TLorentzVector leptonDir = (nextPos - initPos);
	TLorentzVector beamDir(0.,0.,1.);

	//angle is the projection of leptonDir onto beam axis (z-axis)
	double cosTheta = leptonDir.Dot(beamDir) / leptonDir.Mag();
	if(cosTheta<0)cosTheta= -cosTheta;	//cos is an even function but only take positive value
	
	//mass difference squared between neutron and proton = m_n*m_n - m_p*m_p in (GeV/c2)2			
	double deltaMassSquared = 2.42867e-3;
	double protonMass = protonTruthInfo->getMass() * (1./CLHEP::GeV);	
	double leptonMass = 0.;
	leptonMass = nu_event->getFspl().getMass() * (1./CLHEP::GeV);
	double leptonMomRecon = particleMomentum(lepton) * (1./CLHEP::GeV);
	if(verbose_>5){
		std::cout << "\nPROTON MASS: " << protonMass << " GeV/c2";
		std::cout << "\nLEPTON MASS: " << leptonMass << " GeV/c2";
		std::cout << "\nLEPTON MOM RECON: " << leptonMomRecon << " GeV/c";
	}

	double leptonEnergyRecon = TMath::Sqrt( (leptonMomRecon*leptonMomRecon) + (leptonMass*leptonMass) );

	//Apply formulae to reconstruct CCQE neutrino using fspl
	double ccqeNeutrinoMom = ( (leptonEnergyRecon*protonMass) -
				(leptonMass*leptonMass/2.0) ) /
				(protonMass - leptonEnergyRecon + (leptonMomRecon*cosTheta) );

	return ccqeNeutrinoMom;
}

std::vector<HitCollection> myReconstructor::HitsToTracks(HitCollection &hitsVector){

  std::vector<HitCollection> trackVector;
  trackVector.resize(0);
  int trackVtrSize = trackVector.size();

	//loop over hits to sort into a vector of tracks
	for(HitCollection::iterator it = hitsVector.begin(); it != hitsVector.end(); ++it){

		  bool trackSet = false;

		  //get the trackID for hit
		  int fssTrackID = it->getTrackID();
		  int fssParentID = it->getParentID();
		  if(fssParentID != 0)continue;		//skip this hit as only concerned with primary tracks

		  //create a temp SDHit
		  SDHit tempSDHit(*it);

		  trackVtrSize = trackVector.size();

		  // for the first track - to initialise the vector
		  if(trackVtrSize == 0 ){
		  	HitCollection tempVtr;					//create a temp vector for hits
		  	tempVtr.push_back(tempSDHit);				//add this hit to the vector
		  	trackVector.push_back(tempVtr);				//first hit in event should be added
			trackSet = true;
		  }
				
		  //write to the vector while searching through to see if it already exists
		  //hits are recorded in time sequentially t2>t1>t0
		  for(int k = 0;k<trackVtrSize;k++){
			int tempID = 0;

			SDHit tempHit(trackVector.at(k).front());		//create a temp hit to read previous track entries	
			tempID = tempHit.getTrackID();				//read the trackID
						
			//does the track already exist? - append to end of track vector
			if(fssTrackID == tempID && trackSet == false){
				trackVector.at(k).push_back(*it);
				trackSet = true;
			}
			//is it a new trackID? - create a new hit vector at end of track vector
			else if( (k+1) == trackVtrSize && trackSet==false){
				HitCollection tempVtr;				//create a temp vector for hits
				tempVtr.push_back(*it);				//add this hit to the vector
				trackVector.push_back(tempVtr);			//first hit in event should be added
				trackSet = true;
			}
			//if the track has been set then break the loop
			if(trackSet ==true) break;
			
		  	//trackVtrSize = trackVector.size();			//update the track vector size
		  }
		
	}

  return trackVector;

}

std::vector<ScintHitCollection> myReconstructor::HitsToTracks(ScintHitCollection &hitsVector){

  std::vector<ScintHitCollection> trackVector;
  trackVector.resize(0);
  int trackVtrSize = trackVector.size();

	//loop over hits to sort into a vector of tracks
	for(ScintHitCollection::iterator it = hitsVector.begin(); it != hitsVector.end(); ++it){

		  bool trackSet = false;

		  //get the trackID for hit
		  int fssTrackID = it->getTrackID();
		  int fssParentID = it->getParentID();
		  //if(fssParentID != 0)continue;		//skip this hit as only concerned with primary tracks

		  //create a temp SDHit
		  scintHit tempSDHit(*it);

		  trackVtrSize = trackVector.size();

		  // for the first track - to initialise the vector
		  if(trackVtrSize == 0 ){
		  	ScintHitCollection tempVtr;				//create a temp vector for hits
		  	tempVtr.push_back(tempSDHit);				//add this hit to the vector
		  	trackVector.push_back(tempVtr);				//first hit in event should be added
			trackSet = true;
		  }
				
		  //write to the vector while searching through to see if it already exists
		  //hits are recorded in time sequentially t2>t1>t0
		  for(int k = 0;k<trackVtrSize;k++){
			int tempID = 0;

			scintHit tempHit(trackVector.at(k).front());		//create a temp hit to read previous track entries	
			tempID = tempHit.getTrackID();				//read the trackID
						
			//does the track already exist? - append to end of track vector
			if(fssTrackID == tempID && trackSet == false){
				trackVector.at(k).push_back(*it);
				trackSet = true;
			}
			//is it a new trackID? - create a new hit vector at end of track vector
			else if( (k+1) == trackVtrSize && trackSet==false){
				ScintHitCollection tempVtr;				//create a temp vector for hits
				tempVtr.push_back(*it);				//add this hit to the vector
				trackVector.push_back(tempVtr);			//first hit in event should be added
				trackSet = true;
			}
			//if the track has been set then break the loop
			if(trackSet ==true) break;
			
		  	//trackVtrSize = trackVector.size();			//update the track vector size
		  }
		
	}

  return trackVector;

}

int myReconstructor::MatchScintToTpcTracks(ScintHitCollection &scintTrack,std::vector<HitCollection> &tpcTracksVector){

	int matchedTrackPos = -1;
	
	//get the position and momentum of the first scint hit
	TLorentzVector scintFirstHitPos = scintTrack.front().getPosition() * (1./CLHEP::m);
	TLorentzVector scintFirstHitMom = scintTrack.front().getP4() * (1./CLHEP::GeV);

	TLorentzVector diffVector;
	double magnitude = 0.;
	double radius = TMath::Abs(5.0*scintResolution_* (1./CLHEP::m));
	//std::cout << "\nRADIUS = " << radius;
	double thetaCut = 1.0;//in degrees

	int tracksMatched = 0;
	
	//loop over tpc tracks
	while(true){
	  for(int i=0;i<tpcTracksVector.size();i++){
		HitCollection tempTpcTrackVtr = tpcTracksVector.at(i);
		TLorentzVector tpcLastHitPos = tempTpcTrackVtr.back().getPosition() * (1./CLHEP::m);
		TLorentzVector tpcLastHitMom = tempTpcTrackVtr.back().getP4() * (1./CLHEP::GeV);

		diffVector = tpcLastHitPos - scintFirstHitPos;
		magnitude = TMath::Sqrt(diffVector.X()*diffVector.X() + diffVector.Y()*diffVector.Y() +
				 diffVector.Z()*diffVector.Z());
		//std::cout << "\nMAGNITUDE = " << magnitude;
	
		//check if its in the sphere
		if(magnitude<=radius){
		  tracksMatched++;
		  
		  //theta must be less than 5 deg
		  double dotProd = tpcLastHitMom.X()*scintFirstHitMom.X() +
				     tpcLastHitMom.Y()*scintFirstHitMom.Y() +
				     tpcLastHitMom.Z()*scintFirstHitMom.Z();
		  double magTpcMom = TMath::Sqrt(tpcLastHitMom.X()*tpcLastHitMom.X() +
					tpcLastHitMom.Y()*tpcLastHitMom.Y() +
					tpcLastHitMom.Z()*tpcLastHitMom.Z());
		  double magScintMom = TMath::Sqrt(scintFirstHitMom.X()*scintFirstHitMom.X() +
					scintFirstHitMom.Y()*scintFirstHitMom.Y() +
					scintFirstHitMom.Z()*scintFirstHitMom.Z());
		  double cosTheta = dotProd / (magTpcMom*magScintMom);
		  double theta = TMath::Abs(TMath::ACos(cosTheta)*180/TMath::Pi());
		  //std::cout << "\nTHETA = " << theta;
		  //match to the momentum of the track as well
		  if(theta<=thetaCut){
			matchedTrackPos = i;
		  }
		}
	    }
	  if(tracksMatched==1)break;
	  //if more than one is matched increase the restriction on the angle
	  else if(tracksMatched>1){
		thetaCut = thetaCut-0.1;
		radius = radius-1.0;
		if(radius<0 || thetaCut<0)break;
	  }
	  //loop over again with wider radius and wider theta
	  else if(tracksMatched==0){
		thetaCut = thetaCut+0.1;
		radius = radius+1.0;
		if(radius>10. || thetaCut>10.)break;		
	  }
	}

	return matchedTrackPos;
}

*/

