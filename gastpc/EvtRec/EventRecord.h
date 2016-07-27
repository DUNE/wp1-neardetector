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


class gastpc::EventRecord
{
public:
  /// Default constructor
  EventRecord();
  /// Destructor
  ~EventRecord();
  
  ClassDef(gastpc::EventRecord, 1)
};

#endif