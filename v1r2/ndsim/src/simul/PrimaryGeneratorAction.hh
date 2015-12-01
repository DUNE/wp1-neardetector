//____________________________________________________________________________
/*!

\class    PrimaryGeneratorAction

\brief    Class to generate primaries from neutrino vertex in geant4

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
           University of Liverpool

\created  Sep 2012
\last update Apr 2015

*/
//____________________________________________________________________________
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "PhysicsList.hh"
#include "GasTPCDataLib.hh"

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4PrimaryParticle.hh>
#include <G4GeneralParticleSource.hh>
#include <G4RunManager.hh>
#include <G4Event.hh>
#include <G4ParticleDefinition.hh>
#include <Randomize.hh>
#include <globals.hh>

//#include <CLHEP/Units/GlobalPhysicalConstants.h>
#include <TLorentzVector.h>

#include <cmath>
#include <cstdlib>
#include <iostream>

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  PrimaryGeneratorAction() {}
  PrimaryGeneratorAction(PhysicsList*, bool GunOn = false);
  ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event* anEvent);
  void setVerbose(int level) {verbose_ = level;};
  void setNuEvent(NeutrinoEvent* e) {event_ = e;}

 private:
  void AddParticleToVertex(G4PrimaryVertex* v, ParticleDescrShortRecord* p);
  void printProgress(double percent, int i, int n, int m);

  G4RunManager *runMgr_;
	
  G4PrimaryVertex* primVertex_;

  G4GeneralParticleSource* particleGun;

  bool GunOn_;
  PhysicsList* phys;
  NeutrinoEvent* event_;

  int verbose_,beamCount;

};

#endif
