//____________________________________________________________________________
/*!

\class    PiZeroRecon

\brief    Class for reconstructing pi zeros

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Oct 2014
\last update Oct 2014

*/
//____________________________________________________________________________
#ifndef PIZERORECON_HH
#define PIZERORECON_HH

#include "TVector3.h"
#include "TLorentzVector.h"
#include "Tracker.hh"

#include "GasTPCDataLib.hh"

class PiZeroRecon{

  public:
	PiZeroRecon();
	PiZeroRecon(ScintHitCollection *unmatchedHits);
	~PiZeroRecon();

	TLorentzVector GetPhotonUnitMomentum(const ScintHitCollection &track);
	TLorentzVector ReconstructPhotonUnitMomentum(const ScintHitCollection &track, const TLorentzVector &eventVertex);
	TLorentzVector ReconstructPhotonKinematics(const ScintHitCollection &photonTrack, const TLorentzVector &eventVertex,bool useTruth=false,double energyResolution=0.0);

	std::pair<TLorentzVector,TLorentzVector> 
	   GetPhotonPairKinematics(int n, const std::vector<GeantPrimaryParticle> *primaries, const scintTracks &piZeroTracks, const TLorentzVector &eventVertex,bool useTruthE=false);

	double GetTotalEnergy(const scintTracks& photonTracks,double energyResolution=0.0);
	double ReconstructPiZeroEnergy(const std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics);
	double ReconstructPiZeroInvariantMass(const std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics);

	//without using the truth information that are already paired
	std::vector<TLorentzVector> SortPhotons(scintTracks photonTracks, const TLorentzVector &eventVertex, const double massCutValue);
	std::pair<TLorentzVector,TLorentzVector> MatchPhotonPair(const int n, scintTracks &piZeroTracks, const TLorentzVector &eventVertex, const double massCutValue);

	double CalculateAngleBetweenPhotons( const std::pair<TLorentzVector,TLorentzVector> &photonPairMom);
	double CalculateInvariantMass( const std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics, const double angleInRad);
	double CalculateInvariantMass( const std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics);
	double CalculateThreeMomentumMag( const TLorentzVector &vector);

	int GetNumberOfPiZerosInEvent( const std::vector<GeantPrimaryParticle> *primaries) const;
	double GetTotalPiZeroTruthEnergy( const std::vector<GeantPrimaryParticle> *primaries) const;

	int ClosestHitInTrackToVertex( const ScintHitCollection &track, const TLorentzVector &eventVertex);

	bool IsHitInConeProjection( const scintHit &initialHit, const TVector3 &hitPosition, const double thetaInDeg);

        bool FoundPiZero();
	scintTracks FindPhotonTracks( const double coneThetaInDeg, const ScintHitCollection &track, const TLorentzVector &pos);
	void setUnmatchedHits(ScintHitCollection* hits) {unmatchedHits_ = hits;};

	scintTracks * getUnmatchedPhotonTrackVtr()	{return unmatchedPhotonTracks_;};

	void 		SetScaleFactor(double factor)		{scaleFactor_ = factor;};

  private:
	ScintHitCollection* unmatchedHits_;
	scintTracks * unmatchedPhotonTracks_;

	TVector3 tpcOrigin_;
	double theta_;
	double scaleFactor_;

};
#endif
