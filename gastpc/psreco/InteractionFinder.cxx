// -------------------------------------------------------------------
/// \file   InteractionFinder.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#include "InteractionFinder.h"

#include "MCGenInfo.h"
#include "Units.h"

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

#include <iostream>


gastpc::MCGenInfo* InteractionFinder::ProcessEvent(const std::vector<gastpc::MCGenInfo*>& v)
{
  gastpc::MCGenInfo* result = 0;

  for (gastpc::MCGenInfo* mcgi: v) {
    genie::NtpMCEventRecord* gmcrec = mcgi->GetGeneratorRecord();
    genie::Interaction* interaction = (gmcrec->event)->Summary();
    const genie::Target& tgt = interaction->InitState().Tgt();
    if (tgt.Z() != 18) continue;

    double x = (gmcrec->event)->Vertex()->X() * gastpc::meter;
    double y = (gmcrec->event)->Vertex()->Y() * gastpc::meter;
    double limit = (2.46/2.) * gastpc::meter;

    if (std::abs(x) < limit && std::abs(y) < limit) {
      result = mcgi;
      break;
    }
  }

  return result;
}
