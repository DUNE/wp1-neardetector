// -------------------------------------------------------------------
/// \file   MCParticle.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#include "MCParticle.h"


ClassImp(gastpc::MCParticle);


namespace gastpc {

  MCParticle::MCParticle():
    mcid_(-1), pdg_code_(0), family_tree_level_(0),
    parent_(0), ancestor_(0), mcgi_(0)
  {
  }


  MCParticle::~MCParticle()
  {
  }


  void MCParticle::SetInitialXYZT(double x, double y, double z, double t)
  {
    initial_xyzt_.SetX(x);
    initial_xyzt_.SetY(y);
    initial_xyzt_.SetZ(z);
    initial_xyzt_.SetT(t);
  }


  const Vector4D& MCParticle::GetInitialXYZT() const
  {
    return initial_xyzt_;
  }


  void MCParticle::SetFinalXYZT(double x, double y, double z, double t)
  {
    final_xyzt_.SetX(x);
    final_xyzt_.SetY(y);
    final_xyzt_.SetZ(z);
    final_xyzt_.SetT(t);
  }


  const Vector4D& MCParticle::GetFinalXYZT() const
  {
    return final_xyzt_;
  }


  void MCParticle::SetInitialMomentum(double x, double y, double z)
  {
    initial_momentum_.SetX(x);
    initial_momentum_.SetY(y);
    initial_momentum_.SetZ(z);
  }


  const Vector3D& MCParticle::GetInitialMomentum() const
  {
    return initial_momentum_;
  }


  void MCParticle::AddMCTrack(MCTrack* mct)
  {
    tracks_.push_back(mct);
  }


  const std::vector<MCTrack*>& MCParticle::GetMCTracks() const
  {
    return tracks_;
  }


  void MCParticle::AddDaughter(gastpc::MCParticle* p)
  {
    daughters_.push_back(p);
  }


  const std::vector<gastpc::MCParticle*>& MCParticle::GetDaughters() const
  {
    return daughters_;
  }


  void MCParticle::Print(std::ostream& os) const
  {
    os << "MC Particle " << this->GetMCID() << std::endl;
    os << "   - PDG code: "       << this->GetPDGCode()         << std::endl;
    os << "   - Family level: "   << this->GetFamilyTreeLevel() << std::endl;
    os << "   - Initial vertex: " << this->GetInitialXYZT()     << std::endl;
    os << "   - Final vertex: "   << this->GetFinalXYZT()       << std::endl;
    os << "   - Momentum: "       << this->GetInitialMomentum() << std::endl;
  }

} // namespace gastpc

std::ostream& operator << (std::ostream& os, const gastpc::MCParticle& mcp)
{
  mcp.Print(os);
  return os;
}
