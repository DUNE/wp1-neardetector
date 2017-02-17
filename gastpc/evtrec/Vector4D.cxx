// -------------------------------------------------------------------
/// \file   Vector4D.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 17 Feb 2017
// -------------------------------------------------------------------

#include "Vector4D.h"

ClassImp(gastpc::Vector4D);


namespace gastpc {

  Vector4D::Vector4D(): x_(0.), y_(0.), z_(0.), t_(0.)
  {
  }


  Vector4D::Vector4D(double x, double y, double z, double t):
    x_(x), y_(y), z_(z), t_(t)
  {
  }


  Vector4D::~Vector4D()
  {
  }


  void Vector4D::Print(std::ostream& os) const
  {
    os << "(" << x_ << ", " << y_ << ", " << z_ << ", " << t_ << ")";
  }

} // namespace gastpc

std::ostream& operator <<(std::ostream& os, const gastpc::Vector4D& v)
{
  v.Print(os);
  return os;
}
