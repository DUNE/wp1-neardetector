// -------------------------------------------------------------------
/// \file   DuneGArNDVesselGeometry.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 21 June 2016
// -------------------------------------------------------------------

#ifndef DUNE_GARND_VESSEL_GEOMETRY_H
#define DUNE_GARND_VESSEL_GEOMETRY_H

#include "BaseGeometry.h"
#include <G4Types.hh>

/// TO DO.

class DuneGArNDVesselGeometry: public BaseGeometry
{
public:
  /// Constructor
  DuneGArNDVesselGeometry(G4double diameter, G4double length, 
    G4double barrel_thickness, G4double endcap_thickness);
  
  /// Destructor
  virtual ~DuneGArNDVesselGeometry();
  
  virtual void DefineVolumes();

private:
  G4double diameter_;  ///< Internal diameter of pressure vessel
  G4double length_;    ///< Internal length of pressure vessel
  G4double barrel_thickness_; ///< Barrel wall thickness
  G4double endcap_thickness_; ///< Endcap wall thickness
};



#endif