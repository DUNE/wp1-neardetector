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
#include "CosmicsSource.h"
#include "EventGenerationInfo.h"

#include <G4GenericMessenger.hh>
#include <G4Event.hh>


BeamSpillGenerator::BeamSpillGenerator()
{
  msg_ = new G4GenericMessenger(this, "/gastpc/generator/");
  msg_->DeclareMethod("add_ghep_source", &BeamSpillGenerator::AddGHepSource, "");
  msg_->DeclareMethod("add_cosmics_source", &BeamSpillGenerator::AddCosmicsSource, "");
}


BeamSpillGenerator::~BeamSpillGenerator()
{
  delete msg_;
  for (BeamSpillSource* s: sources_) delete s;
  sources_.clear();
}


void BeamSpillGenerator::AddGHepSource(G4String path, G4double mean)
{
  GHepReader* ghep_source = new GHepReader(path, mean);
  sources_.push_back(ghep_source);
}


void BeamSpillGenerator::AddCosmicsSource(G4String path)
{
  CosmicsSource* cosmics_source = new CosmicsSource(path);
  sources_.push_back(cosmics_source);
}


void BeamSpillGenerator::GeneratePrimaries(G4Event* event)
{
  event->SetUserInformation(new EventGenerationInfo());
  for (BeamSpillSource* s: sources_) s->GeneratePrimaryVertices(event);
}
