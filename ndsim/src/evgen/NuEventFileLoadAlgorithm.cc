//____________________________________________________________________________
/*!

\class    NuEventFileLoadAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Oct 2015
*/
//____________________________________________________________________________
#include "NuEventFileLoadAlgorithm.hh"
#include "GasTPCException.hh"

NuEventFileLoadAlgorithm::NuEventFileLoadAlgorithm()
  : entryCount_(0), nuHits_nEntries(0), nuEvents_nEntries(0){
}

NuEventFileLoadAlgorithm::~NuEventFileLoadAlgorithm(){
  delete mcrec;
  delete thdr;
}

void NuEventFileLoadAlgorithm::initialize(std::string inputFileName, TTree* inputTree, std::string inputGeom) {
  inputTree_ = inputTree;
  this->initialize(inputFileName,inputTree_->GetName(),inputGeom);
}

void NuEventFileLoadAlgorithm::initialize(std::string inputFileName, std::string inputTreeName, std::string inputGeom) {

  inputFileName_ = inputFileName;
  inputTreeName_ = inputTreeName;
  inputGeom_     = inputGeom;

  //load the file to read
  if(inputFileName_.find("particlegun") != std::string::npos){
    cout << "-------INFO:: This is a ParticleGun simulation -------" << endl;
  }
  else{
    inputFile_ = new TFile(inputFileName_.c_str());
    if (!inputFile_->IsOpen()) {
      string mesage = "File "+inputFileName_+" not found.";
      string location = "NuEventFileLoadAlgorithm::initialize()";
      throw GasTPCException(mesage, location, GasTPCException::FATAL);
    }
    
    inputFile_->GetObject(inputTreeName_.c_str(), inputTree_);
    if ( !inputTree_ ) {
      string mesage = "TTree "+inputTreeName_+" not found in file "+inputFileName_+".";
      string location = "NuEventFileLoadAlgorithm::initialize()";
      throw GasTPCException(mesage, location, GasTPCException::FATAL);
    }
  }
  //this->setDataAddress<NeutrinoEvent>(nuEvent_, inputTree_);
  nuEvents_nEntries = inputTree_->GetEntries();

  if(inputTreeName_ == "gst"){
    cout << "-------INFO:: Initialize GENIE gst tree -------" << endl;
    
    inputTree_->SetBranchAddress("neu",     &neu);
    inputTree_->SetBranchAddress("tgt",     &tgt);
    inputTree_->SetBranchAddress("fspl",    &fspl);
    inputTree_->SetBranchAddress("nf",      &nf);
    inputTree_->SetBranchAddress("ni",      &ni);
    inputTree_->SetBranchAddress("resid",   &resid);
    inputTree_->SetBranchAddress("pdgf",    &pdgf);
    inputTree_->SetBranchAddress("pdgi",    &pdgi);
    inputTree_->SetBranchAddress("Ev",      &Ev);
    inputTree_->SetBranchAddress("El",      &El);
    inputTree_->SetBranchAddress("pxl",     &pxl);
    inputTree_->SetBranchAddress("pyl",     &pyl);
    inputTree_->SetBranchAddress("pzl",     &pzl);
    inputTree_->SetBranchAddress("pl",      &pl);
    inputTree_->SetBranchAddress("cthl",    &cthl);
    inputTree_->SetBranchAddress("En",      &En);
    inputTree_->SetBranchAddress("pxn",     &pxn);
    inputTree_->SetBranchAddress("pyn",     &pyn);
    inputTree_->SetBranchAddress("pzn",     &pzn);
    inputTree_->SetBranchAddress("vtxx",    &vtxx);
    inputTree_->SetBranchAddress("vtxy",    &vtxy);
    inputTree_->SetBranchAddress("vtxz",    &vtxz);
    inputTree_->SetBranchAddress("vtxt",    &vtxt);
    inputTree_->SetBranchAddress("pxf",     &pxf);
    inputTree_->SetBranchAddress("pyf",     &pyf);
    inputTree_->SetBranchAddress("pzf",     &pzf);
    inputTree_->SetBranchAddress("pf",      &pf);
    inputTree_->SetBranchAddress("Ef",      &Ef);
    inputTree_->SetBranchAddress("cthf",    &cthf);
    inputTree_->SetBranchAddress("pxi",     &pxi);
    inputTree_->SetBranchAddress("pyi",     &pyi);
    inputTree_->SetBranchAddress("pzi",     &pzi);
    inputTree_->SetBranchAddress("Ei",      &Ei);
    inputTree_->SetBranchAddress("cc",      &cc);
    inputTree_->SetBranchAddress("qel",     &qel);
    inputTree_->SetBranchAddress("sea",     &sea);
    inputTree_->SetBranchAddress("mec",     &mec);
    inputTree_->SetBranchAddress("res",     &res);
    inputTree_->SetBranchAddress("dis",     &dis);
    inputTree_->SetBranchAddress("coh",     &coh);
    inputTree_->SetBranchAddress("dfr",     &dfr);
    inputTree_->SetBranchAddress("imd",     &imd);
    inputTree_->SetBranchAddress("nuel",    &nuel);
    inputTree_->SetBranchAddress("wght",    &wght);
    inputTree_->SetBranchAddress("charm",   &charm);
    inputTree_->SetBranchAddress("x",       &x);
    inputTree_->SetBranchAddress("y",       &y);
    inputTree_->SetBranchAddress("Q2",      &Q2);
    inputTree_->SetBranchAddress("W",       &W);
    inputTree_->SetBranchAddress("neut_code", &neut_code);
  }
  else if(inputTreeName_ == "gtree"){
    cout << "-------INFO:: Initialize GENIE gtree -------" << endl;
    //er_tree = dynamic_cast <TTree *>           ( fin.Get("gtree")  );
    thdr    = dynamic_cast <NtpMCTreeHeader *> ( inputFile_->Get("header") );
    inputTree_->SetBranchAddress("gmcrec", &mcrec);
  }
  else if(inputTreeName_ == "pg"){
    cout << "-------INFO:: Initialize Particle Gun Tree -------" << endl;
    inputTree_->SetBranchAddress("Px",     &Px);
    inputTree_->SetBranchAddress("Py",     &Py);
    inputTree_->SetBranchAddress("Pz",     &Pz);
    inputTree_->SetBranchAddress("E",      &E);
    inputTree_->SetBranchAddress("X",      &X);
    inputTree_->SetBranchAddress("Y",      &Y);
    inputTree_->SetBranchAddress("Z",      &Z);
    inputTree_->SetBranchAddress("M",      &M);
    inputTree_->SetBranchAddress("pdg",    &pdg);
  }
}

void NuEventFileLoadAlgorithm::close() {
  inputTree_->Delete();
  inputFile_->Close();
  if(verbose_>0){
    std::cout << "\n--------------------------------------"
	      << "\nINFO::Closing " << inputFileName_ << " file"
	      << "\n--------------------------------------"
	      << std::endl;
  }
}
/*
NeutrinoEvent* NuEventFileLoadAlgorithm::getEvent(int event) {

  if(inputTree_->GetEntry(event)){
    inputTree_->GetEntry(event);
    if(verbose_>4){
      std::cout << "\n=================================="
		<< "\nEvent: " << event
		<< "\n=================================="
		<< std::endl;
    
      _nuEvent->printToStream(std::cout);
    }
    return _nuEvent;
  }
  else return NULL;
  
}
*/
NeutrinoEvent* NuEventFileLoadAlgorithm::getPGEvent(int eventid, int nspills) {
  NeutrinoEvent *_nuEvent = new NeutrinoEvent();
  inputTree_->GetEntry(eventid);

  _nuEvent->setEventID(eventid);
  _nuEvent->setScatteringType(-999);
  _nuEvent->setNeutrinoType(-999);
  _nuEvent->setNuEnergy(-999);
  _nuEvent->setx(-999);
  _nuEvent->sety(-999);
  _nuEvent->setW(-999);
  _nuEvent->setQ2(-999);
  _nuEvent->setNEUTCode(-999);
  _nuEvent->setRunID(0);
  _nuEvent->SetSpillNumber(nspills);

  TLorentzVector vertex(X*CLHEP::m,Y*CLHEP::m,Z*CLHEP::m,0);
  _nuEvent->setPosition(vertex);

  TLorentzVector p4(Px,Py,Pz,E);
  p4 *= CLHEP::GeV;
  
  ParticleDescrShortRecord fspl(pdg, p4);
  //M *= CLHEP::GeV;
  //fspl.setMass(M);
  //_nuEvent->setFspl(fspl);
  _nuEvent->addFss(fspl);

  //print to stream
  if(verbose_>1)_nuEvent->printToStream(std::cout);
  
  return _nuEvent;
}

NeutrinoEvent* NuEventFileLoadAlgorithm::getGstEvent(int eventid, int nspills) {

  //NeutrinoEvent* _nuEvent = new NeutrinoEvent();
  NeutrinoEvent*_nuEvent = new NeutrinoEvent();
  inputTree_->GetEntry(eventid);
  
  _nuEvent->setEventID(eventid);
  //_nuEvent->setQELCC(qel);
  _nuEvent->setScatteringType(cc);
  _nuEvent->setNeutrinoType(neu);
  //_nuEvent->setTopology(sea, qel, mec, res, dis, coh, dfr, imd, nuel, charm);
  //Ev = Ev*CLHEP::GeV;
  _nuEvent->setNuEnergy(Ev);
  _nuEvent->setx(x);
  _nuEvent->sety(y);
  _nuEvent->setW(W);
  _nuEvent->setQ2(Q2);
  //_nuEvent->setresid(resid);
  //_nuEvent->setweight(wght);
  _nuEvent->setNEUTCode(neut_code);

  TRandom3 rand;
  int seed = (int)(Ev+eventid);
  rand.SetSeed(seed);
  _nuEvent->SetSpillNumber(rand.Integer(nspills));
  seed = seed*seed;
  
  TLorentzVector vertex(vtxx*CLHEP::m,vtxy*CLHEP::m,vtxz*CLHEP::m,rand.Uniform(960));
  //vertex *= CLHEP::m;
  _nuEvent->setPosition(vertex);

  //clear previous fss-iss vector
  _nuEvent->clearFssVector();

  // Add lepton kinematics
  TLorentzVector fspl_p4(pxl,pyl,pzl,El);
  fspl_p4 *= CLHEP::GeV;
  int fspl_pdg = fspl;
  ParticleDescrShortRecord fspl(fspl_pdg, fspl_p4);
  //double plmass = sqrt(El*El-pl*pl);
  //plmass *= CLHEP::GeV;
  //fspl.setMass(plmass);
  //_nuEvent->setFspl(fspl);
  _nuEvent->addFss(fspl);

  // Add target nucleon kinematics
  _nuEvent->setNucleonPdg(tgt);
  /*
  TLorentzVector nuc_p4(pxn,pyn,pzn,En);
  nuc_p4 *= CLHEP::GeV;
  int nuc_pdg = tgt;
  ParticleDescrShortRecord nuc(nuc_pdg, nuc_p4);
  _nuEvent->setHitNucleon(nuc);
  */

  // Add final state products
  for(int k=0; k<nf; k++){
    int pdg = pdgf[k];
    TLorentzVector p4(pxf[k],pyf[k],pzf[k],Ef[k]);
    p4 *= CLHEP::GeV;
    ParticleDescrShortRecord p(pdg,p4);
    //double pfmass = sqrt(Ef[k]*Ef[k]-pf[k]*pf[k]);
    //pfmass *= CLHEP::GeV;
    //p.setMass(pfmass);
    _nuEvent->addFss(p);
  }

  // Add initial state products
  for(int k=0; k<ni; k++){
    int pdg = pdgi[k];
    TLorentzVector p4(pxi[k],pyi[k],pzi[k],Ei[k]);
    p4 *= CLHEP::GeV;
    ParticleDescrShortRecord p(pdg,p4);
    //double pi = sqrt(pxi[k]*pxi[k]+pyi[k]*pyi[k]+pzi[k]*pzi[k]);
    //double pimass = sqrt(Ei[k]*Ei[k]-pi*pi);
    //pimass *= CLHEP::GeV;
    //p.setMass(pimass);
    _nuEvent->addIss(p);
  }
  
  //print to stream
  if(verbose_>1)_nuEvent->printToStream(std::cout);
  
  return _nuEvent;
}

EventRecord* NuEventFileLoadAlgorithm::getNtpMCEventRecord(int eventid){
  inputTree_->GetEntry(eventid);

  return mcrec->event;
}

NeutrinoEvent* NuEventFileLoadAlgorithm::getGHepEvent(int eventid, int nspills) {

  NeutrinoEvent* _nuEvent = new NeutrinoEvent();
  inputTree_->GetEntry(eventid);

  NtpMCRecHeader rec_header = mcrec->hdr;
  //EventRecord &  eventr      = *(mcrec->event);
  EventRecord  eventr      = *(mcrec->event);

  TLorentzVector pdummy(0,0,0,0);

  // Go further only if the event is physical
  if(eventr.IsUnphysical()){
    cout << "Skipping unphysical event" << endl;
    mcrec->Clear();
    return NULL;
  }

  //input particles
  GHepParticle * neutrino = eventr.Probe();
  assert(neutrino);
  GHepParticle * target = eventr.Particle(1);
  assert(target);
  GHepParticle * fsl = eventr.FinalStatePrimaryLepton();
  assert(fsl);
  GHepParticle * hitnucl = eventr.HitNucleon();

  // Summary info
  const Interaction * interaction = eventr.Summary();
  const InitialState & init_state = interaction->InitState();
  const ProcessInfo &  proc_info  = interaction->ProcInfo();
  const Kinematics &   kine       = interaction->Kine();
  const XclsTag &      xcls       = interaction->ExclTag();
  const Target &       tgt        = init_state.Tgt();
  
  // Vertex in detector coord system
  TLorentzVector * vtx = eventr.Vertex();

  // Process id
  bool is_qel    = proc_info.IsQuasiElastic();
  bool is_res    = proc_info.IsResonant();
  bool is_dis    = proc_info.IsDeepInelastic();
  bool is_coh    = proc_info.IsCoherent();
  bool is_dfr    = proc_info.IsDiffractive();
  bool is_imd    = proc_info.IsInverseMuDecay();
  bool is_imdanh = proc_info.IsIMDAnnihilation();
  bool is_nuel   = proc_info.IsNuElectronElastic();
  bool is_em     = proc_info.IsEM();
  bool is_weakcc = proc_info.IsWeakCC();
  bool is_weaknc = proc_info.IsWeakNC();
  bool is_mec    = proc_info.IsMEC();
  
  if(!hitnucl) { assert(is_coh || is_imd || is_imdanh || is_nuel); }
  
  // Hit quark - set only for DIS events
  int  qrk  = (is_dis) ? tgt.HitQrkPdg() : 0;     
  bool seaq = (is_dis) ? tgt.HitSeaQrk() : false; 
  
  // Resonance id ($GENIE/src/BaryonResonance/BaryonResonance.h) -
  // set only for resonance neutrinoproduction
  resid = (is_res) ? EResonance(xcls.Resonance()) : -99;
  
  // (qel or dis) charm production?
  charm = xcls.IsCharmEvent();
  
  // Get NEUT equivalent reaction codes (if any)
  neut_code   = utils::ghep::NeutReactionCode(&eventr);

  // Special reaction codes
  if(is_imd){
    neut_code = 80;
    if(neutrino->Pdg() < 0)
      neut_code = -neut_code;
  }
  else if(is_nuel){
    neut_code = 81;
    if(neutrino->Pdg() < 0)
      neut_code = -neut_code;
  }
  else if(is_mec){ // This should already exist, but add just in case
   neut_code = 2;
    if(neutrino->Pdg() < 0)
      neut_code = -neut_code; 
  }
  
  // Get event weight
  wght = eventr.Weight();

  const TLorentzVector & k1 = *(neutrino->P4());                     // v 4-p (k1)
  const TLorentzVector & k2 = *(fsl->P4());                          // l 4-p (k2)
  const TLorentzVector & p1 = (hitnucl) ? *(hitnucl->P4()) : pdummy; // N 4-p (p1)      
  
  double M  = genie::constants::kNucleonMass; 
  TLorentzVector q  = k1-k2;                     // q=k1-k2, 4-p transfer
  Q2 = -1 * q.M2();                       // momemtum transfer
  double v  = (hitnucl) ? q.Energy()       : -1; // v (E transfer to the nucleus)
  x  = (hitnucl) ? 0.5*Q2/(M*v)     : -1; // Bjorken x
  y  = (hitnucl) ? v/k1.Energy()    : -1; // Inelasticity, y = q*P1/k1*P1
  double W2 = (hitnucl) ? M*M + 2*M*v - Q2 : -1; // Hadronic Invariant mass ^ 2
  W  = (hitnucl) ? TMath::Sqrt(W2)  : -1; 
  double t  = 0;
  
  // Get v 4-p at hit nucleon rest-frame
  TLorentzVector k1_rf = k1;         
  if(hitnucl) {
    k1_rf.Boost(-1.*p1.BoostVector());
  }

  _nuEvent->setEventID(eventid);
  //_nuEvent->setQELCC(is_qel);
  _nuEvent->setScatteringType(is_weakcc);
  _nuEvent->setNeutrinoType(neutrino->Pdg());
  //_nuEvent->setTopology(seaq, is_qel, is_mec, is_res, is_dis, is_coh, is_dfr, is_imd, is_nuel, charm);
  Ev = k1.Energy()*CLHEP::GeV;
  _nuEvent->setNuEnergy(Ev);
  _nuEvent->setx(x);
  _nuEvent->sety(y);
  _nuEvent->setW(W);
  _nuEvent->setQ2(Q2);
  //_nuEvent->setresid(resid);
  // _nuEvent->setweight(wght);
  _nuEvent->setNEUTCode(neut_code);

  TRandom3 rand;
  int seed = (int)(Ev+eventid);
  rand.SetSeed(seed);
  _nuEvent->SetSpillNumber(rand.Integer(nspills));
  seed = seed*seed;
  rand.SetSeed(seed);
  
  TLorentzVector vertex(vtx->X()*CLHEP::m,vtx->Y()*CLHEP::m,vtx->Z()*CLHEP::m,rand.Uniform(960));
  //vertex *= CLHEP::m;
  _nuEvent->setPosition(vertex);
  _nuEvent->setRunID(thdr->runnu);

  //clear previous fss-iss vector
  _nuEvent->clearFssVector();
  
  // Add lepton kinematics
  TLorentzVector fspl_p4(k2.Px(),k2.Py(),k2.Pz(),k2.Energy());
  fspl_p4 *= CLHEP::GeV;
  int fspl_pdg = fsl->Pdg();
  ParticleDescrShortRecord fspl(fspl_pdg, fspl_p4);
  //double plmass = sqrt(k2.Energy()*k2.Energy()-k2.P()*k2.P());
  //plmass *= CLHEP::GeV;
  //fspl.setMass(plmass);
  //_nuEvent->setFspl(fspl);
  _nuEvent->addFss(fspl);

  // Add target nucleon kinematics
  pxn = (hitnucl) ? p1.Px()     : 0;
  pyn = (hitnucl) ? p1.Py()     : 0;
  pzn = (hitnucl) ? p1.Pz()     : 0;
  En  = (hitnucl) ? p1.Energy() : 0;
  /*
  TLorentzVector nuc_p4(pxn,pyn,pzn,En);
  nuc_p4 *= CLHEP::GeV;
  int nuc_pdg = target->Pdg();
  ParticleDescrShortRecord nuc(nuc_pdg, nuc_p4);
  _nuEvent->setHitNucleon(nuc);
  */
  _nuEvent->setNucleonPdg(target->Pdg());

  // Extract more info on the hadronic system
  // Only for QEL/RES/DIS/COH/MEC events
  bool study_hadsyst = (is_qel || is_res || is_dis || is_coh || is_mec);
  
  TObjArrayIter piter(&eventr);
  GHepParticle * p = 0;
  int ip=-1;
  // Extract the final state system originating from the hadronic vertex 
  // (after the intranuclear rescattering step)
  vector<int> final_had_syst;
  while( (p = (GHepParticle *) piter.Next()) && study_hadsyst)
    {
      ip++;
      // don't count final state lepton as part hadronic system 
      //if(!is_coh && event.Particle(ip)->FirstMother()==0) continue;
      if(eventr.Particle(ip)->FirstMother()==0) continue;
      if(pdg::IsPseudoParticle(p->Pdg())) continue;
      int pdgc = p->Pdg();
      int ist  = p->Status();
      if(ist==kIStStableFinalState) {
         if (pdgc == kPdgGamma || pdgc == kPdgElectron || pdgc == kPdgPositron)  {
            int igmom = p->FirstMother();
            if(igmom!=-1) {
	      // only count e+'s e-'s or gammas not from decay of pi0
	      if(eventr.Particle(igmom)->Pdg() != kPdgPi0) { final_had_syst.push_back(ip); }
            }
         } else {
            final_had_syst.push_back(ip);
         }
      }
      // now add pi0's that were decayed as short lived particles
      else if(pdgc == kPdgPi0){
	int ifd = p->FirstDaughter();
	int fd_pdgc = eventr.Particle(ifd)->Pdg();
	// just require that first daughter is one of gamma, e+ or e-  
	if(fd_pdgc == kPdgGamma || fd_pdgc == kPdgElectron || fd_pdgc == kPdgPositron){
	  final_had_syst.push_back(ip);
	}
      }
    }//particle-loop
  
  // Add final state products
  for(int k=0; k<final_had_syst.size(); k++){
    p = eventr.Particle(final_had_syst[k]);
    assert(p);

    int pdg = p->Pdg();
    TLorentzVector p4(p->Px(),p->Py(),p->Pz(),p->Energy());
    p4 *= CLHEP::GeV;
    ParticleDescrShortRecord pdr(pdg,p4);
    //double pfmass = p->Mass();
    //pfmass *= CLHEP::GeV;
    //pdr.setMass(pfmass);
    _nuEvent->addFss(pdr);
  }

  // Add initial state products
  ip = -1;
  TObjArrayIter piter_prim(&eventr);
  vector<int> prim_had_syst;
  if(study_hadsyst) {
    // if coherent or free nucleon target set primary states equal to final states
    if(!pdg::IsIon(target->Pdg()) || (is_coh)) {
      vector<int>::const_iterator hiter = final_had_syst.begin();
      for( ; hiter != final_had_syst.end(); ++hiter) {
	prim_had_syst.push_back(*hiter);
      }
    } 
    // otherwise loop over all particles and store indices of those which are hadrons
    // created within the nucleus
    else {
      while( (p = (GHepParticle *) piter_prim.Next()) ){
	ip++;      
	int ist_comp  = p->Status();
	if(ist_comp==kIStHadronInTheNucleus) {
	  prim_had_syst.push_back(ip); 
	}
      }//particle-loop   
      //
      // also include gammas from nuclear de-excitations (appearing in the daughter list of the 
      // hit nucleus, earlier than the primary hadronic system extracted above)
      for(int i = target->FirstDaughter(); i <= target->LastDaughter(); i++) {
	if(i<0) continue;
	if(eventr.Particle(i)->Status()==kIStStableFinalState) { prim_had_syst.push_back(i); }
      }      
    }//freenuc?
  }//study_hadsystem?
  
  for(int k=0; k<prim_had_syst.size(); k++){
    p = eventr.Particle(prim_had_syst[k]);
    assert(p);
    
    int pdg = p->Pdg();
    TLorentzVector p4(p->Px(),p->Py(),p->Pz(),p->Energy());
    p4 *= CLHEP::GeV;
    ParticleDescrShortRecord pdr(pdg,p4);
    //double pi = sqrt(p->Px()*p->Px()+p->Py()*p->Py()+p->Pz()*p->Pz());
    //double pimass = sqrt(p->Energy()*p->Energy()-pi*pi);
    //double pimass = p->Mass();
    //pimass *= CLHEP::GeV;
    //pdr.setMass(pimass);
    _nuEvent->addIss(pdr);
  }

  //print to stream
  if(verbose_>1)_nuEvent->printToStream(std::cout);

  return _nuEvent;
}

bool NuEventFileLoadAlgorithm::getNextEvent(){
  
  if(entryCount_>=nuEvents_nEntries)return false;
  
  if(inputTree_->GetEntry(entryCount_)){
    entryCount_++;
    return true;
  }
  else return false;
}

