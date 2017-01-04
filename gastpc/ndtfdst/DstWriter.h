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
  genie::NtpMCEventRecord* gmcrec; ///< Pointer to Genie's event record

  int RunID;
  int EventID; 
  int Sample;  ///< Sample ID (as defined by VALOR)
  int NTracks; ///< Number of tracks in neutrino interaction

  double Ev;      ///< Neutrino energy
  double Ev_reco; ///< Reconstructed neutrino energy
  double Y;       ///< Inelasticity
  double Y_reco;  ///< Reconstructed inelasticity

  double VertexPosition[4]; ///< Initial vertex (position and time)

  int TrackID[500];         ///< MC track ID number
  int RecoTrack[500];       ///< Reconstructed: 1; Not reco: 0
  int FamilyTreeLevel[500]; ///< Primary: 1; Secondary: 2 ...
  int Pdg[500];             ///< PDG code of each track
  int Pdg_reco[500];        ///< Reconstructed PDG code

  double Momentum[500];      ///< Initial momentum
  double Momentum_reco[500]; ///< Reconstructed momentum

public:
  /// Constructor
  DstWriter();
  /// Destructor
  ~DstWriter();
  
  void OpenFile(const std::string& filename, 
                const std::string& option="RECREATE");

  void CloseFile();

  void Write();

  bool IsFileOpen() const;

private:
  TFile* file_;
  TTree* tree_;
};

#endif