// -------------------------------------------------------------------
/// \file   DuneGArNDMagnetCoilGeometry.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#ifndef DUNE_GARND_MAGNET_COIL_GEOMETRY_H
#define DUNE_GARND_MAGNET_COIL_GEOMETRY_H 

#include "BaseGeometry.h"
#include <G4Types.hh>


class DuneGArNDMagnetCoilGeometry: public BaseGeometry
{
public:
  DuneGArNDMagnetCoilGeometry(G4double width, G4double heigh, G4double length, G4double thickness);
  ~DuneGArNDMagnetCoilGeometry();
  
private:
  void DefineVolumes();

private:
  G4double width_, height_, length_;
  G4double thickness_;
};


#endif