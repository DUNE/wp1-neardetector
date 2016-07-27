// -------------------------------------------------------------------
/// \file   MCParticle.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#ifndef GASTPC_MCPARTICLE_H
#define GASTPC_MCPARTICLE_H

#include <Rtypes.h>

namespace gastpc { class MCParticle; }


/// TODO: Class description

class gastpc::MCParticle 
{
public:
  /// Default constructor
  MCParticle();
  /// Destructor
  ~MCParticle();

  /// Set Geant4's track ID code for this particle
  void SetTrackID(int id);
  /// Return Geant4's track ID code for this particle
  int GetTrackID() const;


private:
  bool primary_; ///< Is this particle a primary?

  int track_id_;
  int pdg_code_; ///< PDG code for particle identification

  const gastpc::MCParticle* mother_; ///< Pointer to mother particle
  std::vector<const gastpc::MCParticle*> daughters_;

  ClassDef(gastpc::MCParticle, 1)
};

inline gastpc::MCParticle::MCParticle(): 
  primary_(false), track_id_(0), pdg_code_(0) {}
inline gastpc::MCParticle::~MCParticle() {}
inline void gastpc::MCParticle::SetTrackID(int id) { track_id_ = id; }
inline int  gastpc::MCParticle::GetTrackID() const { return track_id_; }

#endif