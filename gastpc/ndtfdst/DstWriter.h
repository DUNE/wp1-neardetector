// -------------------------------------------------------------------
/// \file   DstWriter.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 11 Aug 2016
// -------------------------------------------------------------------

#ifndef DST_WRITER_H
#define DST_WRITER_H

#include <string>

namespace genie { class NtpMCEventRecord; }
class TFile;
class TTree;


/// TODO: Class description

class DstWriter
{
public:
  /// Constructor
  DstWriter();
  /// Destructor
  ~DstWriter();
  
  bool OpenFile(const std::string& filename, 
                const std::string& option="RECREATE");

  void CloseFile();

  void Write();

  bool IsFileOpen() const;

public:
  genie::NtpMCEventRecord* gmcrec;
  int     RunID;
  int     EventID;
  int     Sample;
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
  int     NGeantHits[500];

private:
  TFile* file_;
  TTree* tree_;
};

#endif