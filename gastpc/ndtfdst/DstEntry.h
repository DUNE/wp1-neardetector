// -------------------------------------------------------------------
/// \file   DstEntry.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 5 Dec 2016
// -------------------------------------------------------------------

#ifndef DST_ENTRY_H
#define DST_ENTRY_H

namespace genie { class NtpMCEventRecord;}


/// TODO. Class description

class DstEntry
{
public:
  genie::NtpMCEventRecord* gmcrec;
  int     RunID;
  int     EventID;
/*  int     Sample;
  double  Ev_reco;
  double  Ev;
  double  Y;
  double  Y_reco;
  double  VertexPosition[4];
  int     NGeantTracks;
  int     TrackID[500];
  double  Momentum[500];
  int     Pdg[500];
  double  TotalEDep[500];
  double  dEdx[500];
  int     NGeantHits[500];*/

public:
  DstEntry();
  ~DstEntry();
};

inline DstEntry::DstEntry(): gmcrec(0) {}
inline DstEntry::~DstEntry() {}

#endif