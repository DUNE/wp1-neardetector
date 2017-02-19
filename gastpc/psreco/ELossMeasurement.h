// -------------------------------------------------------------------
/// \file   ELossMeasurement.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 18 Feb 2017
// -------------------------------------------------------------------

#ifndef ELOSS_MEASUREMENT_H
#define ELOSS_MEASUREMENT_H

#include <string>
#include <set>

namespace gastpc { class MCTrack; }

class TH1F;


/// TODO: Class description

class ELossMeasurement
{
public:
  /// Constructor
  ELossMeasurement(gastpc::MCTrack*);
  /// Destructor
  ~ELossMeasurement();

  TH1F* Histogram(double min, double max, double truncation=1.0);

private:
  ///
  void ProcessTrack(gastpc::MCTrack*);

private:
  std::string label_;
  std::multiset<double> measurements_;
};

#endif
