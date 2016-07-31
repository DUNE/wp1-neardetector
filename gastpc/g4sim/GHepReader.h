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

class TChain;

namespace genie { class NtpMCEventRecord; }


class GHepReader: public BeamSpillSource
{
public:
  /// Constructor
  GHepReader(const G4String& path, G4double mean);
  /// Destructor
  virtual ~GHepReader();

  virtual void FillG4Event(G4Event*);

private:
  void Initialize(const G4String&);

private:
  TChain* ifiles_;
  genie::NtpMCEventRecord* mcrec_;

  G4int max_entries_;
  G4int current_entry_;

};

#endif