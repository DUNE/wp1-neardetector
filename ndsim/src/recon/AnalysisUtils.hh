//____________________________________________________________________________
/*!

\class    AnalysisUtils

\brief    Analysis Utilities

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
         University of Liverpool

\created June 2016
*/
//____________________________________________________________________________
#ifndef ANALUTILS_HH
#define ANALUTILS_HH

#include <iostream>
#include <vector>

#include <TVector3.h>

#include "GasTPCDataLib.hh"

namespace AnalysisUtils{

  // Find vertices in gas volume
  std::vector<TVector3> FindTPCVertices(std::vector<TrackParticle> Event);

  // Find isolated TPC tracks
  std::vector<TVector3> FindIsoTPCTracks(std::vector<TrackParticle> Event);

  // 
  int OtherTracksFromEcal(std::vector<EcalTrackParticle> Event, TVector3 track_pos, double track_time); 
}

#endif
