#ifndef DRAW_HH
#define DRAW_HH 1

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <cmath>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "TGeoManager.h"
#include "TSystem.h"

#include "TInterpreter.h"
#include "TFrame.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "THistPainter.h"
#include "TPad.h"
#include "TColor.h"

#include "DataCards.hh"

void initDataCards();
void loadDataCards();
void getRunTimeArguments(int argc, char ** argv);

std::string rootFilename_;

DataCards* cards_;
bool cardsLoaded_;

#endif
