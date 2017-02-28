// -------------------------------------------------------------------
/// \file   MomentumSmearer.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#ifndef MOMENTUM_SMEARER_H
#define MOMENTUM_SMEARER_H

#include <utility>

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

  void ProcessParticle(std::pair<gastpc::MCParticle*, gastpc::RecoParticle*>&);

private:
  void ProcessTrack(gastpc::MCTrack*, gastpc::RecoParticle*);
  double SmearPt(double, double);
  double SmearAngle(double, double, double);

private:
  TRandom3* rnd_;
};

#endif
