// -------------------------------------------------------------------
/// \file   NuInteraction.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 2 Aug 2016
// -------------------------------------------------------------------

#include "NuInteraction.h"

#include "MCParticle.h"

#include <Ntuple/NtpMCEventRecord.h>


ClassImp(gastpc::NuInteraction);


namespace gastpc {

  NuInteraction::NuInteraction(): 
    mcid_(-1), gheprec_(0)
  {
  }


  NuInteraction::~NuInteraction()
  {
  }


  void NuInteraction::AddParticle(MCParticle* mcp)
  {
    mcparticles_.push_back(mcp);
  }


  const std::vector<MCParticle*>& NuInteraction::GetParticles() const
  {
    return mcparticles_;
  }

} // namespace gastpc