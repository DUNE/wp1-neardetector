// -------------------------------------------------------------------
/// \file   LBNFBeamTimeDistribution.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 June 2016
// -------------------------------------------------------------------

#ifndef LBNF_BEAM_TIME_DISTRIBUTION_H
#define LBNF_BEAM_TIME_DISTRIBUTION_H

#include <G4Types.hh>
#include <vector>


///

class LBNFBeamTimeDistribution
{
public:
  /// Constructor
  LBNFBeamTimeDistribution();
  /// Destructor
  ~LBNFBeamTimeDistribution();

  G4double TimeOffset();

private:
  void CalculateCPDF();

  G4double global_offset_;     ///< Global time offset
  G4double time_betw_buckets_; ///< Time between buckets
  G4double bucket_time_sigma_; ///< How wide is distribution in bucket

  G4int buckets_per_batch_;
  G4int filled_buckets_per_batch_;

  std::vector<G4double> cpdf_batch_;
  std::vector<G4int> disallowed_batch_mask_;
};

#endif