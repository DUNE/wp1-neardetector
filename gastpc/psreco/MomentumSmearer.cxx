// -------------------------------------------------------------------
/// \file   MomentumSmearer.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#include "MomentumSmearer.h"

#include "MCParticle.h"
#include "RecoParticle.h"
#include "MCTrack.h"
#include "MCHit.h"
#include "Vector3D.h"
#include "Units.h"

#include <TRandom3.h>
#include <TVector3.h>

#include <cmath>


MomentumSmearer::MomentumSmearer(TRandom3* rnd)
{
  rnd_ = rnd;
}


MomentumSmearer::~MomentumSmearer()
{
}


void MomentumSmearer::ProcessParticle(std::pair<gastpc::MCParticle*,
                                                gastpc::RecoParticle*>& part)
{
  // We will only consider muons, electrons, pions and protons
  int pdg = std::abs((part.first)->GetPDGCode());
  if ((pdg != 11) && (pdg != 13) && (pdg != 211) && (pdg != 2212)) return;

  // We will consider only particles that have left a track in the TPC
  gastpc::MCTrack* track = 0;
  for (gastpc::MCTrack* mct: (part.first)->GetMCTracks()) {
    if (mct->GetLabel() == "TPC") {
      track = mct;
      break;
    }
  }
  if (!track) return;

  // Smear momentum and be done
  this->ProcessTrack(track, part.second);
}




void MomentumSmearer::ProcessTrack(gastpc::MCTrack* track,
                                   gastpc::RecoParticle* reco_particle)
{
  gastpc::Vector4D min = track->GetMCParticle()->GetInitialXYZT();;
  gastpc::Vector4D max = track->GetMCParticle()->GetInitialXYZT();;

  for (gastpc::MCHit* hit: track->GetMCHits()) {

    gastpc::Vector4D current = hit->GetXYZT();

    if (current.GetX() < min.GetX()) min.SetX(current.GetX());
    if (current.GetY() < min.GetY()) min.SetY(current.GetY());
    if (current.GetZ() < min.GetZ()) min.SetZ(current.GetZ());
    if (current.GetX() > max.GetX()) max.SetX(current.GetX());
    if (current.GetY() > max.GetY()) max.SetY(current.GetY());
    if (current.GetZ() > max.GetZ()) max.SetZ(current.GetZ());
  }

  double length_transv = max.GetZ() - min.GetZ();
  double length_longit = max.GetY() - min.GetY();

  TVector3 momentum(track->GetMCParticle()->GetInitialMomentum().GetX(),
                    track->GetMCParticle()->GetInitialMomentum().GetY(),
                    track->GetMCParticle()->GetInitialMomentum().GetZ());
  double p_mag  = momentum.Mag();
  double pt_mag = momentum.Perp(TVector3(0.,1.,0.));
  double angle  = std::asin(pt_mag/p_mag);

  double p_reco =
    SmearPt(p_mag, length_transv) / std::sin(SmearAngle(angle, p_mag, length_longit));
  p_reco = p_reco * gastpc::GeV;

  /// The following avoids obtaining absurd values for very short tracks
  /// (typically low-energy protons)
  double limit = 5. * gastpc::cm;
  if (length_transv < limit) p_reco = p_mag;

  gastpc::Vector3D momentum_reco;
  momentum_reco.SetX(momentum.X() * p_reco / p_mag);
  momentum_reco.SetY(momentum.Y() * p_reco / p_mag);
  momentum_reco.SetZ(momentum.Z() * p_reco / p_mag);

  reco_particle->SetInitialMomentum(momentum_reco.GetX(),
                                    momentum_reco.GetY(),
                                    momentum_reco.GetZ());
}


double MomentumSmearer::SmearPt(double Pt, double length)
{
  double sigma_x = 0.1;  // cm
  double magfield = 0.4; // tesla
  double X0 = 1300.;     // cm

  Pt = Pt / gastpc::GeV;
  length = length / gastpc::cm;

  int N = std::ceil(length);

  double sigma_1 = sigma_x * Pt * Pt * std::sqrt(720/(N+4)) / (0.3 * magfield * length * length);
  double sigma_2 = 0.05 * std::sqrt(1.43 * length / X0) / (magfield * length);

  double sigma = std::sqrt(sigma_1*sigma_1 + sigma_2*sigma_2);

  return rnd_->Gaus(Pt, sigma);
}


double MomentumSmearer::SmearAngle(double angle, double P, double length)
{
  double sigma_x = 0.1; // cm
  double X0 = 1300.;    // cm

  length = length / gastpc::cm;
  P = P / gastpc::GeV;

  int N = std::ceil(length);

  double sigma = sigma_x * std::sqrt(12*(N-1)/(N*(N+1))) + 0.015 * std::sqrt(length/X0) / (std::sqrt(3.) * P);

  return rnd_->Gaus(angle, sigma);
}
