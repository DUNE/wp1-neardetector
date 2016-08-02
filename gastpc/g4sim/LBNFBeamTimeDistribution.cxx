// -------------------------------------------------------------------
/// \file   LBNFBeamTimeDistribution.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 June 2016
// -------------------------------------------------------------------

#include "LBNFBeamTimeDistribution.h"

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
}


LBNFBeamTimeDistribution::~LBNFBeamTimeDistribution()
{
}


void LBNFBeamTimeDistribution::CalculateCPDF()
{
  cpdf_batch_.clear();
  G4double sum = 0;

  std::vector<double> batch_intensity(6, 1.0);

  for (size_t i=0; i<batch_intensity.size(); ++i) {
    sum += batch_intensity[i];
    cpdf_batch_.push_back(sum);
  }

  // normalize to unit probability
  for (size_t i=0; i < batch_intensity.size(); ++i) {
    cpdf_batch_[i] /= sum;
  }

  // make sure the mask vector keeps up (but never make it smaller)
  // allowing all new batches
  if ( batch_intensity.size() > disallowed_batch_mask_.size() ) 
    disallowed_batch_mask_.resize(batch_intensity.size(), 0);
}


G4double LBNFBeamTimeDistribution::TimeOffset()
{
  // calculate in small to large

  // pick a time within a bucket
  G4double offset = G4RandGauss::shoot(0.0, bucket_time_sigma_);

  // pick a bucket within a batch
  // assume all ~ buckets constant in batch until we have another model
  G4int bucket_num = 
    std::floor(G4RandFlat::shoot(0., filled_buckets_per_batch_));
  offset +=  time_betw_buckets_ * G4double(bucket_num);

  // pick a batch
  bool   disallowed = true;
  size_t ibatch = 0; 
  size_t nbatch = cpdf_batch_.size();
  double r = 2;
  while (disallowed) {
    r = G4RandFlat::shoot();
    for (ibatch=0; ibatch < nbatch; ++ibatch) {
      if (r <= cpdf_batch_[ibatch]) break;
    }
    disallowed = (disallowed_batch_mask_[ibatch] != 0);
  }
  offset += time_betw_buckets_*G4double(buckets_per_batch_)*G4double(ibatch);

  // finally the global offset
  return (offset + global_offset_);
}
