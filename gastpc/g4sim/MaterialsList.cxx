// -------------------------------------------------------------------
/// \file   MaterialsList.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 17 June 2016
// -------------------------------------------------------------------

#include "MaterialsList.h"

#include <G4Material.hh>
#include <G4Element.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>



G4Material* MaterialsList::Steel304L()
{
  G4String name = "Steel304L";

  // Check whether the material has already been defined
  G4Material* material = G4Material::GetMaterial(name, false);

  if (!material) {
    material = new G4Material(name, 8.03*g/cm3, 4);

    G4Element* Cr = G4NistManager::Instance()->FindOrBuildElement("Cr");
    material->AddElement(Cr, 0.180);

    G4Element* Ni = G4NistManager::Instance()->FindOrBuildElement("Ni");
    material->AddElement(Ni, 0.080);

    G4Element* Mn = G4NistManager::Instance()->FindOrBuildElement("Mn");
    material->AddElement(Mn, 0.020);

    G4Element* Fe = G4NistManager::Instance()->FindOrBuildElement("Fe");
    material->AddElement(Fe, 0.720);
  }

  return material;
}



G4Material* MaterialsList::TitaniumR56323()
{
  G4String name = "TitaniumR56323";

  // Check whether the material has already been defined
  G4Material* material = G4Material::GetMaterial(name, false);

  if (!material) {
    material = new G4Material(name, 4.48*g/cm3, 3);

    G4Element* Ti = G4NistManager::Instance()->FindOrBuildElement("Ti");
    material->AddElement(Ti, 0.945);

    G4Element* Al = G4NistManager::Instance()->FindOrBuildElement("Al");
    material->AddElement(Al, 0.030);

    G4Element* V = G4NistManager::Instance()->FindOrBuildElement("V");
    material->AddElement(V, 0.025);

//    G4Element* Ru = G4NistManager::Instance()->FindOrBuildElement("Ru");
//    material->AddElement(Ru, 0.001);
  }

  return material;
}


G4Material* MaterialsList::GAr()
{
  G4String name = "GAr";

  G4Material* material = G4Material::GetMaterial(name, false);

  if (!material) {
    material = 
      new G4Material(name, 16.11*kg/m3, 1, kStateGas, 300.*kelvin, 10.*bar);
    G4Element* Ar = G4NistManager::Instance()->FindOrBuildElement("Ar");
    material->AddElement(Ar, 1);
  }

  return material;
}


