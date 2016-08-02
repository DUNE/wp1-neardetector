// -------------------------------------------------------------------
/// \file   NuInteraction.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 2 Aug 2016
// -------------------------------------------------------------------

#include "NuInteraction.h"

#include <Ntuple/NtpMCEventRecord.h>


ClassImp(gastpc::NuInteraction);


namespace gastpc {

  NuInteraction::NuInteraction(): 
    spill_num_(-1), nu_energy_(0.), gheprec_(0)
  {
  }


  NuInteraction::~NuInteraction()
  {
  }

} // namespace gastpc