// -------------------------------------------------------------------
/// \file   EventRecord.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#include "EventRecord.h"

#include "NuInteraction.h"
#include "MCParticle.h"
#include "MCTrack.h"

ClassImp(gastpc::EventRecord);


namespace gastpc {

  EventRecord::EventRecord(): run_id_(-1), event_id_(-1)
  {
  }


  EventRecord::~EventRecord()
  {
    this->Clear();
  }


  void EventRecord::Clear()
  {
    for (MCParticle* mcp: mcparticles_) delete mcp;
    mcparticles_.clear();

    for (MCTrack* mct: mctracks_) delete mct;
    mctracks_.clear();

    for (NuInteraction* nuint: nuints_) delete nuint;
    nuints_.clear();
  }


  void EventRecord::Add(MCParticle* p)
  {
    mcparticles_.push_back(p);
  }


  void EventRecord::Add(MCTrack* p)
  {
    mctracks_.push_back(p);
  }


  void EventRecord::Add(NuInteraction* p)
  {
    nuints_.push_back(p);
  }


  const std::vector<MCParticle*>& EventRecord::GetMCParticles() const
  {
    return mcparticles_;
  }


  const std::vector<MCTrack*>& EventRecord::GetMCTracks() const
  {
    return mctracks_;
  }


  const std::vector<NuInteraction*>& EventRecord::GetNuInteractions() const
  {
    return nuints_;
  }

} // namespace gastpc
