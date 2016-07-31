// -------------------------------------------------------------------
/// \file   MCTrack.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 31 July 2016
// -------------------------------------------------------------------

#ifndef GASTPC_MCTRACK_H
#define GASTPC_MCTRACK_H

#include <Rtypes.h>

namespace gastpc { class MCHit; }
namespace gastpc { class MCParticle; }
namespace gastpc { class MCTrack; }


/// TODO: Class description

class gastpc::MCTrack
{
public:
  /// Default constructor
  MCTrack();
  /// Destructor
  ~MCTrack();
  
  void AddHit(gastpc::MCHit*);

  const std::vector<gastpc::MCHit*>& GetHits() const;

  void ClearHits();

  double GetEnergy() const;
  void SetEnergy(double);

  double GetLenght() const;
  void SetLength(double);

private:
  double energy_; ///< Total energy of the track
  double length_; ///< Track length

  std::vector<gastpc::MCHit*> mchits_; ///< Hit collection

  gastpc::MCParticle* mcparticle_; ///< Pointer to associated particle

  ClassDef(gastpc::MCTrack, 1)
};

inline void gastpc::MCTrack::SetEnergy(double e) { energy_ = e; }
inline double gastpc::MCTrack::GetEnergy() const { return energy_; }
inline void gastpc::MCTrack::SetLength(double l) { length_ = l; }
inline double gastpc::MCTrack::GetLenght() const { return length_; }

#endif