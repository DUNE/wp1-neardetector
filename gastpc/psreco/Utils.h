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

  static double Magnitude(const gastpc::Vector3D&);

  template <typename T> static int sgn(T val); // { return (T(0) < val) - (val < T(0));}

private:
  /// Hidden constructor (all methods are static)
  Utils();
  /// Hidden destructor
  ~Utils();
};

template <typename T>
inline int Utils::sgn(T val) { return (T(0) < val) - (val < T(0));}

#endif
