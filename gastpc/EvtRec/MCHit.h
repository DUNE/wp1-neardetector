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
#include <TVector3.h>
#include "MCParticle.h"

namespace gastpc { class MCHit; }


/// TODO: Class description

class gastpc::MCHit
{
public:
  /// Default constructor
  MCHit();
  /// Destructor
  ~MCHit();

  /// Return particle associated to this hit
  const gastpc::MCParticle& GetParticle() const;
  /// Set particle that generated the hit
  void SetParticle(gastpc::MCParticle*);

  void SetAmplitude(double);
  double GetAmplitude() const;

  void SetTime(double);
  double GetTime() const;

  void SetPosition(double, double, double);
  const TVector3& GetPosition() const;

private:
  double amplitude_;
  double time_;
  TVector3 position_;
  gastpc::MCParticle* mcparticle_; ///< Pointer to associated particle

  ClassDef(gastpc::MCHit, 1)
};

inline void gastpc::MCHit::SetAmplitude(double a) { amplitude_ = a; }
inline double gastpc::MCHit::GetAmplitude() const { return amplitude_; }
inline void gastpc::MCHit::SetTime(double t) { time_ = t; }
inline double gastpc::MCHit::GetTime() const { return time_; }
inline void gastpc::MCHit::SetParticle(gastpc::MCParticle* p) 
{ mcparticle_ = p; };
inline const gastpc::MCParticle& gastpc::MCHit::GetParticle() const 
{ return *mcparticle_; } 

#endif