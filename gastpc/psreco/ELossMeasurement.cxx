// -------------------------------------------------------------------
/// \file   ELossMeasurement.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 18 Feb 2017
// -------------------------------------------------------------------

#include "ELossMeasurement.h"

#include "MCParticle.h"
#include "MCTrack.h"
#include "MCHit.h"
#include "Vector4D.h"
#include "Utils.h"
#include "Units.h"

#include <TH1F.h>



ELossMeasurement::ELossMeasurement(gastpc::MCTrack* track)
{
  label_ = std::to_string(track->GetMCParticle()->GetMCID());
  label_.append("_");
  label_.append(track->GetLabel());

  ProcessTrack(track);
}


ELossMeasurement::~ELossMeasurement()
{
}


void ELossMeasurement::ProcessTrack(gastpc::MCTrack* track)
{
  const std::vector<gastpc::MCHit*>& mchits = track->GetMCHits();

  gastpc::Vector4D pre, post;
  pre = mchits[0]->GetXYZT();

  for (unsigned int i=1; i<mchits.size(); ++i) {
    post = mchits[i]->GetXYZT();
    double step_length = Utils::Distance(pre, post);
    double dEdx = mchits[i]->GetAmplitude() / step_length;
    measurements_.insert(dEdx);
    pre = post;
  }
}


double ELossMeasurement::Mean(double truncation)
{
  if (truncation > 1. || truncation < 0.) truncation = 1.0;
  int max_n = std::floor(measurements_.size() * truncation);

  int n = 0;
  double sum = 0.;

  for (double entry: measurements_) {
    if (n >= max_n) break;
    sum += entry;
    ++n;
  }

  return (sum / max_n);
}


TH1F* ELossMeasurement::Histogram(double min, double max, double truncation)
{
  double units = gastpc::keV / gastpc::cm;
  min = min / units;
  max = max / units;
  int num_bins = max - min;

  TH1F* h1 = new TH1F(label_.c_str(), "", num_bins, min, max);

  int n = 0;
  if (truncation > 1. || truncation < 0.) truncation = 1.0;
  int max_n = std::floor(measurements_.size() * truncation);

  for (double entry: measurements_) {
    if (n >= max_n) break;
    h1->Fill(entry/units);
    ++n;
  }

  return h1;
}
