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
\last update June 2016

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
//ClassImp(GeantDaughterParticle)
ClassImp(PionDecayEvent)
ClassImp(MuonDecayEvent)
ClassImp(TrackParticle)
ClassImp(EcalTrackParticle)

///////////////////////////////////////////////////////////////////////////////

ParticleDescrShortRecord::ParticleDescrShortRecord()
: pdg_(0), p4_(0.,0.,0.,0.),/*mass_(0.),*/charge_(0) {}

ParticleDescrShortRecord::ParticleDescrShortRecord(int pdg, TLorentzVector p4)
  : pdg_(pdg), p4_(p4),/*mass_(0.),*/charge_(0) {}

ParticleDescrShortRecord::ParticleDescrShortRecord(const ParticleDescrShortRecord &p)
  : pdg_( p.getPDG() ), p4_( p.getP4() ),/*mass_( p.getMass() ),*/charge_( p.getCharge() ) {}

ParticleDescrShortRecord& ParticleDescrShortRecord::operator=(const ParticleDescrShortRecord &p) {
  pdg_   = p.getPDG();
  p4_    = p.getP4();
  //mass_  = p.getMass();
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

void ParticleDescrRecord::printToStream(std::ostream& stream) {
  ParticleDescrShortRecord::printToStream(stream);

  TLorentzVector pos = getPosition() * (1./CLHEP::m);
  stream << " pos(x,y,z,t)[m] : (" << pos.X() << ", " 
  << pos.Y() << ", " << pos.Z() << ", " << pos.T() << ")"
  << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

void ParticleDescrShortRecord::printToStream(std::ostream& stream) {
  TLorentzVector p4 = getP4() * (1./CLHEP::GeV);
  stream << " pdg : " << getPDG() <<std::endl;
  stream << " p4(Px,Py,Pz,E)[GeV] : (" << p4.X() << ", " 
  << p4.Y() << ", " << p4.Z() << ", " << p4.T() << ")"
  << std::endl;
}
///////////////////////////////////////////////////////////////////////////////

void BaseEventRecord::printToStream(std::ostream& stream) {
  TLorentzVector pos = getPosition() * (1./CLHEP::m);
  stream << "\n pos(x,y,z,t)[m] : (" << pos.X() << ", " 
  << pos.Y() << ", " << pos.Z() << ", " << pos.T() << ")"
  << std::endl;
}
///////////////////////////////////////////////////////////////////////////////

NeutrinoHit::NeutrinoHit(ParticleDescrRecord nu)
: neutrino_(nu) {name_ = "nuHits";}

void NeutrinoHit::printToStream(std::ostream& stream) {
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

void PionDecayEvent::printToStream(std::ostream& stream) {
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

void NeutrinoEvent::printToStream(std::ostream& stream) {
  
  TLorentzVector eventVertex = this->getPosition()*(1./CLHEP::mm);

  stream << "\n=================================================================="
	 << "\n------------------------ NeutrinoEvent ---------------------------"
	 << "\n Neutrino Truth Energy [GeV]: " << this->getNuEnergy()*1./CLHEP::GeV
	 << "\n Neutrino Vertex (x,y,z)[mm]: (" << eventVertex.X() <<","<<eventVertex.Y() <<","<<eventVertex.Z() <<")"
	 << "\n=================================================================="
	 << "\n Fspl: ";
  //fspl_.printToStream(stream);
  stream << "=================================================================="
	 << "\n Primaries: " << fss_.size()
	 << "\n==================================================================" <<std::endl;

  for(int i=0;i<fss_.size();i++){
	fss_.at(i).printToStream(stream);
  }
  stream << "==================================================================" <<std::endl;
}
////////////////////////////////////////////////////////////////////////////

void SDHit::printToStream(std::ostream& stream) {
  stream << "----- " << this->getRecordName() << " -----" << std::endl;
  BaseEventRecord::printToStream(stream);

  stream << " Edep :" << Edep_*(1./CLHEP::GeV) << " [GeV]" << std::endl;
  stream << std::endl;
}

////////////////////////////////////////////////////////////////////////////

void tpcFidHit::printToStream(std::ostream& stream) {
  stream << "----- " << this->getRecordName() << " -----" << std::endl;
  BaseEventRecord::printToStream(stream);
  stream << " Edep :" << Edep_*(1./CLHEP::GeV) << " [GeV]" << std::endl;
  stream << std::endl;
}
////////////////////////////////////////////////////////////////////////////

void scintHit::printToStream(std::ostream& stream) {
  stream << "\n----- " << this->getRecordName() << " -----" << std::endl;
  stream << "*** pdg : " << PDG; 
  BaseEventRecord::printToStream(stream);
  stream << " Edep :" << Edep_*(1./CLHEP::GeV) << " [GeV]" << std::endl;
  stream << std::endl;
}
////////////////////////////////////////////////////////////////////////////

void GeantBasicParticle::printToStream(std::ostream& stream) const {
  stream << "\n --- PDG : " << this->getPDG() << ", TrackID: " << this->getTrackID() << ", ParentID: " << this->getParentID() <<std::endl;
}
////////////////////////////////////////////////////////////////////////////

void GeantParticle::printToStream(std::ostream& stream) {
  stream << "\n pdg : " << getPDG() << ", trackID: " << trackID << ", parentID: " << parentID <<std::endl;

  TLorentzVector p4 = getP4() * (1./CLHEP::GeV);
  stream << " p4(Px,Py,Pz,E)[GeV] : (" << p4.X() << ", " 
	 << p4.Y() << ", " << p4.Z() << ", " << p4.T() << ")"
	 << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
/*
bool GeantDaughterParticle::isPrimary() const{

  if(primaryParent.getTrackID() == this->getTrackID())primary=true;
  else primary = false;

  return primary;
}
*/
///////////////////////////////////////////////////////////////////////////////
void MINDHit::printToStream(std::ostream& stream) {
  stream << "----- " << this->getRecordName() << " -----" << std::endl;
  BaseEventRecord::printToStream(stream);
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

void SimulData::printToStream(std::ostream& stream) {

  stream << "\n----- " << this->getRecordName() << " -----" ;
  stream << "\n * TPC Hits: " << tpc_hits_.size()  << ", edep [GeV]: "<< this->getTotalEDep("tpc");
  stream << "\n * Scint Hits: " << scint_hits_.size() << ", edep [GeV]: "<< this->getTotalEDep("scint");
  stream << "\n--------------------" << std::endl;
}
////////////////////////////////////////////////////////////////////////////
