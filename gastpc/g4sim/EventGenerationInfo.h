// -------------------------------------------------------------------
/// \file   EventGenerationInfo.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 3 Aug 2016
// -------------------------------------------------------------------

#ifndef EVENT_GENERATION_INFO
#define EVENT_GENERATION_INFO

#include <G4VUserEventInformation.hh>


class EventGenerationInfo: public G4VUserEventInformation
{
public:
  EventGenerationInfo();
  ~EventGenerationInfo();

  virtual void Print() const;

private:
  std::vector<genie::NtpMCEventRecord*> gmcrecs_;
};

inline EventGenerationInfo::EventGenerationInfo() {}
inline EventGenerationInfo::~EventGenerationInfo() {}
inline void EventGenerationInfo::Print() const {}

#endif