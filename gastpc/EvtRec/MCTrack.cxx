// -------------------------------------------------------------------
/// \file   MCTrack.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 31 July 2016
// -------------------------------------------------------------------

#include "MCTrack.h"


ClassImp(gastpc::MCTrack);


namespace gastpc {

  MCTrack::MCTrack(): 
    energy_(0.), length_(0.)
  {
  }


  MCTrack::~MCTrack()
  {
  }


  void MCTrack::AddHit(MCHit* hit)
  {
    mchits_.push_back(hit);
  }


  const std::vector<MCHit*>& MCTrack::GetHits() const
  {
    return mchits_;
  }


  void MCTrack::ClearHits()
  {
    mchits_.clear();
  }


} // namespace gastpc