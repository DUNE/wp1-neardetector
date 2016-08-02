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
{
  delete msg_;
  for (BeamSpillSource* s: sources_) delete s;
  sources_.clear();
}


void BeamSpillGenerator::AddGHepSource(G4String path, G4double mean)
{
  // This method adds a GHEP source to the generator.
  // (Similar methods)
  GHepReader* ghep_source = new GHepReader(path, mean);
  sources_.push_back(ghep_source);
}


void BeamSpillGenerator::GeneratePrimaries(G4Event* event)
{
  for (BeamSpillSource* s: sources_) s->GeneratePrimaryVertices(event);
}