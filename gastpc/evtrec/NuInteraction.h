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

namespace gastpc { class NuInteraction; }
namespace genie { class NtpMCEventRecord; }


class gastpc::NuInteraction
{
public:
  NuInteraction();
  virtual ~NuInteraction();
  
  ///
  void SetSpillNumber(int);
  ///
  int GetSpillNumber() const;

  ///
  void SetGenieRecord(genie::NtpMCEventRecord*);
  ///
  genie::NtpMCEventRecord* GetGenieRecord();


private:
  int spill_num_;

  double nu_energy_;

  genie::NtpMCEventRecord* gheprec_; ///< Pointer to genie record

  ClassDef(gastpc::NuInteraction, 1)
};

// Inline definitions //////////////////////////////////////

inline void gastpc::NuInteraction::SetSpillNumber(int n) { spill_num_ = n; }
inline int gastpc::NuInteraction::GetSpillNumber() const { return spill_num_; }

inline void gastpc::NuInteraction::SetGenieRecord(genie::NtpMCEventRecord* g)
  { gheprec_ = g; }
inline genie::NtpMCEventRecord* gastpc::NuInteraction::GetGenieRecord()
  { return gheprec_; }

#endif