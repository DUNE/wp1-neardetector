// -------------------------------------------------------------------
/// \file   MCGenInfo.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 27 Nov 2016
// -------------------------------------------------------------------

#ifndef GASTPC_MC_GEN_INFO_H
#define GASTPC_MC_GEN_INFO_H

#include <Rtypes.h>
#include <vector>

namespace gastpc { class MCGenInfo; }
namespace gastpc { class MCParticle; }
namespace genie  { class NtpMCEventRecord; }


/// TODO: class description

class gastpc::MCGenInfo
{
public:
  /// Default constructor
  MCGenInfo();
  /// Destructor
  virtual ~MCGenInfo();

  ///
  void SetMCID(int);
  ///
  int GetMCID() const;

  ///
  void SetGeneratorRecord(genie::NtpMCEventRecord*);
  ///
  genie::NtpMCEventRecord* GetGeneratorRecord();

  ///
  void AddMCParticle(gastpc::MCParticle*);
  ///
  const std::vector<gastpc::MCParticle*>& GetMCParticles() const;

private:
  int mcid_; ///< Identification number for this interaction
  genie::NtpMCEventRecord* grec_; ///< Pointer to MC generator record
  std::vector<MCParticle*> mcparticles_;

  ClassDef(gastpc::MCGenInfo, 2)
};

// Inline definitions //////////////////////////////////////
inline void gastpc::MCGenInfo::SetMCID(int id) { mcid_ = id; }
inline int  gastpc::MCGenInfo::GetMCID() const { return mcid_; }

#endif
