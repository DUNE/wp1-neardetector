// -------------------------------------------------------------------
/// \file   EventRecord.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#include "EventRecord.h"

#include "MCGenInfo.h"
#include "MCParticle.h"
#include "MCTrack.h"

#include <Ntuple/NtpMCEventRecord.h>

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
    for (MCGenInfo* mcgi: mcgeninfo_) delete mcgi;
    mcgeninfo_.clear();

    for (MCParticle* mcp: mcparticles_) delete mcp;
    mcparticles_.clear();

    for (MCTrack* mct: mctracks_) delete mct;
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


  void EventRecord::Add(MCGenInfo* p)
  {
    mcgeninfo_.push_back(p);
  }


  const std::vector<MCParticle*>& EventRecord::GetMCParticles() const
  {
    return mcparticles_;
  }


  const std::vector<MCTrack*>& EventRecord::GetMCTracks() const
  {
    return mctracks_;
  }


  const std::vector<MCGenInfo*>& EventRecord::GetMCGenInfo() const
  {
    return mcgeninfo_;
  }


  void EventRecord::Print(std::ostream& os) const
  {
    os << "EVENT RECORD " << this->GetEventID()
       << " (RUN " << this->GetRunID() << ")" << std::endl;

    for (gastpc::MCGenInfo* mcgi: this->GetMCGenInfo()) {
      genie::NtpMCEventRecord* gmcrec = mcgi->GetGeneratorRecord();
      std::cout << *gmcrec << std::endl;
      for (gastpc::MCParticle* mcp: mcgi->GetMCParticles()) {
        os << *mcp << std::endl;
      }
    }
  }

} // namespace gastpc

std::ostream& operator << (std::ostream& os, const gastpc::EventRecord& mcp)
{
  mcp.Print(os);
  return os;
}
