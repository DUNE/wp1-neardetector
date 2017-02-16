// -------------------------------------------------------------------
/// \file   PrimaryParticleInfo.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 2 Aug 2016
// -------------------------------------------------------------------

#ifndef PRIMARY_PARTICLE_INFO_H
#define PRIMARY_PARTICLE_INFO_H

#include <G4VUserPrimaryParticleInformation.hh>


/// This class encapsulates the id number of the MC event generator record
/// connected to a given primary particle

class PrimaryParticleInfo: public G4VUserPrimaryParticleInformation
{
public:
  /// Constructor
  PrimaryParticleInfo();
  /// Destructor
  virtual ~PrimaryParticleInfo();

  void  SetEventGenerationID(G4int);
  G4int GetEventGenerationID() const;

private:
  virtual void Print() const;

private:
  G4int id_; ///< ID number of the MC event generator record
};

// Inline definitions //////////////////////////////////////

inline PrimaryParticleInfo::PrimaryParticleInfo():
  G4VUserPrimaryParticleInformation(), id_(-1) {}
inline PrimaryParticleInfo::~PrimaryParticleInfo() {}
inline void PrimaryParticleInfo::SetEventGenerationID(G4int id) { id_ = id; }
inline G4int PrimaryParticleInfo::GetEventGenerationID() const { return id_; }
inline void PrimaryParticleInfo::Print() const {}

#endif
