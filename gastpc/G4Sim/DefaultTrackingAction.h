// -------------------------------------------------------------------
/// \file   DefaultTrackingAction.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef DEFAULT_TRACKING_ACTION_H
#define DEFAULT_TRACKING_ACTION_H

#include <G4UserTrackingAction.hh>


/// TODO: Class description

class DefaultTrackingAction: public G4UserTrackingAction
{
public:
  /// Constructor
  DefaultTrackingAction();
  /// Destructor
  virtual ~DefaultTrackingAction();
  
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);
};

inline DefaultTrackingAction::DefaultTrackingAction(): G4UserTrackingAction() {}
inline DefaultTrackingAction::~DefaultTrackingAction() {}

#endif