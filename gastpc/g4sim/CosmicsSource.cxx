// -------------------------------------------------------------------
/// \file   CosmicsSource.cxx
/// \brief
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 20 Aug 2016
// -------------------------------------------------------------------

#include "CosmicsSource.h"

#include "EventGenerationInfo.h"
#include "PrimaryParticleInfo.h"

#include <G4Event.hh>
#include <Randomize.hh>
#include <G4LorentzVector.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>

#include <TChain.h>
#include <TTree.h>

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>
#include <GHEP/GHepParticle.h>



CosmicsSource::CosmicsSource(const G4String& path):
  BeamSpillSource(),
  ghep_chain_(0), gmcrec_(0)
{
  this->Initialize(path);
}


CosmicsSource::~CosmicsSource()
{
  delete ghep_chain_;
}


void CosmicsSource::Initialize(const G4String& path)
{
  ghep_chain_ = new TChain("gtree");
  ghep_chain_->Add(path.data());
  ghep_chain_->SetBranchAddress("gmcrec", &gmcrec_);
}


void CosmicsSource::GeneratePrimaryVertices(G4Event* event)
{
  // Pick randomly a GHEP record from the file
  G4long min_entry = 0;
  G4long max_entry = ghep_chain_->GetEntries();
  G4long entry_num = G4RandFlat::shootInt(min_entry, max_entry);

  // Select t0 for cosmic window
  G4double cosmic_window_length = 5. * ms;
  G4double  spill_window_length = 9434.5 * ns;
  G4double t0 =
    G4RandFlat::shoot(0., cosmic_window_length - spill_window_length);

  // Fetch the event generation info class in which we'll store a copy
  // of the GHEP record
  EventGenerationInfo* eg_info =
    dynamic_cast<EventGenerationInfo*>(event->GetUserInformation());

  ghep_chain_->GetEntry(entry_num);
  genie::EventRecord* record = gmcrec_->event;

  // Copy the GHEP record into the event generation info object
  // (I use here the Copy() method because using the copy constructor
  // results at runtime in a segmentation violation.)
  genie::NtpMCEventRecord* gmcrec_copy = new genie::NtpMCEventRecord();
  gmcrec_copy->Copy(*gmcrec_);
  eg_info->AddEntry(gmcrec_copy);

  // Get the common vertex position and time for this interaction
  //TLorentzVector* xyzt = record->Vertex();

  // Loop through the particles in the GHEP record
  genie::GHepParticle* gpart = 0;
  TIter gpart_iter(record);
  while ((gpart = dynamic_cast<genie::GHepParticle*>(gpart_iter.Next()))) {

    // We only care about final-state particles (status 1)
    if (gpart->Status() != 1) continue;

    // Find out the particle id using the PDG code
    G4int pdgcode = gpart->Pdg();

    // Skip GENIE's pseudo particles
    if (pdgcode >= 2000000000) continue;

    G4ParticleDefinition* gpart_def =
      G4ParticleTable::GetParticleTable()->FindParticle(pdgcode);

    if (!gpart_def) continue; // Make sure the pointer is defined

    // Get the position and time of the current GHEP particle
    // (the common position is in meters; the relative one, in fermis).
    // If the time does not fall in the spill window, skip this particle.
    if ((gpart->Vt() < t0) ||
        (gpart->Vt() > (t0 + spill_window_length))) continue;

    G4LorentzVector gpart_xyzt;
    gpart_xyzt.setX(gpart->Vx() * fermi);
    gpart_xyzt.setY(gpart->Vy() * fermi);
    gpart_xyzt.setZ(gpart->Vz() * fermi);
    gpart_xyzt.setT(gpart->Vt() * ns);

    G4PrimaryVertex* vertex =
      new G4PrimaryVertex(gpart_xyzt.x(), gpart_xyzt.y(), gpart_xyzt.z(),
                          gpart_xyzt.t());
    event->AddPrimaryVertex(vertex);

    // Create a Geant4 primary particle
    G4PrimaryParticle* particle =
      new G4PrimaryParticle(gpart_def, gpart->Px() * GeV,
                                       gpart->Py() * GeV,
                                       gpart->Pz() * GeV);

    // Add a user info object to the primary particle so that we are able
    // to trace it back to the GHEP record that originated it
    PrimaryParticleInfo* pp_info = new PrimaryParticleInfo();
    pp_info->SetEventGenerationID((eg_info->GetEntries().size())-1);
    particle->SetUserInformation(pp_info);

    // Add some extra info to the Geant4 particle
    particle->SetCharge(gpart->Charge());
    if (gpart->PolzIsSet()) {
      TVector3 polz;
      gpart->GetPolarization(polz);
      particle->SetPolarization(polz.x(), polz.y(), polz.z());
    }

    // Add the particle to the vertex
    vertex->SetPrimary(particle);
  }
}
