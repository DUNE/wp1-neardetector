// -------------------------------------------------------------------
/// \file   MCParticle.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#ifndef GASTPC_MCPARTICLE_H
#define GASTPC_MCPARTICLE_H

#include "Vector3D.h"
#include "Vector4D.h"
#include <Rtypes.h>
#include <iostream>

namespace gastpc { class MCParticle; }
namespace gastpc { class MCGenInfo; }
namespace gastpc { class MCTrack; }


/// TODO: Class description

class gastpc::MCParticle
{
public:
  /// Default constructor
  MCParticle();
  /// Destructor
  virtual ~MCParticle();

  /// Set Monte Carlo ID number for the particle
  void SetMCID(int);
  /// Return Monte Carlo ID for the particle
  int GetMCID() const;

  /// Set particle's PDG code
  void SetPDGCode(int);
  /// Return particle's PDG code
  int GetPDGCode() const;

  ///
  void SetParent(gastpc::MCParticle*);
  ///
  gastpc::MCParticle* GetParent();
  ///
  void SetAncestor(gastpc::MCParticle*);
  ///
  gastpc::MCParticle* GetAncestor();

  ///
  void SetFamilyTreeLevel(int);
  ///
  int GetFamilyTreeLevel() const;
  ///
  bool IsPrimary() const;

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

  ///
  void SetMCGenInfo(gastpc::MCGenInfo*);
  ///
  gastpc::MCGenInfo* GetMCGenInfo();

  ///
  void AddDaughter(gastpc::MCParticle*);
  ///
  const std::vector<gastpc::MCParticle*>& GetDaughters() const;

  ///
  void AddMCTrack(gastpc::MCTrack*);
  ///
  const std::vector<gastpc::MCTrack*>& GetMCTracks() const;

  void Print(std::ostream& os=std::cout) const;

private:
  int mcid_;              ///< Monte Carlo ID number
  int pdg_code_;          ///< PDG code for particle identification
  int family_tree_level_; ///< Primary = 1, Secondary = 2, Tertiary = 3 ...

  gastpc::Vector4D initial_xyzt_; ///< Particle's initial position
  gastpc::Vector4D final_xyzt_;   ///< Particle's final position

  gastpc::Vector3D initial_momentum_; ///< Particle's initial momentum

  gastpc::MCParticle* parent_;   ///< Pointer to parent particle
  gastpc::MCParticle* ancestor_; ///< Pointer to primary ancestor

  gastpc::MCGenInfo* mcgi_; ///< Pointer to generation info

  std::vector<gastpc::MCParticle*> daughters_;
  std::vector<gastpc::MCTrack*> tracks_;

  ClassDef(gastpc::MCParticle, 3)
};

std::ostream& operator << (std::ostream&, const gastpc::MCParticle&);

// Inline definitions //////////////////////////////////////

inline void gastpc::MCParticle::SetMCID(int id)
{ mcid_ = id; }
inline int  gastpc::MCParticle::GetMCID() const
{ return mcid_; }

inline void gastpc::MCParticle::SetPDGCode(int pdg)
{ pdg_code_ = pdg; }
inline int  gastpc::MCParticle::GetPDGCode() const
{ return pdg_code_; }

inline void gastpc::MCParticle::SetFamilyTreeLevel(int g)
{ family_tree_level_ = g; }
inline int  gastpc::MCParticle::GetFamilyTreeLevel() const
{ return family_tree_level_; }
inline bool gastpc::MCParticle::IsPrimary() const
{ return (family_tree_level_ == 1); }

inline void gastpc::MCParticle::SetParent(gastpc::MCParticle* p)
{ parent_ = p; }
inline gastpc::MCParticle* gastpc::MCParticle::GetParent()
{ return parent_; }
inline void gastpc::MCParticle::SetAncestor(gastpc::MCParticle* p)
{ ancestor_ = p; }
inline gastpc::MCParticle* gastpc::MCParticle::GetAncestor()
{ return ancestor_; }

inline void gastpc::MCParticle::SetMCGenInfo(gastpc::MCGenInfo* p)
{ mcgi_ = p; }
inline gastpc::MCGenInfo* gastpc::MCParticle::GetMCGenInfo()
{ return mcgi_; }

#endif
