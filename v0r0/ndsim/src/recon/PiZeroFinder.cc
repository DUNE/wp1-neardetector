//____________________________________________________________________________
/*!

\class    PiZeroFinder

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool

\created  May 2014
\last update June 2014

*/
//____________________________________________________________________________
#include "PiZeroFinder.hh"

PiZeroFinder::PiZeroFinder(){
}

PiZeroFinder::PiZeroFinder(ScintHitCollection * unmatchedHits){
  unmatchedHits_ = unmatchedHits;
  tpcOrigin_.SetXYZ(0.,0.,0.);
  theta_ = 10.0;	// in degrees
}

PiZeroFinder::~PiZeroFinder(){
}

/// methods using truth results and hits
TLorentzVector PiZeroFinder::ReconstructPhotonKinematics(int n,scintTracks &photonTracks,TLorentzVector &eventVertex){

  TLorentzVector photonP4(0.,0.,0.,0.);
  double photonEnergy = 0.;

  int numberOfPhotonsInEvent = photonTracks.size();

  if(numberOfPhotonsInEvent==0)return photonP4;

  if(n>numberOfPhotonsInEvent){
	std::cout << "\n!!! Specified the " << n << "th photon in event but does not exist. Only " << numberOfPhotonsInEvent << " in event!";
	return photonP4;
  }
  
  if(n<1){
	std::cout << "\n!!! Specified the " << n << "th photon in event but does not exist. Must be larger than 0.";
	return photonP4;
  }

  ScintHitCollection * photonTrack = &photonTracks.at(n-1);

  //reconstruct photon momentum from vertex and hit positions
  photonP4 = ReconstructPhotonUnitMomentum(*photonTrack,eventVertex);

  //get the energies from the hits vector
  for(int k=0;k<photonTrack->size();k++){
	photonEnergy += photonTrack->at(k).getEdep();
  }//end loop over photon hits
 
  photonP4.SetE(photonEnergy);

  return photonP4;
}

std::pair<TLorentzVector,TLorentzVector> 
PiZeroFinder::GetPhotonPairKinematics(int n,std::vector<GeantPrimaryParticle> *primaries,scintTracks &piZeroTracks,TLorentzVector &eventVertex,bool useTruthE){

  std::pair<TLorentzVector,TLorentzVector> photonPairKinematics;
  photonPairKinematics.first =  TLorentzVector(0.,0.,0.,0.);
  photonPairKinematics.second = TLorentzVector(0.,0.,0.,0.);

  int numberOfPiZerosInEvent = GetNumberOfPiZerosInEvent(primaries);

  if(n>numberOfPiZerosInEvent){
	std::cout << "\n!!! Specified the " << n << "th pi0 in event but does not exist. Only " << numberOfPiZerosInEvent << " in event!";
	return photonPairKinematics;
  }
  
  int PiZeroCount = 0;
  int PiZeroLocationInVtr = 0;

  TLorentzVector photonOneP4(0.,0.,0.,0.);
  TLorentzVector photonTwoP4(0.,0.,0.,0.);

  //loop over primaries
  for(int i =0;i<primaries->size();i++){
  
    GeantPrimaryParticle * prim = &primaries->at(i);

    //skip anything other than pi0s
    if(prim->getPDG()==111)PiZeroCount++;
    else continue;

    //get the trackID of the pi zero
    int primTrackID = prim->getTrackID();
    int PhotonCount = 0;

    if(PiZeroCount == n){
	PiZeroLocationInVtr = i;

    	//get the daughters/descendants
    	std::vector<GeantBasicParticle> descendants = prim->getDaughterVtr();

    	//sort them by trackID
	Tracker tracker;
    	tracker.SortByTrackID(descendants);
	int lastTrackID =-1;

	//loop over daughters to find pi0s
	for(int j=0;j<descendants.size();j++){

	  GeantBasicParticle * p = &descendants.at(j);
	  p->setP4( p->getP4()*(1./CLHEP::GeV) ); // put it in GeV

	  //only want to count each track once
	  if(p->getTrackID() == lastTrackID)continue;
	  lastTrackID = p->getTrackID();

	  if( (p->getParentID() == primTrackID) && (p->getPDG() ==22) ){
		PhotonCount++;

		if(PhotonCount==1){
		  photonOneP4 = p->getP4();
// 		  std::cout << "\nPhoton1 -- pdg: " << p->getPDG() 
// 				<< ", (Px, Py, Pz) : (" << p->getP4().Px() << ", " << p->getP4().Py() << ", " << p->getP4().Pz() << ") ";
		  continue;
		}
		if(PhotonCount==2){
		  photonTwoP4 = p->getP4();
// 		  std::cout << "\nPhoton2 -- pdg: " << p->getPDG() 
// 				<< ", (Px, Py, Pz) : (" << p->getP4().Px() << ", " << p->getP4().Py() << ", " << p->getP4().Pz() << ") ";
		  break;
		}
	  }//select only first photons
	}//end loop over daughters

    }//selected pi zero in event
  }//end loop over primaries

  //check if it found momentum of both photons - if not use reconstruction from hit positions
  if(CalculateThreeMomentumMag(photonOneP4) == 0.){
	int indexOne = 2*n - 2;
	if(piZeroTracks.size() >= (indexOne + 1))photonOneP4 = ReconstructPhotonUnitMomentum(piZeroTracks.at(indexOne),eventVertex);
// 	std::cout << "\nPhoton1 (recon) -- (Px, Py, Pz) : (" << photonOneP4.Px() << ", " << photonOneP4.Py() << ", " << photonOneP4.Pz() << ") ";
  }
  if(CalculateThreeMomentumMag(photonTwoP4) == 0.){
	int indexTwo = 2*n - 1;
	if(piZeroTracks.size() >= (indexTwo + 1))photonTwoP4 = ReconstructPhotonUnitMomentum(piZeroTracks.at(indexTwo),eventVertex);
// 	std::cout << "\nPhoton2 (recon) -- (Px, Py, Pz) : (" << photonTwoP4.Px() << ", " << photonTwoP4.Py() << ", " << photonTwoP4.Pz() << ") ";
  }

  double photonEnergyOne = 0.;
  double photonEnergyTwo = 0.;

  if(!useTruthE){
    //get the energies from the hits vector
    for(int k=0;k<piZeroTracks.size();k++){
	
	ScintHitCollection * tempVtr = &piZeroTracks.at(k);

	//first photon hits
	if( k==(2*n -2) ){
	  //loop over hits on each track
	  for(int l=0;l<tempVtr->size();l++){
		photonEnergyOne += tempVtr->at(l).getEdep();
	  }
	}
	//second photon hits 
	if( k==(2*n -1) ){
	  //loop over hits on each track
	  for(int l=0;l<tempVtr->size();l++){
		photonEnergyTwo += tempVtr->at(l).getEdep();
	  }
	}
    }//end loop over pi0 tracks
  }
  else{
	photonEnergyOne = TMath::Sqrt( photonOneP4.Px()*photonOneP4.Px() + photonOneP4.Py()*photonOneP4.Py() +  photonOneP4.Pz()*photonOneP4.Pz() );
	photonEnergyTwo = TMath::Sqrt( photonTwoP4.Px()*photonTwoP4.Px() + photonTwoP4.Py()*photonTwoP4.Py() +  photonTwoP4.Pz()*photonTwoP4.Pz() );
  }
 
  photonOneP4.SetE(photonEnergyOne);
  photonTwoP4.SetE(photonEnergyTwo);

  photonPairKinematics.first = photonOneP4;
  photonPairKinematics.second = photonTwoP4;

  return photonPairKinematics;
}

//use three vector for photons as are massless
double PiZeroFinder::CalculateThreeMomentumMag(TLorentzVector &vector){

  double mag = 0.;
  TVector3 threeVector(vector.X(),vector.Y(),vector.Z());

  mag = threeVector.Mag();

  if(mag == 0.){
	threeVector.SetXYZ(vector.Px(),vector.Py(),vector.Pz());
	mag = threeVector.Mag();
  }

  return mag;
}

double PiZeroFinder::ReconstructPiZeroEnergy(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics){

  double energy = 0.;
  energy = photonPairKinematics.first.E() + photonPairKinematics.second.E();

  return energy;
}

std::pair<TLorentzVector,TLorentzVector> PiZeroFinder::MatchPhotonPair(int n,scintTracks &photonTracks,TLorentzVector &eventVertex,double massCutValue){

  std::pair<TLorentzVector,TLorentzVector> photonPair;

  //get the first photon to compare with the rest
  TLorentzVector photonOneKine = ReconstructPhotonKinematics(n,photonTracks,eventVertex);
  photonPair.first = photonOneKine;

  TLorentzVector photonTwoKine(0.,0.,0.,0.);

  int bestMatchPosInVtr = -1;
  double bestMatchMass = 0.;
  //loop over them again to match photon pair
  for(int j=1;j<(photonTracks.size()+1);j++){
	if(n==j)continue;

	TLorentzVector photonTwoKineTemp = ReconstructPhotonKinematics(j,photonTracks,eventVertex);
    	photonPair.second = photonTwoKineTemp;
	double reconMass = ReconstructPiZeroInvariantMass(photonPair); // returns in GeV
	//make sure bestMatchMass is set first
	if(bestMatchMass <= 0.0)bestMatchMass =reconMass;
	//pick the mass closest to the pi zero mass
	if( TMath::Abs(PIZEROMASS*(1./CLHEP::GeV) - reconMass) <= (TMath::Abs(PIZEROMASS*(1./CLHEP::GeV) - bestMatchMass)) ){
	  bestMatchMass =reconMass;
	  bestMatchPosInVtr = j;
	  photonTwoKine = photonTwoKineTemp;
	}	
  }

  //check whether the mass reconstructed is within the cut value
  if( TMath::Abs(PIZEROMASS*(1./CLHEP::GeV) - massCutValue) >=  (TMath::Abs(PIZEROMASS*(1./CLHEP::GeV) - bestMatchMass)) ){
	//remove the pair from the vector
	if( photonTracks.size() > n ){
	  photonTracks.erase( photonTracks.begin()+(n-1) );
	  photonTracks.erase( photonTracks.begin()+bestMatchPosInVtr );
	}
  }
  else{
	photonTwoKine.SetPxPyPzE(0.,0.,0.,0.);	
  }
  photonPair.second = photonTwoKine;  

  return photonPair;
}

//reconstruct the nth pi zero in the event from the truth primary tracks
double PiZeroFinder::ReconstructPiZeroInvariantMass(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics){

  //get the angle
  double alpha = CalculateAngleBetweenPhotons(photonPairKinematics);
  double mass = CalculateInvariantMass(photonPairKinematics,alpha);
  
  //this is in GeV
  return mass;
}

double PiZeroFinder::GetTotalPiZeroTruthEnergy(std::vector<GeantPrimaryParticle> *primaries) const{

}

int PiZeroFinder::GetNumberOfPiZerosInEvent(std::vector<GeantPrimaryParticle> *primaries) const {

  int count = 0;

  //loop over primaries
  for(int i =0;i<primaries->size();i++){
  
    GeantPrimaryParticle * prim = &primaries->at(i);

    //skip anything other than pi0s
    if(prim->getPDG()==111)count++;
  }

  return count;
}

double PiZeroFinder::CalculateAngleBetweenPhotons(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics){

  TLorentzVector photonOneP4 = photonPairKinematics.first;
  TLorentzVector photonTwoP4 = photonPairKinematics.second;

  //angle between them
  TVector3 photonOneP3(photonOneP4.Px(),photonOneP4.Py(),photonOneP4.Pz());
  TVector3 photonTwoP3(photonTwoP4.Px(),photonTwoP4.Py(),photonTwoP4.Pz());

  if( photonOneP3.Mag()==0 || photonTwoP3.Mag() ==0 )return 0.;

  double alpha = TMath::ACos( photonOneP3.Dot(photonTwoP3) / (photonOneP3.Mag()*photonTwoP3.Mag()) );

  //std::cout << "\nAngle is: " << alpha;
  return alpha;
}

double PiZeroFinder::CalculateInvariantMass(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics,double angleInRad){

  double piZeroMass = 0.;
  double photonOneEnergy = photonPairKinematics.first.E();
  double photonTwoEnergy = photonPairKinematics.second.E();

  //std::cout << "\nPhoton Energy One: " << photonOneEnergy << ", Photon Energy Two : " << photonTwoEnergy << " GeV";
  piZeroMass = TMath::Sqrt(4.*photonOneEnergy*photonTwoEnergy*TMath::Power(TMath::Sin(angleInRad/2.),2));

  return piZeroMass;
}

double PiZeroFinder::CalculateInvariantMass(std::pair<TLorentzVector,TLorentzVector> &photonPairKinematics){

  double piZeroMass = 0.;
  double photonOneEnergy = photonPairKinematics.first.E();
  double photonTwoEnergy = photonPairKinematics.second.E();

  //angle between them
  double alpha = CalculateAngleBetweenPhotons(photonPairKinematics);

  piZeroMass = TMath::Sqrt(4.*photonOneEnergy*photonTwoEnergy*TMath::Power(TMath::Sin(alpha/2.),2));

  return piZeroMass;
}

TLorentzVector PiZeroFinder::ReconstructPhotonUnitMomentum(ScintHitCollection &track,TLorentzVector &eventVertex){

  TLorentzVector p4Unit(0.0,0.0,0.0,0.0);

  int vectorPos = ClosestHitInTrackToVertex(track,eventVertex);

  if(vectorPos<0) return p4Unit; // cannot reconstruct momentum if no hits are found

  TVector3 OB(track.at(vectorPos).getPosition().X(),
		track.at(vectorPos).getPosition().Y(),
		track.at(vectorPos).getPosition().Z());

  TVector3 OA(eventVertex.X(),eventVertex.Y(),eventVertex.Z());

  TVector3 AB = OB - OA;

  p4Unit.SetPxPyPzE(AB.X(),AB.Y(),AB.Z(),0.);
  if(AB.Mag()>0)p4Unit *= 1./AB.Mag();

  return p4Unit;
}

int PiZeroFinder::ClosestHitInTrackToVertex(ScintHitCollection &track,TLorentzVector &eventVertex){

  double magnitude = 0.0;
  int hitVtrPos = -1;
  int layerNumber = -1;
  int pdg = 0;

  TVector3 pos3(eventVertex.X(),eventVertex.Y(),eventVertex.Z());

  //loop over the track to find closest hit to TPC
  for(int i =0;i<track.size();i++){
	TVector3 hitGlobalPosition(track.at(i).getPosition().X(),
					track.at(i).getPosition().Y(),
					track.at(i).getPosition().Z());
	TVector3 hitPositionFromVertex = hitGlobalPosition - pos3;
	double hitDistanceFromVertex = hitPositionFromVertex.Mag();

	//set the first hit distance
	if(magnitude==0.0){
		magnitude = hitDistanceFromVertex;
	  	layerNumber = track.at(i).getLayerNumber();
		pdg = track.at(i).getPDG();
	}
		
	if(layerNumber>=track.at(i).getLayerNumber()){
	  if(hitDistanceFromVertex<=magnitude){
	  	magnitude = hitDistanceFromVertex;
	  	layerNumber = track.at(i).getLayerNumber();
		pdg = track.at(i).getPDG();
	  	hitVtrPos = i;
	  }
	}
  }

  //std::cout << "\nClosest Hit is: " << pdg << ", mag: " << magnitude << ", layer number: " << layerNumber << ", at position: " << hitVtrPos << ", in vector.";

  return hitVtrPos;

}

/// methods using cone reconstruction
bool PiZeroFinder::IsHitInConeProjection(scintHit &initialHit,TVector3 &hitPosition,double thetaInDeg){

  //use vector equations to determine if in cone 
  // lambda is the amount need to move along in the initial momentum axis
  // comes from drawing a perp vector to momentum at hit position, call it x
  // so p_{0} dot x =0
  TVector3 initialMomUnit(initialHit.getP4().X(),
			initialHit.getP4().Y(),
			initialHit.getP4().Z());
  
  initialMomUnit *= (1./initialMomUnit.Mag());

  TVector3 initialPos(initialHit.getPosition().X(),
			initialHit.getPosition().Y(),
			initialHit.getPosition().Z());

  double lambda = initialMomUnit.Dot(hitPosition - initialPos);
  double coneRadiusAtHit = TMath::Abs(lambda*TMath::Tan(thetaInDeg*TMath::Pi()/180.));	
  
  TVector3 perpToMomAtHit = hitPosition - initialPos - lambda*initialMomUnit;
 // std::cout << "\nInitial Mom: (" << initialMomUnit.X() << "," << initialMomUnit.Y() << "," << initialMomUnit.Z() << ") ," 
//		<< " InitialPos: (" << initialPos.X() << "," << initialPos.Y() << "," << initialPos.Z() << ") ,"
//		<< " HitPos: (" << hitPosition.X() << "," << hitPosition.Y() << "," << hitPosition.Z() << ") ";
  if(perpToMomAtHit.Mag()<=coneRadiusAtHit)return true;
  else return false;

}

/*
scintTracks PiZeroFinder::FindPhotonTracks(double coneThetaInDeg,ScintHitCollection &track,TLorentzVector &pos){

  scintTracks photonTracks;
  photonTracks.resize(0);

  while(unmatchedHits_->size()>0){
    ScintHitCollection photonTrack;

    int closestHitVtrPos = ClosestHitInTrackToVertex(track,pos);
    if(closestHitVtrPos<0)break;

    scintHit closestHit(unmatchedHits_->at(closestHitVtrPos));
    //remove this hit from the unmatchedHits_ and add to the photonTrack vector
    unmatchedHits_->erase(unmatchedHits_->begin()+closestHitVtrPos);
    photonTrack.push_back(closestHit);

    //loop over the remaining unmatched tracks to find all that fit into cone around momentum vector
    // - assume straight line for photon
    for(int i =0;i<unmatchedHits_->size();i++){
	TVector3 hitPosition(unmatchedHits_->at(i).getPosition().X(),
				unmatchedHits_->at(i).getPosition().Y(),
				unmatchedHits_->at(i).getPosition().Z());

	if( IsHitInConeProjection(closestHit,hitPosition,coneThetaInDeg) ){
  	  //remove this hit from the unmatchedHits_ and add to the photonTrack vector
	  photonTrack.push_back(unmatchedHits_->at(i));
  	  unmatchedHits_->erase(unmatchedHits_->begin()+i);
	  i--;
	}
    }
    photonTracks.push_back(photonTrack);
    photonTrack.resize(0);

  }

  return photonTracks;
}

bool PiZeroFinder::FoundPiZero(){

  int count = 0;

  scintTracks foundPhotons = FindPhotonTracks(theta_);

  if( foundPhotons.size() >0 ){
	//loop over potential photon tracks
	for(int i=0;i<foundPhotons.size();i++){
	  double photonEnergy = 0.;
	  //loop over hits on photon track to give energy deposition
	  for(int j=0;j<foundPhotons.at(i).size();j++){
	  	photonEnergy += foundPhotons.at(i).at(j).getEdep() * (1./CLHEP::MeV);
	  }
	  std::cout << "\n FOUND POTENTIAL PHOTON WITH ENERGY = " << photonEnergy << " MeV" << std::endl;
	  count++;
	}
  	if(count>=2)return true;
  }
  else return false;

}
*/