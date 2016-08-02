// -------------------------------------------------------------------
/// \file   GHepReader.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 7 June 2016
// -------------------------------------------------------------------

#include "GHepReader.h"

#include <G4Event.hh>
#include <G4LorentzVector.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>

#include <TChain.h>
#include <TCollection.h>
//#include <TVector3.h>

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>
#include <GHEP/GHepParticle.h>

#include <map>



GHepReader::GHepReader(const G4String& path, G4double mean):
  BeamSpillSource(mean), 
  ghep_chain_(0), num_entries_(0), current_entry_(0), gmcrec_(0)
{
  this->Initialize(path);
}


GHepReader::~GHepReader()
{
  delete ghep_chain_;
}


void GHepReader::Initialize(const G4String& path)
{
  ghep_chain_ = new TChain("gtree");
  ghep_chain_->Add(path);
  ghep_chain_->SetBranchAddress("gmcrec", &gmcrec_);

  num_entries_ = ghep_chain_->GetEntries();
}


void GHepReader::GeneratePrimaryVertices(G4Event* event)
{
  // Read from the GHEP source as many interaction as required
  // according to the defined mean number of interactions per spill.

  // The G4Event is organized in terms of vertices (a position and time)
  // and primary particles associated to them. In the GHEP record, in contrast,
  // we are given a single vertex (typically, the point at which the neutrino
  // interacted) and relative positions and times for each final-state 
  // particle. We'll keep track of the different Geant4-like vertices we
  // create using the map defined below.
  std::map<G4LorentzVector, G4PrimaryVertex*> vertex_map;

  // Decide how many interactions we'll read
  
  G4int num_interactions;

  if (mean_ <= 0.) 
    num_interactions = 1;
  else 
    num_interactions = BeamSpillSource::NumberOfInteractions();

  for (G4int i=0; i < num_interactions; ++i) {

    ++current_entry_;
    ghep_chain_->GetEntry(current_entry_);
    genie::EventRecord* record = gmcrec_->event;

    // Get the common vertex position and time in the GHEP record
    TLorentzVector* xyzt = record->Vertex();

    // Get a time offset for this interaction
    G4double time_offset = BeamSpillSource::TimeOffset();

    // Loop through the particles in the GHEP record
    genie::GHepParticle* gpart = 0;
    TIter gpart_iter(record);
    while ((gpart = dynamic_cast<genie::GHepParticle*>(gpart_iter.Next()))) {

      // We only care about final-state particles (status 1)
      if (gpart->Status() != 1) continue;

      // Find out the particle id using the PDG code
      G4int pdgcode = gpart->Pdg();
      G4ParticleDefinition* gpart_def =
        G4ParticleTable::GetParticleTable()->FindParticle(pdgcode);

      // Skip the particle if we its PDG code was not recognized
      // (probably a GENIE unphysical particle)
      if (!gpart_def) continue;

      // Get the position and time of the current GHEP particle 
      // (the common position is in meters; the relative one, in fermis)
      G4LorentzVector gpart_xyzt;
      gpart_xyzt.setX(gpart->Vx()*fermi + xyzt->X()*meter);
      gpart_xyzt.setY(gpart->Vy()*fermi + xyzt->Y()*meter);
      gpart_xyzt.setZ(gpart->Vz()*fermi + xyzt->Z()*meter);
      gpart_xyzt.setT(gpart->Vt() + time_offset);

      // Check if there is already a vertex corresponding to that
      // position and time. If not, create it.
      G4PrimaryVertex* vertex = 0;
      auto result = vertex_map.find(gpart_xyzt);
      if (result == vertex_map.end()) {
        // It's not in the map yet
        vertex = new G4PrimaryVertex(gpart_xyzt.x(), 
                                     gpart_xyzt.y(), 
                                     gpart_xyzt.z(),
                                     gpart_xyzt.t());
        vertex_map[gpart_xyzt] = vertex;
        event->AddPrimaryVertex(vertex);
      }
      else {
        vertex = (*result).second;
      }

    // Create a Geant4 primary particle
    G4PrimaryParticle* particle = new G4PrimaryParticle(gpart_def,
                                                        gpart->Px()*GeV,
                                                        gpart->Py()*GeV,
                                                        gpart->Pz()*GeV);

    // Add more info to the Geant4 particle
    particle->SetCharge(gpart->Charge());
    if (gpart->PolzIsSet()) {
      TVector3 polz;
      gpart->GetPolarization(polz);
      particle->SetPolarization(polz.x(),
                                polz.y(),
                                polz.z());
    }

    // Add the particle to the vertex
    vertex->SetPrimary(particle);
    }

    if (current_entry_ == num_entries_) current_entry_ = 0;
  }
}
