//____________________________________________________________________________
/*!

\class    ParticleGunProcessor

\brief    Processor class to implement the geant4 particle gun and tracking

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Oct 2014
\last update Oct 2014

*/
//____________________________________________________________________________
#ifndef PROCESSORS_PARTICLEGUN
#define PROCESSORS_PARTICLEGUN

#include <TRandom3.h>

#include "GasTPCProcessor.hh"
#include "GeometryLoader.hh"
#include "GasTPCDataLib.hh"
#include "DetectorHit.hh"
#include "Geant4TrackingAlgorithm.hh"

class ParticleGunProcessor  : public GasTPCProcessor {
 public:
  ParticleGunProcessor();
  ParticleGunProcessor(GeometryLoader * geomLoader);
  virtual ~ParticleGunProcessor();

  bool process();
  void initOutDataTree();
  void loadInDataTree();
  void initDataCards();
  void loadDataCards();

  void setHitCollections();
  void setTracking();

  void SetupEvent();

 private:

  NeutrinoEvent* 		event_;
  SimulData*      		MChits_;
  GeantTrackingTruth*		trackingRecord_;

  Geant4TrackingAlgorithm g4tracking_;

  GeometryLoader * geomLoader_;

  double Px_,Py_,Pz_;
  double E_,Emax_,Emin_;
  double X_,Y_,Z_;
  double mass_;
  int pdg_;

};

#endif


