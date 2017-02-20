// -------------------------------------------------------------------
/// \file   InteractionFinder.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#include "InteractionFinder.h"

#include "MCGenInfo.h"

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
    genie::EventRecord* record = gmcrec->event;

    TGeoNode* node = gm_->FindNode(record->Vertex()->X() * 100.,
                                   record->Vertex()->Y() * 100.,
                                   record->Vertex()->Z() * 100.);

    std::cout << node->GetName() << std::endl;

    if (strncmp(node->GetName(), "ACTIVE", 6) == 0)  {
      result = mcgi;
      break;
    }
  }

  return result;
}
