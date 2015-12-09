//____________________________________________________________________________
/*!

\class    Utils

\brief    Utilities here

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
         University of Liverpool

\created Oct 2015
*/
//____________________________________________________________________________
#ifndef RECONUTILS_HH
#define RECONUTILS_HH

#include <iostream>

#include <TVector3.h>
#include <TVector2.h>

namespace ReconUtils{

  // TPC dedx resolution
  double DEDXResolution(double pressure, double dist);
  // dEdx truncated
  double DEDXTruncated(double e, int npoints, double trunfraction);
  // Expected dedx from TPC parameterization
  double DEDXExp(double mom, double mass);
  // Smearers
  double Smearer(double val, double res, int seed=999);
  TVector3 Smearer(TVector3 xyz_pos, double res, int seed=999);
  TVector2 Smearer(TVector2 xy_pos, double res, int seed=999);
  // Recon charge
  int getReconCharge(double smearSag, int truthCharge);
  // Track length
  double getTrackLength(TVector3 first, TVector3 last);
  // True transverse momentum
  double getTruthTransMomentum(TLorentzVector momentum);
  // Recon transverse momentum
  double getReconTransMomentum(double magneticField, double trackLength,double sagitta, double res);
  // Recon X momentum component
  double getReconMomentumX(double transvmom, double truthtransmom, double momX, int seed=999);
  // Get track sagitta
  double getTrackSagitta(double magneticField,double trackLength,double transMom);
  // Hit efficiency
  bool IsReconHit(double eff=0.995, int seed=999);
  // Track efficiency
  bool IsReconTrack(int nodes, int seed=999);

  // In Vessel FV
  bool inVesselFV(TVector3 pos, double vesselZ, double vesselR);

  // True interaction mode for Valor
  int getInteractionMode(int neut, int pipm, int pi0, bool cc, bool nc, bool qel, bool mec, bool coh);

}

#endif
