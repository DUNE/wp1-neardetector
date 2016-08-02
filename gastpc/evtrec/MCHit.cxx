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
    amplitude_(0.), xyzt_(TLorentzVector()), mcp_(0)
  {
  }


  MCHit::~MCHit()
  {
  }


  void MCHit::SetXYZT(double x, double y, double z, double t)
  {
    xyzt_.SetXYZT(x, y, z, t);
  }

} // namespace gastpc
