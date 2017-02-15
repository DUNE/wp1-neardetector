// -------------------------------------------------------------------
/// \file   MCParticle.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "MCHit.h"

ClassImp(gastpc::MCHit);


namespace gastpc {

  MCHit::MCHit(): xyzt_(), amplitude_(0.)
  {
  }


  MCHit::~MCHit()
  {
  }


  void MCHit::SetXYZT(double x, double y, double z, double t)
  {
    xyzt_.SetX(x);
    xyzt_.SetY(y);
    xyzt_.SetZ(z);
    xyzt_.SetT(t);
  }


  const Vector4D& MCHit::GetXYZT() const
  {
    return xyzt_;
  }

} // namespace gastpc
