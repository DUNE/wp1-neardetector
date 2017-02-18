// -------------------------------------------------------------------
/// \file   RecoParticle.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 18 Feb 2017
// -------------------------------------------------------------------

#include "RecoParticle.h"
#include "MCParticle.h"

ClassImp(gastpc::RecoParticle);


namespace gastpc {

  RecoParticle::RecoParticle(): pdg_code_(0), mcp_(0)
  {
  }


  RecoParticle::~RecoParticle()
  {
  }


  void RecoParticle::SetInitialXYZT(double x, double y, double z, double t)
  {
    initial_xyzt_.SetX(x);
    initial_xyzt_.SetY(y);
    initial_xyzt_.SetZ(z);
    initial_xyzt_.SetT(t);
  }


  const Vector4D& RecoParticle::GetInitialXYZT() const
  {
    return initial_xyzt_;
  }


  void RecoParticle::SetFinalXYZT(double x, double y, double z, double t)
  {
    final_xyzt_.SetX(x);
    final_xyzt_.SetY(y);
    final_xyzt_.SetZ(z);
    final_xyzt_.SetT(t);
  }


  const Vector4D& RecoParticle::GetFinalXYZT() const
  {
    return final_xyzt_;
  }


  void RecoParticle::SetInitialMomentum(double x, double y, double z)
  {
    initial_momentum_.SetX(x);
    initial_momentum_.SetY(y);
    initial_momentum_.SetZ(z);
  }


  const Vector3D& RecoParticle::GetInitialMomentum() const
  {
    return initial_momentum_;
  }


  void RecoParticle::Print(std::ostream& os) const
  {
    if (mcp_) os << "Reco -> MC Particle " << mcp_->GetMCID() << std::endl;
    else os << "Reco Particle" << std::endl;

    os << "   - PDG code: "       << this->GetPDGCode()         << std::endl;
    os << "   - Initial vertex: " << this->GetInitialXYZT()     << std::endl;
    os << "   - Final vertex: "   << this->GetFinalXYZT()       << std::endl;
    os << "   - Momentum: "       << this->GetInitialMomentum() << std::endl;
  }

} // namespace gastpc

std::ostream& operator << (std::ostream& os, const gastpc::RecoParticle& rp)
{
  rp.Print(os);
  return os;
}
