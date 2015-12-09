//____________________________________________________________________________
/*!

\class    GasTPCAnalysis

\brief    

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2015
\last update Nov 2015
*/
//____________________________________________________________________________
#ifndef GASTPCANALYSIS_HH
#define GASTPCANALYSIS_HH

#include <TTree.h>

#include "UtlXML.hh"

void ReadDatacard(std::string datacard);
void ReadOutputTree(TTree *GasTPCTree);

double deltaSagitta, tpcresolution, magfield, angresolution, scintresolution, dedxCorMIP, dedxCorElec, gaspressure, fiducialXY, fiducialZ;

// The output tree variables
const Int_t kMaxTrack = 500;
Int_t EventID, RunID, FSTopology, neutPdg, neutcode, Target, NGeantHits[kMaxTrack], NGeantTracks, NCEcalTracks, TrackID[kMaxTrack], ParentID[kMaxTrack], Pdg[kMaxTrack], ParentPdg[kMaxTrack], LeftVolume[kMaxTrack], sample, reactionmode;
Double_t FirstGeantHitPosition[kMaxTrack][4], LastGeantHitPosition[kMaxTrack][4], VertexPosition[4], x, y, y_reco, Q2, W, weight, Ev, Ev_reco, dEdx[kMaxTrack], RecondEdx[kMaxTrack], dEdxTruncated[kMaxTrack], RecondEdxTruncated[kMaxTrack], dEdxSigma[kMaxTrack], dEdxMuon[kMaxTrack], dEdxPion[kMaxTrack], dEdxElec[kMaxTrack], dEdxKaon[kMaxTrack], dEdxProt[kMaxTrack], pullmuon[kMaxTrack], pullpion[kMaxTrack], pullelec[kMaxTrack], pullkaon[kMaxTrack], pullprot[kMaxTrack];
Double_t Momentum[kMaxTrack], ReconMomentum[kMaxTrack], ReconMomentumXYZ[kMaxTrack][3], Pt[kMaxTrack], Costheta[kMaxTrack], ReconCostheta[kMaxTrack], TrackLength[kMaxTrack], HitLength[kMaxTrack], TotalEDep[kMaxTrack], ReconPosition[kMaxTrack][4], ReconLastPosition[kMaxTrack][4], ReconTrackLength[kMaxTrack];
Int_t NFSParticles, FSPdg[kMaxTrack],  NISParticles, ISPdg[kMaxTrack], ReconCharge[kMaxTrack], Charge[kMaxTrack], ReconInFV[kMaxTrack];;
Double_t FSMomentum[kMaxTrack], FSCostheta[kMaxTrack];
Int_t PrimaryPdg[kMaxTrack];

Int_t NGamma, GammaPdg[kMaxTrack], GammaParentPdg[kMaxTrack], GammaParentID[kMaxTrack], GammaWhichEcal[kMaxTrack], GammaNHits[kMaxTrack];
Double_t GammaMomentum[kMaxTrack], GammaEDep[kMaxTrack], GammaFirstGeantHitPosition[kMaxTrack][4];

Int_t NNeutron, NeutronWhichEcal[kMaxTrack], NeutronNHits[kMaxTrack];
Double_t NeutronMomentum[kMaxTrack], NeutronEDep[kMaxTrack], NeutronFirstGeantHitPosition[kMaxTrack][4];

Int_t EcalNGeantHits[kMaxTrack], WhichEcal[kMaxTrack], EcalFirstLayer[kMaxTrack], EcalLastLayer[kMaxTrack], NEcals[kMaxTrack];
Double_t EcalMomentum[kMaxTrack], EcalTrackLength[kMaxTrack][3], EcalTotalEDep[kMaxTrack], EcalFirstGeantHitPosition[kMaxTrack][4], EcalLastGeantHitPosition[kMaxTrack][4], EcalShowerWidth[kMaxTrack][3], EcalShowerEDepSlices[kMaxTrack][5];

Int_t NCEcalNGeantHits[kMaxTrack], NCWhichEcal[kMaxTrack], NCPdg[kMaxTrack], NCEcalUnusedHits;
Double_t NCEcalMomentum[kMaxTrack], NCEcalTrackLength[kMaxTrack], NCEcalTotalEDep[kMaxTrack], NCEcalFirstGeantHitPosition[kMaxTrack][4], NCEcalLastGeantHitPosition[kMaxTrack][4];

Int_t IsTPCRecon[kMaxTrack], NTPCTracksInEcal;

genie::NtpMCEventRecord *fAnalNtpMCEventRecord;

#endif
