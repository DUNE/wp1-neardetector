//____________________________________________________________________________
/*!

\class    GasTPCTracking

\brief    Program to load primaries from neutrino verticies and track them
	  Only tracking, no flux or event generation

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  June 2013
\last update Apr 2015
*/
//____________________________________________________________________________
#ifndef GASTPCTRACKING_HH
#define GASTPCTRACKING_HH

#include <iostream>

#include <TTree.h>
#include <TFile.h>
#include <TRandom.h>
#include <TRandom3.h>

#include "UtlXML.hh"

void ReadDatacard(std::string datacard);

std::string fGeometryFile, fSimTargetVolume, fPhysicsList, fEMPhysicsList, fHadPhysicsList;
double fMinEnergyCut, fGammaCut, fElectronCut, fPositronCut, fNeutronCut, fMagneticFieldStrength;
bool fRunVisMacro;
int fTrackingVerbose, fSimEcalVolume;

#endif
