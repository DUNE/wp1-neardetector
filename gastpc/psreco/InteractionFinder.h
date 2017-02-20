// -------------------------------------------------------------------
/// \file   InteractionFinder.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#ifndef INTERACTION_FINDER_H
#define INTERACTION_FINDER_H

#include <vector>

namespace gastpc { class MCGenInfo; }
class TGeoManager;


/// TODO: CLASS DESCRIPTION

class InteractionFinder
{
public:
  /// Constructor
  InteractionFinder(std::string geometry_file);
  /// Destructor
  ~InteractionFinder();

  gastpc::MCGenInfo* ProcessEvent(const std::vector<gastpc::MCGenInfo*>&);

private:
  void LoadGeometry(std::string geometry_file);

private:
  TGeoManager* gm_;
};

#endif
