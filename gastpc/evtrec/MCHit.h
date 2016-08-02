// -------------------------------------------------------------------
/// \file   MCHit.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef GASTPC_MCHIT_H
#define GASTPC_MCHIT_H

#include <Rtypes.h>
#include <TLorentzVector.h>

namespace gastpc { class MCParticle; }
namespace gastpc { class MCHit; }


/// TODO: Class description

class gastpc::MCHit
{
public:
  /// Default constructor
  MCHit();
  /// Destructor
  virtual ~MCHit();

  /// Return particle associated to this hit
  const gastpc::MCParticle& GetParticle() const;
  /// Set particle that generated the hit
  void SetParticle(gastpc::MCParticle*);

  ///
  void SetAmplitude(double);
  ///
  double GetAmplitude() const;

  ///
  void SetXYZT(double, double, double, double);
  ///
  const TLorentzVector& GetXYZT() const;

private:
  double amplitude_;               ///< Hit signal amplitude (e.g. edep)
  TLorentzVector xyzt_;            ///< Hit position and time
  gastpc::MCParticle* mcparticle_; ///< Pointer to associated particle

  ClassDef(gastpc::MCHit, 1)
};

// Inline definitions //////////////////////////////////////

inline void gastpc::MCHit::SetAmplitude(double a) { amplitude_ = a; }
inline double gastpc::MCHit::GetAmplitude() const { return amplitude_; }

inline void gastpc::MCHit::SetParticle(gastpc::MCParticle* p) 
  { mcparticle_ = p; }
inline const gastpc::MCParticle& gastpc::MCHit::GetParticle() const 
  { return *mcparticle_; } 

inline void gastpc::MCHit::SetXYZT(double x, double y, double z, double t)
  { xyzt_.SetXYZT(x,y,z,t); }
inline const TLorentzVector& gastpc::MCHit::GetXYZT() const { return xyzt_; } 

#endif