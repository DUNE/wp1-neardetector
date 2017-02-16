// -------------------------------------------------------------------
/// \file   Vector4D.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 14 Feb 2017
// -------------------------------------------------------------------

#ifndef GASTPC_VECTOR4D_H
#define GASTPC_VECTOR4D_H

#include <Rtypes.h>

namespace gastpc { class Vector4D; }

/// TODO: class description

class gastpc::Vector4D
{
public:
  /// Constructor
  Vector4D(double, double, double, double);
  /// Default constructor
  Vector4D();
  /// Destructor
  virtual ~Vector4D();

  void SetX(double);
  double GetX() const;

  void SetY(double);
  double GetY() const;

  void SetZ(double);
  double GetZ() const;

  void SetT(double);
  double GetT() const;

private:
  double x_, y_, z_, t_;

  ClassDef(gastpc::Vector4D, 1)
};

// Inline definitions //////////////////////////////////////
inline gastpc::Vector4D::Vector4D(double x, double y, double z, double t):
  x_(x), y_(y), z_(z), t_(t) {}
inline gastpc::Vector4D::Vector4D(): x_(0.), y_(0.), z_(0.), t_(0.) {}
inline gastpc::Vector4D::~Vector4D() {}
inline void gastpc::Vector4D::SetX(double x) { x_ = x; }
inline double gastpc::Vector4D::GetX() const { return x_; }
inline void gastpc::Vector4D::SetY(double y) { y_ = y; }
inline double gastpc::Vector4D::GetY() const { return y_; }
inline void gastpc::Vector4D::SetZ(double z) { z_ = z; }
inline double gastpc::Vector4D::GetZ() const { return z_; }
inline void gastpc::Vector4D::SetT(double t) { t_ = t; }
inline double gastpc::Vector4D::GetT() const { return t_; }

#endif
