//____________________________________________________________________________
/*!

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
         University of Liverpool

\created Oct 2015
*/
//____________________________________________________________________________
#include <TRandom3.h>
#include <TLorentzVector.h>

#include "ReconUtils.hh"
//____________________________________________________________________________
double ReconUtils::DEDXResolution(double pressure, double dist){
  return 5.4*pow(pressure*dist,-0.37);
}
//____________________________________________________________________________
double ReconUtils::Smearer(double val, double res, int seed){
  TRandom3 * rand = new TRandom3();
  rand->SetSeed(seed);
  double retval = rand->Gaus(val,res);
  delete rand;
  return retval;
}
//____________________________________________________________________________
TVector3 ReconUtils::Smearer(TVector3 xyz_pos, double res, int seed){
  TRandom3 * rand = new TRandom3();
  rand->SetSeed(seed);
 
  double x = rand->Gaus(xyz_pos.X(),res);
  double y = rand->Gaus(xyz_pos.Y(),res);
  double z = rand->Gaus(xyz_pos.Z(),res);

  TVector3 pos(x,y,z);
  delete rand;
  return pos;
}
//____________________________________________________________________________
TVector2 ReconUtils::Smearer(TVector2 xy_pos, double res, int seed){
  TRandom3 * rand = new TRandom3();
  rand->SetSeed(seed);
 
  double x = rand->Gaus(xy_pos.X(),res);
  double y = rand->Gaus(xy_pos.Y(),res);

  TVector2 pos(x,y);
  delete rand;
  return pos;
}
//____________________________________________________________________________
double ReconUtils::DEDXTruncated(double e, int npoints, double trunfraction){
  return (e/npoints)*trunfraction;
}

double ReconUtils::DEDXExp(double mom, double mass){
  double bg = mom/mass;
  double beta=sqrt((bg*bg)/(1.+bg*bg));

  // Numbers copied from T2K TPC dedx parameterization
  double ExpecteddEP0 = 1.0; //785.0 - changed to make the scaling easier
  double ExpecteddEP1 = 6.047;
  double ExpecteddEP2 = 0.00064;
  double ExpecteddEP3 = 2.308;
  double ExpecteddEP4 = 1.359;

  double func=ExpecteddEP1-pow(beta,ExpecteddEP3)-log(ExpecteddEP2+1./pow(bg,ExpecteddEP4));

  func=func*ExpecteddEP0/pow(beta,ExpecteddEP3);

  return func;
}
//____________________________________________________________________________
int ReconUtils::getReconCharge(double smearSag, int truthCharge){
  if(smearSag < 0)
    truthCharge = -truthCharge;

  return truthCharge;
}
//____________________________________________________________________________
double ReconUtils::getTrackLength(TVector3 first, TVector3 last){
  //don't count the x positions - only transverse coordiantes to mag field
  double trackLength = TMath::Sqrt( TMath::Power(last.Y() - first.Y(),2)  
				    + TMath::Power(last.Z() - first.Z(),2) );

  return trackLength;
}
//____________________________________________________________________________
double ReconUtils::getTruthTransMomentum(TLorentzVector momentum){
  double momentumTrans = TMath::Sqrt((momentum.Y()*momentum.Y()) + (momentum.Z()*momentum.Z()));

  return momentumTrans;

}
//____________________________________________________________________________
double ReconUtils::getReconTransMomentum(double magneticField, double trackLength,double sagitta, double res){
  
  //if it is less than the resolution then let the sagitta = resolution
  if(sagitta < res)
    sagitta = res;

  return magneticField*trackLength*trackLength/(26.7*sagitta);
}
//____________________________________________________________________________
double ReconUtils::getReconMomentumX(double transvmom, double truthtransmom, double momX, int seed){
  TRandom3 * rand = new TRandom3();
  rand->SetSeed(seed);

  double pxres = fabs(1-transvmom/truthtransmom);
  double rval = rand->Gaus(momX,pxres*momX);
  delete rand;
  return rval;
}
//____________________________________________________________________________
double ReconUtils::getTrackSagitta(double magneticField,double trackLength,double transMom){
  
  return magneticField*trackLength*trackLength/(26.7*transMom);
}
//____________________________________________________________________________
bool ReconUtils::IsReconHit(double eff, int seed){
  TRandom3 * rand = new TRandom3();
  rand->SetSeed(seed);

  eff = eff*1000;

  int rval = rand->Integer(1000) + 1;

  delete rand;
  return (rval < eff);
}
//____________________________________________________________________________
bool ReconUtils::IsReconTrack(int nodes, int seed){
  if(nodes < 3)
    return false;
  
  TRandom3 * rand = new TRandom3();
  rand->SetSeed(seed);
  
  //int nnodes[20]={3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
  double eff[20]={0.10, 0.12, 0.14, 0.2, 0.32, 0.49, 0.68, 0.83, 0.91, 0.965, 0.983, 0.988, 0.992, 0.994, 0.995, 0.996, 0.997, 0.998, 0.999, 0.999};
  
  int rval = rand->Integer(1000) + 1;
  delete rand;

  bool toreturn = true;
  if(nodes > 20){
    if(rval > eff[19]*1000)
      toreturn = false;
  }
  else{
    if(rval > eff[nodes-3]*1000)
      toreturn = false;
  }

  return toreturn;    
}
//____________________________________________________________________________
bool ReconUtils::inVesselFV(TVector3 pos, double vesselZ, double vesselR){
  double z_vessel  = 3558.2; // vessel origin in mm
  double z1_vessel = z_vessel + vesselZ;
  double z2_vessel = z_vessel - vesselZ;
  // XY circle
  //double xsq = pos.X()*pos.X();
  //double ysq = pos.Y()*pos.Y();
  //if( (xsq + ysq) > vesselR*vesselR) return false;
  if( abs(pos.X()) > vesselR || abs(pos.Y()) > vesselR) return false;
  // Z height
  if(pos.Z() > z1_vessel || pos.Z() < z2_vessel) return false;

  return true;
}
//____________________________________________________________________________
int ReconUtils::getInteractionMode(int neut, int pipm, int pi0, bool cc, bool nc, bool qel, bool mec, bool coh){

  int reactionmode = -1;
  int addition = -1;
  if(neut == 14) addition = 0;
  else if(neut == -14) addition = 13;
  else if(neut == 12) addition = 26;
  else if(neut == -12) addition = 39;

  if(cc){
    if(qel)
      reactionmode = 0 + addition;
    else if(mec)
      reactionmode = 1 + addition;
    else if(coh)
      reactionmode = 7 + addition;
    else if(pipm == 1 && pi0 == 0)
      reactionmode = 2 + addition;
    else if(pipm == 0 && pi0 == 1)
      reactionmode = 3 + addition;
    else if(pipm == 2 && pi0 == 0)
      reactionmode = 4 + addition;
    else if(pipm == 0 && pi0 == 2)
      reactionmode = 5 + addition;
    else if(pipm == 1 && pi0 == 1)
      reactionmode = 6 + addition;
    else
      reactionmode = 8 + addition;
  }
  else if(nc){
    if(coh)
      reactionmode = 11 + addition;
    else if(pipm == 1 && pi0 == 0)
      reactionmode = 9 + addition;
    else if(pipm == 0 && pi0 == 1)
      reactionmode = 10 + addition;
    else
      reactionmode = 12 + addition;
  }
  else{
    reactionmode = -1;
  }

  return reactionmode;

}
//____________________________________________________________________________
int ReconUtils::getSampleTruth(int neutPdg, int topo, bool fhc){
  int sample = -1;

  if(fhc){
    if(topo < 10){
      if(neutPdg == 14)
        sample = topo;
      else if(neutPdg == -14)
        sample = 9;
      else if(neutPdg == -12 || neutPdg == 12)
        sample = 8;
    }
    else{
      sample = 10;
    }
  }
  else{
    if(topo < 10){
      if(neutPdg == -14)
        sample = topo;
      else if(neutPdg == 14)
        sample = 9;
      else if(neutPdg == -12 || neutPdg == 12)
        sample = 8;
    }
    else{
      sample = 10;
    }
  }

  return sample;

}
