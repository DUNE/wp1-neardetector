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


/// TODO: Class description

class PrimaryParticleInfo: public G4VUserPrimaryParticleInformation
{
public:
  /// Constructor
  PrimaryParticleInfo();
  /// Destructor
  virtual ~PrimaryParticleInfo();

  virtual void Print() const;

  void  SetInteractionID(G4int);
  G4int GetInteractionID() const;

private:
  G4int intid_; ///< Interaction ID
};

// Inline definitions //////////////////////////////////////

inline PrimaryParticleInfo::PrimaryParticleInfo(): 
  G4VUserPrimaryParticleInformation(), intid_(-1) {}
inline PrimaryParticleInfo::~PrimaryParticleInfo() {}
inline void PrimaryParticleInfo::Print() const {}
inline void PrimaryParticleInfo::SetInteractionID(G4int id) { intid_ = id; }
inline G4int PrimaryParticleInfo::GetInteractionID() const { return intid_; }

#endif