// -------------------------------------------------------------------
/// \file   MomentumSmearer.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#ifndef MOMENTUM_SMEARER_H
#define MOMENTUM_SMEARER_H

namespace gastpc { class MCParticle; }
namespace gastpc { class RecoParticle; }
namespace gastpc { class MCTrack; }
namespace gastpc { class Vector3D; }
class TRandom3;


/// TODO: CLASS DESCRIPTION

class MomentumSmearer
{
public:
  /// Constructor
  MomentumSmearer(TRandom3*);
  /// Destructor
  ~MomentumSmearer();

  gastpc::RecoParticle* ProcessParticle(gastpc::MCParticle*);

private:
  gastpc::RecoParticle* ProcessTrack(gastpc::MCTrack*);
  double MomentumMag(const gastpc::Vector3D&);
  double SmearPt(double, double);
  double SmearAngle(double, double, double);

private:
  TRandom3* rnd_;
};

#endif
