// -------------------------------------------------------------------
/// \file   DuneGArNDEcalModuleGeometry.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#ifndef DUNE_GARND_ECAL_MODULE_GEOMETRY_H
#define DUNE_GARND_ECAL_MODULE_GEOMETRY_H 

#include "BaseGeometry.h"
#include <G4Types.hh>


class DuneGArNDEcalModuleGeometry: public BaseGeometry
{
public:
  /// Constructor
  DuneGArNDEcalModuleGeometry(G4double width, G4double height, 
    G4int num_layers, G4double abs_thickness, G4double scint_thickness);
  /// Destructor
  ~DuneGArNDEcalModuleGeometry();

  G4double GetModuleWidth() const;
  G4double GetModuleHeight() const;
  G4double GetModuleDepth() const;

  static G4double CalculateThickness(G4int num_layers,
                                     G4double abs_thickness, 
                                     G4double scint_thickness);
  
private:
  void DefineVolumes();

private:
  G4double width_;  ///< Width of ECAL module
  G4double height_; ///< Height of ECAL module
  G4double depth_;  ///< Depth of ECAL module

  G4int num_layers_;         ///< Number of layers the ECAL consists in
  G4double abs_thickness_;   ///< Thickness of absorber (Pb) layers
  G4double scint_thickness_; ///< Thickness of scintillator layers
};

inline G4double DuneGArNDEcalModuleGeometry::GetModuleWidth() const
{ return width_; }
inline G4double DuneGArNDEcalModuleGeometry::GetModuleHeight() const
{ return height_; }
inline G4double DuneGArNDEcalModuleGeometry::GetModuleDepth() const
{ return depth_; }

inline G4double DuneGArNDEcalModuleGeometry::CalculateThickness(
  G4int num_layers, G4double abs_thickness, G4double scint_thickness)
{ return (num_layers * scint_thickness + (num_layers-1) * abs_thickness); }

#endif