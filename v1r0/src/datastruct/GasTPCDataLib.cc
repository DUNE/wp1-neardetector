//____________________________________________________________________________
/*!

\class    GasTPCDataLib

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Apr 2015

*/
//____________________________________________________________________________

#include "GasTPCDataLib.hh"

ClassImp(ParticleDescrShortRecord)
ClassImp(ParticleDescrRecord)
ClassImp(BaseEventRecord)
ClassImp(NeutrinoHit)
ClassImp(SimulData)
ClassImp(SDHit)
ClassImp(tpcFidHit)
ClassImp(scintHit)
ClassImp(MINDHit)
ClassImp(GeantBasicParticle)
ClassImp(GeantParticle)
ClassImp(GeantDaughterParticle)
ClassImp(PionDecayEvent)
ClassImp(MuonDecayEvent)
ClassImp(ReconData)
ClassImp(GeomVolumeRecord)

///////////////////////////////////////////////////////////////////////////////

ParticleDescrShortRecord::ParticleDescrShortRecord()
: pdg_(0), p4_(0.,0.,0.,0.),mass_(0.),charge_(0) {}

ParticleDescrShortRecord::ParticleDescrShortRecord(int pdg, TLorentzVector p4)
: pdg_(pdg), p4_(p4),mass_(0.),charge_(0) {}

ParticleDescrShortRecord::ParticleDescrShortRecord(const ParticleDescrShortRecord &p)
  : pdg_( p.getPDG() ), p4_( p.getP4() ),mass_( p.getMass() ),charge_( p.getCharge() ) {}

ParticleDescrShortRecord& ParticleDescrShortRecord::operator=(const ParticleDescrShortRecord &p) {
  pdg_   = p.getPDG();
  p4_    = p.getP4();
  mass_  = p.getMass();
  charge_= p.getCharge();
  return *this;
}

///////////////////////////////////////////////////////////////////////////////

ParticleDescrRecord::ParticleDescrRecord()
: ParticleDescrShortRecord::ParticleDescrShortRecord(int(0), TLorentzVector(0.,0.,0.,0.)),
  pos_(0.,0.,0.,0.) {}

ParticleDescrRecord::ParticleDescrRecord(int pdg, TLorentzVector p4, TLorentzVector pos)
: ParticleDescrShortRecord::ParticleDescrShortRecord(pdg, p4), pos_(pos) {}

ParticleDescrRecord::ParticleDescrRecord(const ParticleDescrRecord &p)
: ParticleDescrShortRecord::ParticleDescrShortRecord(p),
  pos_( p.getPosition() ) {}

ParticleDescrRecord& ParticleDescrRecord::operator=(const ParticleDescrRecord &p) {
  pdg_ = p.getPDG();
  p4_  = p.getP4();
  pos_ = p.getPosition();
  return *this;
}
///////////////////////////////////////////////////////////////////////////////

void ParticleDescrRecord::printToStream(ostream& stream) {
  ParticleDescrShortRecord::printToStream(stream);

  TLorentzVector pos = getPosition() * (1./CLHEP::m);
  stream << " pos(x,y,z,t)[m] : (" << pos.X() << ", " 
  << pos.Y() << ", " << pos.Z() << ", " << pos.T() << ")"
  << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

void ParticleDescrShortRecord::printToStream(ostream& stream) {
  TLorentzVector p4 = getP4() * (1./CLHEP::GeV);
  stream << " pdg : " << getPDG() <<std::endl;
  stream << " p4(Px,Py,Pz,E)[GeV] : (" << p4.X() << ", " 
  << p4.Y() << ", " << p4.Z() << ", " << p4.T() << ")"
  << std::endl;
}
///////////////////////////////////////////////////////////////////////////////

void BaseEventRecord::printToStream(ostream& stream) {
  TLorentzVector pos = getPosition() * (1./CLHEP::m);
  stream << "\n pos(x,y,z,t)[m] : (" << pos.X() << ", " 
  << pos.Y() << ", " << pos.Z() << ", " << pos.T() << ")"
  << std::endl;
}
///////////////////////////////////////////////////////////////////////////////

NeutrinoHit::NeutrinoHit(ParticleDescrRecord nu)
: neutrino_(nu) {name_ = "nuHits";}

void NeutrinoHit::printToStream(ostream& stream) {
  stream << "------ NeutrinoHit ------" << std::endl;

  neutrino_.printToStream(stream);

  stream << std::endl;
}
///////////////////////////////////////////////////////////////////////////////

PionDecayEvent::PionDecayEvent(ParticleDescrShortRecord nu,
                               ParticleDescrShortRecord chLepton,
                               TLorentzVector pos)
: neutrino_(nu), chargedLepton_(chLepton),
  BaseEventRecord::BaseEventRecord(pos) {name_ = "piDecays";}

void PionDecayEvent::printToStream(ostream& stream) {
  stream << "----- PionDecayEvent ----" << std::endl;

  if(inFlight()) { stream << " decay in flight" << std::endl;}
  else { stream << " decay at rest" << std::endl;}

  BaseEventRecord::printToStream(stream);

  stream << " neutrino :" << std::endl;
  neutrino_.printToStream(stream);

  //stream << " charged lepton :" << std::endl;
  //chargedLepton_.printToStream(stream);

  stream << std::endl;
}
///////////////////////////////////////////////////////////////////////////////

MuonDecayEvent::MuonDecayEvent(ParticleDescrShortRecord numu,
                               ParticleDescrShortRecord nue,
                               ParticleDescrShortRecord electron,
                               TLorentzVector pos)
: numu_(numu), nue_(nue), electron_(electron),
  BaseEventRecord::BaseEventRecord(pos) {name_ = "muDecays";
}
///////////////////////////////////////////////////////////////////////////////

void NeutrinoEvent::printToStream(ostream& stream) {
  
  TLorentzVector eventVertex = this->getPosition()*(1./CLHEP::mm);

  stream << "\n=================================================================="
	 << "\n------------------------ NeutrinoEvent ---------------------------"
	 << "\n Neutrino Truth Energy [GeV]: " << this->getNuEnergy()*1./CLHEP::GeV
	 << "\n Neutrino Vertex (x,y,z)[mm]: (" << eventVertex.X() <<","<<eventVertex.Y() <<","<<eventVertex.Z() <<")"
	 << "\n=================================================================="
	 << "\n Fspl: ";
	 fspl_.printToStream(stream);
  stream << "=================================================================="
	 << "\n Primaries: " << fss_.size()
	 << "\n==================================================================" <<std::endl;

  for(int i=0;i<fss_.size();i++){
	fss_.at(i).printToStream(stream);
  }
  stream << "==================================================================" <<std::endl;
}
////////////////////////////////////////////////////////////////////////////

void SDHit::printToStream(ostream& stream) {
  stream << "----- " << this->getRecordName() << " -----" << std::endl;
  BaseEventRecord::printToStream(stream);

  stream << " Edep :" << Edep_*(1./CLHEP::GeV) << " [GeV]" << std::endl;
  stream << std::endl;
}

////////////////////////////////////////////////////////////////////////////

void tpcFidHit::printToStream(ostream& stream) {
  stream << "----- " << this->getRecordName() << " -----" << std::endl;
  BaseEventRecord::printToStream(stream);
  stream << " Edep :" << Edep_*(1./CLHEP::GeV) << " [GeV]" << std::endl;
  stream << std::endl;
}
////////////////////////////////////////////////////////////////////////////

void scintHit::printToStream(ostream& stream) {
  stream << "\n----- " << this->getRecordName() << " -----" << std::endl;
  stream << "*** pdg : " << PDG; 
  BaseEventRecord::printToStream(stream);
  stream << " Edep :" << Edep_*(1./CLHEP::GeV) << " [GeV]" << std::endl;
  stream << std::endl;
}
////////////////////////////////////////////////////////////////////////////

void GeantBasicParticle::printToStream(ostream& stream) const {
  stream << "\n --- PDG : " << this->getPDG() << ", TrackID: " << this->getTrackID() << ", ParentID: " << this->getParentID() <<std::endl;
}
////////////////////////////////////////////////////////////////////////////

void GeantParticle::printToStream(ostream& stream) {
  stream << "\n pdg : " << getPDG() << ", trackID: " << trackID << ", parentID: " << parentID <<std::endl;

  TLorentzVector p4 = getP4() * (1./CLHEP::GeV);
  stream << " p4(Px,Py,Pz,E)[GeV] : (" << p4.X() << ", " 
	 << p4.Y() << ", " << p4.Z() << ", " << p4.T() << ")"
	 << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

bool GeantDaughterParticle::isPrimary() const{

  if(primaryParent.getTrackID() == this->getTrackID())primary=true;
  else primary = false;

  return primary;
}

///////////////////////////////////////////////////////////////////////////////
void MINDHit::printToStream(ostream& stream) {
  stream << "----- " << this->getRecordName() << " -----" << std::endl;
  BaseEventRecord::printToStream(stream);
  //stream << " muon :" << std::endl;
  //muon_.printToStream(stream);
  stream << std::endl;
}

////////////////////////////////////////////////////////////////////////////

double SimulData::getTotalEDep(std::string hits_name) const{

  double edep = 0.;

  if(hits_name == "scint"){
    for(int i=0;i<scint_hits_.size();i++){
      edep+=scint_hits_.at(i).getEdep();
    }
  }
  else if(hits_name == "tpc"){
    for(int i=0;i<tpc_hits_.size();i++){
      edep+=tpc_hits_.at(i).getEdep();
    }
  }

  return edep;
}
////////////////////////////////////////////////////////////////////////////

void SimulData::printToStream(ostream& stream) {

  stream << "\n----- " << this->getRecordName() << " -----" ;
  stream << "\n * TPC Hits: " << tpc_hits_.size()  << ", edep [GeV]: "<< this->getTotalEDep("tpc");
  stream << "\n * Scint Hits: " << scint_hits_.size() << ", edep [GeV]: "<< this->getTotalEDep("scint");
  stream << "\n--------------------" << std::endl;
}
////////////////////////////////////////////////////////////////////////////

void MuonRange::printToStream(ostream& stream) {
  stream << "----- MuonRange -----" << std::endl;
  BaseEventRecord::printToStream(stream);
  stream << " range : " << range_*(1./CLHEP::m) << " [m]" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

ReconData::ReconData() : id_(0), truthMom_(0.),reconMom_(0.),ratio_(0.) {

  name_ = "reconData";
}

ReconData::ReconData(int id, double ratio) : id_(id), ratio_(ratio) {
  
  truthMom_ = 0.;
  reconMom_ = 0.;
}

ReconData::ReconData(int id, double truth,double recon) : id_(id), truthMom_(truth),reconMom_(recon) {

  ratio_ = reconMom_/truthMom_;
}

ReconData& ReconData::operator=(const ReconData &r) {
  id_ = r.id_;
  truthMom_  = r.truthMom_;
  reconMom_  = r.reconMom_;
  ratio_ = reconMom_/truthMom_;

  return *this;
}

void ReconData::printToStream(ostream& stream) {
  stream << "\n\t Geant id : " << getID() << ", truth mom [GeV/c]: "<< getTruth() << ", recon mom [GeV/c]: " << getRecon();
}

///////////////////////////////////////////////////////////////////////////////
void GeomVolumeRecord::printToStream(ostream& stream){
  std::cout << "\n****************************************"
	    <<"\nVolume Counts"
	    << "\n\tTpcFidVolume: " << tpcFidCount << " (" << std::setprecision(3) << tpcFidCount*100./totalCount << "%)"
	    << "\n\tVesselVolume: " << vesselCount << " (" << std::setprecision(3) << vesselCount*100./totalCount << "%)"
	    << "\n\tInnerVesselVolume: " << innerVesselCount << " (" << std::setprecision(3) << innerVesselCount*100./totalCount << "%)"
	    << "\n\tScintVolume: " << scintCount << " (" << std::setprecision(3) << scintCount*100./totalCount << "%)"
	    << "\n\tMagnetVolume: " << magnetCount << " (" << std::setprecision(3) << magnetCount*100./totalCount << "%)"
	    << "\n\tMindVolume: " << mindCount << " (" << std::setprecision(3) << mindCount*100./totalCount << "%)"
	    << "\n\tCavityVolume: " << cavityCount << " (" << std::setprecision(3) << cavityCount*100./totalCount << "%)"
    //	  << "\n\tMotherVolume: " << motherCount
	    << "\n\tNotActiveVolume: " << notActiveCount << " (" << std::setprecision(3) << notActiveCount*100./totalCount << "%)"
	    << "\n\tRockVolume: " << rockCount << " (" << std::setprecision(3) << rockCount*100./totalCount << "%)"
	    << "\n\tNo Volume found: " << otherCount << " (" << std::setprecision(3) << otherCount*100./totalCount << "%)"
	    << "\n****************************************\n";
  
}

void GeomVolumeRecord::findVolume(std::string nodeName){

  resetOrigin();
  
  totalCount++;
  if(nodeName.compare( 0,6, "tpcFid" ) ==0){
    tpcFidCount++;
    tpcInteraction = true;
  }
  else if(nodeName.compare( 0,7,"cathode") ==0){
    notActiveCount++;
    notActiveInteraction = true;
  }
  else if(nodeName.compare( 0,5,"anode") ==0){
    notActiveCount++;
    notActiveInteraction = true;
  }
  else if(nodeName.compare( 0,11, "innerVessel" ) ==0){
    innerVesselCount++;
    innerVesselInteraction = true;
  }
  else if(nodeName.compare( 0,6, "vessel" ) ==0){
    vesselCount++;
    vesselInteraction = true;
  }
  else if(nodeName.compare( 0,5, "scint" ) ==0){
    scintCount++;
    scintInteraction = true;
  }
  else if(nodeName.compare( 0,6, "mother" ) ==0){
    innerVesselCount++;
    innerVesselInteraction = true;
  }
  else if(nodeName.compare( 0,6, "magnet" ) ==0){
    magnetCount++;
    magnetInteraction = true;
  }
  else if(nodeName.compare( 0,4, "mind" ) ==0){
    mindCount++;
    mindInteraction = true;
  }
  else if(nodeName.compare( 0,6, "cavity" ) ==0){
    cavityCount++;
    cavityInteraction = true;
  }
  else if(nodeName.compare( 0,4, "rock" ) ==0){
    rockCount++;
    rockInteraction = true;
  }
  else{
    otherCount++;
    otherInteraction = true;
  }

}

void GeomVolumeRecord::addCounts(GeomVolumeRecord &record){

  tpcFidCount 	 	+= record.getTpcCount();
  notActiveCount 	+= record.getNotActiveTpcCount();
  vesselCount	 	+= record.getVesselCount();
  innerVesselCount 	+= record.getInnerVesselCount();
  scintCount 		+= record.getScintCount();
  magnetCount 		+= record.getMagnetCount();
  mindCount 		+= record.getMindCount();
  cavityCount 		+= record.getCavityCount();
  rockCount 		+= record.getRockCount();
  otherCount		+= record.getOtherCount();
  totalCount		+= record.getTotalCount();

}
