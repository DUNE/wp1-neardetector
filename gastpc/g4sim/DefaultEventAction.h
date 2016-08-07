// -------------------------------------------------------------------
/// \file   DefaultEventAction.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 Aug 2016
// -------------------------------------------------------------------

#ifndef DEFAULT_EVENT_ACTION
#define DEFAULT_EVENT_ACTION

#include <G4UserEventAction.hh>

class G4Event;


/// TODO: Class description
class DefaultEventAction: public G4UserEventAction
{
public:
  /// Constructor
  DefaultEventAction();
  /// Destructor
  virtual ~DefaultEventAction();
  
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
};

#endif