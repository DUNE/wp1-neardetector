#ifndef CONSTRUCT_HH
#define CONSTRUCT_HH

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <cmath>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "TGeoManager.h"
#include "TSystem.h"
#include "TGeoVolume.h"
#include "TGeoMatrix.h"
#include "TGeoVolumeEditor.h"
#include "TGDMLParse.h"

#include "NDConstruction.hh"

#include "Materials.hh"
#include "UtlXML.hh"

void ReadDatacard(std::string datacard);

std::string frootFilename, fgdmlFilename, ftpcmaterial, fVesselMaterial, fMagnetMaterial, fEcalMaterial, fEcalAbsMaterial, fRockMaterial, fEcalHadMaterial;
double fdetectorYAxisOffset;
bool fbuildtpc, fbuildvessel, fbuildecal, fbuildrock, fbuildmagnet, fbuildcavity, fEcalAbsBuild, fEcalHadBuild, fEcalBuildLayers;
double ftpcx, ftpcy, ftpcz, ftpcdensity, ftpcPlacementZ;
double fVesselRadiusRatio, fVesselThickness, fVesselRadius, fVesselLength, fVesselPlacementZ, fVesselDensity;
double fEcalLayers, fDsEcalLayers, fBrEcalLayers, fUsEcalLayers, fEcalThickness, fEcalGap, fEcalDensity, fEcalAbsThickness, fEcalAbsDensity, fEcalHadDensity, fEcalHadThickness, fEcalHadLayers;
double fRockRadius, fRockLength, fRockDensity, fRockX, fRockY, fRockZ;
double fMagnetThickness, fMagnetDensity;
double fCavityGap, fCavityDensity;

#endif
