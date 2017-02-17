// -------------------------------------------------------------------
/// \file   MCHit.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef GASTPC_MCHIT_H
#define GASTPC_MCHIT_H

#include "Vector4D.h"
#include <Rtypes.h>
#include <vector>

namespace gastpc { class MCHit; }


/// TODO: Class description

class gastpc::MCHit
{
public:
  /// Default constructor
  MCHit();
  /// Destructor
  virtual ~MCHit();

  /// Set position and time associated to this hit
  void SetXYZT(double, double, double, double);
  /// Return hit position and time
  const gastpc::Vector4D& GetXYZT() const;

  ///
  void SetAmplitude(double);
  ///
  double GetAmplitude() const;

private:
  gastpc::Vector4D xyzt_; ///< Hit position and time
  double amplitude_;      ///< Hit signal amplitude (e.g. edep)

  ClassDef(gastpc::MCHit, 4)
};

// Inline definitions //////////////////////////////////////

inline void gastpc::MCHit::SetAmplitude(double a) { amplitude_ = a; }
inline double gastpc::MCHit::GetAmplitude() const { return amplitude_; }

#endif
