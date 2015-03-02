//____________________________________________________________________________
/*!

\class    LbnoGenieFlux

\brief    Class for flux driver used in GENIE

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef EVGEN_LBNOGENIEFLUX_H
#define EVGEN_LBNOGENIEFLUX_H 1

#include <string>

#include <EVGDrivers/GFluxI.h>
#include <PDG/PDGCodeList.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include <TLorentzVector.h>

#include "LbnoDataLib.hh"
#include "DataCards.hh"

class LbnoGenieFlux : public genie::GFluxI  {
 public:
  LbnoGenieFlux();
  ~LbnoGenieFlux() {};

  // -- methods implementing the GENIE GFluxI interface
  const genie::PDGCodeList& FluxParticles()   {return fluxParticles_;}
  double MaxEnergy()                          {return maxEnergy_;}
  bool GenerateNext();
  bool End();
  int PdgCode()                               {return pdgCode_;}
  double Weight()                             {return  1.0;}
  const TLorentzVector& Momentum()            {return momentum_;}
  const TLorentzVector& Position()            {return position_;}
  long int Index()                            {return count_;}
  void Clear(const Option_t*) {}
  void GenerateWeighted(bool) {}

  void setNeutrino(NeutrinoHit* n)            {neutrino_ = n;}

 private:
  //-- protected methods
  void addFluxParticle(int pdg)               {fluxParticles_.push_back(pdg);}
  void addAllNeutrinos() {
    //int all_nu[6] = {12, -12, 14, -14, 16, -16};
    int all_nu[4] = {12, -12, 14, -14};
    for (int i=0; i<sizeof(all_nu)/sizeof(int); i++) 
      fluxParticles_.push_back(all_nu[i]);
  }
  void setMaxEnergy(double maxEnergy)         {maxEnergy_ = maxEnergy / CLHEP::GeV;}

  //-- private data members
  genie::PDGCodeList fluxParticles_; ///< list of neutrino pdg-codes
  double maxEnergy_;          ///< maximum energy (CLHEP units) [GeV]
  int pdgCode_;               ///< running generated nu pdg-code
  TLorentzVector momentum_;    ///< running generated nu 4-momentum (CLHEP units) [GeV]
  TLorentzVector position_;    ///< running generated nu 4-position (CLHEP units) [m

  int count_;
  NeutrinoHit* neutrino_; 
};

#endif

