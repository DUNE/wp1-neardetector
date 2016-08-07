// -------------------------------------------------------------------
/// \file   UniformMagneticField.cxx
/// \brief  Uniform magnetic field in the direction of the Y axis
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "UniformMagneticField.h"

#include <G4GenericMessenger.hh>
#include <G4SystemOfUnits.hh>



UniformMagneticField::UniformMagneticField(G4double field_strength):
  G4MagneticField(), field_strength_(field_strength)
{
}


UniformMagneticField::~UniformMagneticField()
{
}


void UniformMagneticField::GetFieldValue(const G4double [4], 
                                         double* field) const
{
  field[0] = 0.;
  field[1] = field_strength_;
  field[2] = 0.;
}
