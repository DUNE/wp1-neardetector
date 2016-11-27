// -------------------------------------------------------------------
/// \file   Trajectory.h
/// \brief  Transient record of a Geant4 particle/track.
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <G4VTrajectory.hh>
#include <G4Allocator.hh>
#include <trkgdefs.hh>

class G4ParticleDefinition;
class G4Track;


typedef std::vector<G4VTrajectoryPoint*> TrajectoryPointContainer;


/// Transient record of a Geant4 particle/track. 
/// It stores basic information of the particle and its path through 
/// the geometry. A number of its properties are copied from the track
/// in construction and cannot be modified.

class Trajectory: public G4VTrajectory
{
public:
  /// Constructor given a track
  Trajectory(const G4Track*);
  /// Copy constructor
  Trajectory(const Trajectory&);
  /// Destructor
  virtual ~Trajectory();

  /// Equality operator
  int operator ==(const Trajectory&) const;
  /// Memory allocation operator
  void* operator new(size_t);
  /// Memory deallocation operator
  void operator delete(void*);

public:

  // Setters & Getters

  /// Return pointer to the particle definition 
  /// associated to the track
  G4ParticleDefinition* GetParticleDefinition();
  /// Return name of the particle
  G4String GetParticleName() const;
  /// Return charge of the particle
  G4double GetCharge() const;
  /// Return PDG code of the particle
  G4int GetPDGEncoding () const;

  /// Return name of the physical process that created the track
  const G4String& GetCreatorProcess() const;

  /// Return id number of the associated track
  G4int GetTrackID() const;
  /// Return id number of the parent track
  G4int GetParentID() const;
  /// Return id number of the ancestor track
  G4int GetAncestorID() const;

  /// Return family tree level of the associated track
  G4int GetFamilyTreeLevel() const;

  /// Return initial three-momentum
  G4ThreeVector GetInitialMomentum() const;
  /// Return initial position (creation vertex) in global coordinates
  G4ThreeVector GetInitialPosition() const;
  /// Return final position (decay vertex) in global coordinates
  G4ThreeVector GetFinalPosition() const;
  /// Return final position (decay vertex) in global coordinates
  void SetFinalPosition(const G4ThreeVector&);
  /// Return creation time with respect to the start-of-event time
  G4double GetInitialTime() const;
  /// Return decay time with respect to the start-of-event time
  G4double GetFinalTime() const;
  /// Set decay time with respect to the start-of-event time
  void SetFinalTime(G4double);

  // Trajectory points

  ///
  void RecordTrajectoryPoints(G4bool);
  /// Return the number of trajectory points
  virtual int GetPointEntries() const;
  /// Return the i-th point in the trajectory
  virtual G4VTrajectoryPoint* GetPoint(G4int i) const;
  ///
  virtual void AppendStep(const G4Step*);
  ///
  virtual void MergeTrajectory(G4VTrajectory*);
  ///
  virtual void ShowTrajectory(std::ostream&) const;
  ///
  virtual void DrawTrajectory() const;
  
private:
  /// The default constructor is private. A trajectory can
  /// only be constructed associated to a track.
  Trajectory();

  void FindAncestorID();

private:
  G4ParticleDefinition* pdef_; //< Pointer to the particle definition

  G4int track_id_;    ///< Identification number of the track
  G4int mother_id_;   ///< Identification number of the parent particle
  G4int ancestor_id_; ///< Identification number of ancestral primary particle

  G4int family_tree_level_;  ///< Primary = 1, Secondary = 2, Tertiary = 3 ...

  G4ThreeVector initial_momentum_;

  G4ThreeVector initial_position_;
  G4ThreeVector final_position_;

  G4double initial_time_;
  G4double final_time_;

  G4String creator_process_;

  G4bool rec_trjpoints_;

  TrajectoryPointContainer* trjpoints_;
};

// Inline definitions //////////////////////////////////////

extern G4TRACKING_DLL G4ThreadLocal 
  G4Allocator<Trajectory>* TrajectoryAllocator;

inline void* Trajectory::operator new(size_t)
{ if (!TrajectoryAllocator) TrajectoryAllocator = new G4Allocator<Trajectory>;
  return (void*)TrajectoryAllocator->MallocSingle(); }

inline void Trajectory::operator delete(void* trj)
{ TrajectoryAllocator->FreeSingle((Trajectory*) trj); }

inline G4ParticleDefinition* Trajectory::GetParticleDefinition()
{ return pdef_; }

inline int Trajectory::GetPointEntries() const
{ return trjpoints_->size(); }

inline G4VTrajectoryPoint* Trajectory::GetPoint(G4int i) const
{ return (*trjpoints_)[i]; }

inline G4ThreeVector Trajectory::GetInitialMomentum() const
{ return initial_momentum_; }

inline G4int Trajectory::GetTrackID() const 
{ return track_id_; }

inline G4int Trajectory::GetParentID() const 
{ return mother_id_; }

inline G4int Trajectory::GetAncestorID() const 
{ return ancestor_id_; }

inline G4int Trajectory::GetFamilyTreeLevel() const 
{ return family_tree_level_; }

inline G4ThreeVector Trajectory::GetInitialPosition() const
{ return initial_position_; }

inline G4ThreeVector Trajectory::GetFinalPosition() const
{ return final_position_; }

inline void Trajectory::SetFinalPosition(const G4ThreeVector& x)
{ final_position_ = x; }

inline G4double Trajectory::GetInitialTime() const
{ return initial_time_; }

inline G4double Trajectory::GetFinalTime() const
{ return final_time_; }

inline void Trajectory::SetFinalTime(G4double t)
{ final_time_ = t; }

inline const G4String& Trajectory::GetCreatorProcess() const
{ return creator_process_; }

inline void Trajectory::RecordTrajectoryPoints(G4bool b) 
{ rec_trjpoints_ = b; }

#endif