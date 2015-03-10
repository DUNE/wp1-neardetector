//____________________________________________________________________________
/*!

\class    gunPlot

\brief    Program to make basic plots from particle gun

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author	  Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Oct 2014
\last update Oct 2014

*/
//____________________________________________________________________________
#include <iostream>
#include <vector>
#include <map>

#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TChain.h>

#include "PiZeroRecon.hh"
#include "Tracker.hh"
#include "GasTPCPlotter.hh"

class gunPlot : public GasTPCPlotter {
 public:
  gunPlot() {}
  virtual ~gunPlot();

  void plotGraph();

  void plot();
  void initHistos();
  void write();

  bool setupTrees(TFile & file);
  void initDataCards();
  void loadDataCards();

 private:
  TFile file_;

  TH1F energy_diff_h1d_;
  TH1F reconMass_h1d_;
  TH2F energy_inVsOut_h2d_;
  TH2F angle_truthVsRecon_h2d_; 
  TH2F angleVsReconMass_h2d_;
  TH2F angleVsEnergyDiff_h2d_;

  TGraphErrors energy_inVsMeanOut_gr1_;

  double scalingFactor_,energyCut_,diffCut_;
  double energyResolution_;
  double minEnergy_;
  int numberOfHits_;
  bool useTruth_;
};

gunPlot::~gunPlot(){
}

void gunPlot::initHistos(){

  int binning = maxEnergy_*100;

  //declare histograms
  energy_diff_h1d_.SetNameTitle("energy_diff_h1d_","Energy differnce between in and out");
  energy_diff_h1d_.SetBins(200,-1,1);

  reconMass_h1d_.SetNameTitle("reconMass_h1d_","reconMass_h1d_");
  reconMass_h1d_.SetBins(400,0.,400);

  energy_inVsOut_h2d_.SetNameTitle("energy_inVsOut_h2d_", "Energy in Vs out");
  energy_inVsOut_h2d_.SetBins(binning,0,maxEnergy_,binning,0,maxEnergy_);

  angle_truthVsRecon_h2d_.SetNameTitle("angle_truthVsRecon_h2d_", "Angle truth Vs recon");
  angle_truthVsRecon_h2d_.SetBins(200,-1,1,200,-1,1);

  angleVsReconMass_h2d_.SetNameTitle("angleVsReconMass_h2d_","angleVsReconMass_h2d_");
  angleVsReconMass_h2d_.SetBins(200,-1,1,400,0.,400);

  angleVsEnergyDiff_h2d_.SetNameTitle("angleVsEnergyDiff_h2d_","angleVsEnergyDiff_h2d_");
  angleVsEnergyDiff_h2d_.SetBins(200,-1,1,200,-1,1);
}

void gunPlot::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardInt("numberOfEvents", -1);
  cards_->AddDataCardInt("requiredHits", 100);
  cards_->AddDataCardDouble("maxEnergy",10.);//in GeV
  cards_->AddDataCardDouble("minEnergy",0.);//in GeV
  cards_->AddDataCardDouble("energyResolution",1.8);//in MeV
  cards_->AddDataCardDouble("diffCut",2.0);
  cards_->AddDataCardDouble("scalingFactor",0.568);
  cards_->AddDataCardDouble("energyCut",0.1);
  cards_->AddDataCardBool("useTruth",0);
}

void gunPlot::loadDataCards() {
  inputFileName_  	= cards_->fetchValueString("dataFileName");
  outputFileName_  	= cards_->fetchValueString("plotFileName");
  geomFileName_  	= cards_->fetchValueString("geomFileName");
  selectedVolume_  	= cards_->fetchValueString("selectedVolume");
  events_		= cards_->fetchValueInt("numberOfEvents");
  verbose_ 		= cards_->fetchValueInt("plotterVerbose");
  numberOfHits_ 	= cards_->fetchValueInt("requiredHits");
  maxEnergy_ 		= cards_->fetchValueDouble("maxEnergy");
  minEnergy_ 		= cards_->fetchValueDouble("minEnergy");
  energyResolution_ 	= cards_->fetchValueDouble("energyResolution");
  energyCut_ 		= cards_->fetchValueDouble("energyCut");
  diffCut_ 		= cards_->fetchValueDouble("diffCut");
  scalingFactor_	= cards_->fetchValueDouble("scalingFactor");
  useTruth_		= cards_->fetchValueBool("useTruth");
}

bool gunPlot::setupTrees(TFile & datafile){

  bool result = false;

  if(datafile.Get("Tracking")){
      detectorHitsTree = (TTree*)datafile.Get("Tracking");
      this->setDataAddress<NeutrinoEvent>(nuEvent_, detectorHitsTree);
      this->setDataAddress<SimulData>(simData_, detectorHitsTree);
      this->setDataAddress<GeantTrackingTruth>(trackingRecord_, detectorHitsTree);
      nuEvents_nEntries = detectorHitsTree->GetEntries();
      detHits_nEntries = detectorHitsTree->GetEntries();

      result = true;

    }
    else{
      detectorHitsTree = NULL;
      nuEvent_ = NULL;
      simData_ = NULL;
      trackingRecord_ = NULL;
      detHits_nEntries = 0;
    }

  return result;
}

void gunPlot::plot() {
 
  //exposure
  double exposure = 0;
  
  TFile datafile(inputFileName_.c_str());

  std::cout << "\n===========================================================";
  std::cout << "\nInput File: "<< inputFileName_;
  std::cout << "\n===========================================================";

  if(datafile.GetNkeys()<1){
	std::cout << "\n\t\t !!!!!!!! Bad file, skipping !!!!!!\n";
	return;
  }

  //set data addresses
  if(!setupTrees(datafile)) return;
  
  if(events_==-1)events_=nuEvents_nEntries;

  ///create a tracker object to sort hits into "tracks"///
  Tracker * trackMgr = new Tracker();

  //loop over tracking processor
  for(int i=0;i<nuEvents_nEntries;++i){

	if(i>events_)break; 
	
	detectorHitsTree->GetEntry(i);

	//progress
	if(i%100==0)std::cout << "\nProgress: " << i*100/double(events_) << "%\r";

	double inEnergy = nuEvent_->getFspl().getP4().E() / 1000.; //in GeV
	double outEnergy =0;

	TLorentzVector eventVertex = nuEvent_->getPosition();

	if(inEnergy>maxEnergy_)continue;
	if(inEnergy<minEnergy_)continue;

	ScintHitCollection hits = simData_->getScintHits();
	//loop over hits to sum up energy deposition
	int numberOfScintHits = hits.size();

	if(verbose_>1){
	  std::cout << "\n\nEVENT: " << i;
	  std::cout << "\n----------------------------------------------";
 	  std::cout << "\nNumber of Hits: " << hits.size();

	  nuEvent_->printToStream(std::cout);		
	  simData_->printToStream(std::cout);
	}

	for(int j=0;j<numberOfScintHits;++j){
	  double edep = hits[j].getEdep();

	  //convert energy resolution into GeV from MeV
	  if( (energyResolution_ / 1000.0) > edep )continue;

	  //std::cout << "\nEdep: " << edep;

	  outEnergy += edep;
	  //if( hits[i].getTrackLeftVolume() )std::cout << "\nTrackLeftVolume: " << hits[i].getTrackLeftVolume();
	}

	//scale recon energy by factor
	outEnergy *= 1./scalingFactor_;

	double diff = (inEnergy - outEnergy)/inEnergy;
	
	std::vector<GeantPrimaryParticle> * prims = trackingRecord_->getPrimariesPtr();
                            
        //convert hits into vector of tracks to make it easier to reconstruct each one
        std::vector<ScintHitCollection> piZeroTrackVector = trackMgr->PiZeroPhotonTracks(hits);//,prims);

	PiZeroRecon * piZeroMgr = new PiZeroRecon();
	piZeroMgr->SetScaleFactor(scalingFactor_);

        std::pair<TLorentzVector,TLorentzVector> photonPairKinematRecon;
        std::pair<TLorentzVector,TLorentzVector> photonPairKinematTruth;
	double reconMass = 0.;
	double reconEnergy = 0.;
	double reconCosAngle = 0. ;
	double truthCosAngle = 0.;
	double photon1Energy = 0.;
	double photon2Energy = 0.;
	double positionDiff = 0.;
	int size1 = 0;
	int size2 = 0;

	if(piZeroTrackVector.size()>1){
          photonPairKinematRecon.first = piZeroMgr->ReconstructPhotonKinematics(piZeroTrackVector.at(0),eventVertex,false,energyResolution_/1000.);
          photonPairKinematRecon.second = piZeroMgr->ReconstructPhotonKinematics(piZeroTrackVector.at(1),eventVertex,false,energyResolution_/1000.);

          photonPairKinematTruth.first = piZeroMgr->ReconstructPhotonKinematics(piZeroTrackVector.at(0),eventVertex,true,energyResolution_/1000.);
          photonPairKinematTruth.second = piZeroMgr->ReconstructPhotonKinematics(piZeroTrackVector.at(1),eventVertex,true,energyResolution_/1000.);

	  if( useTruth_ ){
		photonPairKinematRecon.first.SetX( photonPairKinematTruth.first.X() );
		photonPairKinematRecon.first.SetY( photonPairKinematTruth.first.Y() );
		photonPairKinematRecon.first.SetZ( photonPairKinematTruth.first.Z() );

		photonPairKinematRecon.second.SetX( photonPairKinematTruth.second.X() );
		photonPairKinematRecon.second.SetY( photonPairKinematTruth.second.Y() );
		photonPairKinematRecon.second.SetZ( photonPairKinematTruth.second.Z() );
	  }

	  photon1Energy = photonPairKinematRecon.first.E();
	  photon2Energy = photonPairKinematRecon.second.E();

  	  reconMass = piZeroMgr->ReconstructPiZeroInvariantMass(photonPairKinematRecon)*1000.; //in MeV
	  reconEnergy = piZeroMgr->ReconstructPiZeroEnergy(photonPairKinematRecon);

	  double reconAngle = piZeroMgr->CalculateAngleBetweenPhotons( photonPairKinematRecon );
	  double truthAngle = piZeroMgr->CalculateAngleBetweenPhotons( photonPairKinematTruth );

	  reconCosAngle = TMath::Cos(reconAngle);
	  truthCosAngle = TMath::Cos(truthAngle);

	  size1 = piZeroTrackVector.at(0).size();
	  size2 = piZeroTrackVector.at(1).size();

	  if(verbose_>0){
 	    std::cout << "\nSize1: " << size1;
	    std::cout << "\nSize2: " << size2;
	  }
	}

	if(verbose_>0){
	  std::cout <<"\nIn energy: " << inEnergy;
 	  std::cout <<"\nOut energy: " << outEnergy;
	  std::cout <<"\nPiZero Recon energy: " << reconEnergy << " [GeV]";
	  std::cout <<"\nPiZero Recon mass: " << reconMass << " [MeV]";
	}

	//fill histograms -cut on photon energy
	if( (photon1Energy>energyCut_) && (photon2Energy>energyCut_) &&
	    (numberOfHits_ <= size1 ) && (numberOfHits_ <= size2) &&
	    (TMath::Abs(diff) <= diffCut_) ){
	  energy_inVsOut_h2d_.Fill(inEnergy,outEnergy);
	  energy_diff_h1d_.Fill(diff);
	  angle_truthVsRecon_h2d_.Fill(truthCosAngle,reconCosAngle);
	  reconMass_h1d_.Fill(reconMass);
	  angleVsEnergyDiff_h2d_.Fill(truthCosAngle,diff);
	  angleVsReconMass_h2d_.Fill(truthCosAngle,reconMass);
	}

	delete piZeroMgr;
  }

  plotGraph();

  delete trackMgr;
    
  datafile.Close();


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

void gunPlot::plotGraph(){

  int binning = energy_inVsOut_h2d_.GetXaxis()->GetNbins();
  int ybinning = energy_inVsOut_h2d_.GetYaxis()->GetNbins();

  double xbins[binning];
  double xbinsErrors[binning];
 
  double yMeanValue[binning];
  double yErrorValue[binning];

  double binWidth = energy_inVsOut_h2d_.GetXaxis()->GetBinWidth(0);

  for(int i=0;i<binning;++i){
	xbins[i]= binWidth/2. + i*binWidth;
	xbinsErrors[i]= binWidth/2.;
 
	//get the projection of bin x to y axis
	TH1D *h1 = energy_inVsOut_h2d_.ProjectionY("",i,i);
	
	yMeanValue[i] =h1->GetMean();
	yErrorValue[i] = h1->GetMeanError();

	delete h1;

	std::cout << "\nbin: " << i << ", value: " << xbins[i] << ", mean: " << yMeanValue[i];

  }

  energy_inVsMeanOut_gr1_ = TGraphErrors(binning, &xbins[0], &yMeanValue[0],&xbinsErrors[0],&yErrorValue[0]);
  energy_inVsMeanOut_gr1_.SetTitle("energy_inVsMeanOut_gr1_");
  energy_inVsMeanOut_gr1_.SetName("energy_inVsMeanOut_gr1_");


}

void gunPlot::write(){

  energy_inVsOut_h2d_.Write();
  reconMass_h1d_.Write();
  energy_diff_h1d_.Write();
  energy_inVsMeanOut_gr1_.Write();
  angle_truthVsRecon_h2d_.Write();
  angleVsEnergyDiff_h2d_.Write();
  angleVsReconMass_h2d_.Write();
}

int main(int argc, char ** argv) {
  try {
    gunPlot plotter;
    plotter.initDataCards();
    plotter.getRunTimeArguments(argc, argv);
    plotter.loadDataCards();
    if(!plotter.loadGeom())return 0;
    plotter.initHistos();
    plotter.plot();

  } catch (GasTPCException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }

  return 0;
}


