// -------------------------------------------------------------------
/// \file   ELossMeasurement.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 18 Feb 2017
// -------------------------------------------------------------------

#include "Utils.h"

#include "Vector3D.h"
#include "Vector4D.h"

#include <cmath>



double Utils::Distance(const gastpc::Vector3D& a, const gastpc::Vector3D& b)
{
  return std::sqrt( std::pow((b.GetX()-a.GetX()), 2) +
                    std::pow((b.GetY()-a.GetY()), 2) +
                    std::pow((b.GetZ()-a.GetZ()), 2) );
}


double Utils::Distance(const gastpc::Vector4D& a, const gastpc::Vector4D& b)
{
  gastpc::Vector3D a3d(a.GetX(), a.GetY(), a.GetZ());
  gastpc::Vector3D b3d(b.GetX(), b.GetY(), b.GetZ());

  return Utils::Distance(a3d, b3d);
}
