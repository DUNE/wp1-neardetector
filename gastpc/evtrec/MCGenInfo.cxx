// -------------------------------------------------------------------
/// \file   MCGenInfo.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 27 Nov 2016
// -------------------------------------------------------------------

#include "MCGenInfo.h"

#include "MCParticle.h"
#include <Ntuple/NtpMCEventRecord.h>

ClassImp(gastpc::MCGenInfo);


namespace gastpc {

  MCGenInfo::MCGenInfo(): mcid_(-1), grec_(0)
  {
  }


  MCGenInfo::~MCGenInfo()
  {
    grec_->Clear();
    delete grec_;
  }


  void MCGenInfo::SetGeneratorRecord(genie::NtpMCEventRecord* grec)
  {
    grec_ = grec;
  }


  genie::NtpMCEventRecord* MCGenInfo::GetGeneratorRecord()
  {
    return grec_;
  }


  void MCGenInfo::AddMCParticle(MCParticle* mcp)
  {
    mcparticles_.push_back(mcp);
  }


  const std::vector<MCParticle*>& MCGenInfo::GetMCParticles() const
  {
    return mcparticles_;
  }

} // namespace gastpc
