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
  virtual ~MCParticle();

  /// Set Monte Carlo ID number for the particle
  void SetMCID(int id);
  /// Return Monte Carlo ID for the particle
  int GetMCID() const;

  void SetMother(gastpc::MCParticle*);
  gastpc::MCParticle* GetMother();

private:
  bool primary_; ///< Is this particle a primary?
  int mcid_;     ///< Monte Carlo ID number
  int pdg_code_; ///< PDG code for particle identification

  gastpc::MCParticle* mother_; ///< Pointer to mother particle
  //std::vector<const gastpc::MCParticle*> daughters_;

  ClassDef(gastpc::MCParticle, 1)
};

inline void gastpc::MCParticle::SetMCID(int id) { mcid_ = id; }
inline int  gastpc::MCParticle::GetMCID() const { return mcid_; }
inline gastpc::MCParticle* gastpc::MCParticle::GetMother() { return mother_; }
inline void gastpc::MCParticle::SetMother(gastpc::MCParticle* p) { mother_ = p; }

#endif
