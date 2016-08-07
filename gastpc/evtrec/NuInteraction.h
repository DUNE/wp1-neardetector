// -------------------------------------------------------------------
/// \file   NuInteraction.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 2 Aug 2016
// -------------------------------------------------------------------

#ifndef GASTPC_NU_INTERACTION
#define GASTPC_NU_INTERACTION

#include <Rtypes.h>
#include <vector>

namespace gastpc { class NuInteraction; }
namespace gastpc { class MCParticle; }
namespace genie { class NtpMCEventRecord; }


/// TODO: class description

class gastpc::NuInteraction
{
public:
  /// Default constructor
  NuInteraction();
  /// Destructor
  virtual ~NuInteraction();
  
  ///
  void SetMCID(int);
  ///
  int GetMCID() const;

  ///
  void SetGenieRecord(genie::NtpMCEventRecord*);
  ///
  genie::NtpMCEventRecord* GetGenieRecord();

  void AddParticle(gastpc::MCParticle*);
  const std::vector<gastpc::MCParticle*>& GetParticles() const;


private:
  int mcid_; ///< Identification number for this interaction

  genie::NtpMCEventRecord* gheprec_; ///< Pointer to genie record

  std::vector<MCParticle*> mcparticles_;

  ClassDef(gastpc::NuInteraction, 1)
};

// Inline definitions //////////////////////////////////////

inline void gastpc::NuInteraction::SetMCID(int id) { mcid_ = id; }
inline int  gastpc::NuInteraction::GetMCID() const { return mcid_; }

inline void gastpc::NuInteraction::SetGenieRecord(genie::NtpMCEventRecord* g)
{ gheprec_ = g; }
inline genie::NtpMCEventRecord* gastpc::NuInteraction::GetGenieRecord()
{ return gheprec_; }

#endif
