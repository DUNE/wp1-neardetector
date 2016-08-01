// -------------------------------------------------------------------
/// \file   EventRecord.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#include "EventRecord.h"

#include "MCParticle.h"
#include "MCTrack.h"

ClassImp(gastpc::EventRecord);


namespace gastpc {

  EventRecord::EventRecord()
  {}


  EventRecord::~EventRecord()
  {
    mcparticles_.clear();
    mctracks_.clear();
  }


  void EventRecord::Add(MCParticle* p)
  {
    mcparticles_.push_back(p);
  }


  void EventRecord::Add(MCTrack* p)
  {
    mctracks_.push_back(p);
  }


  const std::vector<MCParticle*>& EventRecord::GetMCParticles() const
  {
    return mcparticles_;
  }


  const std::vector<MCTrack*>& EventRecord::GetMCTracks() const
  {
    return mctracks_;
  }

} // namespace gastpc
