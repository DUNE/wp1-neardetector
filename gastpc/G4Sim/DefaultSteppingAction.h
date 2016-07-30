// -------------------------------------------------------------------
/// \file   DefaultSteppingAction.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef DEFAULT_STEPPING_ACTION_H
#define DEFAULT_STEPPING_ACTION_H

#include <G4UserSteppingAction.hh>


/// TODO: Class description

class DefaultSteppingAction: public G4UserSteppingAction
{
public:
  /// Constructor
  DefaultSteppingAction();
  /// Destructor
  virtual ~DefaultSteppingAction();

  virtual void UserSteppingAction(const G4Step*);
};

inline DefaultSteppingAction::DefaultSteppingAction(): G4UserSteppingAction() {}
inline DefaultSteppingAction::~DefaultSteppingAction() {}

#endif