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
    amplitude_(0.), xyzt_(TLorentzVector()), mcparticle_(0)
  {
  }


  MCHit::~MCHit()
  {
  }

} // namespace gastpc

