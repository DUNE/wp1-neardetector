// -------------------------------------------------------------------
/// \file   Vector3D.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 17 Feb 2017
// -------------------------------------------------------------------

#include "Vector3D.h"

ClassImp(gastpc::Vector3D);


namespace gastpc {

  Vector3D::Vector3D(): x_(0.), y_(0.), z_(0.)
  {
  }


  Vector3D::Vector3D(double x, double y, double z): x_(x), y_(y), z_(z)
  {
  }


  Vector3D::~Vector3D()
  {
  }


  void Vector3D::Print(std::ostream& os) const
  {
    os << "(" << x_ << ", " << y_ << ", " << z_ << ")";
  }

} // namespace gastpc

std::ostream& operator <<(std::ostream& os, const gastpc::Vector3D& v)
{
  v.Print(os);
  return os;
}
