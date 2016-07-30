// -------------------------------------------------------------------
/// \file   DefaultRunAction.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef DEFAULT_RUN_ACTION_H
#define DEFAULT_RUN_ACTION_H

#include <G4UserRunAction.hh>


/// TODO: Class description

class DefaultRunAction: public G4UserRunAction
{
public:
  /// Constructor
  DefaultRunAction();
  /// Destructor
  virtual ~DefaultRunAction();
  
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
};

inline DefaultRunAction::DefaultRunAction(): G4UserRunAction() {}
inline DefaultRunAction::~DefaultRunAction() {}

#endif