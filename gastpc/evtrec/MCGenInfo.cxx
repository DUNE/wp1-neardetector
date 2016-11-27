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

  MCGenInfo::MCGenInfo(): mcid_(-1), gheprec_(0)
  {
  }


  MCGenInfo::~MCGenInfo()
  {
    gheprec_->Clear();
    delete gheprec_;
  }


  void MCGenInfo::SetGeneratorRecord(genie::NtpMCEventRecord* rec)
  {
    gheprec_ = rec;
  }


  genie::NtpMCEventRecord* MCGenInfo::GetGeneratorRecord()
  {
    return gheprec_;
  }


  void MCGenInfo::AddParticle(MCParticle* mcp)
  {
    mcparticles_.push_back(mcp);
  }


  const std::vector<MCParticle*>& MCGenInfo::GetParticles() const
  {
    return mcparticles_;
  }

} // namespace gastpc