// -------------------------------------------------------------------
/// \file   NeutrinoEnergy.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 21 Feb 2017
// -------------------------------------------------------------------

#include "NeutrinoEnergy.h"

#include "MCGenInfo.h"
#include "MCParticle.h"
#include "RecoParticle.h"
#include "Units.h"
#include "Utils.h"

#include <Ntuple/NtpMCEventRecord.h>
#include <Interaction/Interaction.h>
#include <Interaction/Kinematics.h>

#include <cmath>



NeutrinoEnergy::NeutrinoEnergy(std::vector<std::pair<gastpc::MCParticle*, gastpc::RecoParticle*>> v):
  true_energy_(0.), reco_energy_(0.), true_y_(0.), reco_y_(0.)
{
  for (auto particle: v) {

    if (!particle.first || !particle.second) continue;

    int reco_pdg = std::abs(particle.second->GetPDGCode());
    double momentum = Utils::Magnitude(particle.second->GetInitialMomentum());

    if (reco_pdg == 11) {
      double mass_e = 0.5109989461 * gastpc::MeV;
      reco_energy_ += std::sqrt(momentum*momentum + mass_e*mass_e);
      reco_y_ = std::sqrt(momentum*momentum + mass_e*mass_e);
    }
    else if (reco_pdg == 13) {
      double mass_mu = 105.6583745 * gastpc::MeV;
      reco_energy_ += std::sqrt(momentum*momentum + mass_mu*mass_mu);
      reco_y_ = std::sqrt(momentum*momentum + mass_mu*mass_mu);
    }
    else if (reco_pdg == 211) {
      double mass_pi = 139.57018 * gastpc::MeV;
      reco_energy_ += std::sqrt(momentum*momentum + mass_pi*mass_pi);
    }
    else if (reco_pdg == 2212) {
      reco_energy_ += momentum;
    }
  }

  // Use first element of vector (any would do) to extract the true
  // neutrino energy and inelasticity from genie record
  gastpc::MCGenInfo* mcgi = v[0].first->GetMCGenInfo();
  genie::NtpMCEventRecord* gmcrec = mcgi->GetGeneratorRecord();
  genie::Interaction* interaction = (gmcrec->event)->Summary();
  true_energy_ = (interaction->InitState().ProbeE(genie::kRfLab)) * gastpc::GeV;

  // Calculate inelasticity
  reco_y_ = 1. - reco_y_ / reco_energy_;
  true_y_ = (interaction->KinePtr()->y(true));
}


NeutrinoEnergy::~NeutrinoEnergy()
{
}


double NeutrinoEnergy::RecoEnergy() const
{
  return reco_energy_;
}


double NeutrinoEnergy::TrueEnergy() const
{
  return true_energy_;
}


double NeutrinoEnergy::RecoY() const
{
  return reco_y_;
}


double NeutrinoEnergy::TrueY() const
{
  return true_y_;
}
