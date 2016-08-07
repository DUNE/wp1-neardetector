// -------------------------------------------------------------------
/// \file   UniformMagneticField.h
/// \brief  Uniform magnetic field in the direction of the Y axis
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef UNIFORM_MAGNETIC_FIELD_H
#define UNIFORM_MAGNETIC_FIELD_H

#include <G4MagneticField.hh>

class G4GenericMessenger;


/// Definition of a uniform magnetic field in the direction of the Y axis
/// (typically, the direction of the electric drift field).

class UniformMagneticField: public G4MagneticField
{
public:
  UniformMagneticField(G4double field_strength=0.);
  virtual ~UniformMagneticField();
  
  virtual void GetFieldValue(const G4double point[4], double* field) const;

  void SetFieldStrength(G4double);
  G4double GetFieldStrength() const;

private:
  G4double field_strength_;
};

inline void UniformMagneticField::SetFieldStrength(G4double b) 
{ field_strength_ = b; }
inline G4double UniformMagneticField::GetFieldStrength() const
{ return field_strength_; }

#endif