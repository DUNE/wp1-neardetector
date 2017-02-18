// -------------------------------------------------------------------
/// \file   EventRecord.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#ifndef GASTPC_EVENT_RECORD_H
#define GASTPC_EVENT_RECORD_H

#include <Rtypes.h>
#include <iostream>

namespace gastpc { class EventRecord; }
namespace gastpc { class MCGenInfo; }
namespace gastpc { class MCParticle; }
namespace gastpc { class MCTrack; }
namespace gastpc { class RecoParticle; }


/// TODO: Class description

class gastpc::EventRecord
{
public:
  /// Default constructor
  EventRecord();
  /// Destructor
  virtual ~EventRecord();

  void SetRunID(int);
  int GetRunID() const;

  void SetEventID(int);
  int GetEventID() const;

  void Add(gastpc::MCParticle*);
  void Add(gastpc::MCTrack*);
  void Add(gastpc::MCGenInfo*);
  void Add(gastpc::RecoParticle*);

  const std::vector<gastpc::MCParticle*>& GetMCParticles() const;
  const std::vector<gastpc::MCTrack*>& GetMCTracks() const;
  const std::vector<gastpc::MCGenInfo*>& GetMCGenInfo() const;
  const std::vector<gastpc::RecoParticle*>& GetRecoParticles() const;

  /// Destroy all objects in the event record
  void Clear();

  void Print(std::ostream& os=std::cout) const;

private:
  int run_id_;
  int event_id_;

  std::vector<gastpc::MCParticle*> mcparticles_;
  std::vector<gastpc::MCTrack*> mctracks_;
  std::vector<gastpc::MCGenInfo*> mcgeninfo_;
  std::vector<gastpc::RecoParticle*> recoparticles_;

  ClassDef(gastpc::EventRecord, 3)
};

std::ostream& operator << (std::ostream&, const gastpc::EventRecord&);

// Inline definitions //////////////////////////////////////
inline void gastpc::EventRecord::SetRunID(int id) { run_id_ = id; }
inline int  gastpc::EventRecord::GetRunID() const { return run_id_; }
inline void gastpc::EventRecord::SetEventID(int id) { event_id_ = id; }
inline int  gastpc::EventRecord::GetEventID() const { return event_id_; }

#endif
