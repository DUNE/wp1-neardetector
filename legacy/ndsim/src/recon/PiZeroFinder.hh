//____________________________________________________________________________
/*!

\class    PiZeroFinder

\brief    Class for reconstructing pi zeros

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool


\created  May 2014
\last update June 2014

*/
//____________________________________________________________________________
#ifndef PIZEROFINDER_HH
#define PIZEROFINDER_HH

#include "TVector3.h"
#include "TLorentzVector.h"
#include "Tracker.hh"

#include "LbnoDataLib.hh"

const double PIZEROMASS = 134.9766*CLHEP::MeV; // in MeV

class PiZeroFinder{

  public:
	PiZeroFinder();
	PiZeroFinder(ScintHitCollection *unmatchedHits);
	~PiZeroFinder();

	//nth photon track in scintillator, originating from pi0
	TLorentzVector ReconstructPhotonKinematics(int n,scintTracks &piZeroTracks,TLorentzVector &eventVertex);
	std::pair<TLorentzVector,TLorentzVector> 
	   GetPhotonPairKinematics(int n,std::vector<GeantPrimaryParticle> *primaries,scintTracks &piZeroTracks,TLorentzVector &eventVertex,bool useTruthE=false);
	double ReconstructPiZeroEnergy(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics);
	double ReconstructPiZeroInvariantMass(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics);
	TLorentzVector ReconstructPhotonUnitMomentum(ScintHitCollection &track,TLorentzVector &eventVertex);

	//without using the truth information that are already paired
	std::pair<TLorentzVector,TLorentzVector> MatchPhotonPair(int n,scintTracks &piZeroTracks,TLorentzVector &eventVertex,double massCutValue);

	double CalculateAngleBetweenPhotons(std::pair<TLorentzVector,TLorentzVector> &photonPairMom);
	double CalculateInvariantMass(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics,double angleInRad);
	double CalculateInvariantMass(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics);
	double CalculateThreeMomentumMag(TLorentzVector &vector);

	int GetNumberOfPiZerosInEvent(std::vector<GeantPrimaryParticle> *primaries) const;
	double GetTotalPiZeroTruthEnergy(std::vector<GeantPrimaryParticle> *primaries) const;

	int ClosestHitInTrackToVertex(ScintHitCollection &track,TLorentzVector &eventVertex);
	bool IsHitInConeProjection(scintHit &initialHit,TVector3 &hitPosition,double thetaInDeg);

        bool FoundPiZero();
	scintTracks FindPhotonTracks(double coneThetaInDeg,ScintHitCollection &track,TLorentzVector &pos);
	void setUnmatchedHits(ScintHitCollection* hits) {unmatchedHits_ = hits;};

	scintTracks * getUnmatchedPhotonTrackVtr()	{return unmatchedPhotonTracks_;};

  private:
	ScintHitCollection* unmatchedHits_;
	scintTracks * unmatchedPhotonTracks_;

	TVector3 tpcOrigin_;
	double theta_;

};
#endif
