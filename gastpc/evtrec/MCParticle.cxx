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
    this->Clear();
  }


  void MCParticle::Clear()
  {
  }


  void MCParticle::SetInitialXYZT(double x, double y, double z, double t)
  {
    initial_xyzt_ = {x, y, z, t};
  }


  const std::vector<double>& MCParticle::GetInitialXYZT() const
  {
    return initial_xyzt_;
  }


  void MCParticle::SetFinalXYZT(double x, double y, double z, double t)
  {
    initial_xyzt_ = {x, y, z, t};
  }


  const std::vector<double>& MCParticle::GetFinalXYZT() const
  {
    return initial_xyzt_;
  }


  void MCParticle::SetInitialMomentum(double x, double y, double z)
  {
    initial_mom = {x, y, z};
  }


  const std::vector<double>& MCParticle::GetInitialMomentum() const
  {
    return initial_mom;
  }


  void MCParticle::AddTrack(MCTrack* mct)
  {
    tracks_.push_back(mct);
  }


  const std::vector<MCTrack*>& MCParticle::GetTracks() const
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
  
  
  void MCParticle::Info(std::ostream& /*os*/) const 
  {
    // os << "MCParticle::Info()" << std::endl;
    // os << " - MC ID: " << this->GetMCID() << std::endl;
    // os << " - PDG code: " << this->GetPDGCode() << std::endl;
  }

} // namespace gastpc


std::ostream& operator << (std::ostream& os, const gastpc::MCParticle& obj)
{
  obj.Info(os);
  return os;
}

