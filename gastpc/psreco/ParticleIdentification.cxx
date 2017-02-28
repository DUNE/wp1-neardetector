// -------------------------------------------------------------------
/// \file   ParticleIdentification.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#include "ParticleIdentification.h"

#include "MCParticle.h"
#include "RecoParticle.h"
#include "Vector3D.h"
#include "Utils.h"
#include "Units.h"
#include "EcalPid.h"

#include <TRandom3.h>


ParticleIdentification::ParticleIdentification(TRandom3* rnd)
{
  rnd_ = rnd;
  ecal_ = new EcalPid();
}


ParticleIdentification::~ParticleIdentification()
{
  delete ecal_;
}


int ParticleIdentification::Electron(gastpc::MCParticle* mcp,
                                     gastpc::RecoParticle* recop)
{
  if (!mcp || !recop) return 0;

  int pdg = mcp->GetPDGCode();
  int sgn = Utils::sgn(pdg);

  // dE/dx measurement in TPC allows the identification of electrons
  // except at crossover with muon and proton curves

  gastpc::Vector3D momentum = recop->GetInitialMomentum();
  double p_mag = Utils::Magnitude(momentum);

  // Crossover with muon
  if (p_mag > (0.1*gastpc::GeV) && p_mag < (0.3*gastpc::GeV)) {
    if (ecal_->LLR_MIP_EM_electron() < 0.) pdg = 13 * sgn;
  }

  // Crossover with protons only a problem if the electron is a positron
  if (sgn < 0) {
    if (p_mag > (1.0*gastpc::GeV) && p_mag < (2.0*gastpc::GeV)) {
      if (ecal_->LLR_EM_HIP_electron() > 0.) pdg = 2212;
    }
  }

  recop->SetPDGCode(pdg);
  return (pdg);
}


int ParticleIdentification::Muon(gastpc::MCParticle* mcp,
                                  gastpc::RecoParticle* recop)
{
  if (!mcp || !recop) return 0;

  int pdg = mcp->GetPDGCode();
  int sgn = Utils::sgn(pdg);

  // High-energy muons are identified with perfect efficiency thanks to
  // muon range instrumentation. For lower-energy muons, we rely on ECAL
  // for separation of muons from pions and electrons
  gastpc::Vector3D momentum = recop->GetInitialMomentum();
  double p_mag = Utils::Magnitude(momentum);

  if (p_mag < (1.5*gastpc::GeV)) {
    if (ecal_->LLR_MIP_PION_muon() > 0.) pdg = -211 * sgn;
  }

  recop->SetPDGCode(pdg);
  return pdg;
}


int ParticleIdentification::Pion(gastpc::MCParticle* mcp,
                                 gastpc::RecoParticle* recop,
                                 bool found_lepton, bool FHC)
{
  if (!mcp || !recop) return 0;

  int pdg = mcp->GetPDGCode();

  // If a lepton was found, pions below 1.5 GeV are identified univocally.
  // If not, they could be identified as a muon if they have the right charge.
  gastpc::Vector3D momentum = recop->GetInitialMomentum();
  double p_mag = Utils::Magnitude(momentum);

  if (p_mag < (1.5*gastpc::GeV)) {
    if (!found_lepton) {
      if (FHC && (pdg < 0)) pdg = 13;
    }
  }
  else {
    if (pdg > 0) {
      if (rnd_->Rndm() > 0.5) pdg = 2212;
    }
  }

  recop->SetPDGCode(pdg);
  return pdg;
}


int ParticleIdentification::Proton(gastpc::MCParticle* mcp,
                                    gastpc::RecoParticle* recop)
{
  if (!mcp || !recop) return 0;
  
  int pdg = mcp->GetPDGCode();

  // dE/dx measurement in TPC allows the identification of protons
  // up to 1.5 GeV/c. Above that, they may be identified as positive pions.
  gastpc::Vector3D momentum = recop->GetInitialMomentum();
  double p_mag = Utils::Magnitude(momentum);

  if (p_mag > (1.5*gastpc::GeV)) {
    if (rnd_->Rndm() > 0.5) pdg = 211;
  }

  recop->SetPDGCode(pdg);
  return pdg;
}
