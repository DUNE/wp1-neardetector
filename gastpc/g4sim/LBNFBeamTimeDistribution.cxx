// -------------------------------------------------------------------
/// \file   LBNFBeamTimeDistribution.cxx
/// \brief  LBNF beam spill time distribution
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 June 2016
// -------------------------------------------------------------------

#include "LBNFBeamTimeDistribution.h"

#include <globals.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>

#include <cmath>



LBNFBeamTimeDistribution::LBNFBeamTimeDistribution():
  global_offset_(0.),
  time_betw_buckets_(1./53.103e6 * second), // ~53 MHz RF frequency
  bucket_time_sigma_(0.750 * nanosecond),
  buckets_per_batch_(84),
  filled_buckets_per_batch_(81),
  disallowed_batch_mask_(6,0) // don't disallow any
{
  std::vector<G4double> batch_intensity(6, 1.0); // 6 equal batches
  CalculateCPDF(batch_intensity);
}


LBNFBeamTimeDistribution::~LBNFBeamTimeDistribution()
{
}


void LBNFBeamTimeDistribution::CalculateCPDF(const std::vector<G4double>& bi)
{
  cpdf_batch_.clear();
  G4double sum = 0;
  size_t nbi = bi.size();

  for (size_t i=0; i<nbi; ++i) {
    sum += bi[i];
    cpdf_batch_.push_back(sum);
  }

  // normalize to unit probability
  for (size_t i=0; i<nbi; ++i) cpdf_batch_[i] /= sum;

  // make sure the mask vector keeps up (but never make it smaller)
  // allowing all new batches
  if (nbi > disallowed_batch_mask_.size()) 
    disallowed_batch_mask_.resize(nbi, 0);
}


G4double LBNFBeamTimeDistribution::TimeOffset()
{
  // We'll calculate the components of the time offset from small to large

  // First, pick a time within a bucket
  G4double offset = G4RandGauss::shoot(0.0, bucket_time_sigma_);

  // Second, pick a bucket within a batch
  G4int bucket_num = 
    std::floor(G4RandFlat::shoot(0., filled_buckets_per_batch_));
  offset +=  time_betw_buckets_ * G4double(bucket_num);

  // Third, pick a batch
  G4bool disallowed = true;
  size_t ibatch = 0; 
  size_t nbatch = cpdf_batch_.size();
  G4double r = 2;
  while (disallowed) {
    r = G4RandFlat::shoot();
    for (ibatch=0; ibatch<nbatch; ++ibatch) 
      if (r <= cpdf_batch_[ibatch]) break;
    disallowed = (disallowed_batch_mask_[ibatch] != 0);
  }
  offset += time_betw_buckets_ * G4double(buckets_per_batch_) * G4double(ibatch);

  // Finally, add the global offset
  return (offset + global_offset_);
}
