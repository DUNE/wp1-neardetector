// -------------------------------------------------------------------
/// \file   MCParticle.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 12 Mar 2016
// -------------------------------------------------------------------

#ifndef GASTPC_MCPARTICLE_H
#define GASTPC_MCPARTICLE_H

#include <Rtypes.h>
#include <TLorentzVector.h>
#include <string>

namespace gastpc { class MCParticle; }


/// TODO: Class description

class gastpc::MCParticle 
{
public:
  /// Default constructor
  MCParticle();
  /// Destructor
  virtual ~MCParticle();

  ///
  void SetPrimary(bool);
  ///
  bool IsPrimary() const;

  /// Set Monte Carlo ID number for the particle
  void SetMCID(int);
  /// Return Monte Carlo ID for the particle
  int GetMCID() const;

  /// Set particle's PDG code
  void SetPDGCode(int);
  /// Return particle's PDG code
  int GetPDGCode() const;

  ///
  void SetInitialXYZT(double, double, double, double);
  ///  
  const TLorentzVector& GetInitialXYZT() const;
  ///
  void SetFinalXYZT(double, double, double, double);
  ///
  const TLorentzVector& GetFinalXYZT() const;

  ///
  void SetInitial4Momentum(double, double, double, double);
  ///
  const TLorentzVector& GetInitial4Momentum() const;
  ///
  void SetFinal4Momentum(double, double, double, double);
  ///
  const TLorentzVector& GetFinal4Momentum() const;

  ///
  void SetCreatorProcess(const std::string&);
  ///
  const std::string& GetCreatorProcess() const;

  ///
  void SetInitialVolume(const std::string&);
  ///
  const std::string& GetInitialVolume() const;
  ///
  void SetFinalVolume(const std::string&);
  ///
  const std::string& GetFinalVolume() const;

  ///
  void SetMother(gastpc::MCParticle*);
  ///
  gastpc::MCParticle* GetMother() const;
  ///
  void AddDaughter(gastpc::MCParticle*);
  ///
  const std::vector<gastpc::MCParticle*>& GetDaughters() const;

private:
  bool primary_; ///< Is this particle a primary?
  
  int mcid_;     ///< Monte Carlo ID number
  int pdg_code_; ///< PDG code for particle identification

  TLorentzVector initial_xyzt_; ///< Particle's initial position
  TLorentzVector final_xyzt_;   ///< Particle's final position

  TLorentzVector initial_4P_; ///< Particle's initial four-momentum
  TLorentzVector final_4P_;   ///< Particle's final four-momentum

  std::string creator_process_;

  std::string initial_volume_;
  std::string final_volume_;

  gastpc::MCParticle* mother_; ///< Pointer to mother particle
  std::vector<gastpc::MCParticle*> daughters_;

  ClassDef(gastpc::MCParticle, 1)
};

// Inline definitions //////////////////////////////////////

inline void gastpc::MCParticle::SetPrimary(bool b) { primary_ = b; }
inline bool gastpc::MCParticle::IsPrimary() const { return primary_; }

inline void gastpc::MCParticle::SetMCID(int id) { mcid_ = id; }
inline int  gastpc::MCParticle::GetMCID() const { return mcid_; }

inline void gastpc::MCParticle::SetPDGCode(int pdg) { pdg_code_ = pdg; }
inline int  gastpc::MCParticle::GetPDGCode() const { return pdg_code_; }

inline gastpc::MCParticle* gastpc::MCParticle::GetMother() const 
  { return mother_; }
inline void gastpc::MCParticle::SetMother(gastpc::MCParticle* p) 
  { mother_ = p; }

inline void gastpc::MCParticle::AddDaughter(gastpc::MCParticle* p)
  { daughters_.push_back(p); }
inline const std::vector<gastpc::MCParticle*>& 
  gastpc::MCParticle::GetDaughters() const { return daughters_; }

inline const TLorentzVector& gastpc::MCParticle::GetInitialXYZT() const
  { return initial_xyzt_; }
inline const TLorentzVector& gastpc::MCParticle::GetFinalXYZT() const
  { return final_xyzt_; }

inline const TLorentzVector& gastpc::MCParticle::GetInitial4Momentum() const
  { return initial_4P_; }
inline const TLorentzVector& gastpc::MCParticle::GetFinal4Momentum() const
  { return final_4P_; }

inline void gastpc::MCParticle::SetCreatorProcess(const std::string& name)
  { creator_process_ = name; }
inline const std::string& gastpc::MCParticle::GetCreatorProcess() const
  { return creator_process_; }

inline void gastpc::MCParticle::SetInitialVolume(const std::string& name)
  { initial_volume_ = name; }
inline const std::string& gastpc::MCParticle::GetInitialVolume() const
  { return initial_volume_; }
inline void gastpc::MCParticle::SetFinalVolume(const std::string& name)
  { final_volume_ = name; }
inline const std::string& gastpc::MCParticle::GetFinalVolume() const
  { return final_volume_; }

#endif
