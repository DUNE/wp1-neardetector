// -------------------------------------------------------------------
/// \file   GHepReader.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 June 2016
// -------------------------------------------------------------------

#ifndef GASTPC_GHEP_READER_H
#define GASTPC_GHEP_READER_H

#include "BeamSpillSource.h"
#include <G4String.hh>

namespace genie { class NtpMCEventRecord; }
class TChain;



/// TODO: Class description

class GHepReader: public BeamSpillSource
{
public:
  /// Constructor
  GHepReader(const G4String& path, G4double mean);
  /// Destructor
  virtual ~GHepReader();

  virtual void GeneratePrimaryVertices(G4Event*);

private:
  void Initialize(const G4String&);

private:
  TChain* ghep_chain_;   ///< Collection of input ghep files
  G4long num_entries_;   ///< Total number of entries in chain
  G4long current_entry_; ///< Last entry number we've read from the source

  genie::NtpMCEventRecord* gmcrec_;
};

#endif