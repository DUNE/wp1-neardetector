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

//#include "Utils/CmdLnArgParser.h"

#include "TpcFiducialConstruction.hh"
#include "TpcVesselConstruction.hh"
#include "RockConstruction.hh"
#include "MagnetConstruction.hh"
#include "CavityConstruction.hh"
#include "ScintillatorConstruction.hh"
#include "MINDConstruction.hh"

#include "Materials.hh"
#include "DataCards.hh"

void initDataCards();
void loadDataCards();
void getRunTimeArguments(int argc, char ** argv);

std::string rootFilename_, gdmlFilename_;

double detectorYAxisOffset_;
double tpcVesselPlacementZ_;
double tpcPlacementZ_;
double gap_;
//int scintLayers_;

TGeoVolume * world;

DataCards* cards_;
bool cardsLoaded_;

#endif
