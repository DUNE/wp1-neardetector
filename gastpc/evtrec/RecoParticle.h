// -------------------------------------------------------------------
/// \file   RecoParticle.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 18 Feb 2017
// -------------------------------------------------------------------

#ifndef GASTPC_RECOPARTICLE_H
#define GASTPC_RECOPARTICLE_H

#include "Vector3D.h"
#include "Vector4D.h"
#include <Rtypes.h>
#include <iostream>

namespace gastpc { class RecoParticle; }
namespace gastpc { class MCParticle; }


/// TODO: Class description

class gastpc::RecoParticle
{
public:
  /// Default constructor
  RecoParticle();
  /// Destructor
  virtual ~RecoParticle();

  void SetPDGCode(int);
  int  GetPDGCode() const;

  /// Set initial position and time of particle
  void SetInitialXYZT(double, double, double, double);
  /// Return initial position and time of particle
  const gastpc::Vector4D& GetInitialXYZT() const;
  /// Set final position and time of particle
  void SetFinalXYZT(double, double, double, double);
  /// Return final position and time of particle
  const gastpc::Vector4D& GetFinalXYZT() const;

  /// Set initial momentum of particle
  void SetInitialMomentum(double, double, double);
  /// Return initial momentum of particle
  const gastpc::Vector3D& GetInitialMomentum() const;


  void SetMCParticle(gastpc::MCParticle*);
  gastpc::MCParticle* GetMCParticle();

  void Print(std::ostream& os=std::cout) const;

private:
  int pdg_code_;          ///< PDG code for particle identification

  gastpc::Vector4D initial_xyzt_; ///< Particle's initial position
  gastpc::Vector4D final_xyzt_;   ///< Particle's final position

  gastpc::Vector3D initial_momentum_; ///< Particle's initial momentum

  gastpc::MCParticle* mcp_;   ///< Pointer to associated MCParticle

  ClassDef(gastpc::RecoParticle, 1)
};

std::ostream& operator << (std::ostream&, const gastpc::RecoParticle&);

// Inline definitions //////////////////////////////////////
inline void gastpc::RecoParticle::SetPDGCode(int pdg) { pdg_code_ = pdg; }
inline int  gastpc::RecoParticle::GetPDGCode() const { return pdg_code_; }
inline void gastpc::RecoParticle::SetMCParticle(gastpc::MCParticle* p) { mcp_ = p; }
inline gastpc::MCParticle* gastpc::RecoParticle::GetMCParticle() { return mcp_; }

#endif
