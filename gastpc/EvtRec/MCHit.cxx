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
    amplitude_(0.), time_(0.), position_(TVector3()), mcparticle_(0)
  {
  }


  MCHit::~MCHit()
  {
  }

} // namespace gastpc

