//____________________________________________________________________________
/*!

\class    GasTPCDataLib

\brief    Header file containing all class types needed for software

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update June 2015
*/
//____________________________________________________________________________

#ifndef DATASTRUCT_DATARECORDLIB_H
#define DATASTRUCT_DATARECORDLIB_H 1

#include <iostream>
#include <iomanip>

#include <TObject.h>
#include <TLorentzVector.h>

#include <CLHEP/Units/SystemOfUnits.h>

//base class for all named data types
class namedRecord {
 public:
  namedRecord() {}
  namedRecord(std::string n) {name_ = n;}
  virtual ~namedRecord() {}

  std::string getRecordName() const {return name_;}
 protected:
  std::string name_;

  ClassDef(namedRecord, 1);
};

//base class for particle classes
class ParticleDescrShortRecord {
 public:
  ParticleDescrShortRecord();
  ParticleDescrShortRecord(int pdg, TLorentzVector p4);
  ParticleDescrShortRecord(const ParticleDescrShortRecord &p);

  virtual ~ParticleDescrShortRecord() {}

  int getPDG() const                                  {return pdg_;}
  //double getMass() const                              {return mass_;}
  TLorentzVector getP4() const                        {return p4_;}

  void setPDG(int pdg)                                {pdg_ = pdg;}
  //void setMass(double mass)                           {mass_ = mass;}
  void setP4(TLorentzVector v)                        {p4_ = v;}

  void setCharge(int ch)	           {charge_ = ch;};
  int getCharge() const	                   {return charge_;};

  ParticleDescrShortRecord &operator=(const ParticleDescrShortRecord &p);

  void printToStream(ostream& stream);

 protected:
  int pdg_,charge_;
  //double mass_;
  TLorentzVector p4_;

  ClassDef(ParticleDescrShortRecord, 1);
};

////////////////////////////////////////////////////////////////////////////////
class ParticleDescrRecord : public ParticleDescrShortRecord {
 public:
  ParticleDescrRecord();
  ParticleDescrRecord(int pdg, TLorentzVector p4, TLorentzVector pos);
  ParticleDescrRecord(const ParticleDescrRecord &p);

  virtual ~ParticleDescrRecord() {}

  TLorentzVector getPosition() const                  {return pos_;}

  void setPosition(TLorentzVector v)                  {pos_ = v;}

  ParticleDescrRecord& operator=(const ParticleDescrRecord &p);

  void printToStream(ostream& stream);

 protected:
  TLorentzVector pos_;

  ClassDef(ParticleDescrRecord, 1);
};

////////////////////////////////////////////////////////////////////////////////
class BaseEventRecord : public namedRecord {
 public:
  BaseEventRecord() : pos_(0., 0., 0., 0.) {}

  BaseEventRecord(TLorentzVector pos)
  //: pos_(pos) {}
  {
    pos_ = pos;
  }

  BaseEventRecord(const BaseEventRecord &e)
    : namedRecord(e.getRecordName()), pos_(e.getPosition()) {}

  virtual ~BaseEventRecord() {}

  TLorentzVector getPosition() const                  {return pos_;}
  void setPosition(TLorentzVector v)                  {pos_ = v;}

  BaseEventRecord& operator=(const BaseEventRecord &e) {
    pos_ = e.getPosition(); return *this;
  }

  void printToStream(ostream& stream);

 protected:
  TLorentzVector pos_;

  ClassDef(BaseEventRecord, 1);
};

////////////////////////////////////////////////////////////////////////////////
class NeutrinoHit : public namedRecord {
 public:
  NeutrinoHit() {name_ = "nuHits";};
  NeutrinoHit(ParticleDescrRecord nu);
  virtual ~NeutrinoHit() {}

  int getPDG() const                                  {return neutrino_.getPDG();}
  TLorentzVector getP4() const                        {return neutrino_.getP4();}
  TLorentzVector getPosition() const                  {return neutrino_.getPosition();}

  void setP4(TLorentzVector v)                        {neutrino_.setP4(v);}
  void setPosition(TLorentzVector v)                  {neutrino_.setPosition(v);}
  void setPDG(int pdg)                                {neutrino_.setPDG(pdg);}

  void printToStream(ostream& stream);

 private:
  ParticleDescrRecord neutrino_;

  ClassDef(NeutrinoHit, 1);
};

////////////////////////////////////////////////////////////////////////////////
class PionDecayEvent : public BaseEventRecord {
 public:
  PionDecayEvent() {name_ = "piDecays";}
  PionDecayEvent(ParticleDescrShortRecord nu,
                 ParticleDescrShortRecord chLepton,
                 TLorentzVector pos);
  virtual ~PionDecayEvent() {}

  int getChLeptonPDG() const                          {return chargedLepton_.getPDG();}
  int getNuPDG() const                                {return neutrino_.getPDG();}

  TLorentzVector geChLeptotP4() const                 {return chargedLepton_.getP4();}
  TLorentzVector getNuP4() const                      {return neutrino_.getP4();}

  bool inFlight() const                               {return inFlight_;}

  void setNuP4(TLorentzVector v)                      {neutrino_.setP4(v);}
  void setNuPDG(int pdg)                              {neutrino_.setPDG(pdg);}

  void setChLeptonP4(TLorentzVector v)                {chargedLepton_.setP4(v);}
  void setChLeptonPDG(int pdg)                        {chargedLepton_.setPDG(pdg);}

  void setInFlight(bool f)                            {inFlight_ = f;}

  void printToStream(ostream& stream);

 private:
  ParticleDescrShortRecord neutrino_;
  ParticleDescrShortRecord chargedLepton_;
  bool inFlight_;

  ClassDef(PionDecayEvent, 1);
};

////////////////////////////////////////////////////////////////////////////////
class MuonDecayEvent : public BaseEventRecord {
 public:
  MuonDecayEvent() {name_ = "muDecays";}
  MuonDecayEvent(ParticleDescrShortRecord numu,
                 ParticleDescrShortRecord nue,
                 ParticleDescrShortRecord electron,
                 TLorentzVector pos);
  virtual ~MuonDecayEvent() {}

  int getNuMuPDG() const                              {return numu_.getPDG();}
  TLorentzVector getNuMuP4() const                    {return numu_.getP4();}

  int getNuEPDG() const                               {return nue_.getPDG();}
  TLorentzVector getNuEP4() const                     {return nue_.getP4();}

  int getElectronPDG() const                          {return numu_.getPDG();}
  TLorentzVector getElectronP4() const                {return numu_.getP4();}

  void setNuMuP4(TLorentzVector v)                    {numu_.setP4(v);}
  void setNuMuPDG(int pdg)                            {numu_.setPDG(pdg);}

  void setNuEP4(TLorentzVector v)                     {nue_.setP4(v);}
  void setNuEPDG(int pdg)                             {nue_.setPDG(pdg);}

  void setElectronP4(TLorentzVector v)                {electron_.setP4(v);}
  void setElectronPDG(int pdg)                        {electron_.setPDG(pdg);}

 private:
  ParticleDescrShortRecord numu_;
  ParticleDescrShortRecord nue_;
  ParticleDescrShortRecord electron_;

  ClassDef(MuonDecayEvent, 1);
};

////////////////////////////////////////////////////////////////////////////////
class NeutrinoEvent : public BaseEventRecord {
 public:
  NeutrinoEvent() {name_ = "nuInteractions";}
  virtual ~NeutrinoEvent() {}

  //ParticleDescrShortRecord getFspl() const                   {return fspl_;}
  //ParticleDescrShortRecord getHitNucleon() const             {return nucl_;}
  std::vector<ParticleDescrShortRecord> getFssVector() const {return fss_;}
  std::vector<ParticleDescrShortRecord> getIssVector() const {return iss_;}

  int getNeutrinoType() const     {return neutrinoType_;}
  int getScatteringType() const   {return scatteringType_;}

  //bool getQELCC()	const	{return qelcc_;};

  double getNuEnergy()		  {return nuEnergy_;}
  double getx()                   {return x_;}
  double gety()                   {return y_;}
  double getW()                   {return W_;}
  double getQ2()                  {return Q2_;}
  //double getWeight()              {return weight_;}
  //int    getresid()               {return resid_;}
  int    getEventID()             {return eventid_;}
  int    getNEUTCode()            {return neutcode_;}
  int    getRunID()               {return runid_;}
  int    getNucleonPdg()          {return nucleonpdg_;}

  //string InteractionTypeAsString() const {return genie::ScatteringType::AsString(scatteringType_);}

  int GetSpillNumber()            {return spillnum_;}

  void clearFssVector()  {std::vector<ParticleDescrShortRecord>().swap(fss_); fss_.clear();fss_.resize(0); 
    std::vector<ParticleDescrShortRecord>().swap(iss_); iss_.clear(); iss_.resize(0);}

  void setNuEnergy(double E)                            {nuEnergy_= E;}

  //void setFspl(ParticleDescrShortRecord p)              {fspl_ = p;}
  //void setHitNucleon(ParticleDescrShortRecord n)        {nucl_ = n;}
  void addFss(ParticleDescrShortRecord p)               {fss_.push_back(p);}
  void addIss(ParticleDescrShortRecord p)               {iss_.push_back(p);}

  void setNeutrinoType(int itr) {neutrinoType_ = itr;}
  void setScatteringType(int sc)   {scatteringType_ = sc;}
  
  void setx(double x) {x_ = x;};
  void sety(double y) {y_ = y;};
  void setW(double W) {W_ = W;};
  void setQ2(double Q2) {Q2_ = Q2;};
  //void setresid(int resid) {resid_ = resid;};
  //void setweight(double weight) {weight_ = weight;};
  void setEventID(int eventid) {eventid_ = eventid;};
  void setNEUTCode(int neut) {neutcode_ = neut;};
  void setRunID(int rid) {runid_ = rid;};
  void setNucleonPdg(int pdg) {nucleonpdg_ = pdg;};

  //void setTopology(bool sea, bool qel, bool mec, bool res, bool dis, bool coh, bool dfr, bool imd, bool nuel, bool charm){sea_ = sea; qel_ = qel; mec_ = mec; res_ = res; dis_ = dis; coh_ = coh; dfr_ = dfr; imd_ = imd; nuel_ = nuel; charm_ = charm;};

  //void setNodeName(std::string nn) {nodeName_ = nn;}
  //std::string getNodeName() const  {return nodeName_;}

  void SetSpillNumber(int spill) {spillnum_ = spill;};

  void printToStream(ostream& stream);

 private:
  //ParticleDescrShortRecord                 fspl_;          // Final state primary lepton.
  //ParticleDescrShortRecord                 nucl_;          // The hit nucleon.
  std::vector<ParticleDescrShortRecord>    fss_;           // Final state secondary particles.
  std::vector<ParticleDescrShortRecord>    iss_;           // Initial state particles.

  int neutrinoType_, scatteringType_, eventid_, neutcode_, runid_;
  int nucleonpdg_;
  //int resid_;
  //std::string nodeName_;
  int spillnum_;

  //bool sea_, qel_, mec_, res_, dis_, coh_, dfr_, imd_, nuel_, charm_;

  double nuEnergy_, x_, y_, Q2_, W_; 
  //double  weight_;

  ClassDef(NeutrinoEvent, 2);
};

////////////////////////////////////////////////////////////////////////////////
class GeantBasicParticle : public ParticleDescrRecord{ 

 public:
  GeantBasicParticle() {}
  GeantBasicParticle(int track,int parent,int pdg){ //: trackID(track),parentID(parent){
    this->setPDG(pdg);
    trackID = track;
    parentID = parent;
  }
  
  virtual ~GeantBasicParticle() {}

  //copy constructor
  GeantBasicParticle(const GeantBasicParticle &p) : ParticleDescrRecord(p){
    trackID = p.getTrackID();
    parentID = p.getParentID();
    pdg_ = p.getPDG();
  }
  //equal operator
  const GeantBasicParticle& operator=(const GeantBasicParticle &p){
    trackID = p.getTrackID();
    parentID = p.getParentID();
    pdg_ = p.getPDG();
    p4_  = p.getP4();
    pos_ = p.getPosition();
    //mass_  = p.getMass();
    return *this;
  }

  int getTrackID() const		{return trackID;};
  int getParentID() const		{return parentID;};

  void setTrackID(int id)		{trackID =id;};
  void setParentID(int id)		{parentID =id;};

  void printToStream(ostream& stream) const;

 protected:

  int parentID,trackID;

  ClassDef(GeantBasicParticle, 1);
};

////////////////////////////////////////////////////////////////////////////////
class GeantParticle : public GeantBasicParticle {

 public:
  GeantParticle() {}
  virtual ~GeantParticle() {}
 
  //copy constructor
  GeantParticle(const GeantParticle &p) : GeantBasicParticle(p){
    eventID = p.getEventID();
    //stepNumber = p.getStepNumber();
    //hitsData = p.getHitsDataPtr();
  }

  //equal operator
  const GeantParticle& operator=(const GeantParticle &p){
    pdg_ = p.getPDG();
    p4_  = p.getP4();
    pos_ = p.getPosition();
    //mass_  = p.getMass();
    eventID = p.getEventID();
    trackID = p.getTrackID();
    parentID = p.getParentID();
    //stepNumber = p.getStepNumber();
    //hitsData = p.getHitsDataPtr();
    return *this;
  }

  //getters
  int getEventID() const		{return eventID;};
  //int getStepNumber() const		{return stepNumber;};
  // Returns true if the particle left the target volume
  bool getExitVolume() const            {return exitvolume;};
  // Returns true if the particle entered the target volume
  bool getEnterVolume() const            {return entervolume;};
  //SimulData * getHitsDataPtr() const 	{return hitsData;};

  //setters
  void setEventID(int id){eventID =id;};
  //void setStepNumber(int n){stepNumber =n;};
  void setExitVolume(bool b){exitvolume =b;};
  void setEnterVolume(bool b){entervolume =b;};
  //void setHitsDataPtr(SimulData * hitsPtr) {hitsData = hitsPtr;};

  void printToStream(ostream& stream);

 protected:
  int eventID;
  //int stepNumber;
  bool exitvolume;
  bool entervolume;

  //SimulData * hitsData;

  ClassDef(GeantParticle, 1);
};

////////////////////////////////////////////////////////////////////////////////
/*
class GeantPrimaryParticle : public GeantParticle {

 public:
  GeantPrimaryParticle() : GeantParticle() {}
  virtual ~GeantPrimaryParticle() {}
 
  //copy constructor
  GeantPrimaryParticle(const GeantPrimaryParticle &p) : GeantParticle(p){
    //daughters = p.getDaughterVtr();
  }

  //equal operator
  //const GeantPrimaryParticle& operator=(const GeantPrimaryParticle &p) {
  //daughters = p.getDaughterVtr();
  //return *this;
  //}

  //daughter
  //std::vector<GeantBasicParticle> getDaughterVtr() const 	 {return daughters;};
  //std::vector<GeantBasicParticle> *getDaughterVtrPtr() 	 	 {return &daughters;};
  //void setDaughterVtr(std::vector<GeantBasicParticle> daught) 	 {daughters = daught;};
  //void addDaughter(GeantBasicParticle daught)		 	 {daughters.push_back(daught);};

 private:

  //std::vector<GeantBasicParticle> daughters;

  ClassDef(GeantPrimaryParticle, 1);
};
*/
////////////////////////////////////////////////////////////////////////////////
/*
class GeantDaughterParticle : public GeantParticle {

 public:
  GeantDaughterParticle() : GeantParticle() {}
  virtual ~GeantDaughterParticle() {}
 
  //copy constructor
  GeantDaughterParticle(const GeantDaughterParticle &p) : GeantParticle(p){
    //primary = p.isPrimary();
    //primaryParent = p.getParent();
  }

  //equal operator
  //const GeantDaughterParticle& operator=(const GeantDaughterParticle &p) {
    //primary = p.isPrimary();
    //primaryParent = p.getParent();	
    //return *this;
  //}

  //parent
  //GeantParticle getParent() const	   {return primaryParent;};
  //void setParent(GeantParticle parent) 	   {primaryParent = parent;};

  //check if it is actually a primary particle
  //bool isPrimary() const;

 private:

  //mutable bool primary;
  //GeantParticle primaryParent;

  ClassDef(GeantDaughterParticle, 1);
};
*/
////////////////////////////////////////////////////////////////////////////////
class GeantTrackingTruth : public namedRecord {

 public:
  GeantTrackingTruth() {name_ = "GeantTrackingTruth";};
  virtual ~GeantTrackingTruth() {
    primaries.resize(0);
    //leavingVolume.resize(0);
    //enteringVolume.resize(0);
    allParticles.resize(0);
  };

  //std::vector<GeantParticle> * getPrimariesPtr() 	{return &primaries;};
  //std::vector<GeantParticle> * getAllParticlesPtr()	{return &allParticles;};
  std::vector<GeantParticle> getPrimariesPtr() 	{return primaries;};
  std::vector<GeantParticle> getAllParticlesPtr()	{return allParticles;};
  

  //std::vector<GeantPrimaryParticle> * getPrimariesPtr() 	{return &primaries;};
  //std::vector<GeantDaughterParticle> * getInParticlesPtr() 	{return &enteringVolume;};
  //std::vector<GeantDaughterParticle> * getOutParticlesPtr()	{return &leavingVolume;};
  //std::vector<GeantDaughterParticle> * getAllParticlesPtr()	{return &allParticles;};
  //std::vector<GeantParticle> * getAllParticlesPtr()	{return &allParticles;};

  void setPrimaries(std::vector<GeantParticle> prims) {primaries = prims;};
  //void setInParticles(std::vector<GeantDaughterParticle> in) {enteringVolume = in;};
  //void setOutParticles(std::vector<GeantDaughterParticle> out) {leavingVolume = out;};
  void setAllParticles(std::vector<GeantParticle> all) {allParticles = all;};

  void clearTracks()		{clearPrimaries(); /*clearInParticles(); clearOutParticles();*/ clearAllParticles();};
  void clearPrimaries()		{std::vector<GeantParticle>().swap(primaries); primaries.resize(0);};
  //void clearInParticles()	{std::vector<GeantDaughterParticle>().swap(enteringVolume); enteringVolume.resize(0);};
  //void clearOutParticles()	{std::vector<GeantDaughterParticle>().swap(leavingVolume); leavingVolume.resize(0);};
  void clearAllParticles()	{std::vector<GeantParticle>().swap(allParticles); allParticles.resize(0);};

 private:
  //std::string volumeName;

  //std::vector<GeantPrimaryParticle> primaries;
  std::vector<GeantParticle> primaries;
  //std::vector<GeantDaughterParticle> enteringVolume;
  //std::vector<GeantDaughterParticle> leavingVolume;
  //std::vector<GeantDaughterParticle> allParticles;
  std::vector<GeantParticle> allParticles;

  ClassDef(GeantTrackingTruth, 1);
};

////////////////////////////////////////////////////////////////////////////////
class SDHit : public BaseEventRecord{
 public:
  SDHit() {name_ = "SDHit";}
  virtual ~SDHit() {}
  
  //copy constructor
  SDHit(const SDHit &e) : BaseEventRecord(e){
    Edep_ = e.getEdep();
    p4 = e.getP4();
    kinEnergy = e.getKinEnergy();
    //nSteps_ = e.getNSteps();
    PDG = e.getPDG();
    trackID = e.getTrackID();
    parentID = e.getParentID();
    trackLeftVolume = e.getTrackLeftVolume();
    pos_ = e.getPosition();
    //parentParticle = e.getParent();
    charge_ = e.getCharge();
  }

  const SDHit& operator=(const SDHit &e){
    Edep_ = e.getEdep();
    p4 = e.getP4();
    kinEnergy = e.getKinEnergy();
    //nSteps_ = e.getNSteps();
    PDG = e.getPDG();
    trackID = e.getTrackID();
    parentID = e.getParentID();
    trackLeftVolume = e.getTrackLeftVolume();
    pos_ = e.getPosition();
    //parentParticle = e.getParent();
    charge_ = e.getCharge();
    return *this;
  }

  //void clearFssVector()  {fss_.resize(0);}

  //void addFss(ParticleDescrRecord p) {fss_.push_back(p);}
  //std::vector<ParticleDescrRecord> getFssVector() const {return fss_;}

  void setEdep(double e)                   {Edep_ = e;}
  double getEdep() const                   {return Edep_;}

  void setP4(TLorentzVector mom)           {p4 = mom;}
  TLorentzVector getP4() const         	   {return p4;}

  void setKinEnergy(double e)              {kinEnergy = e;}
  double getKinEnergy() const              {return kinEnergy;}

  //int getNSteps() const                    {return nSteps_;}
  //void setNSteps(int n)                    {nSteps_ = n;}

  void setPDG(int pdg)			   {PDG = pdg;};
  int getPDG() const			   {return PDG;};

  void setTrackID(int tID)		   {trackID = tID;};
  int getTrackID() const		   {return trackID;};

  void setParentID(int pID)		   {parentID = pID;};
  int getParentID() const		   {return parentID;};

  void setTrackLeftVolume(int tlv)	   {trackLeftVolume = tlv;};
  int getTrackLeftVolume() const	   {return trackLeftVolume;};

  void setCharge(int ch)	           {charge_ = ch;};
  int getCharge() const	                   {return charge_;};

  //void setParent(GeantBasicParticle p)	   {parentParticle = p;};
  //GeantBasicParticle getParent() const     {return parentParticle;};

  void printToStream(ostream& stream);

protected:
  //ParticleDescrRecord fss_;
  //std::vector<ParticleDescrRecord>    fss_;

  int PDG,trackID,parentID, charge_;
  double Edep_,kinEnergy;
  TLorentzVector p4;
  int trackLeftVolume;

  //GeantBasicParticle parentParticle;

  ClassDef(SDHit, 1);
};

////////////////////////////////////////////////////////////////////////////////
class tpcFidHit : public SDHit {
 public:
  tpcFidHit() {name_ = "tpcFidHit";}
  virtual ~tpcFidHit() {}

  void printToStream(ostream& stream);

 private:
 
  ClassDef(tpcFidHit, 1);
};

////////////////////////////////////////////////////////////////////////////////
class scintHit : public SDHit {
 public:
  scintHit() {name_ = "scintHit";}
  scintHit(int layerID){name_ = "scintHit";layerNumber = layerID;}
  scintHit(int ecalID, int layerID){name_ = "scintHit"; ecalNumber =ecalID; layerNumber = layerID;}
  ~scintHit() {}

  int getLayerNumber()	const	{return layerNumber;};
  void setLayerNumber(int no)  {layerNumber = no;};

  int getEcalNumber()	const	{return ecalNumber;};
  void setEcalNumber(int no)  {ecalNumber = no;};

  void printToStream(ostream& stream);

 private:
  int layerNumber, ecalNumber;

  ClassDef(scintHit, 1);
};

/////////////////////////////////////////////////////////////////////////////////////

class MINDHit : public SDHit {
 public:
  MINDHit() {name_ = "MINDHit";}
  virtual ~MINDHit() {}

  void printToStream(ostream& stream);

 private:
  ClassDef(MINDHit, 1);
};

typedef std::vector<SDHit>    HitCollection;
typedef std::vector<scintHit> ScintHitCollection;

//tracks are vectors of HitCollections
//typedef std::vector<HitCollection> tpcTracks;
//typedef std::vector<ScintHitCollection> scintTracks;

/////////////////////////////////////////////////////////////////////////////////////

class SimulData  : public namedRecord {
 public:
  SimulData() {name_ = "SimulData";}
  virtual ~SimulData() {}

  int getEventID()			const	{return eventID;};
  void setEventID(int ev) 			{eventID = ev;};

  double getTotalEDep(std::string hits_name) const;

  //HitCollection getSDHits() 	       const 	{return SD_hits_;}
  HitCollection getTpcFidHits()         const  	{return tpc_hits_;}
  ScintHitCollection getScintHits()     const  	{return scint_hits_;}

  //void setSDHits(HitCollection h)  		{SD_hits_ =h;}
  void setTpcFidHits(HitCollection h) 		{tpc_hits_ = h;}
  void setScintHits(ScintHitCollection h) 	{scint_hits_ = h;}

  void clearSDHits()				{clearTpcFidHits();clearScintHits();}
  void clearTpcFidHits()			{HitCollection().swap(tpc_hits_);tpc_hits_.resize(0);}
  void clearScintHits()				{ScintHitCollection().swap(scint_hits_);scint_hits_.resize(0);}

  void printToStream(ostream& stream);

 private:
  int 			eventID;

  //HitCollection  	SD_hits_;
  HitCollection  	tpc_hits_;
  ScintHitCollection  	scint_hits_;

  ClassDef(SimulData, 1);
};

////////////////////////////////////////////////////////////////////////////////
class MemTestRecord : public namedRecord {
 public:
  MemTestRecord() {name_ = "MemTestRecord";}
  virtual ~MemTestRecord() {}

  void setData(double d) {data_ = d;}
  double getData() const {return data_;}

  void printToStream(ostream& stream) {
    stream << "MemTestRecord  data: " << data_ << std::endl;
  }

 private:
  double data_;

  ClassDef(MemTestRecord, 1);
};

////////////////////////////////////////////////////////////////////////////////

#endif

