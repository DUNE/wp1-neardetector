// -------------------------------------------------------------------
///  \file   EventGenerationInfo.cxx
///  \brief  Wrapper class that contains all used GHEP records.
///
///  \author  <justo.martin-albo \at physics.ox.ac.uk>
///  \date    Created: 3 Aug 2016
// -------------------------------------------------------------------

#include "EventGenerationInfo.h"

#include <Ntuple/NtpMCEventRecord.h>
#include <iostream>


EventGenerationInfo::EventGenerationInfo(): G4VUserEventInformation()
{
}


EventGenerationInfo::~EventGenerationInfo()
{
  for (genie::NtpMCEventRecord* p: gmcrecs_) delete p;
  gmcrecs_.clear();
}


void EventGenerationInfo::AddEntry(genie::NtpMCEventRecord* p)
{
  gmcrecs_.push_back(p);
}


const std::vector<genie::NtpMCEventRecord*>& EventGenerationInfo::GetEntries() const
{
  return gmcrecs_;
}


void EventGenerationInfo::DropEntries()
{
  // Empty the vector. The user is responsible now
  // for the destruction of the GHEP records.
  gmcrecs_.clear();
}
