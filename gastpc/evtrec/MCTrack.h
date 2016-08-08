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
  virtual ~MCTrack();

  ///
  void Clear();
  
  /// Add a hit to the track
  void Add(gastpc::MCHit*);
  /// Return the hits forming this track
  const std::vector<gastpc::MCHit*>& GetHits() const;

  ///
  double GetEnergy() const;
  ///
  void SetEnergy(double);

  double GetLenght() const;
  void SetLength(double);

  void SetLabel(const std::string&);
  const std::string& GetLabel() const;

  void SetParticle(gastpc::MCParticle*);
  gastpc::MCParticle* GetParticle() const;

private:
  double energy_; ///< Total energy of the track
  double length_; ///< Track length

  std::string label_;

  gastpc::MCParticle* mcp_; ///< Pointer to associated particle

  std::vector<gastpc::MCHit*> mchits_; ///< Hit collection

  ClassDef(gastpc::MCTrack, 1)
};

// Inline definitions ////////////////////////////

inline void gastpc::MCTrack::SetEnergy(double e) { energy_ = e; }
inline double gastpc::MCTrack::GetEnergy() const { return energy_; }

inline void gastpc::MCTrack::SetLength(double l) { length_ = l; }
inline double gastpc::MCTrack::GetLenght() const { return length_; }

inline void gastpc::MCTrack::SetLabel(const std::string& l) { label_ = l; }
inline const std::string& gastpc::MCTrack::GetLabel() const { return label_; }

inline void gastpc::MCTrack::SetParticle(gastpc::MCParticle* p) { mcp_ = p; }
inline gastpc::MCParticle* gastpc::MCTrack::GetParticle() const { return mcp_; }

#endif
