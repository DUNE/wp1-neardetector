// -------------------------------------------------------------------
/// \file   EventGenerationInfo.h
/// \brief  Wrapper class that contains all used GHEP records.
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 3 Aug 2016
// -------------------------------------------------------------------

#ifndef EVENT_GENERATION_INFO_H
#define EVENT_GENERATION_INFO_H

#include <G4VUserEventInformation.hh>
#include <vector>

namespace genie { class NtpMCEventRecord; }


/// This class is just a wrapper for a vector that contains all the GHEP
/// records read and used at the primary generation stage.
/// NB.- The destructor of this class DOES destroy the entries in the vector.
/// Therefore, if users want to claim ownership of the objects (for persistency
/// purposes, for instance) they must invoke the DropEntries() method or
/// they may face memory-related runtime errors.

class EventGenerationInfo: public G4VUserEventInformation
{
public:
  /// Constructor
  EventGenerationInfo();
  /// Destructor
  ~EventGenerationInfo();

  /// Add a GHEP record to the vector
  void AddEntry(genie::NtpMCEventRecord*);
  /// Return the vector of GHEP records
  const std::vector<genie::NtpMCEventRecord*>& GetEntries() const;
  /// Clear the vector WITHOUT destroying the entries
  void DropEntries();

private:
  virtual void Print() const;

private:
  std::vector<genie::NtpMCEventRecord*> gmcrecs_;
};

inline void EventGenerationInfo::Print() const {}

#endif
