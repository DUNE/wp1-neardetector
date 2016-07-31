// -------------------------------------------------------------------
/// \file   BeamSpillSource.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 7 June 2016
// -------------------------------------------------------------------

#ifndef GASTPC_BEAM_SPILL_SOURCE_H
#define GASTPC_BEAM_SPILL_SOURCE_H

class G4Event;


class BeamSpillSource
{
public:
  /// Constructor
  BeamSpillSource();
  /// Destructor
  virtual ~BeamSpillSource();

  virtual void FillG4Event(G4Event*) = 0;
};

inline BeamSpillSource::BeamSpillSource() {}
inline BeamSpillSource::~BeamSpillSource() {}

#endif