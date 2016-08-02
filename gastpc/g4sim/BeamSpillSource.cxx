// -------------------------------------------------------------------
/// \file   BeamSpillSource.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 June 2016
// -------------------------------------------------------------------

#include "BeamSpillSource.h"

#include "LBNFBeamTimeDistribution.h"

#include <G4Poisson.hh>
#include <Randomize.hh>



BeamSpillSource::BeamSpillSource(double mean): 
  mean_(0.), time_pdf_(0)
{
  time_pdf_ = new LBNFBeamTimeDistribution();
}


BeamSpillSource::~BeamSpillSource()
{
  delete time_pdf_;
}


int BeamSpillSource::NumberOfInteractions() const
{
  if (mean_ < 10.)
    return G4Poisson(mean_);
  else
    return std::ceil(G4RandGauss::shoot(mean_, std::sqrt(mean_)));
}


double BeamSpillSource::TimeOffset() const
{
  return time_pdf_->TimeOffset();
}