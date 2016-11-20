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

  MCHit::MCHit():
    x_(0.), y_(0.), z_(0.), time_(0.), amplitude_(0.)
  {
  }


  MCHit::~MCHit()
  {
  }


  void MCHit::SetXYZT(double x, double y, double z, double t)
  {
    x_ = x;
    y_ = y;
    z_ = z;
    time_ = t;
  }


  std::vector<double> MCHit::GetXYZT() const
  {
    std::vector<double> v = {x_, y_, z_, time_};
    return v;
  }

} // namespace gastpc
