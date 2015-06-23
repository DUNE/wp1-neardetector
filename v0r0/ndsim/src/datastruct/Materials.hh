//
// Title: Materials
//
// Author: Tom STainer
//
// Description: All materials in the geometry are specified in this class

#ifndef UTILS_MATERIALS_HH
#define UTILS_MATERIALS_HH 1

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>

#include "TGeoMaterial.h"
#include "TGeoElement.h"
#include "TGeoManager.h"

#include "Units.hh"

class Materials {

public:
  Materials();
  Materials(TGeoManager*);
  ~Materials();

  void initialize();
  void DefineElements();
  void DefineMaterials();

  TGeoElement * FindElement(std::string name);
  TGeoMixture * FindMaterial(std::string name,bool deepCopy=false);

  void addMaterial(std::string name, double A, double Z, double density);
  TGeoMixture * copy(const TGeoMixture* mat);

private:
  std::list<TGeoMixture*> materialList;
  std::list<TGeoElement*> elementList;

  TGeoMixture *material_;
  TGeoElement *element_;

  TGeoManager *gManager_;

};

#endif
