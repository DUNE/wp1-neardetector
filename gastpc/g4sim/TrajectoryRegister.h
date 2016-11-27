// -------------------------------------------------------------------
/// \file   TrajectoryRegister.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef TRAJECTORY_REGISTER_H
#define TRAJECTORY_REGISTER_H

#include <map>
class G4VTrajectory;

/// TODO: Class description

class TrajectoryRegister
{
public:
  /// Return a trajectory given its track ID
  static G4VTrajectory* Get(int trackId);
  /// Add a trajectory to the map
  static void Add(G4VTrajectory*);
  /// Clear the map
  static void Clear();

private:
  // Constructors, destructor and assignement op are hidden
  // so that no instance of the class can be created.
  TrajectoryRegister();
  TrajectoryRegister(const TrajectoryRegister&);
  ~TrajectoryRegister();

private:
  static std::map<int, G4VTrajectory*> map_;
};

#endif