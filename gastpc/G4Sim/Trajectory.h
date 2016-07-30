// -------------------------------------------------------------------
/// \file   Trajectory.h
/// \brief  
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
  /// Destructor
  virtual ~Trajectory();
  /// Copy constructor
  Trajectory(const Trajectory&);
  /// Equality operator
  int operator ==(const Trajectory&) const;
  /// Memory allocation operator
  void* operator new(size_t);
  /// Memory deallocation operator
  void operator delete(void*);

    /// Return pointer to the particle definition 
    /// associated to the track
    G4ParticleDefinition* GetParticleDefinition();
    /// Return name of the particle
    G4String GetParticleName() const;
    /// Return charge of the particle
    G4double GetCharge() const;
    /// Return PDG code of the particle
    G4int GetPDGEncoding () const;

    // Return name of the track creator process
    G4String GetCreatorProcess() const;

    /// Return id number of the associated track
    G4int GetTrackID() const;
    /// Return id number of the parent track
    G4int GetParentID() const;

    // Return initial three-momentum
    G4ThreeVector GetInitialMomentum() const;
    // Return initial position (creation vertex) 
    // in global coordinates
    G4ThreeVector GetInitialPosition() const;
    // Return creation time with respect to
    // the start-of-event time
    G4double GetInitialTime() const;

    G4ThreeVector GetFinalPosition() const;
    void SetFinalPosition(const G4ThreeVector&);

    G4double GetFinalTime() const;
    void SetFinalTime(G4double);

    G4double GetTrackLength() const;
    void SetTrackLength(G4double);

    G4double GetEnergyDeposit() const;
    void SetEnergyDeposit(G4double);
  
    G4String GetInitialVolume() const;

    G4String GetDecayVolume() const;
    void SetDecayVolume(G4String);

    // Trajectory points

    /// Return the number of trajectory points
    virtual int GetPointEntries() const;
    /// Return the i-th point in the trajectory
    virtual G4VTrajectoryPoint* GetPoint(G4int i) const;
    ///
    virtual void AppendStep(const G4Step*);
    ///
    virtual void MergeTrajectory(G4VTrajectory*);

    virtual void ShowTrajectory(std::ostream&) const;

    virtual void DrawTrajectory() const;
  
  private:
    /// The default constructor is private. A trajectory can
    /// only be constructed associated to a track.
    Trajectory();
  private:
    G4ParticleDefinition* pdef_; //< Pointer to the particle definition

    G4int track_id_;  ///< Identification number of the track
    G4int mother_id_; ///< Identification number of the parent particle

    G4ThreeVector _initial_momentum;

    G4ThreeVector _initial_position;
    G4ThreeVector _final_position;

    G4double _initial_time;
    G4double _final_time;

    G4double _length;
    G4double _edep;

    G4String creator_process_;

    G4String _initial_volume;
    G4String _decay_volume;

    G4bool _record_trjpoints;

    TrajectoryPointContainer* _trjpoints;

};

extern G4TRACKING_DLL G4ThreadLocal 
  G4Allocator<Trajectory>* TrajectoryAllocator;

inline void* Trajectory::operator new(size_t)
{
  if (!TrajectoryAllocator)
    TrajectoryAllocator = new G4Allocator<Trajectory>;
  return (void*)TrajectoryAllocator->MallocSingle();
}

inline void Trajectory::operator delete(void* trj)
{
  TrajectoryAllocator->FreeSingle((Trajectory*) trj);
}

inline G4ParticleDefinition* Trajectory::GetParticleDefinition()
{ return pdef_; }

inline int Trajectory::GetPointEntries() const
{ return _trjpoints->size(); }

inline G4VTrajectoryPoint* Trajectory::GetPoint(G4int i) const
{ return (*_trjpoints)[i]; }

inline G4ThreeVector Trajectory::GetInitialMomentum() const
{ return _initial_momentum; }

inline G4int Trajectory::GetTrackID() const
{ return track_id_; }

inline G4int Trajectory::GetParentID() const
{ return mother_id_; }

inline G4ThreeVector Trajectory::GetInitialPosition() const
{ return _initial_position; }

inline G4ThreeVector Trajectory::GetFinalPosition() const
{ return _final_position; }

inline void Trajectory::SetFinalPosition(const G4ThreeVector& x)
{ _final_position = x; }

inline G4double Trajectory::GetInitialTime() const
{ return _initial_time; }

inline G4double Trajectory::GetFinalTime() const
{ return _final_time; }

inline void Trajectory::SetFinalTime(G4double t)
{ _final_time = t; }

inline G4double Trajectory::GetTrackLength() const { return _length; }

inline void Trajectory::SetTrackLength(G4double l) { _length = l; }

inline G4double Trajectory::GetEnergyDeposit() const { return _edep; }

inline void Trajectory::SetEnergyDeposit(G4double e) { _edep = e; }

inline G4String Trajectory::GetCreatorProcess() const
{ return creator_process_; }

inline G4String Trajectory::GetInitialVolume() const
{ return _initial_volume; }

inline G4String Trajectory::GetDecayVolume() const
{ return _decay_volume; }

inline void Trajectory::SetDecayVolume(G4String dv)
{ _decay_volume = dv; }


#endif