// -------------------------------------------------------------------
/// \file   UniformMagneticField.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "UniformMagneticField.h"

#include <G4GenericMessenger.hh>
#include <G4SystemOfUnits.hh>



UniformMagneticField::UniformMagneticField():
  G4MagneticField(), msg_(0), field_strength_(0.)
{
  DefineCommands();
}


UniformMagneticField::~UniformMagneticField()
{
  delete msg_;
}


void UniformMagneticField::GetFieldValue(const G4double [4], 
                                         double* field) const
{
  field[0] = 0.;
  field[1] = field_strength_;
  field[2] = 0.;
}


void UniformMagneticField::DefineCommands()
{
  msg_ = new G4GenericMessenger(this, "/gastpc/field/", "Field control");

  G4GenericMessenger::Command& field_strength_cmd =
    msg_->DeclareMethodWithUnit("field_strength", "tesla",
      &UniformMagneticField::SetFieldStrength, "Set field strength.");
  field_strength_cmd.SetRange("value>0.");
}
