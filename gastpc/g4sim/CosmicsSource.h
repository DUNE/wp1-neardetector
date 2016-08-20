// -------------------------------------------------------------------
/// \file   CosmicsSource.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Aug 2016
// -------------------------------------------------------------------

#ifndef COSMICS_SOURCE_H
#define COSMICS_SOURCE_H

#include "BeamSpillSource.h"
#include <G4String.hh>

namespace genie { class NtpMCEventRecord; }
class TChain;


/// TODO: Class description

class CosmicsSource: public BeamSpillSource
{
public:
  /// Constructor
  CosmicsSource(const G4String& path);
  /// Destructor
  virtual ~CosmicsSource();

  virtual void GeneratePrimaryVertices(G4Event*);

private:
  void Initialize(const G4String&);

private:
  TChain* ghep_chain_; ///< Collection of input ghep files
  genie::NtpMCEventRecord* gmcrec_; ///< Pointer to the current GHEP record
};

#endif