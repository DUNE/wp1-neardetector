// -------------------------------------------------------------------
/// \file   NeutrinoEnergy.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#ifndef NEUTRINO_ENERGY_H
#define NEUTRINO_ENERGY_H

#include <vector>

namespace gastpc { class MCParticle; }
namespace gastpc { class RecoParticle; }


/// TODO: CLASS DESCRIPTION

class NeutrinoEnergy
{
public:
  /// Constructor
  NeutrinoEnergy(std::vector<std::pair<gastpc::MCParticle*, gastpc::RecoParticle*>>);
  /// Destructor
  ~NeutrinoEnergy();

  double TrueEnergy() const;
  double RecoEnergy() const;
  double TrueY() const;
  double RecoY() const;



private:
  double true_energy_;
  double reco_energy_;
  double true_y_;
  double reco_y_;
};

#endif
