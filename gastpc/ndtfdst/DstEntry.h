// -------------------------------------------------------------------
/// \file   DstEntry.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 3 Dec 2016
// -------------------------------------------------------------------

#ifndef DST_ENTRY_H
#define DST_ENTRY_H

#include <Rtypes.h>

namespace genie { class NtpMCEventRecord; }


/// TODO. Class description.

class DstEntry
{
public:
  genie::NtpMCEventRecord* gmcrec;
  int     RunID;
  int     EventID;
  int     Sample;
  int     NGeantTracks;
  double  Ev_reco;
  double  Ev;
  double  Y;
  double  Y_reco;
  int     TrackID[500];
  int     Pdg[500];
  int     NGeantHits[500];
  double  VertexPosition[4];
  double  Momentum[500];
  double  TotalEDep[500];
  double  dEdx[500];

public:
  DstEntry();
  virtual ~DstEntry();
  
  ClassDef(DstEntry, 1)
};

inline DstEntry::DstEntry(): gmcrec(0) {}
inline DstEntry::~DstEntry() {}

#endif
