// -------------------------------------------------------------------
/// \file   BaseGeometry.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 9 June 2016
// -------------------------------------------------------------------

#ifndef GASTPC_BASE_GEOMETRY_H
#define GASTPC_BASE_GEOMETRY_H

class G4LogicalVolume;


/// Base class

class BaseGeometry
{
public:
  /// Default constructor
  BaseGeometry();

  /// Destructor
  virtual ~BaseGeometry();

  /// Return the pointer to the logical volume representing the geometry
  G4LogicalVolume* GetLogicalVolume();

protected: 
  /// Set pointer to logical volume representing this geometry
  void SetLogicalVolume(G4LogicalVolume*);

  virtual void DefineVolumes() = 0;

private:
  G4LogicalVolume* logvol_; ///< Pointer to the geometry's logical volume
};

inline BaseGeometry::BaseGeometry(): logvol_(0) {}
inline BaseGeometry::~BaseGeometry() {}
inline void BaseGeometry::SetLogicalVolume(G4LogicalVolume* p) { logvol_ = p; }
inline G4LogicalVolume* BaseGeometry::GetLogicalVolume() 
{ if (!logvol_) DefineVolumes(); return logvol_; }

#endif