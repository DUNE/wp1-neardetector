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

  /// Set particle that generated the hit
  void SetParticle(gastpc::MCParticle*);
  /// Return particle associated to this hit
  gastpc::MCParticle* GetParticle() const;

  ///
  void SetAmplitude(double);
  ///
  double GetAmplitude() const;

  /// Set position and time associated to this hit
  void SetXYZT(double, double, double, double);
  /// Return hit's position and time
  const TLorentzVector& GetXYZT() const;

private:
  double amplitude_;        ///< Hit signal amplitude (e.g. edep)
  TLorentzVector xyzt_;     ///< Hit position and time
  gastpc::MCParticle* mcp_; ///< Pointer to associated particle

  ClassDef(gastpc::MCHit, 1)
};

// Inline definitions //////////////////////////////////////

inline void gastpc::MCHit::SetAmplitude(double a) { amplitude_ = a; }
inline double gastpc::MCHit::GetAmplitude() const { return amplitude_; }

inline void gastpc::MCHit::SetParticle(gastpc::MCParticle* p) { mcp_ = p; }
inline gastpc::MCParticle* gastpc::MCHit::GetParticle() const { return mcp_; } 

inline const TLorentzVector& gastpc::MCHit::GetXYZT() const { return xyzt_; } 

#endif
