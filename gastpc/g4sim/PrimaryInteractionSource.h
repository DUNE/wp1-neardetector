// -------------------------------------------------------------------
/// \file   PrimaryInteractionSource.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 June 2016
// -------------------------------------------------------------------

#ifndef PRIMARY_INTERACTION_SOURCE_H
#define PRIMARY_INTERACTION_SOURCE_H


class PrimaryInteractionSource
{
public:
  /// Constructor
  PrimaryInteractionSource();
  /// Destructor
  virtual ~PrimaryInteractionSource();
  
  void FillG4Event(G4Event*);

  
};


#endif