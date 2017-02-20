// -------------------------------------------------------------------
/// \file   InteractionFinder.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Feb 2017
// -------------------------------------------------------------------

#ifndef INTERACTION_FINDER_H
#define INTERACTION_FINDER_H

#include <string>
#include <vector>

namespace gastpc { class MCGenInfo; }
class TGeoManager;


/// TODO: CLASS DESCRIPTION

class InteractionFinder
{
public:
  /// Constructor
  InteractionFinder(const std::string& geometry);
  /// Destructor
  ~InteractionFinder();

  gastpc::MCGenInfo* ProcessEvent(const std::vector<gastpc::MCGenInfo*>&);

private:
  void LoadGeometry(const std::string&);

private:
  TGeoManager* gm_;
};

#endif
