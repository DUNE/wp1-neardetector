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

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>



GHepReader::GHepReader(const G4String& path, G4double mean):
  BeamSpillSource(), ifiles_(0), mcrec_(0), max_entries_(0), current_entry_(-1)
{
  this->Initialize(path);
}


GHepReader::~GHepReader()
{}


void GHepReader::Initialize(const G4String& path)
{
  ifiles_ = new TChain("gtree");
  ifiles_->Add(path);
  ifiles_->SetBranchAddress("gmcrec", &mcrec_);

  max_entries_ = ifiles_->GetEntries();
}


void GHepReader::FillG4Event(G4Event*)
{
  G4cout << "GHepReader::FillG4Event()" << G4endl;

  current_entry_++;
  ifiles_->GetEntry(current_entry_);

  genie::EventRecord& gevtrec = *(mcrec_->event);

  genie::Interaction* inter = gevtrec.Summary();
  const genie::Target& target = inter->InitState().Tgt();

  G4cout << "Target Z = " << target.Z() << G4endl;


  /*
  // Decide how many interactions we are supposed to read from the file

  G4int num_interactions;

  genie::EventRecord* record;

  for (G4int i=0; i<num_interactions; i++) {

    TLorentzVector* vertex = record->Vertex();

    double spill_time = 
      fGlobalTimeOffset + fHelperRandom->Uniform()*fRandomTimeOffset;

    // add the particles from the interaction
    TIter partitr(record);
    genie::GHepParticle *part = 0;
 
    // GHepParticles return units of GeV/c for p.  the V_i are all in fermis
    // and are relative to the center of the struck nucleus.
    // add the vertex X/Y/Z to the V_i for status codes 0 and 1
    int trackid = 0;

    while ((part = dynamic_cast<genie::GHepParticle *>(partitr.Next()))) {

      // Check particle status

      // Get PDG code for the particle

      double vtx[4] = {part->Vx(), part->Vy(), part->Vz(), part->Vt()};
      vtx[0] = 100.*(part->Vx()*1.e-15 + vertex->X());
      vtx[1] = 100.*(part->Vy()*1.e-15 + vertex->Y());
      vtx[2] = 100.*(part->Vz()*1.e-15 + vertex->Z());
      vtx[3] = part->Vt() + spillTime;
    TLorentzVector pos(vtx[0], vtx[1], vtx[2], vtx[3]);
    TLorentzVector mom(part->Px(), part->Py(), part->Pz(), part->E());
    if(part->PolzIsSet()) {
        TVector3 polz;
        part->GetPolarization(polz);
        tpart.SetPolarization(polz);
      }
    }

     // Is this vertex already in our map?
    G4PrimaryVertex* vertex = 0;
    std::map< CLHEP::HepLorentzVector, G4PrimaryVertex* >::const_iterator result = vertexMap.find( fourpos );
    if ( result == vertexMap.end() ){
141 
          // No, it's not, so create a new vertex and add it to the
142 
          // map.
143 
          vertex = new G4PrimaryVertex(x, y, z, t);
144 
          vertexMap[ fourpos ] = vertex;
145 
146 
          // Add the vertex to the G4Event.
147 
          event->AddPrimaryVertex( vertex );
148 
        }
149 
        else{
150 
          // Yes, it is, so use the existing vertex.
151 
          vertex = (*result).second;
152 
        }
                // Get additional particle information.
155 
        TLorentzVector momentum = particle.Momentum(); // (px,py,pz,E)
156 
        TVector3 polarization = particle.Polarization();
157 
      
158 
        // Get the particle table if necessary.  (Note: we're
159 
        // doing this "late" because I'm not sure at what point
160 
        // the G4 particle table is initialized in the loading process.
161 
        if ( fParticleTable == 0 ){
162 
          fParticleTable = G4ParticleTable::GetParticleTable();
163 
        }
164 
165 
        if ( pdgCode > 1000000000 )
166 
          LOG_DEBUG("ConvertPrimaryToGeant4") << ": %%% Nuclear PDG code = " << pdgCode
167 
                                              << " (x,y,z,t)=(" << x
168 
                                              << "," << y
169 
                                              << "," << z
170 
                                              << "," << t << ")"
171 
                                              << " P=" << momentum.P()
172 
                                              << ", E=" << momentum.E();
173 
174 
        // Get Geant4's definition of the particle.
175 
        G4ParticleDefinition* particleDefinition;
176 
      
177 
        if(pdgCode==0){
178 
          particleDefinition = fParticleTable->FindParticle("opticalphoton");
179 
        }
180 
        else
181 
          particleDefinition = fParticleTable->FindParticle(pdgCode);
182 
183 
        // What if the PDG code is unknown?  This has been a known
184 
        // issue with GENIE.
185 
        if ( particleDefinition == 0 ){
186 
          LOG_DEBUG("ConvertPrimaryToGeant4") << ": %%% Code not found = " << pdgCode;
187 
          fUnknownPDG[ pdgCode ] += 1;
188 
          continue;
189 
        }
190 
      
191 
        // Create a Geant4 particle to add to the vertex.
192 
        G4PrimaryParticle* g4particle = new G4PrimaryParticle( particleDefinition,
193 
                                                               momentum.Px() * CLHEP::GeV,
194 
                                                               momentum.Py() * CLHEP::GeV,
195 
                                                               momentum.Pz() * CLHEP::GeV);
196 
197 

        // Add more particle information the Geant4 particle.
198 
        G4double charge = particleDefinition->GetPDGCharge();
199 
        g4particle->SetCharge( charge );
200 
        g4particle->SetPolarization( polarization.x(),
201 
                                     polarization.y(),
202 
                                     polarization.z() );
203 
      
204 
        // Add the particle to the vertex.
205 
        vertex->SetPrimary( g4particle );
206 
207 
        // Create a PrimaryParticleInformation object, and save
208 
        // the MCTruth pointer in it.  This will allow the
209 
        // ParticleActionList class to access MCTruth
210 
        // information during Geant4's tracking.
211 
        PrimaryParticleInformation* primaryParticleInfo = new PrimaryParticleInformation;
212 
        primaryParticleInfo->SetMCTruth( mct, index );
213 
          
214 
        // Save the PrimaryParticleInformation in the
215 
        // G4PrimaryParticle for access during tracking.
216 
        g4particle->SetUserInformation( primaryParticleInfo );
217 
218 


  }*/
}