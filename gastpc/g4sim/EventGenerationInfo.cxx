// -------------------------------------------------------------------
///  \file   EventGenerationInfo.cxx
///  \brief
///
///  \author  <justo.martin-albo \at physics.ox.ac.uk>
///  \date    Created: 3 Aug 2016
// -------------------------------------------------------------------

#include "EventGenerationInfo.h"

#include <Ntuple/NtpMCEventRecord.h>



EventGenerationInfo::EventGenerationInfo(): G4VUserEventInformation()
{
}


EventGenerationInfo::~EventGenerationInfo()
{
  // This class does not own the pointers to the GHEP records, and thus
  // we will assume that someone else (the gastpc::EventRecord, normally)
  // is taking care of deleting them when they are no longer needed.
}


void EventGenerationInfo::AddEntry(genie::NtpMCEventRecord* p)
{
  gmcrecs_.push_back(p);
}


const std::vector<genie::NtpMCEventRecord*>& EventGenerationInfo::GetEntries() const
{
  return gmcrecs_;
}