// -------------------------------------------------------------------
/// \file   GHepReader.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 7 June 2016
// -------------------------------------------------------------------

#include "GHepReader.h"

#include <G4Event.hh>

#include <TChain.h>


GHepReader::GHepReader(const G4String& path, G4double mean):
  BeamSpillSource(), ifiles_(new TChain(path))
{
}


GHepReader::~GHepReader()
{}


void GHepReader::FillG4Event(G4Event*)
{}