// -------------------------------------------------------------------
/// \file   Vector3D.h
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 14 Feb 2017
// -------------------------------------------------------------------

#ifndef GASTPC_VECTOR3D_H
#define GASTPC_VECTOR3D_H

namespace gastpc { class Vector3D; }

/// TODO: class description

class gastpc::Vector3D
{
public:
  /// Constructor
  Vector3D(double, double, double);
  /// Default constructor
  Vector3D();
  /// Destructor
  virtual ~Vector3D();

  void SetX(double);
  double GetX() const;

  void SetY(double);
  double GetY() const;

  void SetZ(double);
  double GetZ() const;

private:
  double x_, y_, z_;

  ClassDef(gastpc::Vector3D, 1);
};

// Inline definitions //////////////////////////////////////
inline gastpc::Vector3D::Vector3D(double x, double y, double z): x_(x), y_(y), z_(z) {}
inline gastpc::Vector3D::Vector3D(): x_(0.), y_(0.), z_(0.) {}
inline gastpc::Vector3D::~Vector3D() {}
inline void gastpc::Vector3D::SetX(double x) { x_ = x; }
inline double gastpc::Vector3D::GetX() const { return x_; }
inline void gastpc::Vector3D::SetY(double y) { y_ = y; }
inline double gastpc::Vector3D::GetY() const { return y_; }
inline void gastpc::Vector3D::SetZ(double z) { z_ = z; }
inline double gastpc::Vector3D::GetZ() const { return z_; }

#endif
