// -------------------------------------------------------------------
/// \file   BeamSpillGenerator.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 6 June 2016
// -------------------------------------------------------------------

#include "BeamSpillGenerator.h"

#include "BeamSpillSource.h"
#include "GHepReader.h"

#include <G4GenericMessenger.hh>



BeamSpillGenerator::BeamSpillGenerator()
{
  msg_ = new G4GenericMessenger(this, "/gastpc/generator/");
  msg_->DeclareMethod("add_ghep_source", &BeamSpillGenerator::AddGHepSource, "");
}


BeamSpillGenerator::~BeamSpillGenerator()
{}


void BeamSpillGenerator::AddGHepSource(G4String path, G4double mean)
{
  G4cerr << "BeamSpillGenerator::AddGHepSource" << G4endl;
  G4cerr << "Path: " << path << G4endl;
  G4cerr << "Mean: " << mean << G4endl;

  GHepReader* ghep_source = new GHepReader(path, mean);
  sources_.push_back(ghep_source);
}


void BeamSpillGenerator::GeneratePrimaries(G4Event* g4evt)
{
  for (BeamSpillSource* s: sources_) s->FillG4Event(g4evt);
}