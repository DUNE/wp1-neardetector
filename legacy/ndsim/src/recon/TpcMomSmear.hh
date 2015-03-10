//____________________________________________________________________________
/*!

\class    TpcMomSmear

\brief    Class for smearing momentum in the TPC

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool

\created  Jan 2014
\last update June 2014

*/
//____________________________________________________________________________
#ifndef TPCMOMSMEAR_HH
#define TPCMOMSMEAR_HH

#include <TRandom3.h>

#include "GasTPCDataLib.hh"

class TpcMomSmear{

  public:
	TpcMomSmear();
	TpcMomSmear(double magField,double deltaSag);
	~TpcMomSmear();

	void Initialize();

	const double particleMomentum(HitCollection &track);
	const double ccqeReconstruct(ParticleDescrShortRecord * protonTruthInfo,HitCollection &lepton,NeutrinoEvent* nu_event);

 	//getters
  	const double getTrackLength(HitCollection &track);
  	const double getTruthTransMomentum(HitCollection &track);
  	const double getTruthMomentum(HitCollection &track);
  	const double getReconTransMomentum(double trackLength,double sagitta);
  	const double getTrackSagitta(HitCollection &track,double transMom);
  	const double getSmearSagitta(double truthSag);
	const double getReconSagitta(HitCollection &tpcTrack);
  
  private:
	double magneticField;
	double deltaSagitta;

  	TRandom3 * rand;
};
#endif
