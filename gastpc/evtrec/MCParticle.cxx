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
    primary_(false), mcid_(-1), pdg_code_(0), 
    initial_xyzt_(TLorentzVector()), final_xyzt_(TLorentzVector()),
    initial_4P_(TLorentzVector()), final_4P_(TLorentzVector()),
    interaction_(0), mother_(0)
  {
  }


  MCParticle::~MCParticle()
  {
    this->Clear();
  }


  void MCParticle::Clear()
  {
    primary_ = false;
    mcid_ = -1;
    pdg_code_ = 0;
    initial_xyzt_ = TLorentzVector();
    final_xyzt_ = TLorentzVector();
    initial_4P_ = TLorentzVector();
    final_4P_ = TLorentzVector();
    interaction_ = 0;
    mother_ = 0;
    daughters_.clear();
  }


  void MCParticle::SetInitialXYZT(double x, double y, double z, double t)
  {
    initial_xyzt_.SetXYZT(x, y, z, t);
  }


  void MCParticle::SetFinalXYZT(double x, double y, double z, double t)
  {
    final_xyzt_.SetXYZT(x, y, z, t);
  }


  void MCParticle::SetInitial4Momentum(double px, double py, double pz, double E)
  {
    initial_4P_.SetPxPyPzE(px, py, pz, E);
  }


  void MCParticle::SetFinal4Momentum(double px, double py, double pz, double E)
  {
    final_4P_.SetPxPyPzE(px, py, pz, E);
  }


  void MCParticle::Info(std::ostream& os) const 
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

