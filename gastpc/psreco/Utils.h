// -------------------------------------------------------------------
/// \file   Utils.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 19 Feb 2017
// -------------------------------------------------------------------

#ifndef UTILS_H
#define UTILS_H

namespace gastpc { class Vector3D; }
namespace gastpc { class Vector4D; }


/// Collection of utilities

class Utils
{
public:
  /// Calculate the Euclidean distance between two points
  static double Distance(const gastpc::Vector4D&, const gastpc::Vector4D&);
  /// Calculate the Euclidean distance between two points
  static double Distance(const gastpc::Vector3D&, const gastpc::Vector3D&);

private:
  /// Hidden constructor (all methods are static)
  Utils();
  /// Hidden destructor
  ~Utils();
};

#endif
