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

namespace gastpc { class EventRecord; }
namespace gastpc { class MCParticle; }
namespace gastpc { class MCTrack; }


class gastpc::EventRecord
{
public:
  /// Default constructor
  EventRecord();
  /// Destructor
  virtual ~EventRecord();

  void Add(gastpc::MCParticle*);
  void Add(gastpc::MCTrack*);

  const std::vector<gastpc::MCParticle*>& GetMCParticles() const;
  const std::vector<gastpc::MCTrack*>& GetMCTracks() const;

private:
  std::vector<gastpc::MCParticle*> mcparticles_;
  std::vector<gastpc::MCTrack*> mctracks_;

  ClassDef(gastpc::EventRecord, 1)
};

#endif