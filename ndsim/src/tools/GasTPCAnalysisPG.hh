//____________________________________________________________________________
/*!

\class    GasTPCAnalysisPG

\brief    

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2015
\last update Nov 2015
*/
//____________________________________________________________________________
#ifndef GASTPCANALYSISPG_HH
#define GASTPCANALYSISPG_HH

#include <TTree.h>

#include "UtlXML.hh"

void ReadDatacard(std::string datacard);
void ReadOutputTree(TTree *GasTPCTree);

double deltaSagitta, tpcresolution, magfield, angresolution, scintresolution, scintTimeRes, dedxCorMIP, dedxCorElec, gaspressure, fiducialXY, fiducialZ;
int fNSpillsSimulated;

// The output tree variables
const Int_t kMaxTrack = 500;
Int_t EventID, RunID, FSTopology, neutPdg, neutcode, Target, NGeantHits[kMaxTrack], NGeantTracks, NCEcalTracks, SpillNumber, TrackID[kMaxTrack], ParentID[kMaxTrack], Pdg[kMaxTrack], ParentPdg[kMaxTrack], LeftVolume[kMaxTrack], sample, reactionmode;
Double_t FirstGeantHitPosition[kMaxTrack][4], LastGeantHitPosition[kMaxTrack][4], VertexPosition[4], x, y, y_reco, Q2, W, POTWeight, Ev, Ev_reco, dEdx[kMaxTrack], RecondEdx[kMaxTrack], dEdxTruncated[kMaxTrack], RecondEdxTruncated[kMaxTrack], dEdxSigma[kMaxTrack], dEdxMuon[kMaxTrack], dEdxPion[kMaxTrack], dEdxElec[kMaxTrack], dEdxKaon[kMaxTrack], dEdxProt[kMaxTrack], pullmuon[kMaxTrack], pullpion[kMaxTrack], pullelec[kMaxTrack], pullkaon[kMaxTrack], pullprot[kMaxTrack];
Double_t Momentum[kMaxTrack], ReconMomentum[kMaxTrack], ReconMomentumXYZ[kMaxTrack][3], Pt[kMaxTrack], Costheta[kMaxTrack], ReconCostheta[kMaxTrack], TrackLength[kMaxTrack], HitLength[kMaxTrack], TotalEDep[kMaxTrack], ReconPosition[kMaxTrack][4], ReconLastPosition[kMaxTrack][4], ReconTrackLength[kMaxTrack];
Int_t NFSParticles, FSPdg[kMaxTrack],  NISParticles, ISPdg[kMaxTrack], ReconCharge[kMaxTrack], Charge[kMaxTrack], ReconInFV[kMaxTrack];;
Double_t FSMomentum[kMaxTrack], FSCostheta[kMaxTrack];
Int_t PrimaryPdg[kMaxTrack];

Int_t NGamma, GammaPdg[kMaxTrack], GammaParentPdg[kMaxTrack], GammaParentID[kMaxTrack], GammaWhichEcal[kMaxTrack], GammaNHits[kMaxTrack];
Double_t GammaMomentum[kMaxTrack], GammaEDep[kMaxTrack], GammaFirstGeantHitPosition[kMaxTrack][4];

Int_t NNeutron, NeutronWhichEcal[kMaxTrack], NeutronNHits[kMaxTrack];
Double_t NeutronMomentum[kMaxTrack], NeutronEDep[kMaxTrack], NeutronFirstGeantHitPosition[kMaxTrack][4];

Int_t EcalNGeantHits[kMaxTrack], WhichEcal[kMaxTrack], EcalFirstLayer[kMaxTrack], EcalLastLayer[kMaxTrack], NEcals[kMaxTrack];
Double_t EcalMomentum[kMaxTrack], EcalTrackLength[kMaxTrack][3], EcalTotalEDep[kMaxTrack], EcalFirstGeantHitPosition[kMaxTrack][4], EcalLastGeantHitPosition[kMaxTrack][4], EcalShowerWidth[kMaxTrack][3], EcalShowerEDepSlices[kMaxTrack][5],  EcalReconHitPosition[kMaxTrack][4];

Int_t NCEcalNGeantHits[kMaxTrack], NCWhichEcal[kMaxTrack], NCPdg[kMaxTrack], NCEcalUnusedHits;
Double_t NCEcalMomentum[kMaxTrack], NCEcalTrackLength[kMaxTrack], NCEcalTotalEDep[kMaxTrack], NCEcalFirstGeantHitPosition[kMaxTrack][4], NCEcalLastGeantHitPosition[kMaxTrack][4];

Int_t IsTPCRecon[kMaxTrack], NTPCTracksInEcal;

Double_t y_All, Ev_All;

// drifting model pars (see descriptions in GeantSimulationPG.xml config file
double fI0=0,fDiffusionCF=0,fMobility=0,fEField=0,fZ0=0,fDiscFact=1
  ,fDriftVelocity=0
;

genie::NtpMCEventRecord *fAnalNtpMCEventRecord;

#endif
