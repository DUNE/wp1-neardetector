//____________________________________________________________________________
/*!

\class    Tracker

\brief    Class for tracking and track reconstrutction

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  May 2014
\last update June 2015

*/
//____________________________________________________________________________
#ifndef TRACKER_HH
#define TRACKER_HH

#include <iostream>
#include <vector>

#include "TLorentzVector.h"
#include "TVector3.h"
#include "GasTPCDataLib.hh"

class Sphere{
public:
  Sphere() {};
  Sphere(double rad,int i) : eDep(0.0),hitCount(0){
    radius = rad; 
    index = i;
    if(i==1)position.SetXYZ(radius,0,0);
    if(i==2)position.SetXYZ(0,radius,0);
    if(i==3)position.SetXYZ(-radius,0,0);
    if(i==4)position.SetXYZ(0,-radius,0);
    else position.SetXYZ(0,0,0);
  };
  ~Sphere() {};
  
  void Reset(){
    eDep = 0.0;
    hitCount= 0;
    hitsInIntersection.resize(0);
    if(index==1)position.SetXYZ(radius,0,0);
    if(index==2)position.SetXYZ(0,radius,0);
    if(index==3)position.SetXYZ(-radius,0,0);
    if(index==4)position.SetXYZ(0,-radius,0);
    else position.SetXYZ(0,0,0);
  }
  
  //rotate in XY only
  void RotateAndReset(){
    double newX = 0.;
    double newY = 0.;
    
    if(index == 1){
      newX = radius/TMath::Sqrt(2.0);
      newY = radius/TMath::Sqrt(2.0);
    }
    if(index == 2){
      newX = -radius/TMath::Sqrt(2.0);
      newY = radius/TMath::Sqrt(2.0);
    }
    if(index == 3){
      newX = -radius/TMath::Sqrt(2.0);
      newY = -radius/TMath::Sqrt(2.0);
    }
    if(index == 4){
      newX = radius/TMath::Sqrt(2.0);
      newY = -radius/TMath::Sqrt(2.0);
    }
    else {
      newX = 0.;
      newY = 0.;
    }
    
    TVector3 newPos(newX,newY,0);
    setPosition(newPos);
    
    //reset edep and hits
    eDep = 0.0;
    hitCount= 0;	
    hitsInIntersection.resize(0);
  };
  
  //setters
  void setRadius(double rad) {radius = rad;};
  void setPosition(TVector3 pos) {position = pos;};
  void setIndex(int i)  {index = i;};
  void setHitCount(int hits) {hitCount = hits;};
  void incrementHitCount() {hitCount ++;};
  void setEdep(double edep) {eDep = edep;};
  void addEdep(double edep) {eDep += edep;};
  void addToHitCollection(scintHit hit) {hitsInIntersection.push_back(hit);};
  
  //getters
  const double getRadius() {return radius;};
  const TVector3 getPosition() {return position;};
  const int getIndex() {return index;};
  const std::string getName() {return name;};
  const double getEdep() {return eDep;};
  const double getHitCount() {return hitCount;};
  void clearHitCollection() {hitsInIntersection.resize(0);};
  
  ScintHitCollection getHitCollection() {return hitsInIntersection;};
  
private:
  double radius;
  int index; // 1 is right, 2 is top, 3 is left and 4 is bottom
  std::string name;
  TVector3 position;
  
  int hitCount;
  double eDep;
  
  ScintHitCollection hitsInIntersection;
  
};

class Tracker{
  
public:
  Tracker();
  Tracker(double scintRes);
  ~Tracker();
  
  tpcTracks MakeFromHits(HitCollection &hitsVector,bool primaryOnly=true);
  scintTracks MakeFromHits(ScintHitCollection &hitsVector);
  scintTracks PiZeroTracks(ScintHitCollection &hitsVector,std::vector<GeantPrimaryParticle> *primaries);
  scintTracks PiZeroPhotonTracks( const ScintHitCollection &hitsVector );
  scintTracks PiZeroPhotonTracks(ScintHitCollection &hitsVector,std::vector<GeantPrimaryParticle> *primaries);
  
  scintTracks SphereTrackReconstruction(ScintHitCollection &hitsVector);
  bool IsInSphereIntersection(const TVector3 &hitCenterPos,const TVector3 &nextHitPos,const TVector3 &spherePosition);
  bool IsInSphere(const TVector3 &hitCenterPos,const TVector3 &nextHitPos,const double radius);
  
  int MatchScintToTpcTracks(ScintHitCollection &scintTrack,tpcTracks &tpcTracksVector); // returns track position in the tpcTracksVector, -1 if none found
  
  void PrintOutHits(HitCollection &hitsVector);
  void PrintOutHits(ScintHitCollection &hitsVector);
  
  template<class dataTypeOne,class dataTypeTwo>
  bool IsDescendant(int parentTrackId,dataTypeOne* &daugher,dataTypeTwo &vector);
  
  template<class dataType> 
  void SortByTrackID(dataType &vector);
  
private:
  void SortSpheresByCount(std::vector<Sphere *> &sphereVtr);
  
  tpcTracks tpcTracksVtr;
  scintTracks scintTracksVtr;
  
  double scintResolution_;
  double sphereRad_;
};


#endif
