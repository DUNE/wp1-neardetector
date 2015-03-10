//____________________________________________________________________________
/*!

\class    GasTPCFlux

\brief    Program to count and plot particles reaching detector,
	  leaving detector, etc. Basic particle flux analysis

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  June 2014
\last update June 2014

*/
//____________________________________________________________________________
#include <iostream>
#include <iomanip>
#include <map>

#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3F.h>
#include <TFile.h>
#include <TChain.h>
#include <TGeoVolume.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TLorentzVector.h>

#include "GeometryLoader.hh"
#include "GasTPCPlotter.hh"


class myFlux : public GasTPCPlotter {
 public:
  myFlux() {}
  virtual ~myFlux() {}

  void info();
  void initHistos();
  void write();
  void print();
  TLorentzVector findInteractionNodePosition(int eventid);

  bool loadGeom();
  
  void initDataCards();
  void loadDataCards();

 private:
  TFile file_;

  TH1F fluxE_h1d_;
  
  //kinetic energies
  TH1F protonKinEnergy_nonTPC_h1d_;
  TH1F protonKinEnergy_magnetOnly_h1d_;
  TH1F protonKinEnergy_rockOnly_h1d_;
  TH1F pionKinEnergy_nonTPC_h1d_;
  TH1F pionKinEnergy_magnetOnly_h1d_;
  TH1F pionKinEnergy_rockOnly_h1d_;
  TH1F muonKinEnergy_nonTPC_h1d_;
  TH1F muonKinEnergy_magnetOnly_h1d_;
  TH1F muonKinEnergy_rockOnly_h1d_;

  //6 tpc faces - flux leaving each
  // all particles 
  TH1F zPos_allParticles_kinEnergy_h1d_;
  TH1F zNeg_allParticles_kinEnergy_h1d_;
  TH1F yPos_allParticles_kinEnergy_h1d_;
  TH1F yNeg_allParticles_kinEnergy_h1d_;
  TH1F xPos_allParticles_kinEnergy_h1d_;
  TH1F xNeg_allParticles_kinEnergy_h1d_;
  // muons
  TH1F zPos_muon_kinEnergy_h1d_;
  TH1F zNeg_muon_kinEnergy_h1d_;
  TH1F yPos_muon_kinEnergy_h1d_;
  TH1F yNeg_muon_kinEnergy_h1d_;
  TH1F xPos_muon_kinEnergy_h1d_;
  TH1F xNeg_muon_kinEnergy_h1d_;
  // protons
  TH1F zPos_proton_kinEnergy_h1d_;
  TH1F zNeg_proton_kinEnergy_h1d_;
  TH1F yPos_proton_kinEnergy_h1d_;
  TH1F yNeg_proton_kinEnergy_h1d_;
  TH1F xPos_proton_kinEnergy_h1d_;
  TH1F xNeg_proton_kinEnergy_h1d_;
  // pions
  TH1F zPos_pion_kinEnergy_h1d_;
  TH1F zNeg_pion_kinEnergy_h1d_;
  TH1F yPos_pion_kinEnergy_h1d_;
  TH1F yNeg_pion_kinEnergy_h1d_;
  TH1F xPos_pion_kinEnergy_h1d_;
  TH1F xNeg_pion_kinEnergy_h1d_;
  // gammas
  TH1F zPos_gamma_kinEnergy_h1d_;
  TH1F zNeg_gamma_kinEnergy_h1d_;
  TH1F yPos_gamma_kinEnergy_h1d_;
  TH1F yNeg_gamma_kinEnergy_h1d_;
  TH1F xPos_gamma_kinEnergy_h1d_;
  TH1F xNeg_gamma_kinEnergy_h1d_;
  // electron
  TH1F zPos_electron_kinEnergy_h1d_;
  TH1F zNeg_electron_kinEnergy_h1d_;
  TH1F yPos_electron_kinEnergy_h1d_;
  TH1F yNeg_electron_kinEnergy_h1d_;
  TH1F xPos_electron_kinEnergy_h1d_;
  TH1F xNeg_electron_kinEnergy_h1d_;

  //6 tpc faces - angular distribution
  // all particles 
  TH2F zPos_allParticles_cosTheta_vs_kinEnergy_h2d_;
  TH2F zNeg_allParticles_cosTheta_vs_kinEnergy_h2d_;
  TH2F yPos_allParticles_cosTheta_vs_kinEnergy_h2d_;
  TH2F yNeg_allParticles_cosTheta_vs_kinEnergy_h2d_;
  TH2F xPos_allParticles_cosTheta_vs_kinEnergy_h2d_;
  TH2F xNeg_allParticles_cosTheta_vs_kinEnergy_h2d_;
  // muons
  TH2F zPos_muon_cosTheta_vs_kinEnergy_h2d_;
  TH2F zNeg_muon_cosTheta_vs_kinEnergy_h2d_;
  TH2F yPos_muon_cosTheta_vs_kinEnergy_h2d_;
  TH2F yNeg_muon_cosTheta_vs_kinEnergy_h2d_;
  TH2F xPos_muon_cosTheta_vs_kinEnergy_h2d_;
  TH2F xNeg_muon_cosTheta_vs_kinEnergy_h2d_;
  // protons
  TH2F zPos_proton_cosTheta_vs_kinEnergy_h2d_;
  TH2F zNeg_proton_cosTheta_vs_kinEnergy_h2d_;
  TH2F yPos_proton_cosTheta_vs_kinEnergy_h2d_;
  TH2F yNeg_proton_cosTheta_vs_kinEnergy_h2d_;
  TH2F xPos_proton_cosTheta_vs_kinEnergy_h2d_;
  TH2F xNeg_proton_cosTheta_vs_kinEnergy_h2d_;
  // pions
  TH2F zPos_pion_cosTheta_vs_kinEnergy_h2d_;
  TH2F zNeg_pion_cosTheta_vs_kinEnergy_h2d_;
  TH2F yPos_pion_cosTheta_vs_kinEnergy_h2d_;
  TH2F yNeg_pion_cosTheta_vs_kinEnergy_h2d_;
  TH2F xPos_pion_cosTheta_vs_kinEnergy_h2d_;
  TH2F xNeg_pion_cosTheta_vs_kinEnergy_h2d_;
  // gammas
  TH2F zPos_gamma_cosTheta_vs_kinEnergy_h2d_;
  TH2F zNeg_gamma_cosTheta_vs_kinEnergy_h2d_;
  TH2F yPos_gamma_cosTheta_vs_kinEnergy_h2d_;
  TH2F yNeg_gamma_cosTheta_vs_kinEnergy_h2d_;
  TH2F xPos_gamma_cosTheta_vs_kinEnergy_h2d_;
  TH2F xNeg_gamma_cosTheta_vs_kinEnergy_h2d_;
  // electrons
  TH2F zPos_electron_cosTheta_vs_kinEnergy_h2d_;
  TH2F zNeg_electron_cosTheta_vs_kinEnergy_h2d_;
  TH2F yPos_electron_cosTheta_vs_kinEnergy_h2d_;
  TH2F yNeg_electron_cosTheta_vs_kinEnergy_h2d_;
  TH2F xPos_electron_cosTheta_vs_kinEnergy_h2d_;
  TH2F xNeg_electron_cosTheta_vs_kinEnergy_h2d_;

  //all faces together
  TH2F allFaces_allParticles_cosTheta_vs_kinEnergy_h2d_;
  TH2F allFaces_muon_cosTheta_vs_kinEnergy_h2d_;
  TH2F allFaces_proton_cosTheta_vs_kinEnergy_h2d_;
  TH2F allFaces_pion_cosTheta_vs_kinEnergy_h2d_;
  TH2F allFaces_gamma_cosTheta_vs_kinEnergy_h2d_;
  TH2F allFaces_electron_cosTheta_vs_kinEnergy_h2d_;
  TH2F allFaces_neutron_cosTheta_vs_kinEnergy_h2d_;

  //original node position histograms
  TH2F nodePositionXY_h2d_;
  TH2F nodePositionXZ_h2d_;
  TH2F nodePositionYZ_h2d_;

  double range;

  //counts
  //particles from primary vertex, entering inside target,and leaving target detector
  int primCount,inCount,outCount,sumOutCount;
  int vertexMuonCount,vertexProtCount,vertexPionCount,vertexPiZeroCount,vertexElecCount,vertexGammCount,vertexNeutCount,;
  int primMuonCount,primProtCount,primPionCount,primPiZeroCount,primElecCount,primGammCount,primNeutCount,;
  int inFluxPionCount,outFluxPionCount;
  int inFluxProtCount,outFluxProtCount;
  int inFluxMuonCount,outFluxMuonCount;
  int inFluxElecCount,outFluxElecCount;
  int inFluxGammCount,outFluxGammCount;
  int inFluxNeutCount,outFluxNeutCount,outFluxDownNeutCount;
  int outFluxAllParticleCount;

  //entering inside the volume specified and the number of primary nu interactions inside the part of the geometry
  GeomVolumeRecord volumeCountsEvent,volumeCountsIn;

  double exposure,energyCut_;
  int totalNuEvents;
  
  int badFileCount;

  GeometryLoader * geomLoader;
  TGeoManager *geom;
  TGeoVolume * world;
  TGeoVolume * tpc;
  
  double tpcDepth,tpcHeight,tpcWidth;  
};

void myFlux::initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardString("dataFileName", "nuData.root");
  cards_->AddDataCardString("plotFileName", "nuHistos.root");
  cards_->AddDataCardString("geomFileName", "geometry.root");
  cards_->AddDataCardInt("numberOfRuns", 100);
  cards_->AddDataCardInt("plotterVerbose", 0);
  cards_->AddDataCardDouble("maxEnergy", 30.0);	//in GeV
  cards_->AddDataCardDouble("energyCut",1.0);//in MeV
}

void myFlux::loadDataCards() {
  inputFileName_  	= cards_->fetchValueString("dataFileName");
  outputFileName_  	= cards_->fetchValueString("plotFileName");
  geomFileName_  	= cards_->fetchValueString("geomFileName");
  runs_		  	= cards_->fetchValueInt("numberOfRuns");
  verbose_ 		= cards_->fetchValueInt("plotterVerbose");
  maxEnergy_ 		= cards_->fetchValueDouble("maxEnergy");
  energyCut_ 		= cards_->fetchValueDouble("energyCut");
}

void myFlux::initHistos(){
  //counts
  vertexMuonCount = 0;
  vertexPionCount = 0;
  vertexProtCount = 0;
  vertexPiZeroCount = 0;
  vertexGammCount = 0;
  vertexElecCount = 0;
  vertexNeutCount = 0;

  primCount = 0;
  primMuonCount = 0;
  primPionCount = 0;
  primPiZeroCount = 0;
  primProtCount = 0;
  primGammCount = 0;
  primElecCount = 0;
  primNeutCount = 0;

  inCount =0;
  inFluxMuonCount = 0;
  inFluxNeutCount = 0;
  inFluxGammCount = 0;
  inFluxElecCount = 0;
  inFluxProtCount = 0;
  inFluxPionCount = 0;

  outFluxMuonCount = 0;
  outFluxPionCount = 0;
  outFluxProtCount = 0;
  outFluxNeutCount = 0;
  outFluxGammCount = 0;
  outFluxElecCount = 0;
  outFluxDownNeutCount = 0;
  outFluxAllParticleCount = 0;
  
  badFileCount = 0;

  int binning = maxEnergy_*10.;
  int factor = 4;

  //declare histograms
  protonKinEnergy_nonTPC_h1d_.SetNameTitle("protonKinEnergy_nonTPC_h1d", "proton kinetic energy non TPC");
  protonKinEnergy_nonTPC_h1d_.SetBins(binning, 0., maxEnergy_);
  protonKinEnergy_magnetOnly_h1d_.SetNameTitle("protonKinEnergy_magnetOnly_h1d_","proton kinetic energy from interactions in magnet only");
  protonKinEnergy_magnetOnly_h1d_.SetBins(binning,0.,maxEnergy_);
  protonKinEnergy_rockOnly_h1d_.SetNameTitle("protonKinEnergy_rockOnly_h1d_","proton kinetic energy from interactions in rock only");
  protonKinEnergy_rockOnly_h1d_.SetBins(binning,0.,maxEnergy_);

  pionKinEnergy_nonTPC_h1d_.SetNameTitle("pionKinEnergy_nonTPC_h1d", "pion kinetic energy non TPC");
  pionKinEnergy_nonTPC_h1d_.SetBins(binning, 0., maxEnergy_);
  pionKinEnergy_magnetOnly_h1d_.SetNameTitle("pionKinEnergy_magnetOnly_h1d_","pion kinetic energy from interactions in magnet only");
  pionKinEnergy_magnetOnly_h1d_.SetBins(binning,0.,maxEnergy_);
  pionKinEnergy_rockOnly_h1d_.SetNameTitle("pionKinEnergy_rockOnly_h1d_","pion kinetic energy from interactions in rock only");
  pionKinEnergy_rockOnly_h1d_.SetBins(binning,0.,maxEnergy_);

  muonKinEnergy_nonTPC_h1d_.SetNameTitle("muonKinEnergy_nonTPC_h1d", "muon kinetic energy no TPC ");
  muonKinEnergy_nonTPC_h1d_.SetBins(binning, 0., maxEnergy_);
  muonKinEnergy_magnetOnly_h1d_.SetNameTitle("muonKinEnergy_magnetOnly_h1d_","muon kinetic energy from interactions in magnet only");
  muonKinEnergy_magnetOnly_h1d_.SetBins(binning,0.,maxEnergy_);
  muonKinEnergy_rockOnly_h1d_.SetNameTitle("muonKinEnergy_rockOnly_h1d_","muon kinetic energy from interactions in rock only");
  muonKinEnergy_rockOnly_h1d_.SetBins(binning,0.,maxEnergy_);

  //1D Histograms
  //all particles
  zPos_allParticles_kinEnergy_h1d_.SetNameTitle("zPos_allParticles_kinEnergy_h1d_","particles leaving the +z face of the tpc");
  zPos_allParticles_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  zNeg_allParticles_kinEnergy_h1d_.SetNameTitle("zNeg_allParticles_kinEnergy_h1d_","particles leaving the -z face of the tpc");  zNeg_allParticles_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yPos_allParticles_kinEnergy_h1d_.SetNameTitle("yPos_allParticles_kinEnergy_h1d_","particles leaving the +y face of the tpc");
  yPos_allParticles_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yNeg_allParticles_kinEnergy_h1d_.SetNameTitle("yNeg_allParticles_kinEnergy_h1d_","particles leaving the -y face of the tpc");
  yNeg_allParticles_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xPos_allParticles_kinEnergy_h1d_.SetNameTitle("xPos_allParticles_kinEnergy_h1d_","particles leaving the +x face of the tpc");
  xPos_allParticles_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xNeg_allParticles_kinEnergy_h1d_.SetNameTitle("xNeg_allParticles_kinEnergy_h1d_","particles leaving the -x face of the tpc");
  xNeg_allParticles_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);

  //muons
  zPos_muon_kinEnergy_h1d_.SetNameTitle("zPos_muon_kinEnergy_h1d_","muons leaving the +z face of the tpc");
  zPos_muon_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  zNeg_muon_kinEnergy_h1d_.SetNameTitle("zNeg_muon_kinEnergy_h1d_","muons leaving the -z face of the tpc");  
  zNeg_muon_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yPos_muon_kinEnergy_h1d_.SetNameTitle("yPos_muon_kinEnergy_h1d_","muons leaving the +y face of the tpc");
  yPos_muon_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yNeg_muon_kinEnergy_h1d_.SetNameTitle("yNeg_muon_kinEnergy_h1d_","muons leaving the -y face of the tpc");
  yNeg_muon_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xPos_muon_kinEnergy_h1d_.SetNameTitle("xPos_muon_kinEnergy_h1d_","muons leaving the +x face of the tpc");
  xPos_muon_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xNeg_muon_kinEnergy_h1d_.SetNameTitle("xNeg_muon_kinEnergy_h1d_","muons leaving the -x face of the tpc");
  xNeg_muon_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);

  //protons
  zPos_proton_kinEnergy_h1d_.SetNameTitle("zPos_proton_kinEnergy_h1d_","particles leaving the +z face of the tpc");
  zPos_proton_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  zNeg_proton_kinEnergy_h1d_.SetNameTitle("zNeg_proton_kinEnergy_h1d_","particles leaving the -z face of the tpc");  zNeg_proton_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yPos_proton_kinEnergy_h1d_.SetNameTitle("yPos_proton_kinEnergy_h1d_","particles leaving the +y face of the tpc");
  yPos_proton_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yNeg_proton_kinEnergy_h1d_.SetNameTitle("yNeg_proton_kinEnergy_h1d_","particles leaving the -y face of the tpc");
  yNeg_proton_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xPos_proton_kinEnergy_h1d_.SetNameTitle("xPos_proton_kinEnergy_h1d_","particles leaving the +x face of the tpc");
  xPos_proton_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xNeg_proton_kinEnergy_h1d_.SetNameTitle("xNeg_proton_kinEnergy_h1d_","particles leaving the -x face of the tpc");
  xNeg_proton_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);

  //pions
  zPos_pion_kinEnergy_h1d_.SetNameTitle("zPos_pion_kinEnergy_h1d_","particles leaving the +z face of the tpc");
  zPos_pion_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  zNeg_pion_kinEnergy_h1d_.SetNameTitle("zNeg_pion_kinEnergy_h1d_","particles leaving the -z face of the tpc");  
  zNeg_pion_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yPos_pion_kinEnergy_h1d_.SetNameTitle("yPos_pion_kinEnergy_h1d_","particles leaving the +y face of the tpc");
  yPos_pion_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yNeg_pion_kinEnergy_h1d_.SetNameTitle("yNeg_pion_kinEnergy_h1d_","particles leaving the -y face of the tpc");
  yNeg_pion_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xPos_pion_kinEnergy_h1d_.SetNameTitle("xPos_pion_kinEnergy_h1d_","particles leaving the +x face of the tpc");
  xPos_pion_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xNeg_pion_kinEnergy_h1d_.SetNameTitle("xNeg_pion_kinEnergy_h1d_","particles leaving the -x face of the tpc");
  xNeg_pion_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);

  //gammas
  zPos_gamma_kinEnergy_h1d_.SetNameTitle("zPos_gamma_kinEnergy_h1d_","particles leaving the +z face of the tpc");
  zPos_gamma_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  zNeg_gamma_kinEnergy_h1d_.SetNameTitle("zNeg_gamma_kinEnergy_h1d_","particles leaving the -z face of the tpc");  
  zNeg_gamma_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yPos_gamma_kinEnergy_h1d_.SetNameTitle("yPos_gamma_kinEnergy_h1d_","particles leaving the +y face of the tpc");
  yPos_gamma_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yNeg_gamma_kinEnergy_h1d_.SetNameTitle("yNeg_gamma_kinEnergy_h1d_","particles leaving the -y face of the tpc");
  yNeg_gamma_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xPos_gamma_kinEnergy_h1d_.SetNameTitle("xPos_gamma_kinEnergy_h1d_","particles leaving the +x face of the tpc");
  xPos_gamma_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xNeg_gamma_kinEnergy_h1d_.SetNameTitle("xNeg_gamma_kinEnergy_h1d_","particles leaving the -x face of the tpc");
  xNeg_gamma_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);

  //electron
  zPos_electron_kinEnergy_h1d_.SetNameTitle("zPos_electron_kinEnergy_h1d_","particles leaving the +z face of the tpc");
  zPos_electron_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  zNeg_electron_kinEnergy_h1d_.SetNameTitle("zNeg_electron_kinEnergy_h1d_","particles leaving the -z face of the tpc");  
  zNeg_electron_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yPos_electron_kinEnergy_h1d_.SetNameTitle("yPos_electron_kinEnergy_h1d_","particles leaving the +y face of the tpc");
  yPos_electron_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  yNeg_electron_kinEnergy_h1d_.SetNameTitle("yNeg_electron_kinEnergy_h1d_","particles leaving the -y face of the tpc");
  yNeg_electron_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xPos_electron_kinEnergy_h1d_.SetNameTitle("xPos_electron_kinEnergy_h1d_","particles leaving the +x face of the tpc");
  xPos_electron_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);
  xNeg_electron_kinEnergy_h1d_.SetNameTitle("xNeg_electron_kinEnergy_h1d_","particles leaving the -x face of the tpc");
  xNeg_electron_kinEnergy_h1d_.SetBins(binning,0.,maxEnergy_);

  //2D Histograms
  //all particles
  allFaces_allParticles_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("allFaces_allParticles_cosTheta_vs_kinEnergy_h2d_","particles leaving the tpc");
  allFaces_allParticles_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  allFaces_muon_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("allFaces_muon_cosTheta_vs_kinEnergy_h2d_","muons leaving the tpc");
  allFaces_muon_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  allFaces_pion_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("allFaces_pion_cosTheta_vs_kinEnergy_h2d_","charged pions leaving the tpc");
  allFaces_pion_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  allFaces_proton_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("allFaces_proton_cosTheta_vs_kinEnergy_h2d_","protons leaving the tpc");
  allFaces_proton_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  allFaces_gamma_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("allFaces_gamma_cosTheta_vs_kinEnergy_h2d_","photons leaving the tpc");
  allFaces_gamma_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  allFaces_electron_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("allFaces_electron_cosTheta_vs_kinEnergy_h2d_","electrons leaving the tpc");
  allFaces_electron_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  allFaces_neutron_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("allFaces_neutron_cosTheta_vs_kinEnergy_h2d_","neutrons leaving the tpc");
  allFaces_neutron_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);

  zPos_allParticles_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zPos_allParticles_cosTheta_vs_kinEnergy_h2d_","particles leaving the +z face of the tpc");
  zPos_allParticles_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  zNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zNeg_allParticles_cosTheta_vs_kinEnergy_h2d_","particles leaving the -z face of the tpc");  zNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yPos_allParticles_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yPos_allParticles_cosTheta_vs_kinEnergy_h2d_","particles leaving the +y face of the tpc");
  yPos_allParticles_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yNeg_allParticles_cosTheta_vs_kinEnergy_h2d_","particles leaving the -y face of the tpc");
  yNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xPos_allParticles_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xPos_allParticles_cosTheta_vs_kinEnergy_h2d_","particles leaving the +x face of the tpc");
  xPos_allParticles_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xNeg_allParticles_cosTheta_vs_kinEnergy_h2d_","particles leaving the -x face of the tpc");
  xNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);

  //muons
  zPos_muon_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zPos_muon_cosTheta_vs_kinEnergy_h2d_","muons leaving the +z face of the tpc");
  zPos_muon_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  zNeg_muon_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zNeg_muon_cosTheta_vs_kinEnergy_h2d_","muons leaving the -z face of the tpc");  
  zNeg_muon_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yPos_muon_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yPos_muon_cosTheta_vs_kinEnergy_h2d_","muons leaving the +y face of the tpc");
  yPos_muon_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yNeg_muon_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yNeg_muon_cosTheta_vs_kinEnergy_h2d_","muons leaving the -y face of the tpc");
  yNeg_muon_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xPos_muon_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xPos_muon_cosTheta_vs_kinEnergy_h2d_","muons leaving the +x face of the tpc");
  xPos_muon_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xNeg_muon_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xNeg_muon_cosTheta_vs_kinEnergy_h2d_","muons leaving the -x face of the tpc");
  xNeg_muon_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);

  //protons
  zPos_proton_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zPos_proton_cosTheta_vs_kinEnergy_h2d_","particles leaving the +z face of the tpc");
  zPos_proton_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  zNeg_proton_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zNeg_proton_cosTheta_vs_kinEnergy_h2d_","particles leaving the -z face of the tpc");  zNeg_proton_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yPos_proton_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yPos_proton_cosTheta_vs_kinEnergy_h2d_","particles leaving the +y face of the tpc");
  yPos_proton_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yNeg_proton_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yNeg_proton_cosTheta_vs_kinEnergy_h2d_","particles leaving the -y face of the tpc");
  yNeg_proton_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xPos_proton_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xPos_proton_cosTheta_vs_kinEnergy_h2d_","particles leaving the +x face of the tpc");
  xPos_proton_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xNeg_proton_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xNeg_proton_cosTheta_vs_kinEnergy_h2d_","particles leaving the -x face of the tpc");
  xNeg_proton_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);

  //pions
  zPos_pion_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zPos_pion_cosTheta_vs_kinEnergy_h2d_","particles leaving the +z face of the tpc");
  zPos_pion_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  zNeg_pion_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zNeg_pion_cosTheta_vs_kinEnergy_h2d_","particles leaving the -z face of the tpc");  zNeg_pion_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yPos_pion_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yPos_pion_cosTheta_vs_kinEnergy_h2d_","particles leaving the +y face of the tpc");
  yPos_pion_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yNeg_pion_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yNeg_pion_cosTheta_vs_kinEnergy_h2d_","particles leaving the -y face of the tpc");
  yNeg_pion_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xPos_pion_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xPos_pion_cosTheta_vs_kinEnergy_h2d_","particles leaving the +x face of the tpc");
  xPos_pion_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xNeg_pion_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xNeg_pion_cosTheta_vs_kinEnergy_h2d_","particles leaving the -x face of the tpc");
  xNeg_pion_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);

  //gammas
  zPos_gamma_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zPos_gamma_cosTheta_vs_kinEnergy_h2d_","particles leaving the +z face of the tpc");
  zPos_gamma_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  zNeg_gamma_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("zNeg_gamma_cosTheta_vs_kinEnergy_h2d_","particles leaving the -z face of the tpc");  zNeg_gamma_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yPos_gamma_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yPos_gamma_cosTheta_vs_kinEnergy_h2d_","particles leaving the +y face of the tpc");
  yPos_gamma_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  yNeg_gamma_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("yNeg_gamma_cosTheta_vs_kinEnergy_h2d_","particles leaving the -y face of the tpc");
  yNeg_gamma_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xPos_gamma_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xPos_gamma_cosTheta_vs_kinEnergy_h2d_","particles leaving the +x face of the tpc");
  xPos_gamma_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);
  xNeg_gamma_cosTheta_vs_kinEnergy_h2d_.SetNameTitle("xNeg_gamma_cosTheta_vs_kinEnergy_h2d_","particles leaving the -x face of the tpc");
  xNeg_gamma_cosTheta_vs_kinEnergy_h2d_.SetBins(binning*factor,-1,1,binning*factor,0.,maxEnergy_);

  nodePositionXY_h2d_.SetNameTitle("nodePositionXY_h2d_","nodePositionXY_h2d_");
  nodePositionXY_h2d_.SetBins(800,-40000,40000,800,-40000,40000);
  nodePositionXZ_h2d_.SetNameTitle("nodePositionXZ_h2d_","nodePositionYZ_h2d_");
  nodePositionXZ_h2d_.SetBins(800,-40000,40000,1200,-60000,60000);
  nodePositionYZ_h2d_.SetNameTitle("nodePositionYZ_h2d_","nodePositionYZ_h2d_");
  nodePositionYZ_h2d_.SetBins(800,-40000,40000,1200,-60000,60000);

}

bool myFlux::loadGeom(){
  
  std::ifstream infile(geomFileName_.c_str());
  if(!infile.good()){
	std::cout << "\nGeometry file: \n\t" << geomFileName_<< " - does not exist. Please check.";
	return false;
  }

  geomLoader = new GeometryLoader(geomFileName_); 

  //load the geometry file to check nodes
  TFile geomFile(geomFileName_.c_str());
  if(!geomFile.IsZombie()){
     geom = geomLoader->getManager();
     world = geom->GetTopVolume();
     tpc = geomLoader->getTPCVolume();
     
     //in cm
     tpcWidth = geomLoader->getVolumeXBoundary(tpc)/2.;
     tpcHeight = geomLoader->getVolumeYBoundary(tpc)/2.;
     tpcDepth = geomLoader->getVolumeZBoundary(tpc)/2.;
     return true;
  }
  else{
	std::cout << "\nGeometry file: \n\t" << geomFileName_<< " - is not readable. Please check.";
	return false;
  }
}

void myFlux::info() {
 
  //exposure
  exposure = 0;
  totalNuEvents = 0;
  
  std::string filename = inputFileName_;
  std::stringstream ss;

  for(int j = 0;j<runs_;j++){

    ss << j;
    
    std::string filename_run;
    if(runs_==1){
	filename_run = filename;
    }
    else{
	filename_run = filename + ss.str() + ".root";
    }
    std::cout << "\n************************************************************************************************"
		<<"\nFilename being read: " << filename_run
		<< "\n************************************************************************************************";
    TFile datafile(filename_run.c_str());

    //clear stringstream 
    ss.str("");
    if(datafile.GetNkeys()<1){
	std::cout << "\n\t\t !!!!!!!! Bad file, skipping !!!!!!\n";
	badFileCount++;
	continue;	
    }

    //set data addresses
    if(!setupTrees(datafile)) return;
    
    /*    
    std::cout << "\n----------------------------------"
	      << "\n    Looping over entries...   "
	      << "\n----------------------------------\n";
    */

    /// NEUTRINO HITS ///
    if(nuHitTree){
      //loop over nuHit tree
      for(int k=0;k<nuHits_nEntries;k++){
    	nuHitTree->GetEntry(k);
	
    	double nuE = nuHit_->getP4().E() * (1./CLHEP::GeV);
    	fluxE_h1d_.Fill(nuE);
	
	TLorentzVector nuHitPos = nuHit_->getPosition() * (1./ CLHEP::m);
	//if(nuHitPos.X() ==0 && nuHitPos.Y() ==0 && nuHitPos.Z()==0)count++;
	//std::cout << "\nEnergy " << nuE << " GeV "<<std::endl;
      }
    }
    
    //volume nodes
    GeomVolumeRecord geomVolumeCounts,geomVolumeCountsFromInParticles;

    /// NEUTRINO EVENTS ///
    if(nuEventTree){
      this->setBackTracer(nuEventTree);
      //std::cout << "\n!!!!! nuEvents_nEntries " << nuEvents_nEntries;

      bool cc;      
      //loop over nuEvent tree
      for(int k=0;k<nuEvents_nEntries;k++){
	bool rockInteraction = false;
	
    	nuEventTree->GetEntry(k);
	if(nuHitTree)nuHitTree->GetEntry(backTracer_[0]);
	
   	int nuEvFsplPDG = nuEvent_->getFspl().getPDG();
	if(nuEvFsplPDG == 11)vertexElecCount++;
	else if(nuEvFsplPDG == 13)vertexMuonCount++;

	std::vector<ParticleDescrShortRecord> vector = nuEvent_->getFssVector();
	//loop over secondaries
    	for(int index=0;index<vector.size(); index++){
      	  int nuEvFssPDG = vector.at(index).getPDG();
	  //particle types
	  if(nuEvFssPDG == 11)vertexElecCount++;
	  else if(TMath::Abs(nuEvFssPDG) == 13)vertexMuonCount++;
	  else if(nuEvFssPDG == 2212)vertexProtCount++;
	  else if(nuEvFssPDG == 2112)vertexNeutCount++;
	  else if(TMath::Abs(nuEvFssPDG) == 211)vertexPionCount++;
	  else if(nuEvFssPDG == 111)vertexPiZeroCount++;
	  else if(nuEvFssPDG == 22)vertexGammCount++;
	}

	//node names
	std::string nodeName = nuEvent_->getNodeName();
	//volume counts
	geomVolumeCounts.findVolume(nodeName);
/*

	//---------------------Neutrino---------------------------//
    	double nuE = nuEvent_->getNuEnergy();// * (1./CLHEP::GeV);
    	int nuPDG = nuHit_->getPDG();
	
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
		
	if (nuEvFsplPDG==13 || nuEvFsplPDG==-13 
	    || nuEvFsplPDG==11 || nuEvFsplPDG==-11 
	    || nuEvFsplPDG==15 || nuEvFsplPDG==-15)cc = true;
	else cc = false;
		
	//-----------Final State Secondaries-------------------//	
	std::vector<ParticleDescrShortRecord> vector = nuEvent_->getFssVector();
	
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
	  
	  //if(nuEvFssPDG < 10000)totKinEnergy += nuEvFssT;
	  
          if(verbose_>2){
	    std::cout << "\nParticle: " <<nuEvFssPDG 
		      << ", Mass: "<< nuEvFssMass << " GeV/c2"
		      << ", Energy: "<< nuEvFssE << " GeV"
		      << ", KineticEnergy: "<< nuEvFssT << " GeV"
		      << ", In Node: " << nodeName.c_str();
	  }
		
	//double totEnergy = nuEvFssChargedE_CC + nuEvFssNeutronE_CC + nuEvFssGammaE_CC 
	//			+ nuEvFssChargedE_NC + nuEvFssNeutronE_NC + nuEvFssGammaE_NC + nuEvFinalNuE;
	
	//add the hit nucleon energy to give the total primary energy at interaction vertex
	
	  if(verbose_>1){
	  	std::cout << "\n-----------------------------------"
		    << "\nCharged Current Interaction? = " << cc 
		    << "\nNeutrino Energy = " << nuE << " GeV"
		    << "\nNucleon Energy = " << nuEvNuclE << " GeV"
		    << "\nFspl Kinetic Energy = " << nuEvFsplT << " GeV"
		    << "\nFspl Mass = " << nuEvFsplMass << " GeV/c2";

	  }
	}
	
    	TLorentzVector nuHitPos = nuHit_->getPosition() * (1./ CLHEP::m);
    	TLorentzVector nuEvPos = nuEvent_->getPosition() * (1./ CLHEP::m);
*/	
      }
    }	 
    

    /// TRACKING ///
    if(detectorHitsTree){
        this->setBackTracer(detectorHitsTree);
	//loop over events
	for(int k=0;k<detHits_nEntries;k++){
          detectorHitsTree->GetEntry(k);

	  //set backtracers for neutrino event truth info
          if(nuHitTree)nuHitTree->GetEntry(backTracer_[0]);
          nuEventTree->GetEntry(backTracer_[1]);

	  //get the vectors for primary, in and out particles from the volume
  	  std::vector<GeantPrimaryParticle> * prims = trackingRecord_->getPrimariesPtr();
  	  std::vector<GeantDaughterParticle> * ins = trackingRecord_->getInParticlesPtr();
  	  std::vector<GeantDaughterParticle> * outs = trackingRecord_->getOutParticlesPtr();;
	  
	  //loop over primaries
	  for(int l=0;l<prims->size();l++){
        	int fPrimPDG = prims->at(l).getPDG();
	
		primCount++;
        	if(fPrimPDG==13)primMuonCount++;
        	if(fPrimPDG==11)primElecCount++;
        	if(fPrimPDG==22)primGammCount++;
        	if(fPrimPDG==2112)primNeutCount++;
		if(fPrimPDG==2212)primProtCount++;
		if(fPrimPDG==-211 || fPrimPDG ==211)primPionCount++;
		if(fPrimPDG==111)primPiZeroCount++;
   	  }

	  //loop over in particles
	  for(int l=0;l<ins->size();l++){
        	int fInPDG = ins->at(l).getPDG();
        	double fInKEAtVertex = ins->at(l).getP4().E() - ins->at(l).getMass();
		TLorentzVector node_pos = ins->at(l).getParent().getPosition(); //mm
		node_pos *= 0.1;//1./CLHEP::cm; // put into cm

		//apply energy cut - in MeV
		if(fInKEAtVertex <  energyCut_)continue;

		//std::cout << "\n*** Muon Kin energy: " << fInKEAtVertex << " MeV";
		double kinenergy = fInKEAtVertex/1000.0;	//convert to GeV

		// find the original node from primaries or can do it from backtracer
		//nuEvent - check matches the primaries vertex
		std::string nodeName = nuEvent_->getNodeName();
		TLorentzVector nodePos = nuEvent_->getPosition(); // in mm
		//volume counts - where did the particle originate from
		geomVolumeCountsFromInParticles.findVolume(nodeName);
		//find original node - neutrino vertex
		bool rockInteraction = geomVolumeCountsFromInParticles.isRock();
		bool tpcInteraction = geomVolumeCountsFromInParticles.isTpc();
		bool magnetInteraction = geomVolumeCountsFromInParticles.isMagnet();

		//particle type counts
		inCount++;
		if(fInPDG==13)inFluxMuonCount++;
        	if(fInPDG==11)inFluxElecCount++;
        	if(fInPDG==22)inFluxGammCount++;
        	if(fInPDG==2112)inFluxNeutCount++;
		if(fInPDG==2212)inFluxProtCount++;
		if(fInPDG==-211 || fInPDG ==211 || fInPDG ==111)inFluxPionCount++;
     
		nodePositionXY_h2d_.Fill(nodePos.X(),nodePos.Y());
		nodePositionXZ_h2d_.Fill(nodePos.X(),nodePos.Z());
		nodePositionYZ_h2d_.Fill(nodePos.Y(),nodePos.Z());

		if(tpcInteraction==false){
		  if(fInPDG==13)muonKinEnergy_nonTPC_h1d_.Fill(kinenergy);
		  else if(fInPDG==2212)protonKinEnergy_nonTPC_h1d_.Fill(kinenergy);
		  else if(fInPDG==211 || fInPDG==-211)pionKinEnergy_nonTPC_h1d_.Fill(kinenergy);
		}
		if(rockInteraction==true){
		  if(fInPDG==13)muonKinEnergy_rockOnly_h1d_.Fill(kinenergy);
		  else if(fInPDG==2212)protonKinEnergy_rockOnly_h1d_.Fill(kinenergy);
		  else if(fInPDG==211 || fInPDG==-211)pionKinEnergy_rockOnly_h1d_.Fill(kinenergy);
		}
		if(magnetInteraction==true){
		  if(fInPDG==13)muonKinEnergy_magnetOnly_h1d_.Fill(kinenergy);
		  else if(fInPDG==2212)protonKinEnergy_magnetOnly_h1d_.Fill(kinenergy);
		  else if(fInPDG==211 || fInPDG==-211)pionKinEnergy_magnetOnly_h1d_.Fill(kinenergy);
		}
   	  } //end loop over in particles

	  //loop over out particles
	  for(int l=0;l<outs->size();l++){
        	int fOutPDG = outs->at(l).getPDG();
        	double fOutKEAtVertex = outs->at(l).getP4().E() - outs->at(l).getMass();

		if(fOutKEAtVertex<energyCut_)continue;

		TLorentzVector fOutP4 = outs->at(l).getP4();
		double fOutMomX = fOutP4.X();
		double fOutMomY = fOutP4.Y();
		double fOutMomZ = fOutP4.Z();
		TLorentzVector outPosP4 = outs->at(l).getPosition(); //mm
		outPosP4 *= 0.1;//1./CLHEP::cm; // put into cm
		double fOutPosX = outPosP4.X();
		double fOutPosY = outPosP4.Y();
		double fOutPosZ = outPosP4.Z();

		//std::cout << "\n*** Muon Kin energy: " << fInKEAtVertex << " MeV";
		double kinenergy = fOutKEAtVertex/1000.0;	//convert to GeV
         
		if(kinenergy<=0)continue;

     		//in MeV
     		double magMom = TMath::Sqrt(fOutMomX*fOutMomX + fOutMomY*fOutMomY + fOutMomZ*fOutMomZ);
     		double unitXMom = fOutMomX / magMom;
     		double unitYMom = fOutMomY / magMom;
     		double unitZMom = fOutMomZ / magMom;

     		//Positions in cm
     		//+Z plane
     		double lambdaPosZ = ( (tpcDepth/2.) - fOutPosZ)/(unitZMom);	//for vector equation of the line
     		double projXPosZ = fOutPosX + (lambdaPosZ*unitXMom);
     		double projYPosZ = fOutPosY + (lambdaPosZ*unitYMom);
     		//-Z plane
     		double lambdaNegZ = (-(tpcDepth/2.0) - fOutPosZ)/(unitZMom);	//for vector equation of the line
     		double projXNegZ = fOutPosX + (lambdaNegZ*unitXMom);
     		double projYNegZ = fOutPosY + (lambdaNegZ*unitYMom);

     		//+Y plane
     		double lambdaPosY = ( (tpcHeight/2.) - fOutPosY)/(unitYMom);	//for vector equation of the line
     		double projXPosY = fOutPosX + (lambdaPosY*unitXMom);
     		double projZPosY = fOutPosZ + (lambdaPosY*unitZMom);
     		//-Y plane
     		double lambdaNegY = (-(tpcHeight/2.0) - fOutPosY)/(unitYMom);	//for vector equation of the line
     		double projXNegY = fOutPosX + (lambdaNegY*unitXMom);
     		double projZNegY = fOutPosZ + (lambdaNegY*unitZMom);

     		//+X plane
     		double lambdaPosX = ( (tpcWidth/2.) - fOutPosX)/(unitXMom);	//for vector equation of the line
     		double projYPosX = fOutPosY + (lambdaPosX*unitYMom);
     		double projZPosX = fOutPosZ + (lambdaPosX*unitZMom);
     		//-X plane
     		double lambdaNegX = (-(tpcWidth/2.0) - fOutPosX)/(unitXMom);	//for vector equation of the line
     		double projYNegX = fOutPosY + (lambdaNegX*unitYMom);
     		double projZNegX = fOutPosZ + (lambdaNegX*unitZMom);

     		allFaces_allParticles_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));
     		outFluxAllParticleCount++;

    		if(fOutPDG==11){
		  allFaces_electron_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));
		  outFluxElecCount++;
		}
    		else if(fOutPDG==13){
		  allFaces_muon_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));
		  outFluxMuonCount++;
     		}
     		else if(fOutPDG==2212){
		  allFaces_proton_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));
		  outFluxProtCount++;
		}
     		else if(fOutPDG==211 || fOutPDG==-211 || fOutPDG==111){
		  allFaces_pion_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));
		  outFluxPionCount++;
		}
     		else if(fOutPDG==22){
		  allFaces_gamma_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));
		  outFluxGammCount++;
		}
    		else if(fOutPDG==2112){	
		  allFaces_neutron_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));
		  outFluxNeutCount++;
 		}

     		//leave in cm
     		if( (projXPosZ < tpcWidth/2.) && (projXPosZ > -tpcWidth/2.)
		 && (projYPosZ < tpcHeight/2.) && (projYPosZ > -tpcHeight/2.) 
		 && (unitZMom > 0.) ){
		  zPos_allParticles_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  zPos_allParticles_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));

		  if(fOutPDG==13)zPos_muon_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==2212)zPos_proton_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==211 || fOutPDG==-211|| fOutPDG==111)zPos_pion_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==22)zPos_gamma_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==11)zPos_electron_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==2112)outFluxDownNeutCount++;
		}

     		if( (projXNegZ < tpcWidth/2.) && (projXNegZ > -tpcWidth/2.)
		 && (projYNegZ < tpcHeight/2.) && (projYNegZ > -tpcHeight/2.) 
		 && (unitZMom < 0.) ){
		  zNeg_allParticles_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.));
		  zNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));
		  if(fOutPDG==13)zNeg_muon_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==2212)zNeg_proton_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==211 || fOutPDG==-211|| fOutPDG==111)zNeg_pion_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==22)zNeg_gamma_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==11)zNeg_electron_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		}

     		if( (projXPosY < tpcWidth/2.) && (projXPosY > -tpcWidth/2.)
		 && (projZPosY < tpcDepth/2.) && (projZPosY > -tpcDepth/2.)  
		 && (unitYMom > 0.) ){
		  yPos_allParticles_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.));
		  yPos_allParticles_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));

		  if(fOutPDG==13)yPos_muon_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==2212)yPos_proton_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==211 || fOutPDG==-211|| fOutPDG==111)yPos_pion_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==22)yPos_gamma_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==11)yPos_electron_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		}

    		if( (projXNegY < tpcWidth/2.) && (projXNegY > -tpcWidth/2.)
		 && (projZNegY < tpcDepth/2.) && (projZNegY > -tpcDepth/2.)  
		 && (unitYMom < 0.) ){
		  yNeg_allParticles_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.));
		  yNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));

		  if(fOutPDG==13)yNeg_muon_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==2212)yNeg_proton_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==211 || fOutPDG==-211|| fOutPDG==111)yNeg_pion_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==22)yNeg_gamma_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==11)yNeg_electron_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		}	

     		if( (projYPosX < tpcHeight/2.) && (projYPosX > -tpcHeight/2.)
		 && (projZPosX < tpcDepth/2.) && (projZPosX > -tpcDepth/2.)  
		 && (unitXMom > 0.) ){
		  xPos_allParticles_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.));
		  xPos_allParticles_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));

		  if(fOutPDG==13)xPos_muon_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==2212)xPos_proton_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==211 || fOutPDG==-211|| fOutPDG==111)xPos_pion_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==22)xPos_gamma_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==11)xPos_electron_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		}

     		if( (projYNegX < tpcHeight/2.) && (projYNegX > -tpcHeight/2.)
		 && (projZNegX < tpcDepth/2.) && (projZNegX > -tpcDepth/2.)  
		 && (unitXMom < 0.) ){
		  xNeg_allParticles_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.));
 		  xNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.Fill(unitZMom,fOutKEAtVertex*(1./1000.));

		  if(fOutPDG==13)xNeg_muon_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==2212)xNeg_proton_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==211 || fOutPDG==-211|| fOutPDG==111)xNeg_pion_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==22)xNeg_gamma_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		  if(fOutPDG==11)xNeg_electron_kinEnergy_h1d_.Fill(fOutKEAtVertex*(1./1000.)); //convert to GeV
		}

    	  sumOutCount = zPos_allParticles_kinEnergy_h1d_.GetEntries() + zNeg_allParticles_kinEnergy_h1d_.GetEntries() +
		  yPos_allParticles_kinEnergy_h1d_.GetEntries() + yNeg_allParticles_kinEnergy_h1d_.GetEntries() +
		  xPos_allParticles_kinEnergy_h1d_.GetEntries() + xNeg_allParticles_kinEnergy_h1d_.GetEntries();

	  }//end loop over out particles

	}//end loop over entries
    }//end detector hits tree

    datafile.Close();

    if(verbose_>=0){
    	std::cout << "\n********** NEUTRINO VERTICIES **********";
	geomVolumeCounts.printToStream(std::cout);

    	std::cout << "\n************ PARTICLES IN **************"
		  << "\n*********** ORIGINAL VERTEX ************";
	geomVolumeCountsFromInParticles.printToStream(std::cout);
    }

    //add to the total number of events
    volumeCountsEvent.addCounts(geomVolumeCounts);
    volumeCountsIn.addCounts(geomVolumeCountsFromInParticles);

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

    //delete the tree addresses
    deleteTrees();

  } //end loop over files

}
/*
TLorentzVector myFlux::findInteractionNodePosition(int eventID){

  bool match = false;

  TLorentzVector node_pos;

  int entry_pos = eventID;

  int lower_bound = eventID;
  int upper_bound = prim->GetEntries();

  int counter = 0;

  while(!match){
	entry_pos = (upper_bound + lower_bound)/2;	// find middle index point
	prim->GetEntry(entry_pos);

	if(fPrimEventID==eventID){
	  node_pos.SetXYZT(fPrimPosX/10.,fPrimPosY/10.,fPrimPosZ/10.,0); // mm to cm
	  match=true;
	}
	else{
	  if(fPrimEventID>eventID){
	  	upper_bound = entry_pos;
 	  }
	  else{
		lower_bound = entry_pos;
	  }
	  //if there is a problem with the eventid skip after 1000 iterations and kill
	  if(counter >1000){
		std::cout << "\n\t !!! Cannot find original node position, position set to (0,0,0,-1) !!!";
		node_pos.SetXYZT(0,0,0,-1);
		match = true;
	  }
	counter++;
	}
  }

  return node_pos;

}
*/
void myFlux::print(){
   std::cout << "\n================================================================"
	     << "\nNumber of entries in Primaries: " << primCount
	     << "\nNumber of entries in In particles: " << inCount
	     << "\nNumber of entries in Out particles: " << outFluxAllParticleCount << " ,Sum of each face: "<< sumOutCount << std::endl
	     << "\n================================================================"
             << "\n**** Volume origin of In particles ****";
  volumeCountsIn.printToStream(std::cout);
  std::cout  << "\n================================================================"
	     << std::endl;

   std::cout << "\n================================================================"
             << "\n********* Particles of interest ***********"
	     << "\n**** All outgoing : " << outFluxAllParticleCount
	     << "\n**** Muons:"
	     << "\n\t\tPrimary:\t\t" << primMuonCount << std::setprecision(3) << " (" << primMuonCount*100./primCount<< "%)"
	     << "\n\t\tAt Vertex:\t\t" << vertexMuonCount << std::setprecision(3)
             << "\n\t\tIn:\t\t\t" << inFluxMuonCount << std::setprecision(3) << " (" << inFluxMuonCount*100./inCount<< "%)"
             << "\n\t\tOut:\t\t\t" << outFluxMuonCount << std::setprecision(3) << " (" << outFluxMuonCount*100./outFluxAllParticleCount<< "%)"
             << "\n\t\tOut (downstream):\t" <<  zPos_muon_kinEnergy_h1d_.GetEntries()
             << "\n\t\tRatio Out/Primary :\t" << " ("  <<  outFluxMuonCount*100./vertexMuonCount<< "%)"
	     << "\n**** Protons:"
	     << "\n\t\tPrimary:\t\t" << primProtCount << std::setprecision(3) << " (" << primProtCount*100./primCount<< "%)"
	     << "\n\t\tAt Vertex:\t\t" << vertexProtCount << std::setprecision(3)
             << "\n\t\tIn:\t\t\t" << inFluxProtCount << std::setprecision(3) << " (" << inFluxProtCount*100./inCount<< "%)"
             << "\n\t\tOut:\t\t\t" << outFluxProtCount << std::setprecision(3) << " (" << outFluxProtCount*100./outFluxAllParticleCount<< "%)"
             << "\n\t\tOut (downstream):\t" <<  zPos_proton_kinEnergy_h1d_.GetEntries()
             << "\n\t\tRatio Out/Primary:\t" << " ("  <<  outFluxProtCount*100./vertexProtCount<< "%)"
	     << "\n**** Pions:"
	     << "\n\t\tPrimary:\t\t" << primPionCount << std::setprecision(3) << " (" << primPionCount*100./primCount<< "%)"
	     << "\n\t\tAt Vertex:\t\t" << vertexPionCount << std::setprecision(3)
             << "\n\t\tIn:\t\t\t" << inFluxPionCount << std::setprecision(3) << " (" << inFluxPionCount*100./inCount<< "%)"
             << "\n\t\tOut:\t\t\t" << outFluxPionCount << std::setprecision(3) << " (" << outFluxPionCount*100./outFluxAllParticleCount<< "%)" 
             << "\n\t\tOut (downstream):\t" <<  zPos_pion_kinEnergy_h1d_.GetEntries()
             << "\n\t\tRatio Out/Primary:\t" << " ("  <<  outFluxPionCount*100./vertexPionCount<< "%)"
	     << "\n**** Gammas:"
	     << "\n\t\tPrimary:\t\t" << primGammCount << std::setprecision(3) << " (" << primGammCount*100./primCount<< "%)"
	     << "\n\t\tPrimary (PiZeros):\t" << primPiZeroCount << std::setprecision(3) << " (" << primPiZeroCount*100./primCount<< "%)"
	     << "\n\t\tAt Vertex:\t\t" << vertexGammCount << std::setprecision(3)
	     << "\n\t\tAt Vertex (PiZeros):\t" << vertexPiZeroCount << std::setprecision(3)
             << "\n\t\tIn:\t\t\t" << inFluxGammCount << std::setprecision(3) << " (" << inFluxGammCount*100./inCount<< "%)"
             << "\n\t\tOut:\t\t\t" << outFluxGammCount << std::setprecision(3) << " (" << outFluxGammCount*100./outFluxAllParticleCount<< "%)"  
             << "\n\t\tOut (downstream):\t" <<  zPos_gamma_kinEnergy_h1d_.GetEntries()
             << "\n\t\tRatio Out/Primary:\t" << " ("  <<  outFluxGammCount*100./vertexGammCount<< "%)"
	     << "\n**** Electrons:"
	     << "\n\t\tPrimary:\t\t" << primElecCount << std::setprecision(3) << " (" << primElecCount*100./primCount<< "%)"
	     << "\n\t\tAt Vertex:\t\t" << vertexElecCount << std::setprecision(3)
             << "\n\t\tIn:\t\t\t" << inFluxElecCount << std::setprecision(3) << " (" << inFluxElecCount*100./inCount<< "%)"
             << "\n\t\tOut:\t\t\t" << outFluxElecCount << std::setprecision(3) << " (" << outFluxElecCount*100./outFluxAllParticleCount<< "%)" 
             << "\n\t\tOut (downstream):\t" <<  zPos_electron_kinEnergy_h1d_.GetEntries()
             << "\n\t\tRatio Out/Primary:\t" << " ("  <<  outFluxElecCount*100./vertexElecCount<< "%)"
	     << "\n**** Neutrons:"
	     << "\n\t\tPrimary:\t\t" << primNeutCount << std::setprecision(3) << " (" << primNeutCount*100./primCount<< "%)"
	     << "\n\t\tAt Vertex:\t\t" << vertexNeutCount << std::setprecision(3)
             << "\n\t\tIn:\t\t\t" << inFluxNeutCount << std::setprecision(3) << " (" << inFluxNeutCount*100./inCount<< "%)"
             << "\n\t\tOut:\t\t\t" << outFluxNeutCount << std::setprecision(3) << " (" << outFluxNeutCount*100./outFluxAllParticleCount<< "%)" 
             << "\n\t\tOut (downstream):\t" <<  outFluxDownNeutCount 
             << "\n\t\tRatio Out/Primary:\t" << "("  <<  outFluxNeutCount*100./vertexNeutCount<< "%)"
	     << "\n================================================================" <<std::endl;

   std::cout << "\n************** Neutrino Interaction Volume Counts ********************";
   volumeCountsEvent.printToStream(std::cout);

   std::cout  << "\n================================================================"
		<< "\nTotal number of neutrino interactions in geometry: "
		<< totalNuEvents
		<< "\nTotal Exposure [p.o.t]:   "
		<< exposure
	 	<< "\nTPC Interactions:         " 
		<< volumeCountsEvent.getTpcCount()
		<< "\nAverage Exposure [p.o.t]: "
		<< exposure/runs_
	        << "\n================================================================\n"
		<< "\n====================================================="
		<< "\n Files that could not be read: " << badFileCount 
		<< "\n=====================================================";

   std::cout << "\n-----------------------------------------------------------------------------------------"
	    << "\n  Writing to file: " << outputFileName_.c_str()
	    << "\n-----------------------------------------------------------------------------------------\n";
}

void myFlux::write(){
  TFile histofile(outputFileName_.c_str(),"recreate");

  fluxE_h1d_.SetLineWidth(2);
  fluxE_h1d_.Write();

  muonKinEnergy_rockOnly_h1d_.SetLineWidth(2);
  muonKinEnergy_rockOnly_h1d_.SetLineColor(kBlue);
  muonKinEnergy_rockOnly_h1d_.Write();
  pionKinEnergy_rockOnly_h1d_.SetLineWidth(2);
  pionKinEnergy_rockOnly_h1d_.SetLineColor(kBlue);
  pionKinEnergy_rockOnly_h1d_.Write();
  protonKinEnergy_rockOnly_h1d_.SetLineWidth(2);
  protonKinEnergy_rockOnly_h1d_.SetLineColor(kBlue);
  protonKinEnergy_rockOnly_h1d_.Write();

  muonKinEnergy_magnetOnly_h1d_.SetLineWidth(2);
  muonKinEnergy_magnetOnly_h1d_.SetLineColor(kRed);
  muonKinEnergy_magnetOnly_h1d_.Write();
  pionKinEnergy_magnetOnly_h1d_.SetLineWidth(2);
  pionKinEnergy_magnetOnly_h1d_.SetLineColor(kRed);
  pionKinEnergy_magnetOnly_h1d_.Write();
  protonKinEnergy_magnetOnly_h1d_.SetLineWidth(2);
  protonKinEnergy_magnetOnly_h1d_.SetLineColor(kRed);
  protonKinEnergy_magnetOnly_h1d_.Write();

  muonKinEnergy_nonTPC_h1d_.SetLineWidth(2);
  muonKinEnergy_nonTPC_h1d_.SetLineColor(kGreen);
  muonKinEnergy_nonTPC_h1d_.Write();
  pionKinEnergy_nonTPC_h1d_.SetLineWidth(2);
  pionKinEnergy_nonTPC_h1d_.SetLineColor(kGreen);
  pionKinEnergy_nonTPC_h1d_.Write();
  protonKinEnergy_nonTPC_h1d_.SetLineWidth(2);
  protonKinEnergy_nonTPC_h1d_.SetLineColor(kGreen);
  protonKinEnergy_nonTPC_h1d_.Write();

  //all particles
  zPos_allParticles_kinEnergy_h1d_.SetLineWidth(2);
  zPos_allParticles_kinEnergy_h1d_.SetLineColor(kGreen);
  zPos_allParticles_kinEnergy_h1d_.Write();
  zNeg_allParticles_kinEnergy_h1d_.SetLineWidth(2);
  zNeg_allParticles_kinEnergy_h1d_.SetLineColor(kRed);
  zNeg_allParticles_kinEnergy_h1d_.Write();
  yPos_allParticles_kinEnergy_h1d_.SetLineWidth(2);
  yPos_allParticles_kinEnergy_h1d_.SetLineColor(kBlue);
  yPos_allParticles_kinEnergy_h1d_.Write();
  yNeg_allParticles_kinEnergy_h1d_.SetLineWidth(2);
  yNeg_allParticles_kinEnergy_h1d_.SetLineColor(kCyan);
  yNeg_allParticles_kinEnergy_h1d_.Write();
  xPos_allParticles_kinEnergy_h1d_.SetLineWidth(2);
  xPos_allParticles_kinEnergy_h1d_.SetLineColor(kOrange);
  xPos_allParticles_kinEnergy_h1d_.Write();
  xNeg_allParticles_kinEnergy_h1d_.SetLineWidth(2);
  xNeg_allParticles_kinEnergy_h1d_.SetLineColor(kBlack);
  xNeg_allParticles_kinEnergy_h1d_.Write();

  //muons
  zPos_muon_kinEnergy_h1d_.SetLineWidth(2);
  zPos_muon_kinEnergy_h1d_.SetLineColor(kGreen);
  zPos_muon_kinEnergy_h1d_.Write();
  zNeg_muon_kinEnergy_h1d_.SetLineWidth(2);
  zNeg_muon_kinEnergy_h1d_.SetLineColor(kRed);
  zNeg_muon_kinEnergy_h1d_.Write();
  yPos_muon_kinEnergy_h1d_.SetLineWidth(2);
  yPos_muon_kinEnergy_h1d_.SetLineColor(kBlue);
  yPos_muon_kinEnergy_h1d_.Write();
  yNeg_muon_kinEnergy_h1d_.SetLineWidth(2);
  yNeg_muon_kinEnergy_h1d_.SetLineColor(kCyan);
  yNeg_muon_kinEnergy_h1d_.Write();
  xPos_muon_kinEnergy_h1d_.SetLineWidth(2);
  xPos_muon_kinEnergy_h1d_.SetLineColor(kOrange);
  xPos_muon_kinEnergy_h1d_.Write();
  xNeg_muon_kinEnergy_h1d_.SetLineWidth(2);
  xNeg_muon_kinEnergy_h1d_.SetLineColor(kBlack);
  xNeg_muon_kinEnergy_h1d_.Write();

  //protons
  zPos_proton_kinEnergy_h1d_.SetLineWidth(2);
  zPos_proton_kinEnergy_h1d_.SetLineColor(kGreen);
  zPos_proton_kinEnergy_h1d_.Write();
  zNeg_proton_kinEnergy_h1d_.SetLineWidth(2);
  zNeg_proton_kinEnergy_h1d_.SetLineColor(kRed);
  zNeg_proton_kinEnergy_h1d_.Write();
  yPos_proton_kinEnergy_h1d_.SetLineWidth(2);
  yPos_proton_kinEnergy_h1d_.SetLineColor(kBlue);
  yPos_proton_kinEnergy_h1d_.Write();
  yNeg_proton_kinEnergy_h1d_.SetLineWidth(2);
  yNeg_proton_kinEnergy_h1d_.SetLineColor(kCyan);
  yNeg_proton_kinEnergy_h1d_.Write();
  xPos_proton_kinEnergy_h1d_.SetLineWidth(2);
  xPos_proton_kinEnergy_h1d_.SetLineColor(kOrange);
  xPos_proton_kinEnergy_h1d_.Write();
  xNeg_proton_kinEnergy_h1d_.SetLineWidth(2);
  xNeg_proton_kinEnergy_h1d_.SetLineColor(kBlack);
  xNeg_proton_kinEnergy_h1d_.Write();

  //pions
  zPos_pion_kinEnergy_h1d_.SetLineWidth(2);
  zPos_pion_kinEnergy_h1d_.SetLineColor(kGreen);
  zPos_pion_kinEnergy_h1d_.Write();
  zNeg_pion_kinEnergy_h1d_.SetLineWidth(2);
  zNeg_pion_kinEnergy_h1d_.SetLineColor(kRed);
  zNeg_pion_kinEnergy_h1d_.Write();
  yPos_pion_kinEnergy_h1d_.SetLineWidth(2);
  yPos_pion_kinEnergy_h1d_.SetLineColor(kBlue);
  yPos_pion_kinEnergy_h1d_.Write();
  yNeg_pion_kinEnergy_h1d_.SetLineWidth(2);
  yNeg_pion_kinEnergy_h1d_.SetLineColor(kCyan);
  yNeg_pion_kinEnergy_h1d_.Write();
  xPos_pion_kinEnergy_h1d_.SetLineWidth(2);
  xPos_pion_kinEnergy_h1d_.SetLineColor(kOrange);
  xPos_pion_kinEnergy_h1d_.Write();
  xNeg_pion_kinEnergy_h1d_.SetLineWidth(2);
  xNeg_pion_kinEnergy_h1d_.SetLineColor(kBlack);
  xNeg_pion_kinEnergy_h1d_.Write();

  //gammas
  zPos_gamma_kinEnergy_h1d_.SetLineWidth(2);
  zPos_gamma_kinEnergy_h1d_.SetLineColor(kGreen);
  zPos_gamma_kinEnergy_h1d_.Write();
  zNeg_gamma_kinEnergy_h1d_.SetLineWidth(2);
  zNeg_gamma_kinEnergy_h1d_.SetLineColor(kRed);
  zNeg_gamma_kinEnergy_h1d_.Write();
  yPos_gamma_kinEnergy_h1d_.SetLineWidth(2);
  yPos_gamma_kinEnergy_h1d_.SetLineColor(kBlue);
  yPos_gamma_kinEnergy_h1d_.Write();
  yNeg_gamma_kinEnergy_h1d_.SetLineWidth(2);
  yNeg_gamma_kinEnergy_h1d_.SetLineColor(kCyan);
  yNeg_gamma_kinEnergy_h1d_.Write();
  xPos_gamma_kinEnergy_h1d_.SetLineWidth(2);
  xPos_gamma_kinEnergy_h1d_.SetLineColor(kOrange);
  xPos_gamma_kinEnergy_h1d_.Write();
  xNeg_gamma_kinEnergy_h1d_.SetLineWidth(2);
  xNeg_gamma_kinEnergy_h1d_.SetLineColor(kBlack);
  xNeg_gamma_kinEnergy_h1d_.Write();

  //electron
  zPos_electron_kinEnergy_h1d_.SetLineWidth(2);
  zPos_electron_kinEnergy_h1d_.SetLineColor(kGreen);
  zPos_electron_kinEnergy_h1d_.Write();
  zNeg_electron_kinEnergy_h1d_.SetLineWidth(2);
  zNeg_electron_kinEnergy_h1d_.SetLineColor(kRed);
  zNeg_electron_kinEnergy_h1d_.Write();
  yPos_electron_kinEnergy_h1d_.SetLineWidth(2);
  yPos_electron_kinEnergy_h1d_.SetLineColor(kBlue);
  yPos_electron_kinEnergy_h1d_.Write();
  yNeg_electron_kinEnergy_h1d_.SetLineWidth(2);
  yNeg_electron_kinEnergy_h1d_.SetLineColor(kCyan);
  yNeg_electron_kinEnergy_h1d_.Write();
  xPos_electron_kinEnergy_h1d_.SetLineWidth(2);
  xPos_electron_kinEnergy_h1d_.SetLineColor(kOrange);
  xPos_electron_kinEnergy_h1d_.Write();
  xNeg_electron_kinEnergy_h1d_.SetLineWidth(2);
  xNeg_electron_kinEnergy_h1d_.SetLineColor(kBlack);
  xNeg_electron_kinEnergy_h1d_.Write();


  //2D histograms
  allFaces_allParticles_cosTheta_vs_kinEnergy_h2d_.Write();
  allFaces_muon_cosTheta_vs_kinEnergy_h2d_.Write();
  allFaces_proton_cosTheta_vs_kinEnergy_h2d_.Write();
  allFaces_pion_cosTheta_vs_kinEnergy_h2d_.Write();
  allFaces_electron_cosTheta_vs_kinEnergy_h2d_.Write();
  allFaces_gamma_cosTheta_vs_kinEnergy_h2d_.Write();
  allFaces_neutron_cosTheta_vs_kinEnergy_h2d_.Write();

  zPos_allParticles_cosTheta_vs_kinEnergy_h2d_.Write();
  zNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.Write();
  yPos_allParticles_cosTheta_vs_kinEnergy_h2d_.Write();
  yNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.Write();
  xPos_allParticles_cosTheta_vs_kinEnergy_h2d_.Write();
  xNeg_allParticles_cosTheta_vs_kinEnergy_h2d_.Write();

  nodePositionXY_h2d_.Write();
  nodePositionXZ_h2d_.Write();
  nodePositionYZ_h2d_.Write();

  std::cout << "\n----------------------------------"
	    << "\n             Closing...           "
	    << "\n----------------------------------\n";

  histofile.Close();
}

int main(int argc, char ** argv) {
  try {
    myFlux flux;
    flux.initDataCards();
    flux.getRunTimeArguments(argc, argv);
    flux.loadDataCards();
    flux.initHistos();
    if(flux.loadGeom())flux.info();
    flux.write();
    flux.print();

  } catch (GasTPCException e) {
    std::cout << e.GetLocation() << std::endl;
    std::cout << e.GetDescription() << std::endl;
    return 1;
  }

  return 0;
}


