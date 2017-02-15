// -------------------------------------------------------------------
/// \file   MCTrack.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 31 July 2016
// -------------------------------------------------------------------

#include "MCTrack.h"
#include "MCHit.h"

ClassImp(gastpc::MCTrack);


namespace gastpc {

  MCTrack::MCTrack():
    energy_(0.), length_(0.), mcp_(0)
  {
  }


  MCTrack::~MCTrack()
  {
    this->Clear();
  }


  void MCTrack::Clear()
  {
    for (MCHit* mch: mchits_) delete mch;
    mchits_.clear();
    energy_ = 0.;
    length_ = 0.;
  }


  void MCTrack::Add(MCHit* hit)
  {
    mchits_.push_back(hit);
  }


  const std::vector<MCHit*>& MCTrack::GetMCHits() const
  {
    return mchits_;
  }

} // namespace gastpc
