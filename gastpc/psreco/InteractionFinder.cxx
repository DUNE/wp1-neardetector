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

#include <TGeoManager.h>
#include <TGeoNode.h>

#include <iostream>


InteractionFinder::InteractionFinder(const std::string& geometry)
{
  LoadGeometry(geometry);
}


InteractionFinder::~InteractionFinder()
{
}


void InteractionFinder::LoadGeometry(const std::string& geometry)
{
  gm_ = new TGeoManager();
  gm_->Import(geometry.c_str());
  if (!(gm_->GetTopNode())) {
    std::cerr << "ERROR: Failed opening geometry file." << std::endl;
    exit(EXIT_FAILURE);
  }
}


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
    double limit = 2.45/2. * gastpc::meter;

    if (std::abs(x) < limit && std::abs(y) < limit) {
      std::cout << "Argon" << std::endl;
      result = mcgi;
      break;
    }
  }

  return result;
}
