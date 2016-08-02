// -------------------------------------------------------------------
/// \file   BeamSpillSource.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 7 June 2016
// -------------------------------------------------------------------

#ifndef GASTPC_BEAM_SPILL_SOURCE_H
#define GASTPC_BEAM_SPILL_SOURCE_H

class LBNFBeamTimeDistribution;
class G4Event;


class BeamSpillSource
{
public:
  /// Default constructor
  BeamSpillSource(double mean=0.);
  /// Destructor
  virtual ~BeamSpillSource();

  /// Method invoked in the primary generator at the beggining of each event
  virtual void GeneratePrimaryVertices(G4Event*) = 0;

  /// Set the mean number of interactions per spill from this source
  void SetMean(double);
  /// Return the mean number of interactions per spill due to this source
  double GetMean() const;

protected:
  int NumberOfInteractions() const;
  double TimeOffset() const;

protected:
  double mean_; ///< Mean number of interactions per spill from this source
  LBNFBeamTimeDistribution* time_pdf_; ///< Spill time distribution for LBNF
};

inline void BeamSpillSource::SetMean(double m) { mean_ = m; }
inline double BeamSpillSource::GetMean() const { return mean_; }

#endif