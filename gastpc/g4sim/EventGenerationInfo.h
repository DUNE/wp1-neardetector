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
#include <Ntuple/NtpMCEventRecord.h>
#include <vector>


/// TODO: Class description

class EventGenerationInfo: public G4VUserEventInformation
{
public:
  /// Constructor
  EventGenerationInfo();
  /// Destructor
  ~EventGenerationInfo();

  virtual void Print() const;

  void AddEntry(genie::NtpMCEventRecord*);
  const std::vector<genie::NtpMCEventRecord*>& GetEntries() const;

private:
  std::vector<genie::NtpMCEventRecord*> gmcrecs_;
};

// Inline definitions //////////////////////////////////////

inline EventGenerationInfo::EventGenerationInfo(): G4VUserEventInformation() {}
inline EventGenerationInfo::~EventGenerationInfo() {}
inline void EventGenerationInfo::Print() const {}
inline void EventGenerationInfo::AddEntry(genie::NtpMCEventRecord* p)
{ gmcrecs_.push_back(p); }
inline const std::vector<genie::NtpMCEventRecord*>& 
  EventGenerationInfo::GetEntries() const { return gmcrecs_; }

#endif