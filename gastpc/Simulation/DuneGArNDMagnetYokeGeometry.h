// -------------------------------------------------------------------
/// \file   DuneGArNDMagnetYokeGeometry.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#ifndef DUNE_GARND_MAGNET_YOKE_GEOMETRY_H
#define DUNE_GARND_MAGNET_YOKE_GEOMETRY_H

#include "BaseGeometry.h"
#include <G4Types.hh>


class DuneGArNDMagnetYokeGeometry: public BaseGeometry
{
public:
  /// Constructor
  DuneGArNDMagnetYokeGeometry(G4double width, 
                              G4double height, 
                              G4double length,
                              G4double thickness);

  /// Destructor
  virtual ~DuneGArNDMagnetYokeGeometry();

  virtual void DefineVolumes();

private:
  G4double width_;     ///< Inner width
  G4double height_;    ///< Inner height
  G4double length_;    ///< Inner length
  G4double thickness_; ///< Yoke thickness
};


#endif