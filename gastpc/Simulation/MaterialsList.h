// -------------------------------------------------------------------
/// \file   MaterialsList.h
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 17 June 2016
// -------------------------------------------------------------------

#ifndef GASTPC_MATERIALS_LIST_H
#define GASTPC_MATERIALS_LIST_H

class G4Material;


namespace MaterialsList {

  /// Titanium alloy UNS-R56323 (Ti-3Al-2.5V-0.1Ru)
  G4Material* TitaniumR56323();

  /// Stainless steel Type 304L
  G4Material* Steel304L();

} // end namespace MaterialsList

#endif