// -------------------------------------------------------------------
/// \file   UniformMagneticField.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#ifndef UNIFORM_MAGNETIC_FIELD_H
#define UNIFORM_MAGNETIC_FIELD_H

#include <G4MagneticField.hh>

class G4GenericMessenger;


class UniformMagneticField: public G4MagneticField
{
public:
  UniformMagneticField();
  virtual ~UniformMagneticField();
  
  virtual void GetFieldValue(const G4double point[4], double* field) const;

  void SetFieldStrength(G4double);
  G4double GetFieldStrength() const;

private:
  void DefineCommands();

private:
  G4GenericMessenger* msg_;
  G4double field_strength_;
};

inline void UniformMagneticField::SetFieldStrength(G4double b) 
{ field_strength_ = b; }
inline G4double UniformMagneticField::GetFieldStrength() const
{ return field_strength_; }

#endif