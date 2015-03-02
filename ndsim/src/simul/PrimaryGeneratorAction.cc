//____________________________________________________________________________
/*!

\class    PrimaryGeneratorAction

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2014

*/
//____________________________________________________________________________
#include "PrimaryGeneratorAction.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction(PhysicsList* p)
: verboseMode(true), verbose_(0), phys(p), beamCount(0),primVertex_(NULL){

   particleGun = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {

  if(particleGun) delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  //if(primVertex_) delete primVertex_; // -- probably deleted by geant, gives seg fault if delete it here.
  primVertex_ = NULL;

  if(GunOn_ == 1) {
	particleGun->GeneratePrimaryVertex(anEvent);
	energy = particleGun->GetParticleEnergy();
  }

  else{

  //set the initial energy
  energy = event_->getNuEnergy();

  TLorentzVector vertexPos = event_->getPosition();
  primVertex_ = new G4PrimaryVertex(vertexPos.X(),
                                    vertexPos.Y(),
                                    vertexPos.Z(), 0.);
  
  //fspl
  ParticleDescrShortRecord fspl = event_->getFspl();
  AddParticleToVertex(primVertex_, &fspl);

  //add all fss
  std::vector<ParticleDescrShortRecord> fss = event_->getFssVector();
  std::vector<ParticleDescrShortRecord>::iterator itr;

  if(verbose_>0){
    std::cout << "\n\n================= Event " << beamCount << " Summary =================";
    std::cout << "\n---------------------------"
	    << "\nFinal State Primary Lepton "
	    << "\n---------------------------\n";

    fspl.printToStream(std::cout);

    std::cout << "\n--------------------------------"
	    << "\nNumber of fss particles is: "
	    << fss.size()
	    << "\n--------------------------------\n";
  }
  
  for(itr = fss.begin(); itr < fss.end(); ++itr) {
	if((*itr).getPDG() < 100000){
		AddParticleToVertex(primVertex_, &(*itr));
		if(verbose_>0)(*itr).printToStream(std::cout);
	}
  }

  anEvent->AddPrimaryVertex(primVertex_);

  //event_->printToStream(std::cout);
  //vertex->Print();
/*
    runMgr_ = ((RunManager*)G4RunManager::GetRunManager());
    ghepReader_ = runMgr_->getReader();
    //HitsFileWriter* writer = theRunManager->getWriter();

    const NtpMCEventRecord* ntpMCEvent = reader->popNtpMCEventRecord();
    if (!ntpMCEvent) {
        cout << "ERROR: PrimaryGeneratorAction::GeneratePrimaries(): reader->popNtpMCEventRecord() returned 0." << endl;
        exit(1);
    }
    const EventRecord* event = ntpMCEvent->event;

    TLorentzVector* v = event->Vertex();
    G4PrimaryVertex* vertex = new G4PrimaryVertex(v->X()*m, v->Y()*m, v->Z()*m, 0.);

    printProgress(reader->getPercentRead(), reader->getNextEventNo(), reader->getNumUncutEvents(), anEvent->GetEventID());

    if (verboseMode) {
        cout << reader->getNextEventNo() << "\n";
        cout << *event;
        cout << *(event->Summary());
    }

    // find fspl (nu elastic scattering on electron has nu set as fspl => need to find e-)
    int fspl_index = event->FinalStatePrimaryLeptonPosition();
    if (event->Summary()->ProcInfo().IsNuElectronElastic()) {
        fspl_index = event->ParticlePosition(11, kIStStableFinalState, 0);
    }
    GHepParticle* fspl = event->Particle(fspl_index);
    // Fire final state primary lepton (it gets TrackID = 1)
    AddParticleToVertex(vertex, fspl);

    // Fire other final state particles
    int nParticles = event->GetEntries();
    for (int i = 0; i < nParticles; i++) {
        // skip fspl
        if (i == fspl_index) continue;

        GHepParticle *p = event->Particle(i);
        if ((p->Status() == kIStStableFinalState) && (p->Pdg() < 2000000000) &&
            (abs(p->Pdg()) != 12) && (abs(p->Pdg()) != 14) && (abs(p->Pdg()) != 16)) {

            // create new primaries and set them to the vertex
            if (!phys->IsDeffined(p->Pdg()) && (p->Pdg() < 1000000000)) {
                cout << "### Warning " << p->Name() << " - " << p->Pdg() << " is not defined ###" << endl;
            }
            AddParticleToVertex(vertex, p);
        }
    }

    //vertex->Print();
    anEvent->AddPrimaryVertex(vertex);

    // Write genie event data to the G4EventRecord
    //writer->setEvent(event, fspl_index);
*/
  }
 
  beamCount++;
 
}
void PrimaryGeneratorAction::AddParticleToVertex(G4PrimaryVertex* v,
                                                 ParticleDescrShortRecord* p) {
  G4PrimaryParticle* primaryParticle;
  int pdg = p->getPDG();
  TLorentzVector p4 = p->getP4();
  primaryParticle = new G4PrimaryParticle(pdg);
  primaryParticle->Set4Momentum(p4.Px(), p4.Py(), p4.Pz(), p4.E());

  /* else {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->GetIon(6,12,0,0);
    primaryParticle = new G4PrimaryParticle(particle);
    primaryParticle->SetMomentum(p->Px(), p->Py(), p->Pz());
    primaryParticle->SetMass(particle->GetPDGMass());
  }

    primaryParticle->SetCharge(p->Charge()*(eplus/3));
    TVector3 polz;
    p->GetPolarization(polz);
    primaryParticle->SetPolarization(polz.X(), polz.Y(), polz.Z());*/
    v->SetPrimary(primaryParticle);

    //delete primaryParticle;
}

void PrimaryGeneratorAction::printProgress(double percent, int i, int n, int m) {
  static int lastPrintedPercent = -1;
  int p = (int)(percent);
  if (p > lastPrintedPercent) {
    int width = 1 + (int)log10((double)n);
    std::cout << "Completion: "       << std::setw(3)     << p << "%;    ";
    std::cout << "Events read: "      << std::setw(width) << i << "/" << n << ";    ";
    std::cout << "Events simulated: " << std::setw(width) << m << ";" << std::endl;
    lastPrintedPercent = p;
  }
}
