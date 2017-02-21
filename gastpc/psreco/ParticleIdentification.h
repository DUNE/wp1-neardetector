// -------------------------------------------------------------------
/// \file   ParticleIdentification.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#ifndef PARTICLE_IDENTIFICATION_H
#define PARTICLE_IDENTIFICATION_H

namespace gastpc { class MCParticle; }
namespace gastpc { class RecoParticle; }

class EcalPid;
class TRandom3;


/// TODO: CLASS DESCRIPTION

class ParticleIdentification
{
public:
  /// Constructor
  ParticleIdentification(TRandom3*);
  /// Destructor
  ~ParticleIdentification();

  int Electron(gastpc::MCParticle*, gastpc::RecoParticle*);
  int Muon(gastpc::MCParticle*, gastpc::RecoParticle*);
  int Pion(gastpc::MCParticle*, gastpc::RecoParticle*, bool, bool);
  int Proton(gastpc::MCParticle*, gastpc::RecoParticle*);

private:
  EcalPid* ecal_;
  TRandom3* rnd_;
};

#endif
